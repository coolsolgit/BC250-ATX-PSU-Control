# Arduino Nano를 이용한 BC250 ATX-PowerSupply제어

책임지지 않아요, 나도 잘 몰라요.

아두이노 나노 선정이유 : 제일싸서, 알리에서 호환품 3400원, 자체 부트로더 내장해서 별도 프로그래머/디버거 구매 필요없음.

ATX PSU의 5V Standby(상시) 전원에 연결.

BC250과 시리얼통신을 하려면 USB로 연결해두어야 하는데, 이경우 USB전원으로 스위칭.
BC250전원이 켜져있으면 USB로 전원을 공급받고, 꺼지면 VIN(PSU 5V Standby)전원으로 공급받음.

아두이노내부에서 전원 스위칭시 모스펫으로 전원을 USB->VIN으로 변경하는데, 이때 충격이 있는지 모르겠음.

Arduino에 연결된 버튼누름을 인식하여 ATX P_ON HIGH/LOW을 설정하여 PSU전원을 제어함.
BC250꺼질시 PSU연동은 BC250의 TMPS 3.3V Standby전원을 AliveCheck 소스로하여 이게 LOW면 ATX P_ON도 LOW로 변경

BC250이나 아두이노는 회로상 모두 ATX PSU의 GROUND를 공유함, 그래서 전위차가 없으니 옵토커플러나 릴레이를 이용한 ATX P_ON 및 TPMS 절연은 오버스펙같음.
옵토커플러나 릴레이를 추가하여 회로를 구성하려면 회로가 복잡해지고 만능기판같은 추가자재가 필요해져 기구제작이 복잡해짐.

최대한 단순하게 그냥 아두이노하고 스위치만 있으면 만들 수 있게 하고, BC250 기판에 납땜을 하지 않는것을 주안점으로 진행.

Arduino-BC250연결은 USB포트와 BC250 TPMS 점퍼에 듀퐁케이블로 연결(TPMS가 일반점퍼 규격보다 작음주의)

전원이 켜진상태에서 버튼을 누르면 BC250 시리얼포트로 종료신호를 보내고 Linux Python코드에서 이를 수신하면 Linux종료처리 구현함.

BC250 Linux에서(bazzite chchyOS)에 시리얼포트(ttyUSB0등)에 Listening을 걸면 BC250이 리부팅됨.
Nano호환보드의 USB칩인 CH320G문제인지 리눅스커널의 CH320드라이버 문제인지 아직 모르겠음.
하나의 Arduino로 시리얼포트까지 쓰니 문제가 생기는것 같아, 결국 별도의 시리얼포트(USB to TTL)을 구매(알리 2500원)해서 BC250 USB포트에 장착하고
이쪽으로 Arduino Tx/Rx/GND를 연결하니 정상동작함.
