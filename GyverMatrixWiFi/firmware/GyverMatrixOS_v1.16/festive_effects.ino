// ==============================================================================
// Праздничный пакет эффектов для GyverMatrix (WS2812B 15x16)
// Хэллоуин: Тыква Джек, Привидения, Глаза во тьме, Паук и паутина, Котел, Капли крови
// Новый Год: Новогодняя елочка, Праздничная гирлянда, Салют, Метель, Северное сияние
// ==============================================================================

// ------------------------------------------------------------------------------
// 1. ХЭЛЛОУИН: Тыква Джек (Jack-o'-lantern)
// Процедурный светильник-тыква с мерцающим свечным пламенем в глазах и пасти
// ------------------------------------------------------------------------------
void pumpkinRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  FastLED.clear();

  // Живое свечение тыквы (эффект пламени свечи внутри)
  uint8_t pulse = inoise8(millis() / 6, 120);
  CRGB orangeBright = CHSV(18, 255, map(pulse, 0, 255, 210, 255)); // Яркий тыквенный
  CRGB orangeShadow = CHSV(14, 255, map(pulse, 0, 255, 140, 180)); // Ребра тыквы

  // Рисуем тело тыквы (15x16), центр X = 7
  for (int8_t y = 2; y <= 11; y++) {
    for (int8_t x = 2; x <= 12; x++) {
      // Скругление углов тыквы
      if ((y == 2 || y == 11) && (x < 4 || x > 10)) continue;
      if ((y == 3 || y == 10) && (x < 3 || x > 11)) continue;

      // Вертикальные сегменты тыквы (ребра)
      bool isRidge = (x == 3 || x == 5 || x == 7 || x == 9 || x == 11);
      drawPixelXY(x, y, isRidge ? orangeShadow : orangeBright);
    }
  }

  // Зеленый хвостик тыквы сверху
  drawPixelXY(7, 12, CRGB(20, 150, 20));
  drawPixelXY(7, 13, CRGB(25, 170, 25));
  drawPixelXY(8, 14, CRGB(15, 130, 15));

  // Черный цвет для максимальной контрастности вырезанных глаз, носа и рта
  CRGB cutBlack = CRGB(0, 0, 0);

  // Вырезанные треугольные глаза (глубокий черный цвет)
  // Левый глаз:
  drawPixelXY(4, 8, cutBlack);
  drawPixelXY(5, 8, cutBlack);
  drawPixelXY(5, 9, cutBlack);
  drawPixelXY(4, 9, cutBlack);

  // Правый глаз (симметрично относительно X=7):
  drawPixelXY(9, 8, cutBlack);
  drawPixelXY(10, 8, cutBlack);
  drawPixelXY(9, 9, cutBlack);
  drawPixelXY(10, 9, cutBlack);

  // Вырезанный треугольный нос по центру (черный)
  drawPixelXY(7, 7, cutBlack);
  drawPixelXY(7, 8, cutBlack);

  // Зловещая зубастая пасть (черные вырезы на оранжевом фоне)
  // Верхние зубы
  drawPixelXY(4, 5, cutBlack);
  drawPixelXY(6, 5, cutBlack);
  drawPixelXY(8, 5, cutBlack);
  drawPixelXY(10, 5, cutBlack);

  // Основная линия оскала
  for (int8_t x = 3; x <= 11; x++) {
    drawPixelXY(x, 4, cutBlack);
  }

  // Нижние зубы
  drawPixelXY(5, 3, cutBlack);
  drawPixelXY(7, 3, cutBlack);
  drawPixelXY(9, 3, cutBlack);
}

// ------------------------------------------------------------------------------
// 2. ХЭЛЛОУИН: Привидения (Floating Ghosts)
// Парящие полупрозрачные силуэты духов с развевающимися хвостами и черными глазами
// ------------------------------------------------------------------------------
struct Ghost {
  float x;
  float y;
  float vx;
  float vy;
  uint8_t phase;
};
Ghost ghosts[2];

void ghostsRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_BALL;
    ghosts[0] = {1.0, 3.0, 0.28, 0.18, 0};
    ghosts[1] = {8.0, 8.0, -0.25, 0.22, 128};
    FastLED.clear();
  }

  // Затухание для создания эфирного следа
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(180);
  }

  for (byte g = 0; g < 2; g++) {
    ghosts[g].x += ghosts[g].vx;
    ghosts[g].y += ghosts[g].vy;
    ghosts[g].phase += 10;

    // Отражение от краев
    if (ghosts[g].x < 1 || ghosts[g].x > WIDTH - 5) ghosts[g].vx = -ghosts[g].vx;
    if (ghosts[g].y < 2 || ghosts[g].y > HEIGHT - 6) ghosts[g].vy = -ghosts[g].vy;

    int8_t gx = (int8_t)ghosts[g].x;
    int8_t gy = (int8_t)ghosts[g].y;

    CRGB ghostColor = CRGB(200, 235, 255); // Призрачный лунный цвет

    // Голова и тело привидения (4x4)
    drawPixelXY(gx + 1, gy + 3, ghostColor);
    drawPixelXY(gx + 2, gy + 3, ghostColor);
    drawPixelXY(gx, gy + 2, ghostColor);
    drawPixelXY(gx + 1, gy + 2, ghostColor);
    drawPixelXY(gx + 2, gy + 2, ghostColor);
    drawPixelXY(gx + 3, gy + 2, ghostColor);
    drawPixelXY(gx, gy + 1, ghostColor);
    drawPixelXY(gx + 1, gy + 1, ghostColor);
    drawPixelXY(gx + 2, gy + 1, ghostColor);
    drawPixelXY(gx + 3, gy + 1, ghostColor);

    // Глаза привидения (черные пустоты)
    drawPixelXY(gx + 1, gy + 2, CRGB(0, 0, 0));
    drawPixelXY(gx + 2, gy + 2, CRGB(0, 0, 0));

    // Колышущийся волнами хвост
    int8_t tailWave = sin8(ghosts[g].phase) > 128 ? 1 : 0;
    drawPixelXY(gx, gy - tailWave, ghostColor);
    drawPixelXY(gx + 2, gy - (1 - tailWave), ghostColor);
    drawPixelXY(gx + 3, gy - tailWave, ghostColor);
  }
}

