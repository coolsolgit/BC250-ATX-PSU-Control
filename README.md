# Arduino Nano를 이용한 BC250 ATX-PowerSupply제어

BC250채굴기에 ATX PowerSupply를 사용하는경우 PCIE 8핀 전원만 연결되므로 PSU의 전원을 스스로 끌수없어 Arduino Nano를 이용하여 이를 제어
Arduino Nano와 Arduino Uno는 구조가 동일하여 소스변경없이 Uno사용이 가능함

## 제작목표
* 가장 적은 비용
  * Arduino Nano는 알리에서 호환제품을 3000원정도에 구매가능하며 별도의 프로그램머나 디버거가 필요없음
  * Nano에서 BC250으로 종료신호를 보내기 위해 별도의 USB to ttl모듈이 필요함
    > Nano에 내장된 USB칩으로 TTL통신을 하려고 했으나 Arduino는 Serical통신이 시작되면 Reset되도록 설계된것을 나중에 알아 최초계획과는 다르게 추가비용(2000원)이 발생
* 가장 간단한 구조
  * BC250본체에는 납땜작업이 필요없음
  * 추가적인 전자소자나 회로기판이 불필요하게 구성
   > BC250이나 아두이노는 회로상 모두 ATX PSU의 GND를 공유함, 그래서 전위차가 없으니 옵토커플러나 릴레이를 이용한 ATX P_ON 및 TPMS 절연은 오버스펙같음

## 구현기능
* ATX PSU Off상태에서 버튼을 눌러 PSU와 BC250을 On
  * Arduino Nano는 ATX 5V상시전원에 연결
  * BC250은 AutoStart상태로 점퍼(AUTO_PWR_ON)적용
* BC250 On(ATX PSU On)상태에서 버튼을 눌러 PSU와 BC250을 Off
  * BC250의 Linux에서 Serical Port Listeng프로그램([serial_listener.py](https://github.com/coolsolgit/BC250-ATX-PSU-Control/blob/main/serial_listener.py)) 실행필요
  * BC250의 TPMS 3.3V전원을 Nano에 연결하여 AliveCheck Source로 사용
* ATX PSU On 상태에서 4초간 버튼을 눌러 강제로 PSU와 BC250을 Off

```mermaid
flowchart TD
A1((ATX PSU Off)) --> B1{버튼Press}
  B1 -->|전원켜기| C1[(Arduino Nano)]
  C1 -->|PS_ON HIGH| D1[ATX PSU On]
  D1 -->|AUTO_PWR_ON Jumper| E1[BC250 On]
A2((BC250 On)) --> B2{버튼Press}
  B2 -->|전원끄기| C2[(Arduino Nano)]
  C2 -->|TTL신호| D2[(USB to TTL)]
  D2 -->|TTL신호| E2[(Python Listener)]
  E2 -->|poweroff명령어호출| F2[BC250 Off]
  F2 -->|TPMS 3.3V LOW| G2[(Arduino Nano)]
  G2 -->|PS_ON LOW| H2[ATX PSU Off]
A3((ATX PSU On)) --> B3{버튼4초Press}
  B3 -->|강제종료| C3[(Arduino Nano)]
  C3 -->|PS_ON LOW| D3[ATX PSU Off]
  D3 -->|전원차단| E3[BC250 Off]
```


## 배선

TPMS 3.3V Standby
> TPMS 헤더에 듀퐁케이블로 연결(일반점퍼 규격보다 작은 PC메인보드의 USB3.0커넥터와 같은 2.0mm의 작은Pitch)

버튼 및 버튼LED
> 버튼LED의 전압에 맞는 PSU에서 뽑아서 사용

```mermaid
flowchart LR
  subgraph Arduino Nano
    P7[Pin D7]
    P8[Pin D8]
    P9[Pin D9]
    VIN[VIN]
    AGND[GND]
    TX[TX]
    ATGND[GND]
  end
  subgraph BC250
    TPMS[TPMS1 Pin9]
    BUSB[USB]
    PCIE[12V PCI-E]
    BGND[GND]
  end
  subgraph USB to TTL
    RX[RX]
    UGND[GND]
    UUSB[USB]
  end
  subgraph ATX PSU
    5V[5V Standby]
    3.3V[3.3V]
    PS_ON[PS_ON]
    PGND[GND]
    12V[12V]
  end
  subgraph Button
    BTN1[Button+]
    BTN2[Button-]
    LED1[LED+]
    LED2[LED-]
  end
  UUSB ---|USB케이블| BUSB
  TX --- RX
  P7 --- BTN1
  P8 --- PS_ON
  P9 --- TPMS
  AGND --- PGND
  AGND --- BTN2
  AGND --- LED2
  ATGND --- UGND
  3.3V ---|버튼LED허용전압선택| LED1
  5V --- VIN
  12V ---|PCI전원케이블| PCIE
  PGND ----|PCI전원케이블| BGND
```

## Serial port listener적용
CachyOS기준으로 설명

Pathon 및 Sereial Lib설치
```
sudo pacman -Syu
sudo pacman -S python
python --version
sudo pacman -S python-pyserial
```

파이썬코드가 권한없이 poweroff호출가능하도록 권한편집
```
sudo nano /etc/sudoers
```
맨아래줄추가
```
username ALL=(ALL) NOPASSWD: /usr/bin/systemctl poweroff, /usr/bin/shutdown
```

자동실행

crontab으로 등록
```
crontab -e
```
추가
```
@reboot /usr/bin/python3 /MY_PATH/serial_listener.py &
```

CachyOS는 crontab기본설치가 안되있음
```
sudo nano /etc/systemd/system/serial-listen.service
```
추가
```
[Unit]
Description=Shutdown signal monitor
After=network.target

[Service]
Type=simple
ExecStart=/usr/bin/python3 /MY_PATH/serial_listener.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

## BOM

| 명칭  | 가격 | 구매링크 |
| ------------- |:-------------:|-------------|
| Arduino Nano| \3000 | ali link |
| USB to Ttl | \3000 | ali link |
| Debounce Button | \2000 | ali link |


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
