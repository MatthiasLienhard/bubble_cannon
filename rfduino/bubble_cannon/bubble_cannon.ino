#include <RFduinoBLE.h>
#include <Lazarus.h>

Lazarus Lazarus;
//pins
const int button = 3;
const int bubble_switch = 2;
const int led = 4;
const int debounce_time = 100; //button debouncing
byte button_state;
byte bubble_state;
unsigned int start_time = 0;
unsigned int press_time;
unsigned int interval_time = 30;
unsigned int block_time = 0;
unsigned int duration = 0;
//data send by ble
char * buf;
int buflen = 0;

// reset after 2 seconds button pressing
const int button_reset_time = 2000;
void start_bubbles(int sec) {
  digitalWrite(bubble_switch, LOW); //is on!!
  digitalWrite(led, HIGH); //is on!!
  RFduinoBLE.send("BUBBLES!!", 9);
  if (bubble_state == LOW) {
    start_time = millis();
    bubble_state = HIGH;
    duration = SECONDS(sec);
  } else {
    duration = SECONDS(sec) + (millis() - start_time);
  }
}

void stop_bubbles() {
  digitalWrite(bubble_switch, HIGH);
  digitalWrite(led, LOW);
  bubble_state = LOW;
  duration = 0;
}
void setup() {
  RFduinoBLE.deviceName = "BubbleCannon";
  RFduinoBLE.advertisementData = "42";
  // start the BLE stack
  RFduinoBLE.begin();
  pinMode(bubble_switch, OUTPUT);
  digitalWrite(bubble_switch, HIGH);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  pinMode(button, INPUT_PULLUP);
  digitalWrite(button, HIGH);//pullup
  RFduino_pinWake(button, LOW);

  
  button_state = HIGH; //defines not pressed state
  bubble_state = LOW; //no bubbles currently
}
void loop() {
  unsigned long now = millis();
  if (RFduino_pinWoke(button)) {
    delay(debounce_time);//ignore short button press
    RFduino_resetPinWake(button);
    if (digitalRead(button) != button_state) {
      RFduino_pinWake(button, button_state);
      button_state = ! button_state;
      if (button_state == LOW) { //button pressed
        RFduinoBLE.send("Button Pressed", 15);
        press_time = millis();
        if (block_time == 0 || start_time==0 || ( bubble_state == LOW && (millis() - start_time) > (duration + SECONDS( block_time)))) {
          start_bubbles(interval_time);
        }else{
           RFduinoBLE.send("blocked", 7);
        }
      } else if (millis() - press_time > button_reset_time) {
        stop_bubbles();

        RFduinoBLE.send("Button Interrupt", 16); //button Released
      }
    }
  }else if (bubble_state = HIGH && millis() - start_time > duration) {
    RFduinoBLE.send("timeout", 7);
    stop_bubbles();
  }
  unsigned int pausetime=HOURS(1);
  if(duration>0)
      pausetime=min(pausetime, duration-(now-start_time));
  char message [20];
  sprintf(message,"[sleep %d sec]      ",pausetime/1000);
  RFduinoBLE.send(message,16);
  RFduino_ULPDelay(pausetime);    
    
  if (Lazarus.lazarusArising()) {
    //RFduinoBLE.send("[Lazerus]", 10);
    BLE_request();
  }
}
void RFduinoBLE_onReceive(char *data, int len)
{
  buf = data;
  buflen = len;
  //RFduinoBLE.send("call lazarus",13);

  Lazarus.ariseLazarus();
  // if the first byte
  // 1 --> print sensor data
  // 2 --> print hours to next autostart, interval time and duration
  // 8 --> set repeat moisture level and maxrepeats
  // 9 --> set up interval
}
void BLE_request() {
  if (buf[0] == 0) {
    printOptions();
  } else if (buf[0] == 1) {
    if (buflen == 1) {
      start_bubbles(interval_time);
    } else {
      start_bubbles(buf[1]);
    }
  } else if (buf[0] == 2) {
    if (buflen == 1) {
      printInterval();
    } else {
      setInterval(buf[1]);
    }
  } else if (buf[0] == 3) {
    if (buflen == 1) {
      printBlockTime();
    } else {
      setBlockTime(buf[1]);
    }
  } else if (buf[0] == 4) {
    printTemp();
  } else if (buf[0] == 5) {
    printVCC();
  }
}
void setInterval(int sec) {
  interval_time = sec;
}
void setBlockTime(int sec) {
  block_time = sec;
}

void printOptions() {
  RFduinoBLE.send("00:          Options", 20);
  RFduinoBLE.send("01 [int]:  bubbles!!", 20);
  RFduinoBLE.send("02 [int]:   Interval", 20);
  RFduinoBLE.send("03 [int]: block time", 20);
  RFduinoBLE.send("04:print temperature", 20);
  RFduinoBLE.send("05:    print voltage", 20);
}
void printInterval() {
  char message[20];
  sprintf(message, "Interval: %03 seconds", interval_time);
  RFduinoBLE.send(message, 20);
}
void printBlockTime() {
  char message[20];
  sprintf(message, "Block: %03 seconds", block_time);
  RFduinoBLE.send(message, 20);
}
void printTemp() {
  RFduinoBLE.send("Temp: XXX*C", 12);
}
void printVCC() {
  RFduinoBLE.send("VCC: XXX V", 10);
}

