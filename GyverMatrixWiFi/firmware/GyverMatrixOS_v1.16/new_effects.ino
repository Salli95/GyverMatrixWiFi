// ==============================================================================
// 20 Новых тематических эффектов (Раздел "New / Новинки") для GyverMatrix (15x16)
// Кино, Хоррор, Ретро-игры, Космос, Природа и Неон
// ==============================================================================

// ------------------------------------------------------------------------------
// 50. 📺 «Звонок» (The Ring)
// Аналоговый ТВ-шум, проступающий каменный колодец и мерцание VHS-помех
// ------------------------------------------------------------------------------
void theRingRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Аналоговый серый ТВ-статический шум
  for (byte i = 0; i < NUM_LEDS; i++) {
    uint8_t n = random8(12, 45);
    leds[i] = CRGB(n, n, n + (n / 3)); // Легкий синеватый оттенок кинескопа
  }

  // Силуэт колодца по центру (X: 4..10, Y: 3..9)
  uint8_t pulse = inoise8(millis() / 8, 400);
  CRGB stoneCol = CRGB(10, 10, 18);
  CRGB waterGlow = CHSV(130, 255, map(pulse, 0, 255, 60, 240)); // Холодное свечение колодца

  // Каменная кладка колодца
  for (int8_t x = 4; x <= 10; x++) {
    drawPixelXY(x, 3, stoneCol);
    drawPixelXY(x, 4, stoneCol);
    drawPixelXY(x, 8, stoneCol);
  }
  drawPixelXY(4, 5, stoneCol); drawPixelXY(4, 6, stoneCol); drawPixelXY(4, 7, stoneCol);
  drawPixelXY(10, 5, stoneCol); drawPixelXY(10, 6, stoneCol); drawPixelXY(10, 7, stoneCol);

  // Свечение воды внутри жерла
  for (int8_t x = 5; x <= 9; x++) {
    for (int8_t y = 5; y <= 7; y++) {
      drawPixelXY(x, y, waterGlow);
    }
  }

  // Случайный горизонтальный глитч кинескопа
  if (random8() < 25) {
    byte glitchY = random8(HEIGHT);
    for (byte x = 0; x < WIDTH; x++) {
      drawPixelXY(x, glitchY, CRGB(180, 220, 255));
    }
  }
}

// ------------------------------------------------------------------------------
// 51. 🌌 «Очень странные дела» (Stranger Things / Изнанка и Споры)
// Темный мир Upside Down, левитирующие пепельные споры и красные разломы Векны
// ------------------------------------------------------------------------------
struct Spore { float x, y, vy; uint8_t phase; };
static Spore st_spores[8];
static bool st_init = false;

void strangerThingsRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
    st_init = false;
  }

  if (!st_init) {
    for (byte i = 0; i < 8; i++) {
      st_spores[i] = { (float)random8(WIDTH), (float)random8(HEIGHT), 0.08f + (random8(10) / 100.0f), (uint8_t)random8(255) };
    }
    st_init = true;
  }

  // Мрачный темный сине-фиолетовый фон
  for (byte i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(3, 1, 9);

  // Боковые красные разломы Векны (живые пульсирующие трещины)
  uint8_t riftPulse = sin8(millis() / 5);
  for (byte y = 0; y < HEIGHT; y++) {
    uint8_t rw = inoise8(y * 45, millis() / 10);
    if (rw > 110) drawPixelXY(0, y, CHSV(0, 255, map(riftPulse, 0, 255, 120, 255)));
    if (rw > 150) drawPixelXY(1, y, CHSV(350, 255, riftPulse / 2));
    if (rw < 90)  drawPixelXY(WIDTH - 1, y, CHSV(0, 255, map(riftPulse, 0, 255, 100, 240)));
  }

  // Парящие пепельные споры
  for (byte i = 0; i < 8; i++) {
    st_spores[i].y -= st_spores[i].vy;
    st_spores[i].phase += 4;
    float swayX = st_spores[i].x + (sin8(st_spores[i].phase) - 128) / 120.0f;
    if (st_spores[i].y < 0) {
      st_spores[i].y = HEIGHT - 1;
      st_spores[i].x = random8(WIDTH);
    }
    drawPixelXY((int)swayX, (int)st_spores[i].y, CRGB(130, 180, 220));
  }
}

