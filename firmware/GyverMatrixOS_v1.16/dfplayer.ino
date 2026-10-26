#if (USE_DFPLAYER == 1)

void InitializeDfPlayer1() {
  mp3Serial.begin(9600);  
  dfPlayer.begin(mp3Serial, true, true);
  dfPlayer.setTimeOut(2000);
  dfPlayer.EQ(DFPLAYER_EQ_NORMAL);
  dfPlayer.volume(1);
}

void InitializeDfPlayer2() {    
  Serial.print(F("Инициализация MP3 плеера."));
  refreshDfPlayerFiles();    
  Serial.println(String(F("Звуков будильника найдено: ")) + String(alarmSoundsCount));
  Serial.println(String(F("Звуков рассвета найдено: ")) + String(dawnSoundsCount));
  isDfPlayerOk = alarmSoundsCount + dawnSoundsCount > 0;
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      break;
    case WrongStack:
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println(F("USB Inserted!"));
      break;
    case DFPlayerUSBRemoved:
      Serial.println(F("USB Removed!"));
      break;
    case DFPlayerPlayFinished:
      if (!(isAlarming || isPlayAlarmSound) && soundFolder == 0 && soundFile == 0) {
        dfPlayer.stop();
      }
      break;
    case DFPlayerError:
      break;
    default:
      break;
  }
}

void refreshDfPlayerFiles() {
  int16_t cnt = 0, val = 0, new_val = 0;
  alarmSoundsCount = 0;
  dawnSoundsCount = 0;
  while (cnt++ < 5) {
    val = dfPlayer.readFileCountsInFolder(1);     delay(10);
    new_val = dfPlayer.readFileCountsInFolder(1); delay(10);    
    if (val > 0 && new_val > 0 && val == new_val) {
      alarmSoundsCount = val;
      break;
    }
  }
  cnt = 0;
  while (cnt++ < 5) {
    val = dfPlayer.readFileCountsInFolder(2);     delay(10);
    new_val = dfPlayer.readFileCountsInFolder(2); delay(10);     
    if (val > 0 && new_val > 0 && val == new_val) {
      dawnSoundsCount = val;
      break;
    }
  }
}

void PlayAlarmSound() {
  byte sound = alarmSound;
  if (sound == 0) sound = random(1, alarmSoundsCount);
  if (isDfPlayerOk) {
    dfPlayer.stop();
    delay(100);
    dfPlayer.volume(constrain(maxAlarmVolume,1,30));
    dfPlayer.playFolder(1, sound);
    dfPlayer.enableLoop();
    isPlayAlarmSound = true;
  }
}

void PlayDawnSound() {
  byte sound = dawnSound;
  if (sound == 0) sound = random(1, dawnSoundsCount);
  if (isDfPlayerOk) {
    dfPlayer.stop();
    delay(100);
    dfPlayer.volume(1);
    dfPlayer.playFolder(2, sound);
    dfPlayer.enableLoop();
    isPlayAlarmSound = true;
    fadeSoundDirection = 1;
    fadeSoundStepCounter = maxAlarmVolume;
    fadeSoundTimer.setInterval(fadeSoundDuration * 1000 / maxAlarmVolume);
  } else {
    StopSound(1500);
  }
}

void StopSound(int duration) {
  isPlayAlarmSound = false;
  if (duration <= 0) {
    dfPlayer.stop();
    delay(100);
    dfPlayer.volume(0);
    return;
  }
  fadeSoundStepCounter = dfPlayer.readVolume();
  if (fadeSoundStepCounter <= 0) fadeSoundStepCounter = maxAlarmVolume;
  if (fadeSoundStepCounter <= 0) fadeSoundStepCounter = 1;
  fadeSoundDirection = -1;   
  fadeSoundTimer.setInterval(duration / fadeSoundStepCounter);
}

#else

// Пустые заглушки для работы без MP3 плеера (экономия RAM и пинов)
void InitializeDfPlayer1() {}
void InitializeDfPlayer2() { isDfPlayerOk = false; }
void printDetail(uint8_t type, int value) {}
void refreshDfPlayerFiles() {}
void PlayAlarmSound() {}
void PlayDawnSound() {}
void StopSound(int duration) { isPlayAlarmSound = false; }

#endif