// ------------------------------------------------------------------------------
// 3. ХЭЛЛОУИН: Глаза во тьме (Spooky Eyes)
// Мигающие и оглядывающиеся пары светящихся глаз монстров в темноте
// ------------------------------------------------------------------------------
int8_t eyeX = 5, eyeY = 7;
int8_t lookOffset = 0;
uint8_t eyeState = 0; // 0-открытие, 1-ожидание/взгляд, 2-моргание, 3-закрытие
uint8_t eyeBrightness = 0;
CRGB eyeColor = CRGB::Red;
unsigned long eyeTimer = 0;

void spookyEyesRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_BALL;
    eyeState = 0;
    eyeBrightness = 0;
    eyeColor = CRGB::Red;
    FastLED.clear();
  }

  FastLED.clear();

  if (eyeState == 0) { // Плавное открытие глаз
    eyeBrightness += 15;
    if (eyeBrightness >= 250) {
      eyeBrightness = 255;
      eyeState = 1;
      eyeTimer = millis();
    }
  } else if (eyeState == 1) { // Глаза оглядываются
    if (millis() - eyeTimer > 800) {
      lookOffset = random(-1, 2); // -1: влево, 0: прямо, 1: вправо
      eyeTimer = millis();
      if (random(0, 4) == 0) {
        eyeState = 2; // моргнуть
      } else if (random(0, 5) == 0) {
        eyeState = 3; // раствориться в темноте
      }
    }
  } else if (eyeState == 2) { // Моргание (кратковременное закрытие)
    eyeBrightness = 0;
    eyeState = 1;
    eyeTimer = millis();
  } else if (eyeState == 3) { // Исчезновение
    if (eyeBrightness > 20) eyeBrightness -= 20;
    else {
      eyeBrightness = 0;
      // Новое случайное место и цвет глаз
      eyeX = random(1, WIDTH - 6);
      eyeY = random(3, HEIGHT - 4);
      lookOffset = 0;
      byte colType = random(0, 3);
      if (colType == 0) eyeColor = CRGB(255, 10, 10);     // Кроваво-красные
      else if (colType == 1) eyeColor = CRGB(255, 200, 0); // Желтые хищные
      else eyeColor = CRGB(0, 255, 40);                    // Ядовито-зеленые
      eyeState = 0;
    }
  }

  CRGB curCol = eyeColor;
  curCol.nscale8(eyeBrightness);

  if (eyeBrightness > 0) {
    // Левый глаз (2x2) и правый глаз (2x2)
    drawPixelXY(eyeX, eyeY, curCol);
    drawPixelXY(eyeX + 1, eyeY, curCol);
    drawPixelXY(eyeX, eyeY + 1, curCol);
    drawPixelXY(eyeX + 1, eyeY + 1, curCol);

    drawPixelXY(eyeX + 3, eyeY, curCol);
    drawPixelXY(eyeX + 4, eyeY, curCol);
    drawPixelXY(eyeX + 3, eyeY + 1, curCol);
    drawPixelXY(eyeX + 4, eyeY + 1, curCol);

    // Черные зрачки, направленные в сторону взгляда
    CRGB pupil = CRGB(0, 0, 0);
    drawPixelXY(eyeX + 1 + lookOffset, eyeY, pupil);
    drawPixelXY(eyeX + 4 + lookOffset, eyeY, pupil);
  }
}

// ------------------------------------------------------------------------------
// 4. ХЭЛЛОУИН: Паук и паутина (Spider & Web)
// Паук на шелковой нити спускается вниз, двигает лапками и поднимается
// ------------------------------------------------------------------------------
int8_t spiderY = 15;
int8_t spiderDir = -1;
uint8_t spiderLegPhase = 0;

void spiderRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_BALL;
    spiderY = 15;
    spiderDir = -1;
    FastLED.clear();
  }

  FastLED.clear();

  // Нити паутины в верхних углах
  CRGB webColor = CRGB(35, 35, 45);
  for (byte i = 0; i < 4; i++) {
    drawPixelXY(i, HEIGHT - 1 - i, webColor);
    drawPixelXY(WIDTH - 1 - i, HEIGHT - 1 - i, webColor);
  }

  // Вертикальная паутинная нить от потолка по центру X=7
  for (int8_t y = HEIGHT - 1; y >= spiderY; y--) {
    drawPixelXY(7, y, CRGB(65, 65, 75));
  }

  // Движение паука вверх-вниз
  spiderLegPhase++;
  if (spiderLegPhase % 3 == 0) {
    spiderY += spiderDir;
    if (spiderY <= 3) {
      spiderY = 3;
      spiderDir = 1;
    } else if (spiderY >= HEIGHT - 2) {
      spiderY = HEIGHT - 2;
      spiderDir = -1;
    }
  }

  // Тело паука (X=6..8, Y=spiderY..spiderY-1)
  CRGB spiderBody = CRGB(160, 15, 35);
  drawPixelXY(7, spiderY, spiderBody);
  drawPixelXY(6, spiderY - 1, spiderBody);
  drawPixelXY(7, spiderY - 1, spiderBody);
  drawPixelXY(8, spiderY - 1, spiderBody);

  // Глазки паука (светящиеся точки)
  drawPixelXY(6, spiderY, CRGB(255, 200, 0));
  drawPixelXY(8, spiderY, CRGB(255, 200, 0));

  // Лапки паука (шевелятся)
  CRGB legColor = CRGB(110, 10, 25);
  int8_t legOffset = (spiderLegPhase % 2 == 0) ? 1 : 0;
  drawPixelXY(5, spiderY + legOffset, legColor);
  drawPixelXY(9, spiderY + legOffset, legColor);
  drawPixelXY(5, spiderY - 1 - legOffset, legColor);
  drawPixelXY(9, spiderY - 1 - legOffset, legColor);
}

// ------------------------------------------------------------------------------
// 5. ХЭЛЛОУИН: Ведьмин котёл (Witch Cauldron)
// Кипящее неоново-зеленое варево с всплывающими пузырьками
// ------------------------------------------------------------------------------
struct Bubble {
  int8_t x;
  int8_t y;
  uint8_t speed;
};
Bubble bubbles[4];

void cauldronRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_NOISE_LAVA;
    for (byte i = 0; i < 4; i++) {
      bubbles[i] = {(int8_t)random(3, 12), (int8_t)random(4, 7), (uint8_t)random(1, 3)};
    }
    FastLED.clear();
  }

  FastLED.clear();

  // Чугунный черный котел внизу
  for (int8_t x = 2; x <= 12; x++) {
    drawPixelXY(x, 1, CRGB(30, 25, 30));
    drawPixelXY(x, 2, CRGB(40, 35, 40));
    drawPixelXY(x, 3, CRGB(30, 25, 30));
  }
  drawPixelXY(1, 3, CRGB(45, 40, 45));
  drawPixelXY(13, 3, CRGB(45, 40, 45));

  // Угли под котлом
  for (int8_t x = 4; x <= 10; x++) {
    uint8_t spark = inoise8(x * 30, millis() / 3);
    drawPixelXY(x, 0, CHSV(map(spark, 0, 255, 0, 25), 255, spark));
  }

  // Поверхность кипящего зелья (волны зеленого и фиолетового цвета)
  for (int8_t x = 2; x <= 12; x++) {
    uint8_t wave = inoise8(x * 40, millis() / 4);
    CRGB slimeCol = (wave > 140) ? CRGB(0, 255, 30) : CRGB(160, 0, 220);
    drawPixelXY(x, 4, slimeCol);
    if (wave > 180) drawPixelXY(x, 5, CRGB(50, 255, 70));
  }

  // Всплывающие и лопающиеся пузыри
  for (byte i = 0; i < 4; i++) {
    bubbles[i].y += bubbles[i].speed;
    if (bubbles[i].y >= HEIGHT - 2) {
      bubbles[i].y = 4;
      bubbles[i].x = random(3, 12);
      bubbles[i].speed = random(1, 3);
    }
    drawPixelXY(bubbles[i].x, bubbles[i].y, CRGB(80, 255, 60));
  }
}

// ------------------------------------------------------------------------------
// 6. ХЭЛЛОУИН: Кровавые капли (Blood Drips)
// Темно-красные капли стекают с верхнего края матрицы и разбиваются внизу
// ------------------------------------------------------------------------------
struct Drip {
  int8_t x;
  float y;
  float speed;
  bool active;
};
Drip drips[3];

void bloodDripsRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_BALLS;
    for (byte i = 0; i < 3; i++) {
      drips[i] = {(int8_t)random(1, WIDTH - 1), (float)(HEIGHT - 2), 0.1, false};
    }
    FastLED.clear();
  }

  // Легкое угасание следов капель
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(210);
  }

  // Верхняя кромка крови
  for (byte x = 0; x < WIDTH; x++) {
    drawPixelXY(x, HEIGHT - 1, CRGB(140, 0, 10));
  }

  // Обработка падающих капель
  for (byte i = 0; i < 3; i++) {
    if (!drips[i].active) {
      if (random(0, 15) == 0) {
        drips[i].active = true;
        drips[i].x = random(1, WIDTH - 1);
        drips[i].y = HEIGHT - 2;
        drips[i].speed = 0.2;
      }
    } else {
      drips[i].speed += 0.08; // Ускорение свободного падения
      drips[i].y -= drips[i].speed;

      if (drips[i].y <= 0) {
        // Всплеск внизу
        drawPixelXY(drips[i].x, 0, CRGB(200, 0, 20));
        if (drips[i].x > 0) drawPixelXY(drips[i].x - 1, 0, CRGB(160, 0, 10));
        if (drips[i].x < WIDTH - 1) drawPixelXY(drips[i].x + 1, 0, CRGB(160, 0, 10));
        drips[i].active = false;
      } else {
        drawPixelXY(drips[i].x, (int8_t)drips[i].y, CRGB(220, 0, 20));
        drawPixelXY(drips[i].x, (int8_t)drips[i].y + 1, CRGB(150, 0, 10));
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 7. НОВЫЙ ГОД: Новогодняя ёлочка (Christmas Tree)
// Зеленая ель с мигающей разноцветной гирляндой и сияющей звездой (центр X=7)
// ------------------------------------------------------------------------------
void christmasTreeRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    FastLED.clear();
  }

  FastLED.clear();

  // Снежный покров внизу (y = 0)
  for (int8_t x = 0; x < WIDTH; x++) {
    drawPixelXY(x, 0, CRGB(40, 50, 75));
  }

  // Коричневый ствол ели по центру (X=7)
  drawPixelXY(7, 1, CRGB(110, 55, 15));
  drawPixelXY(7, 2, CRGB(110, 55, 15));

  // Хвоя (3 яруса ели, центр X=7)
  CRGB pineDark = CRGB(0, 95, 20);
  CRGB pineLight = CRGB(10, 145, 30);

  // Нижний ярус (y: 3..6)
  for (int8_t y = 3; y <= 6; y++) {
    int8_t halfW = (6 - y) + 2; // y=3 -> halfW=5 (x: 2..12); y=6 -> halfW=2 (x: 5..9)
    for (int8_t x = 7 - halfW; x <= 7 + halfW; x++) {
      drawPixelXY(x, y, (x + y) % 2 == 0 ? pineDark : pineLight);
    }
  }

  // Средний ярус (y: 7..9)
  for (int8_t y = 7; y <= 9; y++) {
    int8_t halfW = (9 - y) + 2; // y=7 -> halfW=4 (x: 3..11); y=9 -> halfW=2 (x: 5..9)
    for (int8_t x = 7 - halfW; x <= 7 + halfW; x++) {
      drawPixelXY(x, y, (x + y) % 2 == 0 ? pineDark : pineLight);
    }
  }

  // Верхний ярус (y: 10..12)
  for (int8_t y = 10; y <= 12; y++) {
    int8_t halfW = (12 - y) + 1; // y=10 -> halfW=3 (x: 4..10); y=12 -> halfW=1 (x: 6..8)
    for (int8_t x = 7 - halfW; x <= 7 + halfW; x++) {
      drawPixelXY(x, y, (x + y) % 2 == 0 ? pineDark : pineLight);
    }
  }

  // Верхушка - сияющая золотая звезда (y: 13..15, центр X=7)
  uint8_t starPulse = sin8(millis() / 5);
  CRGB starColor = CHSV(42, 240, map(starPulse, 0, 255, 190, 255));
  drawPixelXY(7, 13, starColor);
  drawPixelXY(7, 14, starColor);
  drawPixelXY(6, 13, starColor);
  drawPixelXY(8, 13, starColor);
  drawPixelXY(7, 15, starColor);

  // Мерцающая праздничная гирлянда на ветках (симметричные координаты)
  static const int8_t lightsPos[][2] PROGMEM = {
    {3, 3}, {7, 3}, {11, 3},
    {4, 5}, {10, 5},
    {5, 7}, {7, 7}, {9, 7},
    {6, 9}, {8, 9},
    {5, 10}, {9, 10},
    {7, 11}
  };
  static const byte lightsCount = sizeof(lightsPos) / sizeof(lightsPos[0]);

  uint8_t t = millis() / 70;
  for (byte i = 0; i < lightsCount; i++) {
    int8_t lx = pgm_read_byte(&(lightsPos[i][0]));
    int8_t ly = pgm_read_byte(&(lightsPos[i][1]));
    CRGB lColor = CHSV((t * 12 + i * 45) % 255, 255, 255);
    drawPixelXY(lx, ly, lColor);
  }
}

// ------------------------------------------------------------------------------
// 8. НОВЫЙ ГОД: Праздничная гирлянда (Holiday Garland)
// Волновые переливы теплых новогодних огней: золото, рубин, изумруд и сапфир
// ------------------------------------------------------------------------------
void garlandRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_NOISE_RAINBOW;
    FastLED.clear();
  }

  uint8_t t = millis() / 35;
  for (int8_t y = 0; y < HEIGHT; y++) {
    for (int8_t x = 0; x < WIDTH; x++) {
      uint8_t wave = sin8(x * 20 + y * 25 + t);
      uint8_t bulbColor = (x * 3 + y * 7 + t / 4) % 4;

      CRGB col;
      switch (bulbColor) {
        case 0: col = CRGB(255, 20, 20);   break; // Рубиново-красный
        case 1: col = CRGB(20, 220, 30);   break; // Елочный зеленый
        case 2: col = CRGB(255, 180, 20);  break; // Теплое золото
        case 3: col = CRGB(20, 100, 255);  break; // Зимний сапфир
      }

      col.nscale8(map(wave, 0, 255, 40, 255));

      // Эффект случайных ярких искорок
      if (random(0, 60) == 0) col = CRGB::White;

      drawPixelXY(x, y, col);
    }
  }
}

