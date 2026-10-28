#pragma once

// ==============================================================================
// Встроенный автономный Web-портал для GyverMatrix (Cyber-Purple Edition)
// Работает 100% автономно на ESP8266 PROGMEM без интернета
// ==============================================================================

const char PAGE_MAIN[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>GyverMatrix • Offline Backup Panel</title>
<link rel="icon" type="image/svg+xml" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'><defs><linearGradient id='bg' x1='0%' y1='0%' x2='100%' y2='100%'><stop offset='0%' stop-color='%23121726'/><stop offset='100%' stop-color='%23060810'/></linearGradient><linearGradient id='border' x1='0%' y1='0%' x2='100%' y2='100%'><stop offset='0%' stop-color='%2300ffaa'/><stop offset='50%' stop-color='%2300d4ff'/><stop offset='100%' stop-color='%23b026ff'/></linearGradient><radialGradient id='matrixGlow' cx='50%' cy='50%' r='50%'><stop offset='0%' stop-color='%2300e5ff' stop-opacity='0.3'/><stop offset='100%' stop-color='%2300e5ff' stop-opacity='0'/></radialGradient><filter id='ledGlow' x='-50%' y='-50%' width='200%' height='200%'><feGaussianBlur in='SourceGraphic' stdDeviation='2.2' result='blur'/><feMerge><feMergeNode in='blur'/><feMergeNode in='SourceGraphic'/></feMerge></filter></defs><rect x='2' y='2' width='60' height='60' rx='15' fill='url(%23bg)'/><circle cx='32' cy='32' r='26' fill='url(%23matrixGlow)'/><rect x='2' y='2' width='60' height='60' rx='15' fill='none' stroke='url(%23border)' stroke-width='2.2'/><g opacity='0.15'><circle cx='19' cy='19' r='7.5' fill='%23ffffff'/><circle cx='32' cy='19' r='7.5' fill='%23ffffff'/><circle cx='45' cy='19' r='7.5' fill='%23ffffff'/><circle cx='19' cy='32' r='7.5' fill='%23ffffff'/><circle cx='32' cy='32' r='8' fill='%23ffffff'/><circle cx='45' cy='32' r='7.5' fill='%23ffffff'/><circle cx='19' cy='45' r='7.5' fill='%23ffffff'/><circle cx='32' cy='45' r='7.5' fill='%23ffffff'/><circle cx='45' cy='45' r='7.5' fill='%23ffffff'/></g><g filter='url(%23ledGlow)'><circle cx='19' cy='19' r='5.2' fill='%23ff1744'/><circle cx='32' cy='19' r='5.2' fill='%23ff9100'/><circle cx='45' cy='19' r='5.2' fill='%23ffd600'/><circle cx='19' cy='32' r='5.2' fill='%2300e676'/><circle cx='32' cy='32' r='6' fill='%2300e5ff'/><circle cx='45' cy='32' r='5.2' fill='%232979ff'/><circle cx='19' cy='45' r='5.2' fill='%23d500f9'/><circle cx='32' cy='45' r='5.2' fill='%23ff007f'/><circle cx='45' cy='45' r='5.2' fill='%2300ffaa'/></g><circle cx='19' cy='19' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='32' cy='19' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='45' cy='19' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='19' cy='32' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='32' cy='32' r='2.7' fill='%23ffffff'/><circle cx='45' cy='32' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='19' cy='45' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='32' cy='45' r='2.2' fill='%23ffffff' opacity='0.95'/><circle cx='45' cy='45' r='2.2' fill='%23ffffff' opacity='0.95'/></svg>">
<style>
:root {
  --bg: #0b0714;
  --surface: #130d22;
  --card: #1b122f;
  --card-border: rgba(168, 85, 247, 0.22);
  --accent: #a855f7;
  --accent-hover: #c084fc;
  --accent-glow: rgba(168, 85, 247, 0.4);
  --accent-dim: rgba(168, 85, 247, 0.14);
  --danger: #ef4444;
  --danger-dim: rgba(239, 68, 68, 0.15);
  --text: #f3f0fa;
  --text-muted: #9d8ec2;
  --radius: 14px;
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; -webkit-tap-highlight-color: transparent; }
body { background: var(--bg); color: var(--text); padding-bottom: 74px; min-height: 100vh; }
.header { position: sticky; top: 0; z-index: 100; background: rgba(11, 7, 20, 0.92); backdrop-filter: blur(12px); border-bottom: 1px solid var(--card-border); padding: 12px 16px; display: flex; align-items: center; justify-content: space-between; }
.logo-wrap { display: flex; align-items: center; gap: 8px; }
.logo { font-size: 1.15rem; font-weight: 800; color: #fff; display: flex; align-items: center; gap: 6px; }
.logo span { color: var(--accent); }
.badge-offline { font-size: 0.68rem; font-weight: 700; text-transform: uppercase; letter-spacing: 0.5px; padding: 4px 8px; border-radius: 8px; background: var(--accent-dim); color: var(--accent); border: 1px solid var(--accent); }
.wifi-badge { font-size: 0.75rem; color: var(--text-muted); display: flex; align-items: center; gap: 5px; }
.wifi-badge strong { color: #fff; font-weight: 600; }
.container { max-width: 620px; margin: 0 auto; padding: 14px; }
.card { background: var(--card); border: 1px solid var(--card-border); border-radius: var(--radius); padding: 16px; margin-bottom: 14px; box-shadow: 0 8px 24px rgba(0,0,0,0.45); }
.card-title { font-size: 0.95rem; font-weight: 700; color: #fff; margin-bottom: 12px; display: flex; justify-content: space-between; align-items: center; }
.btn { padding: 10px 14px; border-radius: 10px; border: 1px solid var(--card-border); background: var(--surface); color: var(--text); font-weight: 600; font-size: 0.9rem; cursor: pointer; transition: all 0.2s; display: inline-flex; align-items: center; justify-content: center; gap: 6px; }
.btn:hover, .btn:active { border-color: var(--accent); }
.btn-primary { background: var(--accent); color: #fff; border-color: var(--accent); box-shadow: 0 4px 14px var(--accent-glow); }
.btn-primary:hover { background: var(--accent-hover); }
.btn-danger { background: var(--danger-dim); color: var(--danger); border-color: var(--danger); }
.btn-danger:hover { background: var(--danger); color: #fff; }
.btn-preset { padding: 12px; font-size: 0.88rem; width: 100%; }
.btn-preset.active { background: var(--accent); color: #fff; border-color: var(--accent); }
.grid-presets { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; margin-bottom: 12px; }
.slider-group { margin-bottom: 14px; }
.slider-header { display: flex; justify-content: space-between; font-size: 0.85rem; color: var(--text-muted); margin-bottom: 6px; }
.slider-header span strong { color: var(--accent); }
input[type="range"] { -webkit-appearance: none; width: 100%; height: 8px; border-radius: 4px; background: #261b40; outline: none; }
input[type="range"]::-webkit-slider-thumb { -webkit-appearance: none; width: 22px; height: 22px; border-radius: 50%; background: var(--accent); cursor: pointer; box-shadow: 0 0 10px var(--accent-glow); }
.categories { display: flex; gap: 6px; overflow-x: auto; padding-bottom: 8px; margin-bottom: 12px; }
.cat-chip { padding: 6px 12px; border-radius: 20px; font-size: 0.8rem; background: var(--surface); color: var(--text-muted); border: 1px solid var(--card-border); cursor: pointer; white-space: nowrap; }
.cat-chip.active { background: var(--accent); color: #fff; border-color: var(--accent); }
.effects-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(130px, 1fr)); gap: 8px; max-height: 420px; overflow-y: auto; padding-right: 4px; }
.eff-btn { padding: 10px 8px; font-size: 0.82rem; border-radius: 10px; background: var(--surface); border: 1px solid var(--card-border); color: var(--text); text-align: left; cursor: pointer; }
.eff-btn.active { background: var(--accent-dim); border-color: var(--accent); color: #fff; font-weight: 700; }
.nav-bar { position: fixed; bottom: 0; left: 0; right: 0; background: rgba(11, 7, 20, 0.95); backdrop-filter: blur(12px); border-top: 1px solid var(--card-border); display: flex; justify-content: space-around; padding: 6px 0; z-index: 100; }
.nav-item { display: flex; flex-direction: column; align-items: center; font-size: 0.7rem; color: var(--text-muted); background: none; border: none; cursor: pointer; padding: 4px 10px; gap: 3px; }
.nav-item.active { color: var(--accent); font-weight: 700; }
.nav-item svg { width: 20px; height: 20px; fill: currentColor; }
.tab-content { display: none; }
.tab-content.active { display: block; }
.pass-wrapper { position: relative; width: 100%; margin-bottom: 12px; }
.pass-wrapper input { width: 100%; padding: 12px 42px 12px 14px; background: #0e091a; border: 1px solid var(--card-border); border-radius: 10px; color: #fff; font-size: 0.95rem; outline: none; }
.pass-wrapper input:focus { border-color: var(--accent); }
.btn-eye { position: absolute; right: 8px; top: 50%; transform: translateY(-50%); background: none; border: none; font-size: 1.15rem; cursor: pointer; color: var(--text-muted); padding: 4px 6px; }
.canvas-grid { display: grid; grid-template-columns: repeat(15, 1fr); gap: 2px; aspect-ratio: 15/16; background: #0c0817; padding: 6px; border-radius: 12px; border: 1px solid var(--card-border); touch-action: none; margin-bottom: 12px; }
.pixel { background: #1a1230; border-radius: 2px; cursor: pointer; }
.dpad { display: grid; grid-template-columns: repeat(3, 56px); grid-template-rows: repeat(3, 56px); gap: 6px; justify-content: center; margin: 14px 0; }
.dpad-btn { width: 56px; height: 56px; border-radius: 12px; background: var(--surface); border: 1px solid var(--card-border); color: #fff; font-size: 1.3rem; display: flex; align-items: center; justify-content: center; cursor: pointer; }
.dpad-btn:active { background: var(--accent); }
</style>
</head>
<body>

<header class="header">
  <div class="logo-wrap">
    <div class="logo">⚡ GyverMatrix <span>•</span></div>
    <div class="badge-offline">ESP8266 Offline</div>
  </div>
  <div class="wifi-badge">
    <span id="st-dot">🟣</span>
    <span id="st-ssid">Загрузка...</span>
  </div>
</header>

<main class="container">

  <!-- 1. ГЛАВНАЯ -->
  <div id="tab-main" class="tab-content">
    <div class="card">
      <div class="card-title">
        <span>Питание и дисплей</span>
        <button id="btn-pwr" class="btn btn-danger" onclick="togglePower()">Выключить</button>
      </div>
      <div class="slider-group">
        <div class="slider-header">
          <span>Яркость</span>
          <span><strong id="lbl-br">128</strong> / 255</span>
        </div>
        <input type="range" id="rng-br" min="1" max="255" value="128" oninput="onBright(this.value)">
      </div>
      <div class="slider-group">
        <div class="slider-header">
          <span>Скорость эффектов</span>
          <span><strong id="lbl-sp">50%</strong></span>
        </div>
        <input type="range" id="rng-sp" min="1" max="100" value="50" oninput="onSpeed(this.value)">
      </div>
    </div>

    <div class="card">
      <div class="card-title">Быстрые режимы</div>
      <div class="grid-presets">
        <button class="btn btn-preset" id="p-lamp" onclick="sendSpc(2)">💡 Лампа</button>
        <button class="btn btn-preset" id="p-clock" onclick="sendSpc(1)">⏰ Часы</button>
        <button class="btn btn-preset" id="p-fire" onclick="sendSpc(5)">🔥 Камин</button>
        <button class="btn btn-preset" id="p-night" onclick="sendSpc(4)">🌙 Ночные</button>
      </div>
      <div style="display:flex; gap:8px;">
        <button class="btn" style="flex:1;" onclick="stepMode('prev')">⏮ Назад</button>
        <button class="btn" id="btn-auto" style="flex:1;" onclick="toggleAuto()">Авто: ВЫКЛ</button>
        <button class="btn" style="flex:1;" onclick="stepMode('next')">Вперед ⏭</button>
      </div>
    </div>
  </div>

  <!-- 2. ЭФФЕКТЫ -->
  <div id="tab-effects" class="tab-content active">
    <div class="card">
      <div class="card-title">Каталог эффектов</div>
      <div class="categories">
        <button class="cat-chip active" onclick="filterEff('all', this)">Все</button>
        <button class="cat-chip" onclick="filterEff('new', this)">✨ Новинки</button>
        <button class="cat-chip" onclick="filterEff('halloween', this)">🎃 Хэллоуин / Сияние</button>
        <button class="cat-chip" onclick="filterEff('xmas', this)">🎄 Новый Год</button>
        <button class="cat-chip" onclick="filterEff('classic', this)">🔥 Классика</button>
        <button class="cat-chip" onclick="filterEff('noise', this)">🌊 Шумы</button>
      </div>
      <div class="effects-grid" id="eff-grid"></div>
    </div>
  </div>

  <!-- 3. ТЕКСТ -->
  <div id="tab-text" class="tab-content">
    <div class="card">
      <div class="card-title">Бегущая строка</div>
      <input type="text" id="txt-input" placeholder="Введите текст сообщения..." style="width:100%; padding:12px; background:#0e091a; border:1px solid var(--card-border); border-radius:10px; color:#fff; font-size:1rem; margin-bottom:12px; outline:none;">
      <div style="display:flex; gap:8px; align-items:center; margin-bottom:14px;">
        <input type="color" id="txt-col" value="#a855f7" style="width:44px; height:44px; border:none; border-radius:10px; background:none; cursor:pointer;">
        <button class="btn" style="flex:1;" onclick="sendText('color')">Фикс. цвет</button>
        <button class="btn" style="flex:1;" onclick="sendText('1')">🌈 Радуга</button>
        <button class="btn" style="flex:1;" onclick="sendText('2')">🎲 Случайный</button>
      </div>
      <button class="btn btn-primary" style="width:100%;" onclick="sendText('color')">🚀 Отправить на матрицу</button>
    </div>
  </div>

  <!-- 4. РИСОВАНИЕ -->
  <div id="tab-draw" class="tab-content">
    <div class="card">
      <div class="card-title">
        <span>Холст 15×16</span>
        <button class="btn btn-danger" style="padding:6px 10px; font-size:0.8rem;" onclick="clearDraw()">Очистить</button>
      </div>
      <div class="canvas-grid" id="cnv"></div>
      <div style="display:flex; gap:8px; align-items:center;">
        <input type="color" id="draw-col" value="#a855f7" style="width:44px; height:44px; border:none; border-radius:10px; background:none; cursor:pointer;">
        <button class="btn" style="flex:1;" onclick="fillDraw()">Залить холст</button>
      </div>
    </div>
  </div>

  <!-- 5. ИГРЫ -->
  <div id="tab-games" class="tab-content">
    <div class="card">
      <div class="card-title">Игры и бот</div>
      <div style="display:grid; grid-template-columns:1fr 1fr; gap:8px; margin-bottom:12px;">
        <button class="btn" onclick="startGame(0)">🐍 Змейка</button>
        <button class="btn" onclick="startGame(1)">🧱 Тетрис</button>
        <button class="btn" onclick="startGame(2)">🌀 Лабиринт</button>
        <button class="btn" onclick="startGame(3)">🏃 Раннер</button>
        <button class="btn" onclick="startGame(4)">🐦 Flappy</button>
        <button class="btn" onclick="startGame(5)">🏓 Арканоид</button>
      </div>
      <button id="btn-bot" class="btn btn-primary" style="width:100%; margin-bottom:12px;" onclick="toggleBot()">🤖 Авто-режим: ВКЛ (Бот играет сам)</button>
      <div class="dpad">
        <div></div><button class="dpad-btn" onclick="sendDpad(0)">▲</button><div></div>
        <button class="dpad-btn" onclick="sendDpad(3)">◀</button><div></div><button class="dpad-btn" onclick="sendDpad(1)">▶</button>
        <div></div><button class="dpad-btn" onclick="sendDpad(2)">▼</button><div></div>
      </div>
    </div>
  </div>

  <!-- 6. НАСТРОЙКИ WI-FI -->
  <div id="tab-wifi" class="tab-content">
    <div class="card">
      <div class="card-title">Подключение к сети Wi-Fi</div>
      <div style="display:flex; gap:8px; margin-bottom:12px;">
        <select id="wifi-sel" onchange="document.getElementById('wifi-ssid').value=this.value" style="flex:1; padding:12px; background:#0e091a; border:1px solid var(--card-border); border-radius:10px; color:#fff; outline:none;">
          <option value="">Нажмите 🔄 для поиска</option>
        </select>
        <button class="btn" onclick="scanWifi()">🔄</button>
      </div>
      <input type="text" id="wifi-ssid" placeholder="Имя Wi-Fi сети (SSID)" style="width:100%; padding:12px; background:#0e091a; border:1px solid var(--card-border); border-radius:10px; color:#fff; margin-bottom:12px; outline:none;" required>
      
      <div class="pass-wrapper">
        <input type="password" id="wifi-pass" placeholder="Пароль от Wi-Fi">
        <button type="button" class="btn-eye" onclick="togglePassVisibility()" id="btn-eye-toggle">👁</button>
      </div>

      <button class="btn btn-primary" style="width:100%; margin-bottom:14px;" onclick="saveWifi()">Подключить к Wi-Fi</button>
      <button class="btn btn-danger" style="width:100%;" onclick="forgetWifi()">🗑 Забыть сеть и сбросить Wi-Fi</button>
    </div>
  </div>

</main>

<nav class="nav-bar">
  <button class="nav-item" onclick="switchNav('main', this)">🏠<span>Главная</span></button>
  <button class="nav-item active" onclick="switchNav('effects', this)">⭐<span>Эффекты</span></button>
  <button class="nav-item" onclick="switchNav('text', this)">🔤<span>Текст</span></button>
  <button class="nav-item" onclick="switchNav('draw', this)">🎨<span>Рисовать</span></button>
  <button class="nav-item" onclick="switchNav('games', this)">🎮<span>Игры</span></button>
  <button class="nav-item" onclick="switchNav('wifi', this)">⚙️<span>Сеть</span></button>
</nav>

<script>
const EFFECTS = [
  { id: 50, name: '📺 Звонок', cat: 'new' },
  { id: 51, name: '🌌 ОСД / Изнанка', cat: 'new' },
  { id: 52, name: '🚫 Охотники', cat: 'new' },
  { id: 53, name: '☕️ Твин Пикс', cat: 'new' },
  { id: 54, name: '🏙 Бегущий', cat: 'new' },
  { id: 55, name: '🟡 Пакман', cat: 'new' },
  { id: 56, name: '👾 Захватчики', cat: 'new' },
  { id: 57, name: '🍄 Марио', cat: 'new' },
  { id: 58, name: '🌀 Соник', cat: 'new' },
  { id: 59, name: '⚡️ Покебол', cat: 'new' },
  { id: 60, name: '🕳 Черная дыра', cat: 'new' },
  { id: 61, name: '✨ Гиперпрыжок', cat: 'new' },
  { id: 62, name: '🛸 Луч НЛО', cat: 'new' },
  { id: 63, name: '🌑 Затмение', cat: 'new' },
  { id: 64, name: '🪼 Медузы', cat: 'new' },
  { id: 65, name: '⚡️ Гроза', cat: 'new' },
  { id: 66, name: '⏳ Песочные часы', cat: 'new' },
  { id: 67, name: '💧 Капли дождя', cat: 'new' },
  { id: 68, name: '📊 Эквалайзер', cat: 'new' },
  { id: 69, name: '🪩 Диско-шар', cat: 'new' },
  { id: 35, name: '🎃 Тыква Джек', cat: 'halloween' },
  { id: 46, name: '🩸 Кровавый лифт', cat: 'halloween' },
  { id: 47, name: '🔶 Ковер Оверлук', cat: 'halloween' },
  { id: 48, name: '🪞 REDRUM', cat: 'halloween' },
  { id: 49, name: '❄️ Ледяной лабиринт', cat: 'halloween' },
  { id: 36, name: '👻 Привидения', cat: 'halloween' },
  { id: 37, name: '👀 Глаза во тьме', cat: 'halloween' },
  { id: 38, name: '🕷 Паук и сеть', cat: 'halloween' },
  { id: 39, name: '🧪 Котел ведьмы', cat: 'halloween' },
  { id: 40, name: '🩸 Капли крови', cat: 'halloween' },
  { id: 41, name: '🎄 Ёлочка', cat: 'xmas' },
  { id: 42, name: '✨ Гирлянда', cat: 'xmas' },
  { id: 43, name: '🎆 Салют', cat: 'xmas' },
  { id: 44, name: '❄️ Метель', cat: 'xmas' },
  { id: 45, name: '🌌 Сев. сияние', cat: 'xmas' },
  { id: 12, name: '🌨 Снегопад', cat: 'xmas' },
  { id: 20, name: '🔥 Огонь', cat: 'classic' },
  { id: 14, name: '💻 Matrix', cat: 'classic' },
  { id: 18, name: '🌈 Радуга', cat: 'classic' },
  { id: 19, name: '📐 Радуга диаг.', cat: 'classic' },
  { id: 15, name: '💫 Звездопад', cat: 'classic' },
  { id: 17, name: '🔮 Шарики', cat: 'classic' },
  { id: 13, name: '🎉 Конфетти', cat: 'classic' },
  { id: 16, name: '◽️ Кубик', cat: 'classic' },
  { id: 3,  name: '🎨 Цветной шум', cat: 'noise' },
  { id: 4,  name: '☁️ Облака', cat: 'noise' },
  { id: 5,  name: '🌋 Лава', cat: 'noise' },
  { id: 6,  name: '⚡️ Плазма', cat: 'noise' },
  { id: 10, name: '🌲 Шумящий лес', cat: 'noise' },
  { id: 11, name: '🌊 Океан', cat: 'noise' }
];

let isDrawing = false;
let isPower = true;
let isBot = true;
let brTimer = null, spTimer = null;

document.addEventListener('DOMContentLoaded', () => {
  renderEff('all');
  initCanvas();
  loadState();
  setInterval(loadState, 3000);
});

async function api(path, params = {}) {
  try {
    const u = new URL(path, window.location.origin);
    Object.keys(params).forEach(k => u.searchParams.append(k, params[k]));
    const res = await fetch(u.toString(), { method: 'POST' });
    return await res.json();
  } catch(e) {}
}

async function loadState() {
  try {
    const res = await fetch('/api/state');
    const d = await res.json();
    const stSsid = document.getElementById('st-ssid');
    const stDot = document.getElementById('st-dot');
    if (d.ssid) {
      stSsid.innerHTML = `<strong>${d.ssid}</strong> (${d.ip})`;
      stDot.textContent = d.apMode ? '🟡' : '🟣';
    }
    if (d.brightness !== undefined && !brTimer) {
      document.getElementById('rng-br').value = d.brightness;
      document.getElementById('lbl-br').textContent = d.brightness;
    }
    if (d.speed !== undefined && !spTimer) {
      const pct = Math.round(1 + ((255 - d.speed) / (255 - 15)) * 99);
      document.getElementById('rng-sp').value = pct;
      document.getElementById('lbl-sp').textContent = pct + '%';
    }
    isPower = d.power;
    const btnPwr = document.getElementById('btn-pwr');
    btnPwr.textContent = isPower ? 'Выключить' : 'Включить';
    btnPwr.className = isPower ? 'btn btn-danger' : 'btn btn-primary';
    
    document.getElementById('btn-auto').textContent = d.autoplay ? 'Авто: ВКЛ' : 'Авто: ВЫКЛ';

    document.querySelectorAll('.btn-preset').forEach(b => b.classList.remove('active'));
    if (d.specialMode) {
      if (d.specialModeId === 2) document.getElementById('p-lamp')?.classList.add('active');
      if (d.specialModeId === 1) document.getElementById('p-clock')?.classList.add('active');
      if (d.specialModeId === 5) document.getElementById('p-fire')?.classList.add('active');
      if (d.specialModeId === 4) document.getElementById('p-night')?.classList.add('active');
    }
  } catch(e) {}
}

function renderEff(cat) {
  const g = document.getElementById('eff-grid');
  g.innerHTML = '';
  const list = cat === 'all' ? EFFECTS : EFFECTS.filter(e => e.cat === cat);
  list.forEach(e => {
    const b = document.createElement('button');
    b.className = 'eff-btn';
    b.textContent = e.name;
    b.onclick = () => {
      document.querySelectorAll('.eff-btn').forEach(x => x.classList.remove('active'));
      b.classList.add('active');
      api('/api/mode', { id: e.id });
    };
    g.appendChild(b);
  });
}

function filterEff(cat, el) {
  document.querySelectorAll('.cat-chip').forEach(c => c.classList.remove('active'));
  el.classList.add('active');
  renderEff(cat);
}

function togglePower() {
  isPower = !isPower;
  api('/api/power', { state: isPower ? 1 : 0 });
  loadState();
}

function onBright(v) {
  document.getElementById('lbl-br').textContent = v;
  clearTimeout(brTimer);
  brTimer = setTimeout(() => { api('/api/brightness', { val: v }); brTimer = null; }, 80);
}

function onSpeed(pct) {
  document.getElementById('lbl-sp').textContent = pct + '%';
  const delay = Math.round(255 - ((pct - 1) / 99) * (255 - 15));
  clearTimeout(spTimer);
  spTimer = setTimeout(() => { api('/api/speed', { val: delay }); spTimer = null; }, 80);
}

function sendSpc(id) {
  api('/api/special', { id: id });
  loadState();
}

function stepMode(dir) { api('/api/step', { dir: dir }); loadState(); }
function toggleAuto() { api('/api/autoplay', { val: document.getElementById('btn-auto').textContent.includes('ВЫКЛ') ? 1 : 0 }); loadState(); }

function sendText(mode) {
  const t = document.getElementById('txt-input').value.trim();
  if (!t) return alert('Введите текст!');
  let col = mode === 'color' ? document.getElementById('txt-col').value.replace('#', '0x') : mode;
  api('/api/text', { msg: t, color: col });
}

function initCanvas() {
  const c = document.getElementById('cnv');
  c.innerHTML = '';
  for (let y = 15; y >= 0; y--) {
    for (let x = 0; x < 15; x++) {
      const p = document.createElement('div');
      p.className = 'pixel';
      p.onpointerdown = () => { isDrawing = true; paint(p, x, y); };
      p.onpointerenter = () => { if (isDrawing) paint(p, x, y); };
      c.appendChild(p);
    }
  }
  window.onpointerup = () => isDrawing = false;
}

function paint(p, x, y) {
  const col = document.getElementById('draw-col').value;
  p.style.background = col;
  api('/api/draw', { x: x, y: y, color: col.replace('#', '0x') });
}

function clearDraw() {
  document.querySelectorAll('#cnv .pixel').forEach(p => p.style.background = '#1a1230');
  api('/api/clear');
}

function fillDraw() {
  const col = document.getElementById('draw-col').value;
  document.querySelectorAll('#cnv .pixel').forEach(p => p.style.background = col);
  api('/api/fill', { color: col.replace('#', '0x') });
}

function startGame(g) { api('/api/game', { action: 'start', game: g, demo: isBot ? 1 : 0 }); }
function toggleBot() {
  isBot = !isBot;
  document.getElementById('btn-bot').textContent = isBot ? '🤖 Авто-режим: ВКЛ (Бот играет сам)' : '🎮 Ручной режим';
  api('/api/game', { action: 'demo', val: isBot ? 1 : 0 });
}
function sendDpad(b) { api('/api/game', { action: 'button', btn: b }); }

function togglePassVisibility() {
  const inp = document.getElementById('wifi-pass');
  const btn = document.getElementById('btn-eye-toggle');
  if (inp.type === 'password') {
    inp.type = 'text';
    btn.textContent = '🙈';
  } else {
    inp.type = 'password';
    btn.textContent = '👁';
  }
}

async function scanWifi() {
  const sel = document.getElementById('wifi-sel');
  sel.innerHTML = '<option>Сканирование...</option>';
  try {
    const res = await fetch('/api/scan');
    const nets = await res.json();
    sel.innerHTML = '<option value="">-- Выберите сеть --</option>';
    nets.forEach(n => {
      if (n.ssid) {
        const o = document.createElement('option');
        o.value = n.ssid;
        o.textContent = `${n.ssid} (${n.rssi} dBm)${n.secure ? ' 🔒' : ''}`;
        sel.appendChild(o);
      }
    });
  } catch(e) { sel.innerHTML = '<option>Ошибка сканирования</option>'; }
}

async function saveWifi() {
  const s = document.getElementById('wifi-ssid').value.trim();
  const p = document.getElementById('wifi-pass').value;
  if (!s) return alert('Введите имя сети SSID');
  await api('/api/wifi', { ssid: s, pass: p });
  alert('Параметры сохранены. Плата перезагружается...');
}

async function forgetWifi() {
  if (confirm('Сбросить сохраненную Wi-Fi сеть? Плата перезагрузится в режим точки доступа MatrixAP.')) {
    await api('/api/forget_wifi');
    alert('Сеть сброшена. Перезагрузка...');
  }
}

function switchNav(id, el) {
  document.querySelectorAll('.tab-content').forEach(t => t.classList.remove('active'));
  document.querySelectorAll('.nav-item').forEach(n => n.classList.remove('active'));
  document.getElementById('tab-' + id)?.classList.add('active');
  el.classList.add('active');
}
</script>
</body>
</html>
)rawliteral";
