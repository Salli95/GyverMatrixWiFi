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

  // Рисуем тело тыквы (15x16), центр X = 7
  for (int8_t y = 2; y <= 11; y++) {
    for (int8_t x = 2; x <= 12; x++) {
      // Скругление углов тыквы
      if ((y == 2 || y == 11) && (x < 4 || x > 10)) continue;
      if ((y == 3 || y == 10) && (x < 3 || x > 11)) continue;

      // Вертикальные сегменты тыквы (ребра)
      bool isRidge = (x == 3 || x == 5 || x == 7 || x == 9 || x == 11);
      CRGB pumpkinColor = isRidge ? CRGB(180, 50, 0) : CRGB(255, 90, 0);
      drawPixelXY(x, y, pumpkinColor);
    }
  }

  // Зеленый хвостик тыквы сверху
  drawPixelXY(7, 12, CRGB(20, 140, 20));
  drawPixelXY(7, 13, CRGB(20, 150, 20));
  drawPixelXY(8, 14, CRGB(15, 120, 15));

  // Мерцающее пламя внутри прорезей (шум огня)
  uint8_t flicker1 = inoise8(millis() / 4, 100);
  uint8_t flicker2 = inoise8(millis() / 5, 200);

  CRGB flame1 = CHSV(map(flicker1, 0, 255, 20, 42), 255, map(flicker1, 0, 255, 190, 255));
  CRGB flame2 = CHSV(map(flicker2, 0, 255, 20, 42), 255, map(flicker2, 0, 255, 190, 255));

  // Треугольные вырезанные глаза (симметрично относительно X=7)
  // Левый глаз:
  drawPixelXY(4, 8, flame1);
  drawPixelXY(5, 8, flame1);
  drawPixelXY(5, 9, flame2);

  // Правый глаз:
  drawPixelXY(10, 8, flame2);
  drawPixelXY(9, 8, flame2);
  drawPixelXY(9, 9, flame1);

  // Треугольный нос по центру
  drawPixelXY(7, 7, flame1);

  // Зловещая зубастая пасть (симметрично относительно X=7)
  drawPixelXY(4, 5, flame1);
  drawPixelXY(5, 4, flame2);
  drawPixelXY(6, 5, flame1);
  drawPixelXY(7, 4, flame2);
  drawPixelXY(8, 5, flame1);
  drawPixelXY(9, 4, flame2);
  drawPixelXY(10, 5, flame1);
  drawPixelXY(7, 3, flame1);
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