// ------------------------------------------------------------------------------
// 9. НОВЫЙ ГОД: Салют / Фейерверк (Fireworks)
// Взлетающие ракеты с дымным следом и сферическим разлетом искр под гравитацией
// ------------------------------------------------------------------------------
struct FireworkSpark {
  float x;
  float y;
  float vx;
  float vy;
  CRGB color;
  bool active;
};

#define MAX_SPARKS 24
FireworkSpark sparks[MAX_SPARKS];
float rocketX, rocketY, rocketVy;
bool rocketActive = false;
CRGB rocketColor;

void fireworksRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_SPARKLES;
    rocketActive = false;
    for (byte i = 0; i < MAX_SPARKS; i++) sparks[i].active = false;
    FastLED.clear();
  }

  // Затухание искр
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(190);
  }

  // Запуск новой ракеты, если текущая не активна и искры догорели
  if (!rocketActive) {
    bool hasActiveSparks = false;
    for (byte i = 0; i < MAX_SPARKS; i++) {
      if (sparks[i].active) { hasActiveSparks = true; break; }
    }
    if (!hasActiveSparks) {
      rocketActive = true;
      rocketX = random(2, WIDTH - 2);
      rocketY = 0;
      rocketVy = random(7, 11) / 10.0;
      rocketColor = CHSV(random(0, 255), 255, 255);
    }
  }

  // Полет ракеты
  if (rocketActive) {
    rocketY += rocketVy;
    drawPixelXY((int8_t)rocketX, (int8_t)rocketY, CRGB::White);
    drawPixelXY((int8_t)rocketX, (int8_t)rocketY - 1, rocketColor);

    // Взрыв ракеты в верхней части
    if (rocketY >= random(9, 14) || rocketVy <= 0.1) {
      rocketActive = false;
      // Спавн искр взрыва
      for (byte i = 0; i < MAX_SPARKS; i++) {
        float angle = (i * (360.0 / MAX_SPARKS)) * 0.0174533; // В радианы
        float speed = random(3, 9) / 10.0;
        sparks[i].x = rocketX;
        sparks[i].y = rocketY;
        sparks[i].vx = cos(angle) * speed;
        sparks[i].vy = sin(angle) * speed;
        sparks[i].color = rocketColor;
        sparks[i].active = true;
      }
    }
  }

  // Разлет искр салюта
  for (byte i = 0; i < MAX_SPARKS; i++) {
    if (sparks[i].active) {
      sparks[i].x += sparks[i].vx;
      sparks[i].y += sparks[i].vy;
      sparks[i].vy -= 0.04; // Гравитация

      if (sparks[i].x < 0 || sparks[i].x >= WIDTH || sparks[i].y < 0 || sparks[i].y >= HEIGHT) {
        sparks[i].active = false;
      } else {
        drawPixelXY((int8_t)sparks[i].x, (int8_t)sparks[i].y, sparks[i].color);
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 10. НОВЫЙ ГОД: Метель / Пурга (Blizzard)
// Плотные косые снежные потоки с порывами ветра и сугробом внизу
// ------------------------------------------------------------------------------
uint8_t snowDrift[WIDTH];

void blizzardRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_SNOW;
    for (byte x = 0; x < WIDTH; x++) snowDrift[x] = 0;
    FastLED.clear();
  }

  // Сдвиг снега вниз и по диагонали (ветер вправо)
  for (int8_t y = 0; y < HEIGHT - 1; y++) {
    for (int8_t x = 0; x < WIDTH; x++) {
      int8_t fromX = (x == 0) ? WIDTH - 1 : x - 1;
      // Не стирать сугроб внизу
      if (y >= snowDrift[x]) {
        drawPixelXY(x, y, getPixColorXY(fromX, y + 1));
      }
    }
  }

  // Генерация снежинок в верхней строке
  for (byte x = 0; x < WIDTH; x++) {
    if (random(0, 4) == 0) {
      drawPixelXY(x, HEIGHT - 1, CRGB(210, 240, 255));
    } else {
      drawPixelXY(x, HEIGHT - 1, CRGB(0, 0, 0));
    }
  }

  // Медленный рост сугроба внизу матрицы
  if (random(0, 30) == 0) {
    byte rx = random(0, WIDTH);
    if (snowDrift[rx] < 3) snowDrift[rx]++;
  }
  // Медленное таяние сугроба
  if (random(0, 60) == 0) {
    byte rx = random(0, WIDTH);
    if (snowDrift[rx] > 0) snowDrift[rx]--;
  }

  // Отрисовка сугроба
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < snowDrift[x]; y++) {
      drawPixelXY(x, y, CRGB(200, 230, 255));
    }
  }
}

