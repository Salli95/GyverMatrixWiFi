#pragma once

// ==============================================================================
// Встроенный адаптивный Web-интерфейс GyverMatrix (Cyberpunk / Neo-Matrix темная тема)
// Хранится в Flash-памяти (PROGMEM) и полностью автономен (работает без интернета)
// ==============================================================================

const char PAGE_MAIN[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="ru">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
<title>GyverMatrix Control</title>
<style>
:root {
  --bg-dark: #0f111a;
  --card-bg: rgba(26, 30, 46, 0.85);
  --accent: #00ffaa;
  --accent-glow: rgba(0, 255, 170, 0.4);
  --danger: #ff3366;
  --halloween: #ff7700;
  --xmas: #33ccff;
  --text: #e0e6ed;
  --text-muted: #8892b0;
  --border: rgba(255, 255, 255, 0.08);
}
* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; -webkit-tap-highlight-color: transparent; }
body { background: var(--bg-dark); color: var(--text); padding-bottom: 70px; min-height: 100vh; }
header { background: rgba(15, 17, 26, 0.95); backdrop-filter: blur(10px); padding: 14px 20px; border-bottom: 1px solid var(--border); display: flex; align-items: center; justify-content: space-between; position: sticky; top: 0; z-index: 100; }
.logo { font-size: 1.25rem; font-weight: 700; letter-spacing: 1px; color: var(--accent); text-shadow: 0 0 10px var(--accent-glow); display: flex; align-items: center; gap: 8px; }
.status-pill { font-size: 0.75rem; padding: 4px 10px; border-radius: 12px; background: rgba(0,255,170,0.15); color: var(--accent); border: 1px solid var(--accent); }

/* Tabs Navigation */
.nav-bar { position: fixed; bottom: 0; left: 0; right: 0; background: rgba(15, 17, 26, 0.98); border-top: 1px solid var(--border); display: flex; justify-content: space-around; padding: 8px 4px; z-index: 100; backdrop-filter: blur(12px); }
.nav-btn { background: none; border: none; color: var(--text-muted); font-size: 0.75rem; display: flex; flex-direction: column; align-items: center; gap: 4px; cursor: pointer; flex: 1; transition: color 0.2s; }
.nav-btn.active { color: var(--accent); font-weight: 600; }
.nav-btn svg { width: 20px; height: 20px; stroke: currentColor; fill: none; stroke-width: 2; }

/* Main Content */
.container { max-width: 600px; margin: 0 auto; padding: 16px; }
.tab-pane { display: none; }
.tab-pane.active { display: block; animation: fadeIn 0.25s ease-out; }
@keyframes fadeIn { from { opacity: 0; transform: translateY(6px); } to { opacity: 1; transform: translateY(0); } }

/* Cards & Controls */
.card { background: var(--card-bg); border: 1px solid var(--border); border-radius: 16px; padding: 16px; margin-bottom: 16px; box-shadow: 0 8px 24px rgba(0,0,0,0.3); backdrop-filter: blur(8px); }
.card-title { font-size: 0.95rem; font-weight: 600; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 12px; display: flex; align-items: center; justify-content: space-between; }
.control-row { display: flex; align-items: center; justify-content: space-between; margin-bottom: 14px; }
.control-row:last-child { margin-bottom: 0; }

/* Sliders */
.slider-container { display: flex; flex-direction: column; gap: 6px; width: 100%; }
.slider-header { display: flex; justify-content: space-between; font-size: 0.85rem; }
input[type="range"] { -webkit-appearance: none; width: 100%; height: 6px; background: rgba(255,255,255,0.1); border-radius: 3px; outline: none; }
input[type="range"]::-webkit-slider-thumb { -webkit-appearance: none; width: 20px; height: 20px; border-radius: 50%; background: var(--accent); cursor: pointer; box-shadow: 0 0 10px var(--accent-glow); }