// ------------------------------------------------------------------------------
// 52. 🚫 «Охотники за привидениями» (Ghostbusters / Лизун & Протонный луч)
// Кислотно-зеленый призрак и пересекающиеся протонные лучи с искрами
// ------------------------------------------------------------------------------
void ghostbustersRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // Позиция Лизуна (восьмерка по центру)
  int8_t gx = 7 + (sin8(millis() / 8) - 128) / 36;
  int8_t gy = 8 + (cos8(millis() / 9) - 128) / 45;

  // Рисуем Лизуна (кислотно-зеленый купол)
  CRGB slimer = CRGB(30, 255, 30);
  for (int8_t dx = -1; dx <= 1; dx++) {
    drawPixelXY(gx + dx, gy + 1, slimer);
    drawPixelXY(gx + dx, gy, slimer);
  }
  drawPixelXY(gx, gy + 2, slimer);
  drawPixelXY(gx - 1, gy - 1, slimer);
  drawPixelXY(gx + 1, gy - 1, slimer);
  // Глаза призрака (желтые)
  drawPixelXY(gx - 1, gy + 1, CRGB(255, 255, 0));
  drawPixelXY(gx + 1, gy + 1, CRGB(255, 255, 0));

  // Оранжевый и лазурный протонные лучи из нижних углов
  byte beamSteps = 9;
  for (byte step = 0; step < beamSteps; step++) {
    float t = step / (float)beamSteps;
    int bx1 = (int)(0 + t * (gx - 0)) + (random8(3) - 1);
    int by1 = (int)(0 + t * (gy - 0));
    drawPixelXY(bx1, by1, CRGB(255, 120, 0)); // Оранжевый луч

    int bx2 = (int)((WIDTH - 1) + t * (gx - (WIDTH - 1))) + (random8(3) - 1);
    int by2 = (int)(0 + t * (gy - 0));
    drawPixelXY(bx2, by2, CRGB(0, 190, 255)); // Лазурный луч
  }
  // Электрические искры вокруг призрака
  drawPixelXY(gx + random8(3) - 1, gy + random8(3) - 1, CRGB(255, 255, 255));
}

// ------------------------------------------------------------------------------
// 53. ☕️ «Твин Пикс» (Twin Peaks / Черный Вигвам)
// Шевронный пол с зигзагом и колышущиеся красные бархатные шторы
// ------------------------------------------------------------------------------
void twinPeaksRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Верх: бархатные красные портьеры с волнами складок (Y: 5..15)
  uint8_t curtainWave = millis() / 7;
  for (byte x = 0; x < WIDTH; x++) {
    uint8_t fold = sin8(x * 38 + curtainWave);
    CRGB redVelvet = CHSV(0, 255, map(fold, 0, 255, 70, 235));
    for (byte y = 5; y < HEIGHT; y++) {
      drawPixelXY(x, y, redVelvet);
    }
  }

  // Низ: черно-белый шевронный пол Черного Вигвама (Y: 0..4)
  static uint8_t chevronOffset = 0;
  chevronOffset++;
  for (byte y = 0; y <= 4; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      // Зигзаг: инверсия угла в центре
      int pattern = (x < 7) ? (x + y + (chevronOffset / 4)) : ((WIDTH - 1 - x) + y + (chevronOffset / 4));
      bool isWhite = (pattern % 4) < 2;
      drawPixelXY(x, y, isWhite ? CRGB(210, 210, 210) : CRGB(10, 10, 10));
    }
  }
}

