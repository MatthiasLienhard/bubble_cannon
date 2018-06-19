# bubble_cannon
remote control bubble machine with rfduino
* hacking this device: 
  * https://www.amazon.de/Schramm-Seifenblasenmaschine-Seifenblasen-Seifenblasenkanone-Seifenblasenpistole/dp/B0749FBRF5/ref=sr_1_1_sspa?ie=UTF8&qid=1529399392&sr=8-1-spons&keywords=seifenblasenmaschine&psc=1
  * powered by 4*1.5V A Bateries = 6V
  * current: ~1.5A  (wtf!!! 10 Wats? Its just a fan)
  * enough space next to the motor for microcontroller
  * bubble revolver with 10 rings, one turn takes ?? seconds
* rfduino
  * based on nordic nRF51822
  * runs on 3V
  * ble ultra low power (good for standby mode)
  * arduino IDE
  * add hardware button 
    * fire for certain time (eg 10 sec)
    * block the button after fire for certain time
    * plug: headphone jack?
    * maybe add led
  * small smartphone app to controll modes:
    * fire once (x sec)
    * fire interval (x sec, y sec breaks)
    * fire random (x sec, y sec mean breaks, z break sd breaks)
    * controll button action
* additional components:
  * voltage regulater (dropdown to 3V)
  * button with led in small case
  * relais
  * maybe additional fan to blow bubbles up
  
