# ESP8266-AS7265x-Server

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Hobby spectroscope using [SparkFun AS7265x](https://learn.sparkfun.com/tutorials/spectral-triad-as7265x-hookup-guide) (and similar AMS sensors), running web server on ESP8266 ([ESP-WROOM-02](https://www.switch-science.com/catalog/2346/), NodeMCU, etc.).

![ESP8266-AS7265x-Server](./Artwortks/screenshot.png "Sample screen of ESP8266-AS7265x-Server")

I use Arduino IDE, but it should run with other tools such as [PlatformIO](https://platformio.org).

## About [AS7265x](https://www.sparkfun.com/products/15050)

AS7265x has three optical sensors to obtain 18-band spectroscopic intensity between 410 and 940 nm, which means from UV to NIR. This is not a professional instrument which will cost x20 or more of this. But this is a [Maker](https://makezine.com)'s choice, isn't it?

![AS7265x](https://cdn.sparkfun.com/r/500-500/assets/parts/1/3/3/9/3/15050-SparkFun_Triad_Spectroscopy_Sensor_-_AS7265x__Qwiic_-01.jpg "Overview of AS7265x")

My personal interest is to use it to measure the chromaticity of LEDs in my [kch-rgbw-lib](https://github.com/kchinzei/kch-rgbw-lib) to accurately calculate composite LED colors. I need to do it in a dark room, that's why the chart is in very dark tone.

### On accuracy of AS7265x

Please note that every AS7265x is calibrated by using typical LEDs illuminating a 'white' object. As LED is far from the uniform flat spectrum, for the above purpose, you will need to re-calibrate each band to flatten the relative intensity of these bands.

## Requirements

- ESP8266 2MB flash or more.
- [AMS AS7265x Triad Spectral Sensor](https://www.sparkfun.com/products/15050) or,
- [AMS AS726x VIS/NIR Spectral Sensor](https://learn.sparkfun.com/tutorials/as726x-nirvi/all) or,
- [AMS AS7341 10-Channel Color Sensor](https://learn.adafruit.com/adafruit-as7341-10-channel-light-color-sensor-breakout/arduino)
- [SparkFun AS7265x Arduino Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library) - see note below
- [SparkFun AS726x Arduino Library](https://github.com/sparkfun/Sparkfun_AS726X_Arduino_Library) - optional
- [Adafruit_AS7341 Library](https://github.com/adafruit/Adafruit_AS7341) - optional
- [AS726XX-CommonLib](https://github.com/kchinzei/AS726XX-CommonLib) - wrapper for AS7265x/726x/7341
- [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)
- [AsyncWiFiManager](https://github.com/alanswx/ESPAsyncWiFiManager)
- [Arduino ESP8266 filesystem uploader](https://github.com/esp8266/arduino-esp8266fs-plugin) or equivalent
- [Chart.min.js](https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js)

#### [Feb-03, 2020] Workaround for a bug in [AS7265x Arduino Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library)

[AS7265x Arduino Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library) has a bug around the indicator LED that affects its while LED bulb.
While a bug fix has been pull-requested already, please use [my fix here](https://github.com/kchinzei/SparkFun_AS7265x_Arduino_Library) until the patch to be applied.

#### AS7262/7263 Support

It can also work with AS7262/7263 breakout boards. You need to download my [AS726XX-CommonLib](https://github.com/kchinzei/AS726XX-CommonLib) and [SparkFun AS726x Arduino Library](https://github.com/sparkfun/Sparkfun_AS726X_Arduino_Library) put them in your Arduino libraries directory.

#### AS7341 Support

AMS AS7341 is a newer spectral sensor after AS726xx series. It's modernized and a lot affordable than its brothers. You also need to download my [AS726XX-CommonLib](https://github.com/kchinzei/AS726XX-CommonLib) and [Adafruit_AS7341 Library](https://github.com/adafruit/Adafruit_AS7341).

## Installation

Very simplified flow is like this:
1. Wire AS7265x and ESP8266 as I2C; SDA/SCL of AS7265x to IO4/IO5 of ESP, and 3V3, GND.
1. Clone or download this repository.
1. Open ESP8266-AS7265x-Server.ino
1. You may want to change OTA password in this file. Find and modify `you_must_set_your_pw`, or simply delete this line.
1. You may also want to change Timezone and NTP servers.
1. Download [Chart.min.js](https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.min.js) and put into 'data' folder. Alternatively you can directly source it in index.html.
1. [Install Arduino core for ESP8266](https://github.com/esp8266/Arduino#installing-with-boards-manager) Select "ESP8266 Board" in Tools >> Board menu. I choose 2MB(FS:512KB OTA:~768KB) for "Flash size".
1. Also install [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer), [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) and [AsyncWiFiManager](https://github.com/alanswx/ESPAsyncWiFiManager).
1. Upload contents of 'data' folder to ESP8266. You can use [Arduino ESP8266 filesystem uploader](https://github.com/esp8266/arduino-esp8266fs-plugin).
1. Compile and burn ESP8266.
1. Connect your PC/smartphone to wifi SSID `ESP_XXXXXXX` with `XXXXXXX` an hardware identifier for your ESP8266. Connecting this will prompt you to enter your Wifi setting. You must do it within 180 seconds.
1. Once it is done, you should observe a blue LED on AS7265x blinks. If not, something wrong. If OK, you should be able to connect to your ESP8266 by http://esp8266.local. And the spectroscopy is updated every 1 second. Enjoy!

## To Dos

- Uploader for a text file containing custom calibration parameters.
- [Http update instead of OTA](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html#http-server)

## References

Some part of code is written by learning from

- [A Beginner's Guide to the ESP8266](https://tttapa.github.io/ESP8266/Chap01%20-%20ESP8266.html) by [Pieter](https://tttapa.github.io)
- [ESP32 Remote Control with WebSocket](https://m1cr0lab-esp32.github.io/remote-control-with-websocket/websocket-setup/) by [m1cr0lab](https://github.com/m1cr0lab)
- [ESP32: Webサーバ上でリアルタイムグラフ表示(Chart.js)](https://web.is.tokushima-u.ac.jp/wp/blog/2019/07/12/esp32-webサーバ上でグラフ表示chart-js/) [徳島大学技術支援部  辻 明典さま]

# License

The MIT License (MIT)
Copyright (c) K. Chinzei (kchinzei@gmail.com)
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