/* Buttons & Grid */
.btn { background: rgba(255,255,255,0.06); border: 1px solid var(--border); color: var(--text); padding: 10px 16px; border-radius: 12px; font-size: 0.9rem; font-weight: 500; cursor: pointer; transition: all 0.2s; display: inline-flex; align-items: center; justify-content: center; gap: 8px; }
.btn:active { transform: scale(0.97); }
.btn-primary { background: var(--accent); color: #000; font-weight: 600; border: none; box-shadow: 0 0 15px var(--accent-glow); }
.btn-danger { background: rgba(255,51,102,0.15); color: var(--danger); border-color: var(--danger); }
.btn-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 10px; }
.btn-grid-3 { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; }

/* Effect Chips Grid */
.effect-section { margin-bottom: 16px; }
.effect-section-title { font-size: 0.85rem; font-weight: 600; color: var(--accent); margin-bottom: 8px; text-transform: uppercase; }
.effects-grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(130px, 1fr)); gap: 8px; }
.effect-card { background: rgba(255,255,255,0.04); border: 1px solid var(--border); border-radius: 10px; padding: 10px; font-size: 0.85rem; text-align: center; cursor: pointer; transition: all 0.2s; }
.effect-card:active, .effect-card.active { border-color: var(--accent); background: rgba(0,255,170,0.15); color: var(--accent); font-weight: 600; }
.effect-card.halloween { border-color: rgba(255,119,0,0.4); }
.effect-card.halloween.active { background: rgba(255,119,0,0.25); color: var(--halloween); border-color: var(--halloween); }
.effect-card.xmas { border-color: rgba(51,204,255,0.4); }
.effect-card.xmas.active { background: rgba(51,204,255,0.25); color: var(--xmas); border-color: var(--xmas); }

/* Inputs */
input[type="text"], input[type="password"], select { width: 100%; background: rgba(255,255,255,0.05); border: 1px solid var(--border); padding: 12px; border-radius: 10px; color: var(--text); font-size: 0.95rem; outline: none; margin-bottom: 10px; }
input:focus, select:focus { border-color: var(--accent); }

