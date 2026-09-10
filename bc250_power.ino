const int PIN_NUM_BTN  = 7;   //전원버튼
const int PIN_NUM_PON  = 8;   //ATX파워 P_ON케이블
const int PIN_NUM_TMPS = 9;   //TPMS 3.3V Standby -> Alive체크 소스로 이용

const int INTERVAL_BTN   = 20; //버튼누름 동작기준 MilSec
const int INTERVAL_FORCE = 4000; //강제종료누름 동작기준 MilSec
const int INTERVAL_TMPS  = 500; //BC250 TPMS Off -> Atx Off넘어가는 MilSec

unsigned long timerPress    = 0; //버튼상태변경시점 UpTime
unsigned long timerForce    = 0; //강제종료누름시점 UpTime
unsigned long timerTpmsOff  = 0; //TPMS가 Off로 변경된 시점 UpTime

bool systemUp      = false; //현재 On/Off상태
bool pressCleared  = false; //버튼누름 동작완료
bool stateFix      = HIGH; //확정된 버튼누름상태
bool stateTemp     = HIGH; //확정되기전 변경된 버튼누름상태

void atxOn()  { pinMode(PIN_NUM_PON, OUTPUT); digitalWrite(PIN_NUM_PON, LOW); digitalWrite(LED_BUILTIN, HIGH); }
void atxOff() { pinMode(PIN_NUM_PON, INPUT); digitalWrite(LED_BUILTIN, LOW); }
void powerOn()  { systemUp = true;  atxOn();  timerTpmsOff = millis(); }
void powerOff() { systemUp = false; atxOff(); }

void setup() {
  atxOff();
  pinMode(PIN_NUM_BTN, INPUT_PULLUP);
  pinMode(PIN_NUM_TMPS, INPUT);
  pinMode(PIN_NUM_BTN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  bool stateNow = digitalRead(PIN_NUM_BTN); //현재 버튼눌림상태
  unsigned long timerNow = millis(); //현재 Arduino UpTime MilSec
  
  //버튼눌림상태가 변함
  if (stateNow != stateTemp) {
    timerPress = timerNow;
    stateTemp = stateNow;
  }

  //오동작 방지위해 버튼 누름상태는 0.02초가 지나야 상태변경 인정, RealWorld용 안전장치
  if (stateNow != stateFix && (timerNow-timerPress) > INTERVAL_BTN) {
    stateFix = stateNow;

    if (stateFix == LOW) {
      timerForce = timerNow; //강제종료누름 시작
      pressCleared = false;

      //꺼진상태에서 눌리면 Atx On, 켜진상태에서 눌리면 BC250 USB시리얼포트로 종료신호전송
      if (!systemUp) {
        powerOn();
        pressCleared = true; //버튼누름 동작완료
      }
      else {
        Serial.println("SHELL_SHUTDOWN");
      }
    }
    
  }

  //버튼4초누름(강제종료) -> Atx Off
  if (systemUp && stateFix == LOW && !pressCleared && (timerNow-timerForce) > INTERVAL_FORCE) {
    powerOff();
    pressCleared = true;
  }

  //TPMS Off(0.5초유지) -> Atx Off
  if (systemUp) {    
    if (digitalRead(PIN_NUM_TMPS) == HIGH) {
      timerTpmsOff = timerNow;
    }
    else {
      if (timerNow-timerTpmsOff) > INTERVAL_TMPS)
        powerOff();
    }
  }

}