// ------------------------------------------------------------------------------
// 11. НОВЫЙ ГОД: Северное сияние (Aurora Borealis)
// Волновые колышущиеся переливы изумрудных, лазурных и фиолетовых оттенков
// ------------------------------------------------------------------------------
void auroraRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_NOISE_OCEAN;
    FastLED.clear();
  }

  uint16_t t = millis() / 4;
  for (int8_t x = 0; x < WIDTH; x++) {
    for (int8_t y = 0; y < HEIGHT; y++) {
      // Двумерный процедурный шум
      uint8_t noiseVal = inoise8(x * 35, y * 30 + t, t / 3);

      // Северное сияние ярче в верхне-средней части матрицы
      uint8_t heightMask = sin8(y * 14);
      uint8_t bright = qmul8(noiseVal, heightMask);

      // Палитра северного сияния: Изумруд (96) -> Бирюза (128) -> Фиолет (192)
      uint8_t hueVal = map(noiseVal, 0, 255, 85, 195);
      CRGB auroraCol = CHSV(hueVal, 240, bright);

      drawPixelXY(x, y, auroraCol);
    }
  }
}


// ==============================================================================
// СПЕЦЭФФЕКТЫ ПО МОТИВАМ ФИЛЬМА «СИЯНИЕ» (THE SHINING)
// 12. Кровавый лифт (Blood Elevator)
// 13. Ковер отеля Оверлук (Overlook Carpet)
// 14. Надпись REDRUM (REDRUM Mirror)
// 15. Ледяной лабиринт (Frozen Maze)
// ==============================================================================

// ------------------------------------------------------------------------------
// 12. КРОВАВЫЙ ЛИФТ (The Blood Elevator)
// Металлические двери отеля "Оверлук" распахиваются, и из них вырывается
// лавина бурлящей багровой крови, затапливающая матрицу снизу доверху.
// ------------------------------------------------------------------------------
uint8_t elevPhase = 0;      // 0: закрыт, 1: открывается, 2: поток крови, 3: затоплен, 4: спад
uint8_t elevDoorOpen = 0;   // 0..7
uint8_t elevBloodLevel = 0; // 0..16
uint32_t elevTimer = 0;

void bloodElevatorRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    elevPhase = 0;
    elevDoorOpen = 0;
    elevBloodLevel = 0;
    elevTimer = millis();
    FastLED.clear();
  }

  uint32_t now = millis();

  // Логика смены фаз лифта
  if (elevPhase == 0) { // Двери закрыты 2.5 секунды
    if (now - elevTimer > 2500) {
      elevPhase = 1;
      elevTimer = now;
    }
  } else if (elevPhase == 1) { // Двери плавно открываются
    if (now - elevTimer > 75) {
      elevDoorOpen++;
      elevTimer = now;
      if (elevDoorOpen >= 7) {
        elevPhase = 2;
        elevBloodLevel = 1;
      }
    }
  } else if (elevPhase == 2) { // Волна крови поднимается
    if (now - elevTimer > 90) {
      elevBloodLevel++;
      elevTimer = now;
      if (elevBloodLevel >= HEIGHT) {
        elevPhase = 3;
        elevTimer = now;
      }
    }
  } else if (elevPhase == 3) { // Полное затопление 3 секунды
    if (now - elevTimer > 3000) {
      elevPhase = 4;
      elevTimer = now;
    }
  } else if (elevPhase == 4) { // Кровь спадает и двери закрываются
    if (now - elevTimer > 60) {
      if (elevBloodLevel > 0) elevBloodLevel--;
      if (elevDoorOpen > 0) elevDoorOpen--;
      elevTimer = now;
      if (elevBloodLevel == 0 && elevDoorOpen == 0) {
        elevPhase = 0;
        elevTimer = now;
      }
    }
  }

  FastLED.clear();

  // Рисуем нутро лифта (глубокий кровавый мрак шахты)
  for (int8_t y = 0; y < HEIGHT; y++) {
    for (int8_t x = 0; x < WIDTH; x++) {
      drawPixelXY(x, y, CRGB(30, 2, 4));
    }
  }

  // Рисуем раздвижные бронзовые створки дверей лифта
  CRGB doorMetal = CRGB(55, 42, 25);
  CRGB doorTrim = CRGB(85, 68, 38);
  int8_t leftEdge = 6 - elevDoorOpen;
  int8_t rightEdge = 8 + elevDoorOpen;

  if (leftEdge >= 0) {
    for (int8_t x = 0; x <= leftEdge; x++) {
      for (int8_t y = 0; y < 13; y++) {
        drawPixelXY(x, y, (x == leftEdge || x == 0) ? doorTrim : doorMetal);
      }
    }
  }
  if (rightEdge < WIDTH) {
    for (int8_t x = rightEdge; x < WIDTH; x++) {
      for (int8_t y = 0; y < 13; y++) {
        drawPixelXY(x, y, (x == rightEdge || x == WIDTH - 1) ? doorTrim : doorMetal);
      }
    }
  }

  // Верхняя золотая арка и указатель этажа (y: 13..15)
  for (int8_t x = 0; x < WIDTH; x++) {
    drawPixelXY(x, 13, CRGB(70, 55, 30));
  }
  drawPixelXY(6, 14, CRGB(180, 140, 30));
  drawPixelXY(7, 14, CRGB(255, 215, 60)); // Светящаяся стрелка этажа
  drawPixelXY(8, 14, CRGB(180, 140, 30));
  drawPixelXY(7, 15, CRGB(220, 175, 40));

  // Бурлящая лавина крови
  if (elevPhase >= 2) {
    for (int8_t y = 0; y < elevBloodLevel && y < HEIGHT; y++) {
      for (int8_t x = 0; x < WIDTH; x++) {
        uint8_t wave = inoise8(x * 45 + (now / 4), y * 40 - (now / 3), now / 5);
        CRGB bloodCol;
        if (y == elevBloodLevel - 1) {
          // Пена на гребне
          bloodCol = (wave > 150) ? CRGB(255, 120, 140) : CRGB(235, 20, 35);
        } else if (wave > 170) {
          bloodCol = CRGB(210, 5, 20); // Яркая струя
        } else if (wave > 90) {
          bloodCol = CRGB(150, 0, 12); // Основной массив крови
        } else {
          bloodCol = CRGB(90, 0, 8);   // Глубокий темный сгусток
        }
        drawPixelXY(x, y, bloodCol);
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 13. КОВЕР ОТЕЛЯ «ОВЕРЛУК» (The Overlook Carpet)
// Знаменитый шестиугольный орнамент Дэвида Хикса из коридора отеля.
// Гипнотическая перспектива движения вперед (взгляд Дэнни на велосипеде).
// ------------------------------------------------------------------------------
void overlookCarpetRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_NOISE_MADNESS;
    FastLED.clear();
  }

  uint8_t scrollY = millis() / 45;

  CRGB colOrange = CRGB(255, 95, 0);   // Яркий оранжевый центр
  CRGB colTerra  = CRGB(160, 35, 10);  // Терракотово-красный
  CRGB colDark   = CRGB(15, 4, 4);     // Темный шоколадный контур

  for (int8_t y = 0; y < HEIGHT; y++) {
    int16_t mapY = (y + scrollY) % 8;
    for (int8_t x = 0; x < WIDTH; x++) {
      int16_t mapX = (x + (mapY >= 4 ? 3 : 0)) % 6;
      int16_t subY = mapY % 4;

      // Определение гексагональных слоев узора Дэвида Хикса
      bool isBorder = (mapX == 0 || mapX == 5 || (subY == 0 && (mapX == 2 || mapX == 3)));
      bool isTerra = (mapX == 1 || mapX == 4 || subY == 0 || subY == 3);

      CRGB pixelCol;
      if (isBorder) {
        pixelCol = colDark;
      } else if (isTerra) {
        pixelCol = colTerra;
      } else {
        pixelCol = colOrange;
      }

      drawPixelXY(x, y, pixelCol);
    }
  }
}

// ------------------------------------------------------------------------------
// 14. НАДПИСЬ REDRUM (REDRUM Mirror)
// Кровавая надпись на темной деревянной двери, которая мерцает
// и в зеркальном отражении оборачивается в «MURDER».
// ------------------------------------------------------------------------------
// Растровый 3x5 алфавит для R, E, D, U, M
static const uint16_t FONT_SHINING[] PROGMEM = {
  0b111101111101101, // R: 0
  0b111100110100111, // E: 1
  0b110101101101110, // D: 2
  0b101101101101111, // U: 3
  0b101111111101101  // M: 4
};

static void drawCharShining(int8_t startX, int8_t startY, uint8_t charIdx, CRGB col) {
  uint16_t glyph = pgm_read_word(&(FONT_SHINING[charIdx]));
  for (int8_t r = 0; r < 5; r++) {
    for (int8_t c = 0; c < 3; c++) {
      if ((glyph >> (14 - (r * 3 + c))) & 1) {
        drawPixelXY(startX + c, startY + (4 - r), col);
      }
    }
  }
}

struct BloodDrop {
  int8_t x;
  float y;
  float speed;
  bool active;
};
BloodDrop redrumDrops[3];

void redrumRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_IMAGE;
    for (byte i = 0; i < 3; i++) redrumDrops[i].active = false;
    FastLED.clear();
  }

  uint32_t now = millis();
  bool isMurder = ((now / 3500) % 2) == 1; // Каждые 3.5 секунды меняется на MURDER

  FastLED.clear();

  // Фон старой деревянной двери
  for (int8_t y = 0; y < HEIGHT; y++) {
    for (int8_t x = 0; x < WIDTH; x++) {
      uint8_t woodGrain = inoise8(x * 60, y * 15);
      CRGB woodCol = (woodGrain > 140) ? CRGB(35, 18, 10) : CRGB(22, 10, 6);
      drawPixelXY(x, y, woodCol);
    }
  }

  // Мерцающий кроваво-красный оттенок букв
  uint8_t flicker = inoise8(now / 5, 200);
  CRGB textCol = (flicker > 50) ? CRGB(235, 5, 15) : CRGB(160, 0, 10);

  // Отрисовка двух строк букв 3x5
  if (!isMurder) {
    // RED (y: 9..13)
    drawCharShining(2, 9, 0, textCol); // R
    drawCharShining(6, 9, 1, textCol); // E
    drawCharShining(10, 9, 2, textCol); // D

    // RUM (y: 3..7)
    drawCharShining(2, 3, 0, textCol); // R
    drawCharShining(6, 3, 3, textCol); // U
    drawCharShining(10, 3, 4, textCol); // M
  } else {
    // MUR (y: 9..13)
    drawCharShining(2, 9, 4, textCol); // M
    drawCharShining(6, 9, 3, textCol); // U
    drawCharShining(10, 9, 0, textCol); // R

    // DER (y: 3..7)
    drawCharShining(2, 3, 2, textCol); // D
    drawCharShining(6, 3, 1, textCol); // E
    drawCharShining(10, 3, 0, textCol); // R
  }

  // Стекающие струйки крови
  for (byte i = 0; i < 3; i++) {
    if (!redrumDrops[i].active) {
      if (random(0, 20) == 0) {
        redrumDrops[i].active = true;
        redrumDrops[i].x = (i == 0) ? 4 : (i == 1 ? 8 : 12);
        redrumDrops[i].y = 3.0;
        redrumDrops[i].speed = 0.15;
      }
    } else {
      redrumDrops[i].speed += 0.05;
      redrumDrops[i].y -= redrumDrops[i].speed;
      if (redrumDrops[i].y <= 0) {
        drawPixelXY(redrumDrops[i].x, 0, CRGB(200, 0, 10));
        redrumDrops[i].active = false;
      } else {
        drawPixelXY(redrumDrops[i].x, (int8_t)redrumDrops[i].y, CRGB(220, 0, 15));
      }
    }
  }
}