// ------------------------------------------------------------------------------
// 54. 🏙 «Бегущий по лезвию» (Blade Runner / Cyberpunk City)
// Небоскребы ночного мегаполиса, светящиеся окна и неоновый кислотный дождь
// ------------------------------------------------------------------------------
static const uint8_t bld_heights[15] = { 8, 8, 12, 12, 6, 6, 14, 14, 14, 9, 9, 13, 13, 7, 7 };

void bladeRunnerRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Небо: глубокий темный сине-фиолетовый
  for (byte i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(2, 2, 8);

  // Небоскребы
  for (byte x = 0; x < WIDTH; x++) {
    uint8_t h = bld_heights[x];
    for (byte y = 0; y <= h; y++) {
      CRGB bldCol = CRGB(8, 8, 16);
      // Окна в зданиях
      if (y > 1 && y < h && (y % 2 == 0) && (x % 2 == 1)) {
        uint8_t winNoise = inoise8(x * 100, y * 100 + (millis() / 500));
        if (winNoise > 140) bldCol = (x > 7) ? CRGB(255, 180, 40) : CRGB(0, 220, 255);
      }
      drawPixelXY(x, y, bldCol);
    }
  }

  // Неоновая реклама на высотке в центре (X: 6..8, Y: 14)
  CRGB neonSign = ((millis() / 250) % 2 == 0) ? CRGB(255, 20, 140) : CRGB(180, 0, 90);
  drawPixelXY(7, 14, neonSign);
  drawPixelXY(6, 14, neonSign);
  drawPixelXY(8, 14, neonSign);

  // Падающие неоновые капли дождя
  for (byte r = 0; r < 4; r++) {
    byte rx = random8(WIDTH);
    byte ry = random8(HEIGHT);
    drawPixelXY(rx, ry, CRGB(100, 180, 255));
  }
}

// ------------------------------------------------------------------------------
// 55. 🟡 «Пакман» (Pac-Man & Blinky)
// Бегущий Пакман с раскрывающимся ртом, поедающий светящиеся точки, и призрак
// ------------------------------------------------------------------------------
void pacmanRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // Синие стены лабиринта сверху и снизу
  for (byte x = 0; x < WIDTH; x++) {
    drawPixelXY(x, 2, CRGB(0, 40, 200));
    drawPixelXY(x, 13, CRGB(0, 40, 200));
  }

  // Точки еды в коридоре (Y: 7)
  for (byte x = 1; x < WIDTH; x += 3) {
    drawPixelXY(x, 7, CRGB(255, 210, 120));
  }

  // Движение Пакмана по строке
  int8_t px = (millis() / 90) % (WIDTH + 10) - 4;
  bool mouthOpen = ((millis() / 150) % 2) == 0;

  // Рисуем Пакмана (3x3 желтый)
  CRGB pacCol = CRGB(255, 230, 0);
  for (int8_t dy = -1; dy <= 1; dy++) {
    for (int8_t dx = -1; dx <= 1; dx++) {
      if (mouthOpen && dx == 1 && dy == 0) continue; // Открытый рот
      drawPixelXY(px + dx, 7 + dy, pacCol);
    }
  }

  // Красный призрак Блинки бежит сзади
  int8_t bx = px - 6;
  CRGB ghostCol = CRGB(255, 20, 20);
  for (int8_t dy = -1; dy <= 1; dy++) {
    for (int8_t dx = -1; dx <= 1; dx++) {
      drawPixelXY(bx + dx, 7 + dy, ghostCol);
    }
  }
  // Синие глаза призрака
  drawPixelXY(bx, 8, CRGB(255, 255, 255));
  drawPixelXY(bx + 1, 8, CRGB(0, 100, 255));
}

