// ==============================================================================
// GyverMatrix Web Control (Pro Edition)
// Управление матрицей по Wi-Fi: автопоиск, эффекты, бегущая строка, рисование, игры
// ==============================================================================

const DEFAULT_IP = 'http://192.168.0.103';

// Если запущен локальный сервер (localhost:3000), используем встроенный прокси
const isLocalServer = (window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1');

let matrixHost = localStorage.getItem('gyver_matrix_host');
if (!matrixHost || matrixHost === 'http://matrix.local') {
  matrixHost = DEFAULT_IP;
}

let isScanning = false;
let isConnected = false;
let curColor = '#ff0044';
let isDrawing = false;
let isPowerOn = true;
let isAutoplay = false;
let isGameDemo = true;
let textMode = 'color'; // 'color', 'rainbow', 'random'
let lastUserInteraction = 0;
let brightTimeout = null;
let speedTimeout = null;

// Каталог эффектов с ID прошивки GyverMatrixOS v1.16
const ALL_EFFECTS = [
  // Хэллоуин и «Сияние»
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

  // Новый Год
  { id: 41, name: '🎄 Ёлочка', cat: 'xmas' },
  { id: 42, name: '✨ Гирлянда', cat: 'xmas' },
  { id: 43, name: '🎆 Салют', cat: 'xmas' },
  { id: 44, name: '❄️ Метель', cat: 'xmas' },
  { id: 45, name: '🌌 Сев. сияние', cat: 'xmas' },
  { id: 12, name: '🌨 Снегопад', cat: 'xmas' },

  // Классика
  { id: 20, name: '🔥 Огонь', cat: 'classic' },
  { id: 14, name: '💻 The Matrix', cat: 'classic' },
  { id: 18, name: '🌈 Радуга', cat: 'classic' },
  { id: 19, name: '📐 Радуга диаг.', cat: 'classic' },
  { id: 15, name: '💫 Звездопад', cat: 'classic' },
  { id: 17, name: '🔮 Шарики', cat: 'classic' },
  { id: 13, name: '🎉 Конфетти', cat: 'classic' },
  { id: 16, name: '◽️ Кубик', cat: 'classic' },

  // Шумы
  { id: 3, name: '🎨 Цветной шум', cat: 'noise' },
  { id: 4, name: '☁️ Облака', cat: 'noise' },
  { id: 5, name: '🌋 Лава', cat: 'noise' },
  { id: 6, name: '⚡️ Плазма', cat: 'noise' },
  { id: 10, name: '🌲 Шумящий лес', cat: 'noise' },
  { id: 11, name: '🌊 Океан', cat: 'noise' }
];

document.addEventListener('DOMContentLoaded', () => {
  renderEffects('all');
  initCanvas();
  updateHostDisplay();
  checkConnection();

  // Фоновый опрос статуса каждые 3 секунды
  setInterval(checkConnection, 3000);
});

function updateHostDisplay() {
  const el = document.getElementById('cur-host-display');
  if (el) el.textContent = matrixHost;
  const input = document.getElementById('manual-host-input');
  if (input) input.value = matrixHost;
}

// ------------------------------------------------------------------------------
// Сетевые запросы
// ------------------------------------------------------------------------------
async function fetchWithTimeout(url, options = {}, timeout = 2500) {
  const controller = new AbortController();
  const id = setTimeout(() => controller.abort(), timeout);
  try {
    const res = await fetch(url, { ...options, signal: controller.signal });
    clearTimeout(id);
    return res;
  } catch (err) {
    clearTimeout(id);
    throw err;
  }
}

function getBaseUrl() {
  if (isLocalServer) {
    return window.location.origin;
  }
  return matrixHost.endsWith('/') ? matrixHost.slice(0, -1) : matrixHost;
}

async function sendApi(endpoint, params = {}) {
  try {
    const base = getBaseUrl();
    const url = new URL(endpoint, base);
    Object.keys(params).forEach(k => url.searchParams.append(k, params[k]));
    const res = await fetchWithTimeout(url.toString(), { method: 'POST' });
    setConnected(true);
    return await res.json();
  } catch (e) {
    console.warn('API error:', e);
    if (!isLocalServer) {
      tryFallbackHost();
    }
  }
}

async function tryFallbackHost() {
  if (matrixHost !== DEFAULT_IP) {
    try {
      const res = await fetchWithTimeout(`${DEFAULT_IP}/api/state`, { method: 'GET' }, 1200);
      if (res.ok) {
        matrixHost = DEFAULT_IP;
        localStorage.setItem('gyver_matrix_host', matrixHost);
        updateHostDisplay();
        setConnected(true);
        const data = await res.json();
        applyState(data);
      }
    } catch (err) {}
  }
}

let isChecking = false;
async function checkConnection() {
  if (isScanning || isChecking) return;
  isChecking = true;
  try {
    const base = getBaseUrl();
    const res = await fetchWithTimeout(`${base}/api/state`, { method: 'GET' }, 2000);
    if (res.ok) {
      const data = await res.json();
      setConnected(true);
      applyState(data);
      return true;
    }
  } catch (e) {
    if (!isLocalServer && matrixHost !== DEFAULT_IP) {
      try {
        const res2 = await fetchWithTimeout(`${DEFAULT_IP}/api/state`, { method: 'GET' }, 1500);
        if (res2.ok) {
          matrixHost = DEFAULT_IP;
          localStorage.setItem('gyver_matrix_host', matrixHost);
          updateHostDisplay();
          setConnected(true);
          const data2 = await res2.json();
          applyState(data2);
          return true;
        }
      } catch (e2) {}
    }
    setConnected(false);
    return false;
  } finally {
    isChecking = false;
  }
}

function setConnected(online) {
  isConnected = online;
  const ind = document.getElementById('status-ind');
  const txt = document.getElementById('status-text');
  if (online) {
    if (ind) ind.className = 'status-indicator online';
    if (txt) txt.textContent = 'Подключено';
    const banner = document.getElementById('discovery-banner');
    if (banner) banner.style.display = 'none';
  } else {
    if (ind) ind.className = 'status-indicator offline';
    if (txt) txt.textContent = 'Офлайн';
    const banner = document.getElementById('discovery-banner');
    if (banner) banner.style.display = 'flex';
  }
}

function applyState(data) {
  const now = Date.now();
  if (now - lastUserInteraction > 3500) {
    // Яркость (1..255)
    if (data.brightness !== undefined) {
      const elVal = document.getElementById('val-bright');
      const elSlider = document.getElementById('slider-bright');
      if (elVal) elVal.textContent = data.brightness;
      if (elSlider) elSlider.value = data.brightness;
    }

    // Скорость: конвертируем миллисекунды задержки матрицы (10..255) в проценты (1..100%)
    // 255 мс = 1% (самый медленный), 15 мс = 100% (самый быстрый)
    if (data.speed !== undefined) {
      const percent = Math.round(1 + ((255 - data.speed) / (255 - 15)) * 99);
      const clamped = Math.max(1, Math.min(100, percent));
      const elVal = document.getElementById('val-speed');
      const elSlider = document.getElementById('slider-speed');
      if (elVal) elVal.textContent = clamped + '%';
      if (elSlider) elSlider.value = clamped;
    }
  }

  // Питание
  if (data.power !== undefined) {
    isPowerOn = data.power;
    const btn = document.getElementById('btn-power');
    if (btn) {
      btn.textContent = isPowerOn ? 'Выключить' : 'Включить';
      btn.className = isPowerOn ? 'btn btn-danger' : 'btn btn-primary';
    }
  }

  // Autoplay
  if (data.autoplay !== undefined) {
    isAutoplay = data.autoplay;
    const btn = document.getElementById('btn-autoplay');
    if (btn) {
      btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
      btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
    }
  }

  // Подсветка быстрых спец-режимов
  document.querySelectorAll('.btn-preset').forEach(b => b.classList.remove('active'));
  if (data.specialMode && data.specialModeId !== undefined) {
    if (data.specialModeId === 2) document.getElementById('preset-lamp')?.classList.add('active');
    else if (data.specialModeId === 1) document.getElementById('preset-clock')?.classList.add('active');
    else if (data.specialModeId === 5) document.getElementById('preset-fire')?.classList.add('active');
    else if (data.specialModeId === 4) document.getElementById('preset-night')?.classList.add('active');
  }

  // Подсветка активной игры
  if (data.gamemode) {
    document.querySelectorAll('.btn-game').forEach(b => {
      // Game modes: 21 (Snake), 22 (Tetris), 23 (Maze), 24 (Runner), 25 (Flappy), 26 (Arkanoid)
      const gameMap = { 21: '0', 22: '1', 23: '2', 24: '3', 25: '4', 26: '5' };
      if (gameMap[data.mode] === b.dataset.game) {
        b.classList.add('active');
      } else {
        b.classList.remove('active');
      }
    });
  } else {
    document.querySelectorAll('.btn-game').forEach(b => b.classList.remove('active'));
  }

  // Состояние демо-режима для игр
  if (data.gameDemo !== undefined) {
    isGameDemo = data.gameDemo;
    updateGameDemoUi();
  }

  // Подсветка карточки текущего эффекта
  if (data.mode !== undefined && !data.specialMode && !data.gamemode) {
    document.querySelectorAll('.effect-card').forEach(card => {
      if (parseInt(card.dataset.id) === data.mode) {
        card.classList.add('active');
      } else {
        card.classList.remove('active');
      }
    });
  }
}

// ------------------------------------------------------------------------------
// Сканер локальной сети
// ------------------------------------------------------------------------------
async function startIpScan() {
  if (isScanning) return;
  isScanning = true;

  const btn = document.getElementById('btn-scan');
  const desc = document.getElementById('discovery-desc');
  const progWrap = document.getElementById('scan-progress-wrap');
  const prog = document.getElementById('scan-progress');

  if (btn) { btn.disabled = true; btn.textContent = 'Поиск...'; }
  if (progWrap) progWrap.style.display = 'block';
  if (desc) desc.textContent = 'Проверяем адрес 192.168.0.103...';

  // 1. Проверяем сохраненный IP
  try {
    const res = await fetchWithTimeout('http://192.168.0.103/api/state', { method: 'GET' }, 1200);
    if (res.ok) {
      foundMatrix('http://192.168.0.103');
      return;
    }
  } catch (e) {}

  // 2. Сканируем 192.168.0.X и 192.168.1.X
  const subnets = ['192.168.0', '192.168.1'];
  let total = subnets.length * 254;
  let scanned = 0;

  for (const subnet of subnets) {
    if (desc) desc.textContent = `Сканируем сеть ${subnet}.X...`;
    const batchSize = 25;
    for (let i = 1; i <= 254; i += batchSize) {
      if (!isScanning) return;
      const promises = [];
      for (let j = i; j < i + batchSize && j <= 254; j++) {
        const targetIp = `http://${subnet}.${j}`;
        promises.push(
          fetchWithTimeout(`${targetIp}/api/state`, { method: 'GET' }, 650)
            .then(res => { if (res.ok) return targetIp; throw new Error(); })
            .catch(() => null)
        );
      }
      const results = await Promise.all(promises);
      scanned += promises.length;
      if (prog) prog.style.width = Math.round((scanned / total) * 100) + '%';
      const found = results.find(r => r !== null);
      if (found) {
        foundMatrix(found);
        return;
      }
    }
  }

  if (desc) desc.textContent = 'Матрица не найдена. Укажите IP вручную.';
  if (btn) { btn.disabled = false; btn.textContent = 'Найти в сети'; }
  if (progWrap) progWrap.style.display = 'none';
  isScanning = false;
}

function foundMatrix(host) {
  matrixHost = host;
  localStorage.setItem('gyver_matrix_host', host);
  updateHostDisplay();
  setConnected(true);

  const desc = document.getElementById('discovery-desc');
  if (desc) desc.textContent = `Найдено устройство: ${host}!`;

  const btn = document.getElementById('btn-scan');
  if (btn) { btn.disabled = false; btn.textContent = 'Найдено!'; }
  const progWrap = document.getElementById('scan-progress-wrap');
  if (progWrap) progWrap.style.display = 'none';
  isScanning = false;

  checkConnection();
}

function promptManualIp() {
  const ip = prompt('Введите IP адрес матрицы (например 192.168.0.103):', matrixHost.replace('http://', ''));
  if (ip && ip.trim()) {
    let clean = ip.trim();
    if (!clean.startsWith('http')) clean = 'http://' + clean;
    matrixHost = clean;
    localStorage.setItem('gyver_matrix_host', matrixHost);
    updateHostDisplay();
    checkConnection();
  }
}

function saveManualHost() {
  const input = document.getElementById('manual-host-input');
  if (!input) return;
  const val = input.value.trim();
  if (val) {
    let clean = val;
    if (!clean.startsWith('http')) clean = 'http://' + clean;
    matrixHost = clean;
    localStorage.setItem('gyver_matrix_host', matrixHost);
    updateHostDisplay();
    checkConnection();
    alert('Адрес сохранен: ' + matrixHost);
  }
}

function openIpModal() {
  switchTab('settings', document.querySelectorAll('.nav-btn')[5]);
}

// ------------------------------------------------------------------------------
// Управление эффектами и быстрые режимы
// ------------------------------------------------------------------------------
function renderEffects(filter = 'all') {
  const container = document.getElementById('effects-container');
  if (!container) return;
  container.innerHTML = '';

  const filtered = filter === 'all' ? ALL_EFFECTS : ALL_EFFECTS.filter(e => e.cat === filter);

  filtered.forEach(eff => {
    const card = document.createElement('div');
    card.className = `effect-card cat-${eff.cat}`;
    card.dataset.id = eff.id;
    card.textContent = eff.name;
    card.onclick = () => {
      document.querySelectorAll('.effect-card').forEach(c => c.classList.remove('active'));
      card.classList.add('active');
      setMode(eff.id);
    };
    container.appendChild(card);
  });
}

function filterEffects(cat, el) {
  document.querySelectorAll('.chip').forEach(c => c.classList.remove('active'));
  el.classList.add('active');
  renderEffects(cat);
}

function setMode(id) {
  sendApi('/api/mode', { id: id });
}

function togglePower() {
  isPowerOn = !isPowerOn;
  const btn = document.getElementById('btn-power');
  if (btn) {
    btn.textContent = isPowerOn ? 'Выключить' : 'Включить';
    btn.className = isPowerOn ? 'btn btn-danger' : 'btn btn-primary';
  }
  sendApi('/api/power', { state: isPowerOn ? 1 : 0 });
}

// Регулировка яркости с дебаунсом
function setBrightness(v) {
  lastUserInteraction = Date.now();
  const elVal = document.getElementById('val-bright');
  if (elVal) elVal.textContent = v;

  if (brightTimeout) clearTimeout(brightTimeout);
  brightTimeout = setTimeout(() => {
    sendApi('/api/brightness', { val: v });
  }, 70);
}

// ПРАВИЛЬНАЯ РЕГУЛИРОВКА СКОРОСТИ (Инвертированная: 1% = медленно, 100% = быстро)
function setSpeed(percent) {
  lastUserInteraction = Date.now();
  const elVal = document.getElementById('val-speed');
  if (elVal) elVal.textContent = percent + '%';

  // Конвертируем проценты (1..100%) в задержку таймера в миллисекундах (255..15мс)
  const delayMs = Math.round(255 - ((percent - 1) / 99) * (255 - 15));
  const clamped = Math.max(10, Math.min(255, delayMs));

  if (speedTimeout) clearTimeout(speedTimeout);
  speedTimeout = setTimeout(() => {
    sendApi('/api/speed', { val: clamped });
  }, 70);
}

// Быстрые спец-режимы
function sendSpecial(id) {
  document.querySelectorAll('.btn-preset').forEach(b => b.classList.remove('active'));
  if (id === 2) document.getElementById('preset-lamp')?.classList.add('active');
  else if (id === 1) document.getElementById('preset-clock')?.classList.add('active');
  else if (id === 5) document.getElementById('preset-fire')?.classList.add('active');
  else if (id === 4) document.getElementById('preset-night')?.classList.add('active');

  sendApi('/api/special', { id: id });
}

function stepMode(dir) {
  sendApi('/api/step', { dir: dir });
}

function toggleAutoplay() {
  isAutoplay = !isAutoplay;
  const btn = document.getElementById('btn-autoplay');
  if (btn) {
    btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
    btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
  }
  sendApi('/api/autoplay', { val: isAutoplay ? 1 : 0 });
}

// ------------------------------------------------------------------------------
// Бегущая строка (выбор градиента любого цвета)
// ------------------------------------------------------------------------------
function onTextColorInput(hex) {
  textMode = 'color';
  document.getElementById('text-color-hex').textContent = hex.toUpperCase();
  const sample = document.getElementById('text-preview-sample');
  if (sample) {
    sample.style.color = hex;
    sample.style.background = 'none';
    sample.style.webkitBackgroundClip = 'unset';
    sample.textContent = 'Пример текста';
  }
  document.querySelectorAll('.mode-chip').forEach(c => c.classList.remove('active'));
}

function setTextMode(mode, el, color) {
  textMode = mode;
  document.querySelectorAll('.mode-chip').forEach(c => c.classList.remove('active'));
  if (el) el.classList.add('active');

  const sample = document.getElementById('text-preview-sample');
  const picker = document.getElementById('text-color-picker');
  const hexDisplay = document.getElementById('text-color-hex');

  if (mode === 'rainbow') {
    if (sample) {
      sample.style.background = 'linear-gradient(90deg, #ff0000, #ff7700, #ffff00, #00ff00, #00ffff, #0000ff, #aa00ff)';
      sample.style.webkitBackgroundClip = 'text';
      sample.style.webkitTextFillColor = 'transparent';
      sample.textContent = '🌈 Радужный текст';
    }
  } else if (mode === 'random') {
    if (sample) {
      sample.style.background = 'linear-gradient(90deg, #ff00ff, #00ffff, #ffff00)';
      sample.style.webkitBackgroundClip = 'text';
      sample.style.webkitTextFillColor = 'transparent';
      sample.textContent = '🎨 Разноцветные буквы';
    }
  } else if (mode === 'color') {
    if (color) {
      picker.value = color;
      hexDisplay.textContent = color.toUpperCase();
    }
    if (sample) {
      sample.style.background = 'none';
      sample.style.webkitBackgroundClip = 'unset';
      sample.style.webkitTextFillColor = picker.value;
      sample.textContent = 'Пример текста';
    }
  }
}

function sendRunningText() {
  const txt = document.getElementById('run-text').value.trim();
  if (!txt) {
    alert('Пожалуйста, введите текст для бегущей строки!');
    return;
  }

  let colParam = '0xffffff';
  if (textMode === 'rainbow') {
    colParam = '1';
  } else if (textMode === 'random') {
    colParam = '2';
  } else {
    const hex = document.getElementById('text-color-picker').value;
    colParam = hex.replace('#', '0x').toUpperCase();
  }

  sendApi('/api/text', { msg: txt, color: colParam });
}

// ------------------------------------------------------------------------------
// Игры и Демо-режим (Авто-игра)
// ------------------------------------------------------------------------------
function startSelectedGame(gameIndex, modeId) {
  document.querySelectorAll('.btn-game').forEach(b => {
    b.classList.toggle('active', b.dataset.game == gameIndex);
  });
  sendApi('/api/game', { action: 'start', game: gameIndex, demo: isGameDemo ? 1 : 0 });
}

function toggleGameDemo() {
  isGameDemo = !isGameDemo;
  updateGameDemoUi();
  sendApi('/api/game', { action: 'demo', val: isGameDemo ? 1 : 0 });
}

function updateGameDemoUi() {
  const btn = document.getElementById('btn-game-demo');
  const hint = document.getElementById('game-mode-hint');
  if (btn) {
    if (isGameDemo) {
      btn.textContent = '🤖 Авто-режим игры: ВКЛ (Бот играет сам)';
      btn.className = 'btn btn-primary';
      if (hint) hint.textContent = 'Бот играет сам на фоне. Нажмите стрелку геймпада для ручного управления.';
    } else {
      btn.textContent = '🎮 Ручной режим (Управление с геймпада)';
      btn.className = 'btn btn-secondary';
      if (hint) hint.textContent = 'Вы управляете игрой! Нажимайте стрелки ниже.';
    }
  }
}

function sendGameDpad(btnCode) {
  // Нажатие кнопки переводит в ручной режим
  if (isGameDemo) {
    isGameDemo = false;
    updateGameDemoUi();
  }
  sendApi('/api/game', { action: 'button', btn: btnCode });
}

// ------------------------------------------------------------------------------
// Холст 15x16 (Рисование)
// ------------------------------------------------------------------------------
function initCanvas() {
  const canvasEl = document.getElementById('matrix-canvas');
  if (!canvasEl) return;
  canvasEl.innerHTML = '';

  for (let y = 15; y >= 0; y--) {
    for (let x = 0; x < 15; x++) {
      const px = document.createElement('div');
      px.className = 'pixel';
      px.dataset.x = x;
      px.dataset.y = y;
      px.addEventListener('pointerdown', () => { isDrawing = true; paintPixel(px); });
      px.addEventListener('pointerenter', () => { if (isDrawing) paintPixel(px); });
      canvasEl.appendChild(px);
    }
  }
  window.addEventListener('pointerup', () => isDrawing = false);
}

function setColor(c, el) {
  curColor = c.toUpperCase();
  document.querySelectorAll('.color-dot').forEach(d => d.classList.remove('active'));
  if (el) el.classList.add('active');
  const customInput = document.getElementById('custom-draw-color');
  const customHex = document.getElementById('custom-draw-hex');
  if (customInput) customInput.value = c;
  if (customHex) customHex.textContent = curColor;
}

function setCustomDrawColor(val) {
  curColor = val.toUpperCase();
  document.querySelectorAll('.color-dot').forEach(d => d.classList.remove('active'));
  const customHex = document.getElementById('custom-draw-hex');
  if (customHex) customHex.textContent = curColor;
}

function paintPixel(px) {
  px.style.background = curColor;
  sendApi('/api/draw', {
    x: px.dataset.x,
    y: px.dataset.y,
    color: curColor.replace('#', '0x')
  });
}

function clearCanvas() {
  document.querySelectorAll('#matrix-canvas .pixel').forEach(p => p.style.background = '#181a20');
  sendApi('/api/clear');
}

function fillCanvas() {
  document.querySelectorAll('#matrix-canvas .pixel').forEach(p => p.style.background = curColor);
  sendApi('/api/fill', { color: curColor.replace('#', '0x') });
}

// ------------------------------------------------------------------------------
// Забыть сеть Wi-Fi
// ------------------------------------------------------------------------------
async function forgetMatrixWifi() {
  const ok = confirm(
    'Забыть сохраненную сеть Wi-Fi на матрице?\n\n' +
    'Плата сотрет данные сети, перезагрузится и запустит точку доступа MatrixAP (пароль: 12341234).\n' +
    'После этого вы сможете подключить ее к новому роутеру по адресу http://192.168.4.1.'
  );
  if (!ok) return;

  try {
    await sendApi('/api/forget_wifi');
  } catch (e) {}

  setConnected(false);
  alert('Команда отправлена! Плата перезагружается в режиме MatrixAP (192.168.4.1).');
}

// ------------------------------------------------------------------------------
// Навигация по вкладкам
// ------------------------------------------------------------------------------
function switchTab(tabId, btn) {
  document.querySelectorAll('.tab-pane').forEach(el => el.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(el => el.classList.remove('active'));

  const tab = document.getElementById('tab-' + tabId);
  if (tab) tab.classList.add('active');
  if (btn) btn.classList.add('active');
}
