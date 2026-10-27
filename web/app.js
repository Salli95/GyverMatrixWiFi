// ==============================================================================
// GyverMatrix Web Control (Pro Edition)
// Управление матрицей по Wi-Fi (автопоиск, эффекты, бегущая строка, рисование, геймпад)
// ==============================================================================

const DEFAULT_IP = 'http://192.168.0.103';
let matrixHost = localStorage.getItem('gyver_matrix_host');

if (!matrixHost || matrixHost === 'http://matrix.local') {
  // По умолчанию отдаем предпочтение проверенному локальному IP
  matrixHost = DEFAULT_IP;
}

let isScanning = false;
let isConnected = false;
let curColor = '#ff0044';
let isDrawing = false;
let isPowerOn = true;
let isAutoplay = true;
let lastUserInteraction = 0;
let brightTimeout = null;
let speedTimeout = null;

// Каталог всех эффектов с их точными ID прошивки GyverMatrixOS v1.16
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

  // Фоновый опрос статуса каждые 4 секунды
  setInterval(checkConnection, 4000);
});

function updateHostDisplay() {
  const el = document.getElementById('cur-host-display');
  if (el) el.textContent = matrixHost;
  const input = document.getElementById('manual-host-input');
  if (input) input.value = matrixHost;
}

// ------------------------------------------------------------------------------
// Сетевые вызовы к ESP8266
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

async function sendApi(endpoint, params = {}) {
  try {
    const base = matrixHost.endsWith('/') ? matrixHost.slice(0, -1) : matrixHost;
    const url = new URL(endpoint, base);
    Object.keys(params).forEach(k => url.searchParams.append(k, params[k]));
    const res = await fetchWithTimeout(url.toString(), { method: 'POST' });
    setConnected(true);
    return await res.json();
  } catch (e) {
    console.warn('API error:', e);
    // Пробуем альтернативный адрес, если текущий недоступен
    tryFallbackHost();
  }
}

async function tryFallbackHost() {
  if (matrixHost !== DEFAULT_IP) {
    try {
      const res = await fetchWithTimeout(`${DEFAULT_IP}/api/state`, { method: 'GET' }, 1000);
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

async function sendCmd(cmd) {
  return await sendApi('/api/cmd', { cmd: cmd });
}

let isChecking = false;
async function checkConnection() {
  if (isScanning || isChecking) return;
  isChecking = true;
  try {
    const base = matrixHost.endsWith('/') ? matrixHost.slice(0, -1) : matrixHost;
    const res = await fetchWithTimeout(`${base}/api/state`, { method: 'GET' }, 2000);
    if (res.ok) {
      const data = await res.json();
      setConnected(true);
      applyState(data);
      return true;
    }
  } catch (e) {
    // Пробуем резервный адрес
    if (matrixHost !== DEFAULT_IP) {
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
  // Не перебиваем слайдеры, если пользователь сам двигал их в последние 3.5 секунды
  const now = Date.now();
  if (now - lastUserInteraction > 3500) {
    if (data.brightness !== undefined) {
      const elVal = document.getElementById('val-bright');
      const elSlider = document.getElementById('slider-bright');
      if (elVal) elVal.textContent = data.brightness;
      if (elSlider) elSlider.value = data.brightness;
    }
    if (data.speed !== undefined) {
      const elVal = document.getElementById('val-speed');
      const elSlider = document.getElementById('slider-speed');
      if (elVal) elVal.textContent = data.speed;
      if (elSlider) elSlider.value = data.speed;
    }
  }

  if (data.power !== undefined) {
    isPowerOn = data.power;
    const btn = document.getElementById('btn-power');
    if (btn) {
      btn.textContent = isPowerOn ? 'Выключить' : 'Включить';
      btn.className = isPowerOn ? 'btn btn-danger' : 'btn btn-primary';
    }
  }

  if (data.autoplay !== undefined) {
    isAutoplay = data.autoplay;
    const btn = document.getElementById('btn-autoplay');
    if (btn) {
      btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
      btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
    }
  }

  // Подсветка карточки текущего режима
  if (data.mode !== undefined) {
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

  // 2. Проверяем mDNS
  try {
    const res = await fetchWithTimeout('http://matrix.local/api/state', { method: 'GET' }, 1200);
    if (res.ok) {
      foundMatrix('http://matrix.local');
      return;
    }
  } catch (e) {}

  // 3. Сканируем 192.168.0.X и 192.168.1.X
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
// Управление эффектами
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

// Плавная регулировка яркости с дебаунсом (без спама на ESP8266)
function setBrightness(v) {
  lastUserInteraction = Date.now();
  const elVal = document.getElementById('val-bright');
  if (elVal) elVal.textContent = v;

  if (brightTimeout) clearTimeout(brightTimeout);
  brightTimeout = setTimeout(() => {
    sendApi('/api/brightness', { val: v });
  }, 70);
}

// Плавная регулировка скорости с дебаунсом (без спама на ESP8266)
function setSpeed(v) {
  lastUserInteraction = Date.now();
  const elVal = document.getElementById('val-speed');
  if (elVal) elVal.textContent = v;

  if (speedTimeout) clearTimeout(speedTimeout);
  speedTimeout = setTimeout(() => {
    sendApi('/api/speed', { val: v });
  }, 70);
}

function toggleAutoplay() {
  isAutoplay = !isAutoplay;
  const btn = document.getElementById('btn-autoplay');
  if (btn) {
    btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
    btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
  }
  sendCmd(isAutoplay ? '$16 0' : '$16 1');
}

function sendRunningText() {
  const txt = document.getElementById('run-text').value.trim();
  const col = document.getElementById('text-color-preset').value;
  if (!txt) return;
  sendApi('/api/text', { msg: txt, color: col });
}

function sendButton(code) {
  sendCmd('$' + code);
}

// ------------------------------------------------------------------------------
// Холст 15x16 для рисования
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
  curColor = c;
  document.querySelectorAll('.color-dot').forEach(d => d.classList.remove('active'));
  el.classList.add('active');
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
  document.querySelectorAll('#matrix-canvas .pixel').forEach(p => p.style.background = '#151515');
  sendApi('/api/clear');
}

function fillCanvas() {
  document.querySelectorAll('#matrix-canvas .pixel').forEach(p => p.style.background = curColor);
  sendApi('/api/fill', { color: curColor.replace('#', '0x') });
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
