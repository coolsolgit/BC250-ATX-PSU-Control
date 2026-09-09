const int  PIN_NUM_BTN  = 7;   // 전원버튼
const int  PIN_NUM_PON  = 8;   // ATX파워 P_ON신호케이블
const int  PIN_NUM_TMPS = 9;   // TPMS 3.3V Alive

const int MIL_BTN_INTERVAL   = 20; // 버튼누름 동작 기준시간
const int MIL_FORCE_INTERVAL = 5000; // 강제종료누름 동작 기준시간
const int MIL_TMPS_INTERVAL  = 500; // TPMS Off -> Atx Off넘어가는 기준시간

unsigned long pressTimer      = 0; //버튼상태변경시점
unsigned long pressForceTimer = 0; //강제종료누름시점
unsigned long tpmsOffTimer    = 0; // TPMS가 Off로 변경된 시점

bool systemUp      = false; // 현재 On/Off상태
bool pressCleared  = false; //버튼누름 동작완료
bool stateFix      = HIGH; //확정된 버튼누름상태
bool stateTemp     = HIGH; //확정되기전 변경된 버튼누름상태

void atxOn()  { pinMode(PIN_NUM_PON, OUTPUT); digitalWrite(PIN_NUM_PON, LOW); digitalWrite(LED_BUILTIN, HIGH); }
void atxOff() { pinMode(PIN_NUM_PON, INPUT); digitalWrite(LED_BUILTIN, LOW); }
void powerOn()  { systemUp = true;  atxOn();  tpmsOffTimer = millis(); }
void powerOff() { systemUp = false; atxOff(); }

void setup() {
  atxOff();
  pinMode(PIN_NUM_BTN, INPUT_PULLUP);
  pinMode(PIN_NUM_TMPS, INPUT);
  pinMode(PIN_NUM_BTN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  bool stateNow = digitalRead(PIN_NUM_BTN);
  
  // 버튼눌림상태가 변함
  if (stateNow != stateTemp) {
    pressTimer = millis();
    stateTemp = stateNow;
  }

  // 오동작 방지위해 버튼 누름상태는 0.03초가 지나야 상태변경 인정
  if (stateNow != stateFix && (millis()-pressTimer) > MIL_BTN_INTERVAL) {
    stateFix = stateNow;

    if (stateFix == LOW) {
      pressForceTimer = millis(); //강제종료누름 시작
      pressCleared = false;

      if (!systemUp) {
        powerOn();
        pressCleared = true; //버튼누름 동작완료
      }
      else {
        Serial.println("SHUTDOWN");
      }
    }
  }

  // 버튼 5초이상누름 -> Atx Off
  if (stateFix == LOW && systemUp && !pressCleared && (millis()-pressForceTimer) >= MIL_FORCE_INTERVAL) {
    powerOff();
    pressCleared = true;
  }

  // TPMS Off -> Atx Off
  if (systemUp) {
    if (digitalRead(PIN_NUM_TMPS) == HIGH) {
      tpmsOffTimer = millis();
    }
    else {
      if ((millis()-tpmsOffTimer) >= MIL_TMPS_INTERVAL)
        powerOff();
    }
  }

}