// ------------------------------------------------------------------------------
// 56. 👾 «Космические захватчики» (Space Invaders)
// Марширующий строй 8-битных пришельцев и лазерная пушка
// ------------------------------------------------------------------------------
void spaceInvadersRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  int8_t marchOffset = (sin8(millis() / 15) - 128) / 45;
  bool frame = ((millis() / 350) % 2) == 0;
  CRGB invaderCol = CRGB(0, 255, 80);

  // Два пришельца 3x3 на верхних строках
  int8_t invX[2] = { (int8_t)(3 + marchOffset), (int8_t)(9 + marchOffset) };
  for (byte i = 0; i < 2; i++) {
    int8_t ix = invX[i];
    int8_t iy = 11;
    // Голова и тело
    drawPixelXY(ix, iy + 1, invaderCol);
    drawPixelXY(ix - 1, iy, invaderCol);
    drawPixelXY(ix, iy, invaderCol);
    drawPixelXY(ix + 1, iy, invaderCol);
    // Шевелящиеся щупальца
    if (frame) {
      drawPixelXY(ix - 1, iy - 1, invaderCol);
      drawPixelXY(ix + 1, iy - 1, invaderCol);
    } else {
      drawPixelXY(ix, iy - 1, invaderCol);
      drawPixelXY(ix - 1, iy + 2, invaderCol);
      drawPixelXY(ix + 1, iy + 2, invaderCol);
    }
  }

  // Лазерная пушка внизу (Y: 1)
  int8_t cannonX = 7 + (sin8(millis() / 10) - 128) / 36;
  drawPixelXY(cannonX, 1, CRGB(0, 180, 255));
  drawPixelXY(cannonX - 1, 0, CRGB(0, 140, 220));
  drawPixelXY(cannonX, 0, CRGB(0, 180, 255));
  drawPixelXY(cannonX + 1, 0, CRGB(0, 140, 220));

  // Выстрел лазером вверх
  int8_t laserY = (millis() / 40) % HEIGHT;
  drawPixelXY(cannonX, laserY, CRGB(255, 50, 50));
}

// ------------------------------------------------------------------------------
// 57. 🍄 «Марио: Огненный шар» (Mario Fireball)
// Прыгающий фаербол Марио с искрами и кирпичный блок
// ------------------------------------------------------------------------------
void marioFireRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // Кирпичный пол снизу (Y: 0..1)
  for (byte x = 0; x < WIDTH; x++) {
    drawPixelXY(x, 0, CRGB(180, 60, 20));
    drawPixelXY(x, 1, (x % 3 == 0) ? CRGB(60, 20, 5) : CRGB(200, 70, 25));
  }

  // Блок '?' в воздухе (X: 6..8, Y: 10..12)
  for (int8_t bx = 6; bx <= 8; bx++) {
    for (int8_t by = 10; by <= 12; by++) {
      drawPixelXY(bx, by, CRGB(220, 150, 20));
    }
  }
  // Мигающий символ вопроса
  CRGB qCol = ((millis() / 200) % 2) ? CRGB(255, 255, 255) : CRGB(255, 210, 60);
  drawPixelXY(7, 11, qCol);

  // Параболический прыгающий огненный шар
  int8_t fx = (millis() / 70) % WIDTH;
  // Парабола отскока: abs(sin)
  int8_t fy = 2 + (abs(sin8(millis() / 4) - 128) * 8) / 128;

  // Огненное ядро и хвост
  drawPixelXY(fx, fy, CRGB(255, 255, 220));
  drawPixelXY(fx, fy + 1, CRGB(255, 120, 0));
  drawPixelXY(fx - 1, fy, CRGB(255, 50, 0));
  drawPixelXY(fx, fy - 1, CRGB(220, 30, 0));
}

