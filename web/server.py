import http.server
import socketserver
import urllib.request
import urllib.error
import urllib.parse
import socket
import subprocess
import time
import os
import sys

PORT = 3000
WEB_DIR = os.path.dirname(os.path.abspath(__file__))

# Кеш для быстрого mDNS-разрешения
_dns_cache = {
    'ip': None,
    'expires': 0
}

def is_private_ip(ip):
    if not ip or not isinstance(ip, str):
        return False
    parts = ip.split('.')
    if len(parts) != 4:
        return False
    try:
        nums = [int(p) for p in parts]
        if nums[0] == 10:
            return True
        if nums[0] == 192 and nums[1] == 168:
            return True
        if nums[0] == 172 and 16 <= nums[1] <= 31:
            return True
    except ValueError:
        return False
    return False

def resolve_matrix_host():
    """Разрешает matrix.local в реальный IP с кешированием на 60 секунд."""
    now = time.time()
    if _dns_cache['ip'] and now < _dns_cache['expires']:
        return f"http://{_dns_cache['ip']}"
    
    # 1. Попытка через mDNS (matrix.local)
    try:
        ip = socket.gethostbyname("matrix.local")
        if ip and is_private_ip(ip.strip()):
            _dns_cache['ip'] = ip.strip()
            _dns_cache['expires'] = now + 60
            return f"http://{ip.strip()}"
    except Exception:
        pass
    
    # 2. Быстрый поиск по MAC-адресу платы в ARP таблице
    try:
        out = subprocess.check_output("arp -a", shell=True).decode('cp866', errors='ignore')
        for line in out.splitlines():
            if "84-f3-eb-23-ea-6a" in line.lower():
                found_ip = line.split()[0]
                if is_private_ip(found_ip):
                    _dns_cache['ip'] = found_ip
                    _dns_cache['expires'] = now + 60
                    return f"http://{found_ip}"
    except Exception:
        pass

    return "http://matrix.local"

class ThreadingTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    allow_reuse_address = True
    daemon_threads = True

class ProxyAndStaticHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=WEB_DIR, **kwargs)

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, HEAD')
        self.send_header('Access-Control-Allow-Headers', '*')
        self.end_headers()

    def do_HEAD(self):
        if self.path.startswith('/api/'):
            self.proxy_request('HEAD')
        else:
            super().do_HEAD()

    def do_GET(self):
        if self.path.startswith('/api/'):
            self.proxy_request('GET')
        else:
            super().do_GET()

    def do_POST(self):
        if self.path.startswith('/api/'):
            self.proxy_request('POST')
        else:
            self.send_error(404, "Not Found")

    def get_target_base(self):
        # 1. Custom header от фронтенда (фронтенд передает активный IP или хост)
        target = self.headers.get('X-Matrix-Target')
        if target and target.strip():
            clean = target.strip().rstrip('/')
            if is_private_ip(clean.replace('http://', '')) or 'matrix.local' in clean:
                return clean
        
        # 2. Переменная окружения (если задана)
        if 'MATRIX_TARGET' in os.environ and os.environ['MATRIX_TARGET']:
            return os.environ['MATRIX_TARGET'].rstrip('/')

        # 3. Универсальное автоматическое разрешение mDNS / ARP
        return resolve_matrix_host()

    def proxy_request(self, method):
        target_base = self.get_target_base()
        target_url = f"{target_base}{self.path}"
        content_length = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_length) if content_length > 0 else None

        req = urllib.request.Request(target_url, data=body, method=method)
        for key, val in self.headers.items():
            if key.lower() not in ['host', 'content-length', 'x-matrix-target']:
                req.add_header(key, val)

        try:
            with urllib.request.urlopen(req, timeout=3.5) as resp:
                self.send_response(resp.status)
                for k, v in resp.getheaders():
                    if k.lower() not in ['transfer-encoding', 'content-encoding', 'content-length']:
                        self.send_header(k, v)
                self.send_header('Access-Control-Allow-Origin', '*')
                content = resp.read()
                self.send_header('Content-Length', str(len(content)))
                self.end_headers()
                if method != 'HEAD':
                    self.wfile.write(content)
        except urllib.error.HTTPError as e:
            self.send_response(e.code)
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            if method != 'HEAD':
                self.wfile.write(e.read())
        except Exception as e:
            # Fallback к AP режиму матрицы
            if "matrix.local" in target_base:
                try:
                    fallback_url = f"http://192.168.4.1{self.path}"
                    req2 = urllib.request.Request(fallback_url, data=body, method=method)
                    with urllib.request.urlopen(req2, timeout=1.5) as resp:
                        self.send_response(resp.status)
                        for k, v in resp.getheaders():
                            if k.lower() not in ['transfer-encoding', 'content-encoding', 'content-length']:
                                self.send_header(k, v)
                        self.send_header('Access-Control-Allow-Origin', '*')
                        content = resp.read()
                        self.send_header('Content-Length', str(len(content)))
                        self.end_headers()
                        if method != 'HEAD':
                            self.wfile.write(content)
                        return
                except Exception:
                    pass

            self.send_response(502)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            if method != 'HEAD':
                self.wfile.write(f'{{"error":"Proxy error: {str(e)}"}}'.encode('utf-8'))

if __name__ == '__main__':
    with ThreadingTCPServer(("", PORT), ProxyAndStaticHTTPRequestHandler) as httpd:
        print(f"GyverMatrix Universal Dynamic Proxy Server running at http://localhost:{PORT}")
        httpd.serve_forever()
