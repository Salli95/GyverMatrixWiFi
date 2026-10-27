#pragma once

// ==============================================================================
// Встроенный Web-портал настройки Wi-Fi для GyverMatrix
// Хранится в Flash-памяти ESP8266 (PROGMEM), легковесный и не нагружает плату
// ==============================================================================

const char PAGE_MAIN[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>GyverMatrix Wi-Fi Setup</title>
<style>
:root {
  --bg: #0b0d14;
  --card: #151926;
  --accent: #00ffaa;
  --text: #e0e6ed;
  --text-dim: #8892b0;
  --border: rgba(255,255,255,0.08);
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
body { background: var(--bg); color: var(--text); padding: 20px; display: flex; justify-content: center; align-items: center; min-height: 100vh; }
.card { background: var(--card); border: 1px solid var(--border); border-radius: 16px; padding: 24px; width: 100%; max-width: 400px; box-shadow: 0 12px 36px rgba(0,0,0,0.6); }
.header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 16px; }
.logo { font-size: 1.3rem; font-weight: 700; color: var(--accent); letter-spacing: 0.5px; }
.badge { font-size: 0.75rem; padding: 4px 10px; border-radius: 10px; background: rgba(0,255,170,0.15); color: var(--accent); border: 1px solid rgba(0,255,170,0.3); }
.badge.ap { background: rgba(255,170,0,0.15); color: #ffaa00; border-color: rgba(255,170,0,0.3); }
label { display: block; font-size: 0.85rem; color: var(--text-dim); margin-bottom: 6px; }
input, select { width: 100%; padding: 12px 14px; background: #0c0e15; border: 1px solid var(--border); border-radius: 10px; color: #fff; font-size: 0.95rem; margin-bottom: 14px; outline: none; }
input:focus, select:focus { border-color: var(--accent); }
.row-flex { display: flex; gap: 8px; margin-bottom: 14px; }
.btn { width: 100%; padding: 12px; background: var(--accent); color: #000; font-weight: 700; border: none; border-radius: 10px; cursor: pointer; font-size: 1rem; transition: opacity 0.2s; }
.btn:hover { opacity: 0.9; }
.btn-icon { width: 48px; flex-shrink: 0; background: #0c0e15; border: 1px solid var(--border); color: var(--accent); font-size: 1.2rem; border-radius: 10px; cursor: pointer; }
.btn-sec { background: transparent; color: var(--accent); border: 1px solid var(--accent); margin-top: 10px; text-decoration: none; display: block; text-align: center; }
.info { font-size: 0.8rem; color: var(--text-dim); line-height: 1.5; margin-top: 18px; text-align: center; border-top: 1px solid var(--border); padding-top: 14px; }
.info strong { color: var(--accent); }
.btn-forget { background: transparent; border: 1px solid #ff3366; color: #ff3366; margin-top: 10px; font-size: 0.85rem; padding: 10px; }
.btn-forget:hover { background: rgba(255,51,102,0.15); }
</style>
</head>
<body>
<div class="card">
  <div class="header">
    <div class="logo">⚡ GyverMatrix</div>
    <div id="st-badge" class="badge">Загрузка...</div>
  </div>

  <form onsubmit="saveWifi(event)">
    <label>Найденные сети Wi-Fi</label>
    <div class="row-flex">
      <select id="wifi-select" onchange="onSelectSsid(this.value)" style="margin-bottom:0;">
        <option value="">Нажмите 🔄 для поиска</option>
      </select>
      <button type="button" class="btn-icon" onclick="scanWifi()" title="Сканировать">🔄</button>
    </div>

    <label>Имя сети (SSID)</label>
    <input type="text" id="wifi-ssid" placeholder="Имя вашей Wi-Fi сети" required>

    <label>Пароль сети</label>
    <input type="password" id="wifi-pass" placeholder="Пароль от Wi-Fi">

    <button type="submit" class="btn" id="btn-save">Подключить к Wi-Fi</button>
  </form>
  <button type="button" class="btn btn-forget" onclick="forgetWifi()">Забыть сеть и сбросить Wi-Fi</button>

  <div class="info">
    IP матрица: <strong id="ip-val">...</strong><br>
    Управление эффектами доступно с внешнего сайта или по адресу:
    <a href="http://localhost:3000" id="link-panel" style="color:var(--accent); text-decoration:none; font-weight:600; display:block; margin-top:6px;">Открыть пульт управления</a>
  </div>
</div>

<script>
document.addEventListener('DOMContentLoaded', () => {
  loadState();
  scanWifi();
});

async function loadState() {
  try {
    const res = await fetch('/api/state');
    const data = await res.json();
    const badge = document.getElementById('st-badge');
    const ipVal = document.getElementById('ip-val');
    ipVal.textContent = data.ip;
    if (data.apMode) {
      badge.textContent = 'Точка доступа';
      badge.className = 'badge ap';
    } else {
      badge.textContent = 'В сети: ' + data.ip;
      badge.className = 'badge';
    }
  } catch (e) {}
}

async function scanWifi() {
  const sel = document.getElementById('wifi-select');
  sel.innerHTML = '<option value="">Сканирование сетей...</option>';
  try {
    const res = await fetch('/api/scan');
    const nets = await res.json();
    sel.innerHTML = '<option value="">-- Выберите найденную сеть --</option>';
    nets.forEach(n => {
      if (n.ssid && n.ssid.trim()) {
        const opt = document.createElement('option');
        opt.value = n.ssid;
        opt.textContent = `${n.ssid} (${n.rssi} dBm)${n.secure ? ' 🔒' : ''}`;
        sel.appendChild(opt);
      }
    });
  } catch (e) {
    sel.innerHTML = '<option value="">Ошибка сканирования</option>';
  }
}

function onSelectSsid(val) {
  if (val) document.getElementById('wifi-ssid').value = val;
}

async function saveWifi(e) {
  e.preventDefault();
  const ssid = document.getElementById('wifi-ssid').value.trim();
  const pass = document.getElementById('wifi-pass').value.trim();
  if (!ssid) return;

  const btn = document.getElementById('btn-save');
  btn.disabled = true;
  btn.textContent = 'Сохранение...';

  try {
    await fetch('/api/wifi', {
      method: 'POST',
      headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
      body: `ssid=${encodeURIComponent(ssid)}&pass=${encodeURIComponent(pass)}`
    });
    alert('Настройки сохранены! Плата перезагружается и подключается к ' + ssid);
  } catch (err) {
    alert('Настройки отправлены! Плата перезагружается...');
  }
}

async function forgetWifi() {
  if (!confirm('Забыть сохраненную сеть Wi-Fi? Плата перейдет в режим точки доступа MatrixAP (192.168.4.1) и перезагрузится.')) return;
  try {
    await fetch('/api/forget_wifi', { method: 'POST' });
  } catch (e) {}
  alert('Сеть удалена. Плата перезагружается в режиме MatrixAP (пароль 12341234)');
}
</script>
</body>
</html>
)rawliteral";