// ------------------------------------------------------------------------------
// 58. 🌀 «Соник: Кольца» (Sonic Ring Rush)
// Вращающееся золотое кольцо с 3D-бликом и синий скоростной росчерк
// ------------------------------------------------------------------------------
void sonicRingsRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // 3D-вращение кольца: ширина кольца плавно меняется от 1 до 5 пикселей
  uint8_t spinPhase = millis() / 6;
  int8_t ringW = (abs(sin8(spinPhase) - 128) * 5) / 128;
  if (ringW < 1) ringW = 1;

  int8_t cx = 7, cy = 8;
  CRGB goldBright = CRGB(255, 220, 30);
  CRGB goldShadow = CRGB(180, 120, 10);

  // Рисуем эллиптическое кольцо в 3D
  for (int8_t dy = -3; dy <= 3; dy++) {
    if (abs(dy) == 3) {
      for (int8_t dx = -ringW / 2; dx <= ringW / 2; dx++) drawPixelXY(cx + dx, cy + dy, goldBright);
    } else if (abs(dy) == 2) {
      drawPixelXY(cx - ringW, cy + dy, goldBright);
      drawPixelXY(cx + ringW, cy + dy, goldShadow);
    } else {
      drawPixelXY(cx - ringW, cy + dy, goldBright);
      drawPixelXY(cx + ringW, cy + dy, goldShadow);
    }
  }

  // Синий скоростной росчерк Соника, проносящийся по нижней части (Y: 3)
  int8_t sonicX = (millis() / 25) % (WIDTH + 14) - 7;
  for (int8_t trail = 0; trail < 6; trail++) {
    int8_t tx = sonicX - trail;
    if (tx >= 0 && tx < WIDTH) {
      drawPixelXY(tx, 3, CHSV(160, 255, 255 - trail * 40));
    }
  }
}

// ------------------------------------------------------------------------------
// 59. ⚡️ «Покебол» (Pokéball Charge)
// Культовый покебол с пульсирующей кнопкой и всполохами поимки
// ------------------------------------------------------------------------------
void pokeballRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  int8_t cx = 7, cy = 8;
  int8_t r = 5;

  // Рисуем круг покебола
  for (int8_t y = cy - r; y <= cy + r; y++) {
    for (int8_t x = cx - r; x <= cx + r; x++) {
      int dist2 = (x - cx) * (x - cx) + (y - cy) * (y - cy);
      if (dist2 <= r * r) {
        if (y > cy) {
          drawPixelXY(x, y, CRGB(255, 0, 0)); // Верхняя красная половина
        } else if (y < cy) {
          drawPixelXY(x, y, CRGB(220, 220, 220)); // Нижняя белая половина
        } else {
          drawPixelXY(x, y, CRGB(15, 15, 15)); // Черная разделительная линия
        }
      }
    }
  }

  // Центральная светящаяся кнопка
  uint8_t btnGlow = sin8(millis() / 4);
  CRGB btnCol = CHSV(130, 200, map(btnGlow, 0, 255, 120, 255));
  drawPixelXY(cx, cy, btnCol);
  drawPixelXY(cx - 1, cy, CRGB(30, 30, 30));
  drawPixelXY(cx + 1, cy, CRGB(30, 30, 30));
}

