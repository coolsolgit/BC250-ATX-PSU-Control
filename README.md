# Arduino Nano를 이용한 BC250 ATX-PowerSupply제어

책임지지 않아요, 나도 잘 몰라요.

아두이노 나노 선정이유 : 제일싸서, 알리에서 호환품 3400원, 자체 부트로더 내장해서 별도 프로그래머/디버거 구매 필요없음.

ATX PSU의 5V Standby(상시) 전원에 연결.

Arduino에 연결된 버튼누름을 인식하여 ATX P_ON HIGH/LOW을 설정하여 PSU전원을 제어함.
BC250꺼질시 PSU연동은 BC250의 TMPS 3.3V Standby전원을 AliveCheck 소스로하여 이게 LOW면 ATX P_ON도 LOW로 변경

BC250이나 아두이노는 회로상 모두 ATX PSU의 GND를 공유함, 그래서 전위차가 없으니 옵토커플러나 릴레이를 이용한 ATX P_ON 및 TPMS 절연은 오버스펙같음.
옵토커플러나 릴레이를 추가하여 회로를 구성하려면 회로가 복잡해지고 만능기판같은 추가자재가 필요해져 기구제작이 복잡해짐.

최대한 단순하게 그냥 아두이노하고 스위치만 있으면 만들 수 있게 하고, BC250 기판에 납땜을 하지 않는것을 주안점으로 진행.

Arduino-BC250연결은 USB포트와 BC250 TPMS 점퍼에 듀퐁케이블로 연결(TPMS가 일반점퍼 규격보다 작음주의)

전원이 켜진상태에서 버튼을 누르면 BC250 시리얼포트로 종료신호를 보내고 Linux Python코드에서 이를 수신하면 Linux종료처리 구현함.

Arduino UNO/NANO에서는 SerialPort가 열리는 순간 Arduino가 Reset되도록 설계되어있음.
이러한 이유로 당초계확과는 별도의 USB to TTL 모듈을 장착해야 정상적인 ATX전원관리가 가능.
Nano에 캐피시터나 저항을 붙여서 Reset을 방지할수 있는 방법이 있다고 하나, 그냥 TTL모듈을 사서 연결하여 해결.
시리얼포트(USB to TTL)을 구매(알리 2500원)해서 BC250 USB포트에 장착하고
이쪽으로 nano:TX <-> usb_ttl:RX 및 양쪽 GND를 연결하니 정상동작함.
