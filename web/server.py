import http.server
import socketserver
import urllib.request
import urllib.error
import urllib.parse
import os
import sys

PORT = 3000
MATRIX_TARGET = "http://192.168.0.103"
WEB_DIR = os.path.dirname(os.path.abspath(__file__))

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

    def proxy_request(self, method):
        target_url = f"{MATRIX_TARGET}{self.path}"
        content_length = int(self.headers.get('Content-Length', 0))
        body = self.rfile.read(content_length) if content_length > 0 else None

        req = urllib.request.Request(target_url, data=body, method=method)
        for key, val in self.headers.items():
            if key.lower() not in ['host', 'content-length']:
                req.add_header(key, val)

        try:
            with urllib.request.urlopen(req, timeout=3.0) as resp:
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
            self.send_response(502)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            if method != 'HEAD':
                self.wfile.write(f'{{"error":"Proxy error: {str(e)}"}}'.encode('utf-8'))

if __name__ == '__main__':
    with ThreadingTCPServer(("", PORT), ProxyAndStaticHTTPRequestHandler) as httpd:
        print(f"GyverMatrix Web Server with API proxy running at http://localhost:{PORT}")
        httpd.serve_forever()