/* Drawing Canvas 16x16 */
.canvas-wrapper { display: flex; flex-direction: column; align-items: center; gap: 12px; }
#matrix-canvas { display: grid; grid-template-columns: repeat(15, 1fr); gap: 1px; background: #000; padding: 4px; border-radius: 8px; border: 1px solid var(--border); touch-action: none; width: min(90vw, 320px); height: min(90vw, 320px); }
.pixel { background: #151515; border-radius: 2px; }
.palette { display: flex; gap: 6px; flex-wrap: wrap; justify-content: center; }
.color-dot { width: 28px; height: 28px; border-radius: 50%; border: 2px solid transparent; cursor: pointer; }
.color-dot.active { border-color: #fff; transform: scale(1.15); }

/* Game Pad */
.dpad { display: grid; grid-template-columns: repeat(3, 70px); grid-template-rows: repeat(3, 70px); gap: 6px; justify-content: center; margin: 16px auto; }
.dpad-btn { background: rgba(255,255,255,0.08); border: 1px solid var(--border); border-radius: 14px; color: var(--text); font-size: 1.4rem; display: flex; align-items: center; justify-content: center; cursor: pointer; }
.dpad-btn:active { background: var(--accent); color: #000; }
</style>
</head>
<body>

<header>
  <div class="logo">
    <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="7" height="7"/><rect x="14" y="3" width="7" height="7"/><rect x="14" y="14" width="7" height="7"/><rect x="3" y="14" width="7" height="7"/></svg>
    GyverMatrix
  </div>
  <div class="status-pill" id="conn-status">Online</div>
</header>

<div class="container">
  <!-- 1. ТАБ: ГЛАВНАЯ (DASHBOARD) -->
  <div id="tab-dash" class="tab-pane active">
    <div class="card">
      <div class="card-title">
        <span>Питание и яркость</span>
        <button class="btn btn-danger" id="btn-power" onclick="togglePower()">Выключить</button>
      </div>
      <div class="slider-container" style="margin-top:10px;">
        <div class="slider-header">
          <span>Яркость</span>
          <span id="val-bright">32</span>
        </div>
        <input type="range" id="slider-bright" min="1" max="255" value="32" oninput="setBrightness(this.value)">
      </div>
      <div class="slider-container" style="margin-top:16px;">
        <div class="slider-header">
          <span>Скорость эффектов</span>
          <span id="val-speed">80</span>
        </div>
        <input type="range" id="slider-speed" min="10" max="255" value="80" oninput="setSpeed(this.value)">
      </div>
    </div>

    <div class="card">
      <div class="card-title">Быстрые режимы</div>
      <div class="btn-grid-3">
        <button class="btn" onclick="sendCmd('$14 2')">💡 Лампа</button>
        <button class="btn" onclick="sendCmd('$14 1')">⏰ Часы</button>
        <button class="btn" onclick="sendCmd('$14 5')">🔥 Камин</button>
        <button class="btn" onclick="sendCmd('$14 4')">🌙 Ночные</button>
        <button class="btn" onclick="sendCmd('$16 2')">⏮ Назад</button>
        <button class="btn" onclick="sendCmd('$16 3')">⏭ Вперед</button>
      </div>
      <div class="control-row" style="margin-top:14px;">
        <span>Автосмена (Autoplay)</span>
        <button class="btn" id="btn-autoplay" onclick="toggleAutoplay()">ВКЛ</button>
      </div>
    </div>
  </div>

  <!-- 2. ТАБ: ЭФФЕКТЫ -->
  <div id="tab-effects" class="tab-pane">
    <div class="card">
      <!-- Хэллоуин -->
      <div class="effect-section">
        <div class="effect-section-title" style="color:var(--halloween);">🎃 Хэллоуин</div>
        <div class="effects-grid">
          <div class="effect-card halloween" onclick="setMode(35)">🎃 Тыква Джек</div>
          <div class="effect-card halloween" onclick="setMode(36)">👻 Привидения</div>
          <div class="effect-card halloween" onclick="setMode(37)">👀 Глаза во тьме</div>
          <div class="effect-card halloween" onclick="setMode(38)">🕷 Паук и сеть</div>
          <div class="effect-card halloween" onclick="setMode(39)">🧪 Котел ведьмы</div>
          <div class="effect-card halloween" onclick="setMode(40)">🩸 Капли крови</div>
        </div>
      </div>

      <!-- Новый Год -->
      <div class="effect-section">
        <div class="effect-section-title" style="color:var(--xmas);">🎄 Новый Год</div>
        <div class="effects-grid">
          <div class="effect-card xmas" onclick="setMode(41)">🎄 Ёлочка</div>
          <div class="effect-card xmas" onclick="setMode(42)">✨ Гирлянда</div>
          <div class="effect-card xmas" onclick="setMode(43)">🎆 Салют</div>
          <div class="effect-card xmas" onclick="setMode(44)">❄️ Метель</div>
          <div class="effect-card xmas" onclick="setMode(45)">🌌 Сев. сияние</div>
          <div class="effect-card xmas" onclick="setMode(12)">🌨 Снегопад</div>
        </div>
      </div>

      <!-- Классика -->
      <div class="effect-section">
        <div class="effect-section-title">🔥 Классические эффекты</div>
        <div class="effects-grid">
          <div class="effect-card" onclick="setMode(20)">🔥 Огонь</div>
          <div class="effect-card" onclick="setMode(14)">💻 Matrix</div>
          <div class="effect-card" onclick="setMode(18)">🌈 Радуга</div>
          <div class="effect-card" onclick="setMode(19)">📐 Радуга диаг.</div>
          <div class="effect-card" onclick="setMode(15)">💫 Звездопад</div>
          <div class="effect-card" onclick="setMode(17)">🔮 Шарики</div>
          <div class="effect-card" onclick="setMode(13)">✨ Конфетти</div>
        </div>
      </div>

      <!-- Шумы -->
      <div class="effect-section">
        <div class="effect-section-title">🌊 Процедурные шумы</div>
        <div class="effects-grid">
          <div class="effect-card" onclick="setMode(3)">Цветной шум</div>
          <div class="effect-card" onclick="setMode(4)">Облака</div>
          <div class="effect-card" onclick="setMode(5)">Лава</div>
          <div class="effect-card" onclick="setMode(6)">Плазма</div>
          <div class="effect-card" onclick="setMode(10)">Шумящий лес</div>
          <div class="effect-card" onclick="setMode(11)">Океан</div>
        </div>
      </div>
    </div>
  </div>

  <!-- 3. ТАБ: БЕГУЩАЯ СТРОКА -->
  <div id="tab-text" class="tab-pane">
    <div class="card">
      <div class="card-title">Бегущий текст</div>
      <input type="text" id="run-text" placeholder="Введите ваш текст..." maxlength="60">
      <div class="control-row">
        <span>Цвет текста</span>
        <select id="text-color-preset" style="width:160px; margin-bottom:0;">
          <option value="0xffffff">Белый</option>
          <option value="0xff0044">Красный</option>
          <option value="0x00ff66">Зеленый</option>
          <option value="0x00ccff">Голубой</option>
          <option value="0xffcc00">Желтый</option>
          <option value="1">Радужный</option>
          <option value="2">Случайный</option>
        </select>
      </div>
      <button class="btn btn-primary" style="width:100%; margin-top:14px;" onclick="sendRunningText()">Отправить на матрицу</button>
    </div>
  </div>

  <!-- 4. ТАБ: РИСОВАНИЕ 16x16 -->
  <div id="tab-draw" class="tab-pane">
    <div class="card canvas-wrapper">
      <div class="card-title" style="width:100%;">
        <span>Холст 16x16</span>
        <div style="display:flex; gap:6px;">
          <button class="btn" style="padding:6px 10px;" onclick="clearCanvas()">Очистить</button>
          <button class="btn" style="padding:6px 10px;" onclick="fillCanvas()">Залить</button>
        </div>
      </div>
      <div id="matrix-canvas"></div>
      <div class="palette">
        <div class="color-dot active" style="background:#ff0044;" onclick="setColor('#ff0044', this)"></div>
        <div class="color-dot" style="background:#ff7700;" onclick="setColor('#ff7700', this)"></div>
        <div class="color-dot" style="background:#ffcc00;" onclick="setColor('#ffcc00', this)"></div>
        <div class="color-dot" style="background:#00ff66;" onclick="setColor('#00ff66', this)"></div>
        <div class="color-dot" style="background:#00ccff;" onclick="setColor('#00ccff', this)"></div>
        <div class="color-dot" style="background:#b300ff;" onclick="setColor('#b300ff', this)"></div>
        <div class="color-dot" style="background:#ffffff;" onclick="setColor('#ffffff', this)"></div>
        <div class="color-dot" style="background:#000000;" onclick="setColor('#000000', this)"></div>
      </div>
    </div>
  </div>

  <!-- 5. ТАБ: ИГРЫ -->
  <div id="tab-games" class="tab-pane">
    <div class="card">
      <div class="card-title">Игровой режим</div>
      <div class="btn-grid-3" style="margin-bottom:16px;">
        <button class="btn" onclick="setMode(21)">Змейка</button>
        <button class="btn" onclick="setMode(22)">Тетрис</button>
        <button class="btn" onclick="setMode(24)">Runner</button>
        <button class="btn" onclick="setMode(25)">Flappy</button>
        <button class="btn" onclick="setMode(26)">Арканоид</button>
        <button class="btn" onclick="setMode(23)">Лабиринт</button>
      </div>

      <div class="dpad">
        <div></div>
        <button class="dpad-btn" onclick="sendButton(10)">▲</button>
        <div></div>
        <button class="dpad-btn" onclick="sendButton(13)">◀</button>
        <div></div>
        <button class="dpad-btn" onclick="sendButton(11)">▶</button>
        <div></div>
        <button class="dpad-btn" onclick="sendButton(12)">▼</button>
        <div></div>
      </div>
    </div>
  </div>

  <!-- 6. ТАБ: НАСТРОЙКИ WI-FI -->
  <div id="tab-settings" class="tab-pane">
    <div class="card">
      <div class="card-title">Подключение к роутеру (Wi-Fi)</div>
      <p style="font-size:0.85rem; color:var(--text-muted); margin-bottom:12px;">
        Настройте подключение без перепрошивки платы через кабель.
      </p>
      <div style="display:flex; gap:8px; margin-bottom:10px;">
        <button class="btn" style="flex:1;" onclick="scanWifi()">🔍 Найти сети</button>
      </div>
      <select id="wifi-ssid-select" onchange="document.getElementById('wifi-ssid').value=this.value;">
        <option value="">-- Выберите найденную сеть --</option>
      </select>
      <input type="text" id="wifi-ssid" placeholder="Имя сети (SSID)">
      <input type="password" id="wifi-pass" placeholder="Пароль от сети">
      <button class="btn btn-primary" style="width:100%;" onclick="saveWifi()">Сохранить и подключить</button>
    </div>

    <div class="card">
      <div class="card-title">Информация об устройстве</div>
      <div class="control-row"><span>Адрес mDNS:</span><span style="color:var(--accent);">http://matrix.local</span></div>
      <div class="control-row"><span>Точка доступа:</span><span>MatrixAP (192.168.4.1)</span></div>
      <div class="control-row"><span>Прошивка:</span><span>GyverMatrixOS v1.16</span></div>
    </div>
  </div>
</div>

<!-- Нижняя навигация -->
<nav class="nav-bar">
  <button class="nav-btn active" onclick="switchTab('dash', this)">
    <svg viewBox="0 0 24 24"><path d="M3 9l9-7 9 7v11a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2z"/><polyline points="9 22 9 12 15 12 15 22"/></svg>
    Главная
  </button>
  <button class="nav-btn" onclick="switchTab('effects', this)">
    <svg viewBox="0 0 24 24"><polygon points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"/></svg>
    Эффекты
  </button>
  <button class="nav-btn" onclick="switchTab('text', this)">
    <svg viewBox="0 0 24 24"><polyline points="4 7 4 4 20 4 20 7"/><line x1="9" y1="20" x2="15" y2="20"/><line x1="12" y1="4" x2="12" y2="20"/></svg>
    Текст
  </button>
  <button class="nav-btn" onclick="switchTab('draw', this)">
    <svg viewBox="0 0 24 24"><path d="M12 19l7-7 3 3-7 7-3-3z"/><path d="M18 13l-1.5-7.5L2 2l3.5 14.5L13 18l5-5z"/></svg>
    Рисование
  </button>
  <button class="nav-btn" onclick="switchTab('games', this)">
    <svg viewBox="0 0 24 24"><rect x="2" y="6" width="20" height="12" rx="6"/><circle cx="8" cy="12" r="2"/><line x1="15" y1="10" x2="15" y2="10"/><line x1="17" y1="12" x2="17" y2="12"/></svg>
    Игры
  </button>
  <button class="nav-btn" onclick="switchTab('settings', this)">
    <svg viewBox="0 0 24 24"><circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 0 1 0 2.83 2 2 0 0 1-2.83 0l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 0 1-2 2 2 2 0 0 1-2-2v-.09A1.65 1.65 0 0 0 9 19.4a1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 0 1-2.83 0 2 2 0 0 1 0-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 0 1-2-2 2 2 0 0 1 2-2h.09A1.65 1.65 0 0 0 4.6 9a1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 0 1 0-2.83 2 2 0 0 1 2.83 0l.06.06a1.65 1.65 0 0 0 1.82.33H9a1.65 1.65 0 0 0 1-1.51V3a2 2 0 0 1 2-2 2 2 0 0 1 2 2v.09a1.65 1.65 0 0 0 1 1.51 1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 0 1 2.83 0 2 2 0 0 1 0 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82V9a1.65 1.65 0 0 0 1.51 1H21a2 2 0 0 1 2 2 2 2 0 0 1-2 2h-.09a1.65 1.65 0 0 0-1.51 1z"/></svg>
    Wi-Fi
  </button>
</nav>

<script>
let curColor = '#ff0044';
let isDrawing = false;
let isPowerOn = true;
let isAutoplay = true;

function switchTab(tabId, el) {
  document.querySelectorAll('.tab-pane').forEach(p => p.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
  document.getElementById('tab-' + tabId).classList.add('active');
  el.classList.add('active');
}

// API Calls
async function sendApi(endpoint, params = {}) {
  try {
    const url = new URL(endpoint, window.location.origin);
    Object.keys(params).forEach(k => url.searchParams.append(k, params[k]));
    const res = await fetch(url, { method: 'POST' });
    return await res.json();
  } catch (e) {
    console.warn('API error:', e);
  }
}

async function sendCmd(cmd) {
  return await sendApi('/api/cmd', { cmd: cmd });
}

function togglePower() {
  isPowerOn = !isPowerOn;
  const btn = document.getElementById('btn-power');
  btn.textContent = isPowerOn ? 'Выключить' : 'Включить';
  btn.className = isPowerOn ? 'btn btn-danger' : 'btn btn-primary';
  sendApi('/api/power', { state: isPowerOn ? 1 : 0 });
}

function setBrightness(v) {
  document.getElementById('val-bright').textContent = v;
  sendApi('/api/brightness', { val: v });
}

function setSpeed(v) {
  document.getElementById('val-speed').textContent = v;
  sendApi('/api/speed', { val: v });
}

function setMode(id) {
  document.querySelectorAll('.effect-card').forEach(c => c.classList.remove('active'));
  if (event && event.currentTarget) event.currentTarget.classList.add('active');
  sendApi('/api/mode', { id: id });
}

function toggleAutoplay() {
  isAutoplay = !isAutoplay;
  const btn = document.getElementById('btn-autoplay');
  btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
  btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
  sendCmd(isAutoplay ? '$16 0' : '$16 1');
}

function sendRunningText() {
  const txt = document.getElementById('run-text').value.trim();
  const col = document.getElementById('text-color-preset').value;
  if (!txt) return;
  sendApi('/api/text', { msg: txt, color: col });
}

function sendButton(btnCode) {
  sendCmd('$' + btnCode);
}

// Canvas 16x16
const canvasEl = document.getElementById('matrix-canvas');
for (let y = 15; y >= 0; y--) {
  for (let x = 0; x < 15; x++) {
    const px = document.createElement('div');
    px.className = 'pixel';
    px.dataset.x = x;
    px.dataset.y = y;
    px.addEventListener('pointerdown', (e) => { isDrawing = true; paintPixel(px); });
    px.addEventListener('pointerenter', (e) => { if (isDrawing) paintPixel(px); });
    canvasEl.appendChild(px);
  }
}
window.addEventListener('pointerup', () => isDrawing = false);

function setColor(c, el) {
  curColor = c;
  document.querySelectorAll('.color-dot').forEach(d => d.classList.remove('active'));
  el.classList.add('active');
}

function paintPixel(px) {
  px.style.background = curColor;
  const hex = curColor.replace('#', '0x');
  sendApi('/api/draw', { x: px.dataset.x, y: px.dataset.y, color: hex });
}

function clearCanvas() {
  document.querySelectorAll('.pixel').forEach(p => p.style.background = '#151515');
  sendApi('/api/clear');
}

function fillCanvas() {
  document.querySelectorAll('.pixel').forEach(p => p.style.background = curColor);
  sendApi('/api/fill', { color: curColor.replace('#', '0x') });
}

// Wi-Fi Scanning
async function scanWifi() {
  const sel = document.getElementById('wifi-ssid-select');
  sel.innerHTML = '<option value="">Сканирование сетей...</option>';
  try {
    const res = await fetch('/api/scan');
    const nets = await res.json();
    sel.innerHTML = '<option value="">-- Выберите найденную сеть --</option>';
    nets.forEach(n => {
      const opt = document.createElement('option');
      opt.value = n.ssid;
      opt.textContent = `${n.ssid} (${n.rssi} dBm)${n.secure ? ' 🔒' : ''}`;
      sel.appendChild(opt);
    });
  } catch (e) {
    sel.innerHTML = '<option value="">Ошибка сканирования</option>';
  }
}

async function saveWifi() {
  const ssid = document.getElementById('wifi-ssid').value.trim();
  const pass = document.getElementById('wifi-pass').value.trim();
  if (!ssid) { alert('Введите имя сети SSID!'); return; }
  if (confirm(`Подключить матрицу к "${ssid}"?`)) {
    await sendApi('/api/wifi', { ssid: ssid, pass: pass });
    alert('Настройки сохранены! Плата перезагружается и подключается к ' + ssid);
  }
}
</script>
</body>
</html>
)rawliteral";