// ------------------------------------------------------------------------------
// 60. 🕳 «Черная дыра» (Gargantua / Black Hole)
// Горизонт событий в центре и гравитационно искривленный аккреционный диск
// ------------------------------------------------------------------------------
void blackHoleRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Вращающийся аккреционный диск плазмы
  uint8_t angle = millis() / 8;
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      float dx = x - 7.0f;
      float dy = (y - 7.5f) * 1.8f; // Сжатие по вертикали для перспективы диска
      float dist = sqrt(dx * dx + dy * dy);

      if (dist < 2.5f) {
        drawPixelXY(x, y, CRGB(0, 0, 0)); // Абсолютный горизонт событий
      } else if (dist < 7.5f) {
        // Завихрения света в аккреционном диске
        uint8_t noise = inoise8(x * 40 + angle, y * 40, dist * 30);
        // Эффект Доплера: левая сторона ярче и горячее
        uint8_t doppler = (x < 7) ? 255 : 160;
        CRGB plasmaCol = CHSV(map(dist, 2.5f, 7.5f, 15, 38), 255, map(noise, 0, 255, 100, doppler));
        drawPixelXY(x, y, plasmaCol);
      } else {
        drawPixelXY(x, y, CRGB(1, 1, 4));
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 61. ✨ «Гиперпространство» (Hyperdrive Warp Jump)
// Звезды, вытягивающиеся в радиальные световые гиперпространственные полосы
// ------------------------------------------------------------------------------
struct StarWarp { float x, y, z; };
static StarWarp warp_stars[16];
static bool warp_init = false;

void hyperdriveRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
    warp_init = false;
  }
  FastLED.clear();

  if (!warp_init) {
    for (byte i = 0; i < 16; i++) {
      warp_stars[i] = { (float)(random8(120) - 60), (float)(random8(120) - 60), (float)random8(1, 120) };
    }
    warp_init = true;
  }

  for (byte i = 0; i < 16; i++) {
    warp_stars[i].z -= 3.2f; // Полет сквозь пространство
    if (warp_stars[i].z <= 1.0f) {
      warp_stars[i] = { (float)(random8(120) - 60), (float)(random8(120) - 60), 120.0f };
    }

    // Перспективная проекция
    float k = 8.0f / warp_stars[i].z;
    int px = 7 + (int)(warp_stars[i].x * k);
    int py = 8 + (int)(warp_stars[i].y * k);

    // Хвост луча гиперпространства
    float kPrev = 8.0f / (warp_stars[i].z + 6.0f);
    int pxPrev = 7 + (int)(warp_stars[i].x * kPrev);
    int pyPrev = 8 + (int)(warp_stars[i].y * kPrev);

    if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
      drawPixelXY(px, py, CRGB(255, 255, 255));
      if (pxPrev >= 0 && pxPrev < WIDTH && pyPrev >= 0 && pyPrev < HEIGHT) {
        drawPixelXY(pxPrev, pyPrev, CRGB(60, 160, 255));
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 62. 🛸 «Похищение НЛО» (UFO Tractor Beam)
// Летающая тарелка, неоновый луч света и левитирующий объект
// ------------------------------------------------------------------------------
void ufoBeamRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // НЛО сверху (X: 4..10, Y: 13..15)
  for (int8_t x = 5; x <= 9; x++) drawPixelXY(x, 15, CRGB(140, 140, 180));
  for (int8_t x = 4; x <= 10; x++) drawPixelXY(x, 14, CRGB(180, 180, 210));
  // Бегущие габаритные огни тарелки
  uint8_t ufoLed = (millis() / 120) % 7 + 4;
  drawPixelXY(ufoLed, 14, CRGB(255, 220, 0));

  // Зеленый конусообразный луч похищения вниз
  uint8_t beamPulse = sin8(millis() / 6);
  CRGB beamCol = CHSV(96, 255, map(beamPulse, 0, 255, 40, 160));
  for (int8_t y = 0; y <= 13; y++) {
    int8_t spread = (13 - y) / 3;
    for (int8_t x = 7 - spread; x <= 7 + spread; x++) {
      drawPixelXY(x, y, beamCol);
    }
  }

  // Левитирующий белый пиксель, поднимающийся по лучу
  int8_t cowY = (millis() / 150) % 13;
  drawPixelXY(7, cowY, CRGB(255, 255, 255));
  drawPixelXY(7, cowY + 1, CRGB(255, 255, 255));
}

// ------------------------------------------------------------------------------
// 63. 🌑 «Солнечное затмение» (Solar Eclipse)
// Темный диск луны и переливающиеся протуберанцы солнечной короны
// ------------------------------------------------------------------------------
void solarEclipseRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  int8_t cx = 7, cy = 8;
  uint8_t angle = millis() / 10;

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      float dist = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
      if (dist < 3.2f) {
        drawPixelXY(x, y, CRGB(0, 0, 0)); // Черный диск луны
      } else if (dist < 7.5f) {
        // Вспышки солнечной короны
        uint8_t flare = inoise8(x * 50 + angle, y * 50, dist * 40);
        CRGB coronaCol = CHSV(28, map(dist, 3.2f, 7.5f, 180, 255), map(flare, 0, 255, 80, 255));
        drawPixelXY(x, y, coronaCol);
      } else {
        drawPixelXY(x, y, CRGB(0, 0, 0));
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 64. 🪼 «Неоновые медузы» (Deep Sea Jellyfish)
// Парящие в бездне биолюминесцентные медузы с волнистыми щупальцами
// ------------------------------------------------------------------------------
void jellyfishRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // Две медузы (циановая и розовая)
  for (byte j = 0; j < 2; j++) {
    int8_t jx = (j == 0) ? 4 : 10;
    int8_t jy = (millis() / (j == 0 ? 90 : 120) + (j * 8)) % (HEIGHT + 6) - 3;
    uint8_t pulse = sin8(millis() / 5 + j * 100);
    int8_t bellW = (pulse > 140) ? 2 : 1; // Пульсация купола

    CRGB jCol = (j == 0) ? CRGB(0, 240, 255) : CRGB(255, 40, 180);

    // Купол медузы
    for (int8_t dx = -bellW; dx <= bellW; dx++) {
      drawPixelXY(jx + dx, jy, jCol);
    }
    drawPixelXY(jx, jy + 1, jCol);

    // Волнистые щупальца
    for (int8_t ty = 1; ty <= 4; ty++) {
      int8_t sway = (sin8(millis() / 7 + ty * 40) - 128) / 90;
      drawPixelXY(jx - 1 + sway, jy - ty, jCol / 2);
      drawPixelXY(jx + 1 + sway, jy - ty, jCol / 2);
    }
  }
}

// ------------------------------------------------------------------------------
// 65. ⚡️ «Гроза и молнии» (Thunderstorm)
// Темное грозовое небо, струи дождя и ослепительные ветвистые молнии
// ------------------------------------------------------------------------------
void thunderstormRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Грозовой фон
  for (byte i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(2, 2, 8);

  // Струи дождя
  for (byte x = 0; x < WIDTH; x++) {
    if (random8() < 50) {
      byte ry = random8(HEIGHT);
      drawPixelXY(x, ry, CRGB(60, 120, 180));
    }
  }

  // Вспышка молнии
  static uint8_t flashCounter = 0;
  if (random8() < 8 && flashCounter == 0) {
    flashCounter = 3; // Длительность вспышки 3 кадра
  }

  if (flashCounter > 0) {
    flashCounter--;
    // Зигзаг молнии от верха до низа
    int8_t lx = random8(4, 11);
    for (int8_t ly = HEIGHT - 1; ly >= 0; ly--) {
      lx += (random8(3) - 1);
      drawPixelXY(lx, ly, CRGB(255, 255, 255));
      drawPixelXY(lx + 1, ly, CRGB(180, 220, 255));
    }
  }
}

// ------------------------------------------------------------------------------
// 66. ⏳ «Песочные часы» (Cyber Hourglass)
// Неоновые песочные часы со ссыпающимися частицами и переворотом
// ------------------------------------------------------------------------------
void hourglassRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  CRGB frameCol = CRGB(140, 60, 255); // Фиолетовый корпус
  CRGB sandCol = CRGB(255, 210, 40);  // Золотистый песок

  // Контур песочных часов (X: 3..11, Y: 1..14)
  for (int8_t x = 3; x <= 11; x++) {
    drawPixelXY(x, 14, frameCol); // Верх
    drawPixelXY(x, 1, frameCol);  // Дно
  }
  // Наклонные стенки
  for (int8_t step = 0; step <= 5; step++) {
    drawPixelXY(3 + step, 14 - step, frameCol);
    drawPixelXY(11 - step, 14 - step, frameCol);
    drawPixelXY(3 + step, 1 + step, frameCol);
    drawPixelXY(11 - step, 1 + step, frameCol);
  }

  // Падающие песчинки через горлышко (X: 7, Y: 6..8)
  drawPixelXY(7, 8, sandCol);
  drawPixelXY(7, 7, sandCol);
  drawPixelXY(7, 6, sandCol);

  // Песок внизу (накапливающийся холмик)
  uint8_t sandHeight = (millis() / 800) % 5;
  for (int8_t h = 0; h <= sandHeight; h++) {
    for (int8_t sx = 7 - h; sx <= 7 + h; sx++) {
      drawPixelXY(sx, 2 + h, sandCol);
    }
  }
}

