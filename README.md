# SC-Led-Arduino

Arduino-based LED control firmware for electric scooter integration.  
Designed to work with **GyverHub** and mobile companion app **SC-Android**.

This firmware handles front/back LED states and provides remote lighting control over MQTT/WebSockets using a lightweight Arduino environment.

## Features

- Remote LED control via SC-Android
- Integration with GyverHub UI
- ESP8266/ESP32 support
- MQTT + WebSockets communication
- FastLED-based LED effects

## Dependencies

Arduino IDE + the following libraries:

- PubSubClient (`pubsubclient-2.8`)
- EEManager
- ArduinoWebSockets (`arduinoWebSockets-2.3.6`)
- GyverHub (`GyverHub-main`)
- FastLED (`FastLED-3.6.0`)

## Hardware

Tested with:
- ESP8266 (Generic ESP8266 Module)  
Compatible with ESP32 (board JSON link included)

## Installation

1. Install **Arduino IDE**

2. Add Boards JSON to Boards Manager:
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```
```
https://dl.espressif.com/dl/package_esp32_index.json
```

3. Install required libraries into:
`C:\Users\<User>\Documents\Arduino\libraries`

4. Select board:
`Generic ESP8266 Module`

5. Compile and flash.

## Related Projects

- GyverHub UI:
  https://github.com/GyverLibs/GyverHub/

- SC-Android companion app:
  https://github.com/denis-hik/SC-Android

## Preview

<img src="/photo.jpg" width="60%" />