// ------------------------------------------------------------------------------
// 15. ЛЕДЯНОЙ ЛАБИРИНТ (The Frozen Maze)
// Заснеженный живой лабиринт отеля ночью в пурге.
// Луч фонарика Джека ищет путь сквозь заснеженные стены изгороди.
// ------------------------------------------------------------------------------
float torchX = 7.0;
float torchY = 7.0;
float torchVx = 0.25;
float torchVy = 0.18;

void frozenMazeRoutine() {
  if (loadingFlag) {
    loadingFlag = false;
    modeCode = MC_NOISE_OCEAN;
    torchX = 7.0;
    torchY = 7.0;
    torchVx = 0.22;
    torchVy = 0.16;
    FastLED.clear();
  }

  // Движение луча фонарика по лабиринту
  torchX += torchVx;
  torchY += torchVy;
  if (torchX <= 2.0 || torchX >= 12.0) torchVx = -torchVx;
  if (torchY <= 2.0 || torchY >= 13.0) torchVy = -torchVy;

  FastLED.clear();

  // Рисуем стены лабиринта (паттерн живой изгороди)
  for (int8_t y = 0; y < HEIGHT; y++) {
    for (int8_t x = 0; x < WIDTH; x++) {
      bool isWall = false;
      // Внешняя ограда
      if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) isWall = true;
      // Внутренние перегородки лабиринта
      if (y == 4 && (x >= 2 && x <= 6)) isWall = true;
      if (y == 4 && (x >= 8 && x <= 12)) isWall = true;
      if (y == 8 && (x >= 4 && x <= 10)) isWall = true;
      if (y == 12 && (x >= 2 && x <= 5)) isWall = true;
      if (y == 12 && (x >= 9 && x <= 12)) isWall = true;
      if (x == 4 && (y >= 5 && y <= 7)) isWall = true;
      if (x == 10 && (y >= 9 && y <= 11)) isWall = true;

      // Расстояние до фонарика Джека
      float dx = x - torchX;
      float dy = y - torchY;
      float distSq = dx * dx + dy * dy;

      if (isWall) {
        // Заснеженные темные кусты
        CRGB hedgeDark = CRGB(8, 24, 38);
        if (distSq < 18.0) {
          // Освещено фонариком (теплый свет на морозных ветках)
          uint8_t light = map(constrain((int)distSq, 0, 18), 0, 18, 200, 20);
          drawPixelXY(x, y, CRGB(light, light * 0.8, light * 0.5));
        } else {
          drawPixelXY(x, y, hedgeDark);
        }
      } else {
        // Проходы лабиринта
        if (distSq < 10.0) {
          // Свет фонаря на снежном полу
          uint8_t glow = map(constrain((int)distSq, 0, 10), 0, 10, 240, 40);
          drawPixelXY(x, y, CRGB(glow, glow * 0.85, glow * 0.4));
        } else {
          // Замерзшая земля в темноте
          drawPixelXY(x, y, CRGB(2, 6, 12));
        }
      }
    }
  }

  // Яркая точка фонарика в руках
  drawPixelXY((int8_t)torchX, (int8_t)torchY, CRGB(255, 245, 180));

  // Хлопья метели, кружащие над лабиринтом
  for (byte i = 0; i < 4; i++) {
    int8_t sx = random(0, WIDTH);
    int8_t sy = random(0, HEIGHT);
    drawPixelXY(sx, sy, CRGB(210, 240, 255));
  }
}