// ------------------------------------------------------------------------------
// 67. 💧 «Капли на стекле» (Rain on Glass)
// Размытые ночные городские огни и стекающие капли дождя
// ------------------------------------------------------------------------------
void rainGlassRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  // Городские боке-огни на фоне
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      uint8_t b = inoise8(x * 60, y * 60, millis() / 20);
      leds[getPixelNumber(x, y)] = CHSV(x * 12 + y * 4, 180, map(b, 0, 255, 10, 50));
    }
  }

  // Стекающие капли воды
  static int8_t dropsX[4] = { 3, 7, 11, 14 };
  static float dropsY[4] = { 15.0f, 15.0f, 15.0f, 15.0f };
  for (byte i = 0; i < 4; i++) {
    dropsY[i] -= 0.15f + (i * 0.05f);
    if (dropsY[i] < 0) {
      dropsY[i] = 15.0f;
      dropsX[i] = random8(WIDTH);
    }
    int dy = (int)dropsY[i];
    drawPixelXY(dropsX[i], dy, CRGB(200, 240, 255));
    if (dy < HEIGHT - 1) drawPixelXY(dropsX[i], dy + 1, CRGB(80, 140, 200));
  }
}

// ------------------------------------------------------------------------------
// 68. 📊 «Спектральный эквалайзер» (VU Peak Meter)
// Скачущие столбики эквалайзера с пиковыми индикаторами
// ------------------------------------------------------------------------------
void spectrumVuRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // 7 столбиков эквалайзера (ширина 2 пикселя, шаг 2)
  for (byte col = 0; col < 7; col++) {
    int8_t x = col * 2 + 1;
    // Высота столбика
    uint8_t h = inoise8(col * 90, millis() / 6);
    int8_t barH = map(h, 0, 255, 1, HEIGHT - 1);

    for (int8_t y = 0; y <= barH; y++) {
      CRGB c;
      if (y < 7)       c = CRGB(0, 255, 60);   // Зеленый низ
      else if (y < 12) c = CRGB(255, 220, 0);  // Желтая середина
      else             c = CRGB(255, 20, 20);  // Красный пик

      drawPixelXY(x, y, c);
      drawPixelXY(x + 1, y, c);
    }
    // Зависшая пиковая точка
    drawPixelXY(x, barH, CRGB(255, 255, 255));
    drawPixelXY(x + 1, barH, CRGB(255, 255, 255));
  }
}

// ------------------------------------------------------------------------------
// 69. 🪩 «Диско-шар» (Disco Mirror Ball)
// Вращающийся диско-шар и переливающиеся зеркальные блики
// ------------------------------------------------------------------------------
void discoBallRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }
  FastLED.clear();

  // Сам зеркальный шар вверху по центру (X: 6..8, Y: 13..15)
  for (int8_t x = 6; x <= 8; x++) {
    for (int8_t y = 13; y <= 15; y++) {
      drawPixelXY(x, y, CRGB(180, 180, 200));
    }
  }
  drawPixelXY(7, 15, CRGB(255, 255, 255)); // Блик на шаре

  // Разлетающиеся вращающиеся цветные блики по всему танцполу
  uint8_t rot = millis() / 7;
  for (byte i = 0; i < 18; i++) {
    int8_t bx = (sin8(rot + i * 35) * WIDTH) / 256;
    int8_t by = (cos8(rot + i * 28) * 12) / 256;
    CRGB spotCol = CHSV(i * 30 + rot, 240, 255);
    drawPixelXY(bx, by, spotCol);
  }
}
