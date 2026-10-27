// ==============================================================================
// GyverMatrix Cloud Web Control (Vercel Edition)
// Автопоиск матрицы в Wi-Fi сети (IP Scanner) + управление эффектами, текстом и холстом
// ==============================================================================

let matrixHost = localStorage.getItem('gyver_matrix_host') || 'http://192.168.0.103';
let isScanning = false;
let isConnected = false;
let curColor = '#ff0044';
let isDrawing = false;
let isPowerOn = true;
let isAutoplay = true;

// Каталог всех эффектов
const ALL_EFFECTS = [
  // Хэллоуин
  { id: 35, name: '🎃 Тыква Джек', cat: 'halloween' },
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
    setConnected(false);
  }
}

async function sendCmd(cmd) {
  return await sendApi('/api/cmd', { cmd: cmd });
}

async function checkConnection() {
  if (isScanning) return;
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
    setConnected(false);
    return false;
  }
}

function setConnected(online) {
  isConnected = online;
  const ind = document.getElementById('status-ind');
  const txt = document.getElementById('status-text');
  if (online) {
    ind.className = 'status-indicator online';
    txt.textContent = 'Подключено';
    document.getElementById('discovery-banner').style.display = 'none';
  } else {
    ind.className = 'status-indicator offline';
    txt.textContent = 'Офлайн';
    document.getElementById('discovery-banner').style.display = 'flex';
  }
}

function applyState(data) {
  if (data.brightness !== undefined) {
    document.getElementById('val-bright').textContent = data.brightness;
    document.getElementById('slider-bright').value = data.brightness;
  }
  if (data.speed !== undefined) {
    document.getElementById('val-speed').textContent = data.speed;
    document.getElementById('slider-speed').value = data.speed;
  }
  if (data.power !== undefined) {
    isPowerOn = data.power;
    const btn = document.getElementById('btn-power');
    btn.textContent = isPowerOn ? 'Выключить' : 'Включить';
    btn.className = isPowerOn ? 'btn btn-danger' : 'btn btn-primary';
  }
  if (data.autoplay !== undefined) {
    isAutoplay = data.autoplay;
    const btn = document.getElementById('btn-autoplay');
    btn.textContent = isAutoplay ? 'ВКЛ' : 'ВЫКЛ';
    btn.className = isAutoplay ? 'btn btn-primary' : 'btn';
  }
}

// ------------------------------------------------------------------------------
// Автопоиск матрицы в локальной сети (IP Scanner)
// ------------------------------------------------------------------------------
async function startIpScan() {
  if (isScanning) return;
  isScanning = true;

  const btn = document.getElementById('btn-scan');
  const desc = document.getElementById('discovery-desc');
  const progWrap = document.getElementById('scan-progress-wrap');
  const prog = document.getElementById('scan-progress');

  btn.disabled = true;
  btn.textContent = 'Поиск...';
  progWrap.style.display = 'block';
  desc.textContent = 'Проверяем адрес http://matrix.local...';

  // 1. Сначала пробуем mDNS
  try {
    const res = await fetchWithTimeout('http://matrix.local/api/state', { method: 'GET' }, 1200);
    if (res.ok) {
      foundMatrix('http://matrix.local');
      return;
    }
  } catch (e) {}

  // 2. Проверяем режим SoftAP
  try {
    const res = await fetchWithTimeout('http://192.168.4.1/api/state', { method: 'GET' }, 800);
    if (res.ok) {
      foundMatrix('http://192.168.4.1');
      return;
    }
  } catch (e) {}

  // 3. Сканируем популярные домашние подсети: 192.168.1.X и 192.168.0.X
  const subnets = ['192.168.1', '192.168.0'];
  let totalIps = subnets.length * 254;
  let scanned = 0;

  for (const subnet of subnets) {
    desc.textContent = `Сканируем сеть ${subnet}.X...`;

    // Сканируем пачками по 20 IP параллельно для скорости
    const batchSize = 20;
    for (let i = 1; i <= 254; i += batchSize) {
      if (!isScanning) return;

      const promises = [];
      for (let j = i; j < i + batchSize && j <= 254; j++) {
        const ip = `http://${subnet}.${j}`;
        promises.push(
          fetchWithTimeout(`${ip}/api/state`, { method: 'GET' }, 650)
            .then(res => {
              if (res.ok) return ip;
              throw new Error();
            })
            .catch(() => null)
        );
      }

      const results = await Promise.all(promises);
      scanned += promises.length;
      prog.style.width = Math.round((scanned / totalIps) * 100) + '%';

      const found = results.find(r => r !== null);
      if (found) {
        foundMatrix(found);
        return;
      }
    }
  }

  // Если не найдено
  desc.textContent = 'Матрица не найдена. Укажите IP вручную.';
  btn.disabled = false;
  btn.textContent = 'Найти в сети';
  progWrap.style.display = 'none';
  isScanning = false;
}

function foundMatrix(host) {
  matrixHost = host;
  localStorage.setItem('gyver_matrix_host', host);
  updateHostDisplay();
  setConnected(true);

  const desc = document.getElementById('discovery-desc');
  desc.textContent = `Найдено устройство: ${host}!`;

  const btn = document.getElementById('btn-scan');
  btn.disabled = false;
  btn.textContent = 'Найдено!';
  document.getElementById('scan-progress-wrap').style.display = 'none';
  isScanning = false;

  setTimeout(() => {
    document.getElementById('discovery-banner').style.display = 'none';
  }, 2500);
}

function promptManualIp() {
  const ip = prompt('Введите IP адрес или хост матрицы:', matrixHost);
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
  const val = document.getElementById('manual-host-input').value.trim();
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
  container.innerHTML = '';

  const filtered = filter === 'all' ? ALL_EFFECTS : ALL_EFFECTS.filter(e => e.cat === filter);

  filtered.forEach(eff => {
    const card = document.createElement('div');
    card.className = `effect-card cat-${eff.cat}`;
    card.textContent = eff.name;
    card.onclick = (e) => {
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

function sendButton(code) {
  sendCmd('$' + code);
}

// ------------------------------------------------------------------------------
// Холст 16x16 для рисования
// ------------------------------------------------------------------------------
function initCanvas() {
  const canvasEl = document.getElementById('matrix-canvas');
  canvasEl.innerHTML = '';

  for (let y = 15; y >= 0; y--) {
    for (let x = 0; x < 16; x++) {
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
}

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
  document.querySelectorAll('.pixel').forEach(p => p.style.background = '#181a20');
  sendApi('/api/clear');
}

function fillCanvas() {
  document.querySelectorAll('.pixel').forEach(p => p.style.background = curColor);
  sendApi('/api/fill', { color: curColor.replace('#', '0x') });
}

// ------------------------------------------------------------------------------
// Навигация по табам
// ------------------------------------------------------------------------------
function switchTab(tabId, el) {
  document.querySelectorAll('.tab-pane').forEach(p => p.classList.remove('active'));
  document.querySelectorAll('.nav-btn').forEach(b => b.classList.remove('active'));
  document.getElementById('tab-' + tabId).classList.add('active');
  if (el) el.classList.add('active');
}

