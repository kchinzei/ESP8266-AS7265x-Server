/*
  The MIT License (MIT)
  Copyright (c) Kiyo Chinzei (kchinzei@gmail.com)
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
*/
/*
  AS7265x_Bulb.h
  Make Asayake to Wake Project.
  Kiyo Chinzei
  https://github.com/kchinzei/ESP8266-AS7265x-Server
*/

#ifndef _as7265x_bulb_h_
#define _as7265x_bulb_h_

#ifdef _AS726XX_H
  #define AS726XX_CLASS AS726XX
#elif defined(_SPARKFUN_AS7265X_H)
  #define AS726XX_CLASS AS7265X
#elif defined(_AS726X_h)
  #define AS726XX_CLASS AS726X
#else
  Please include all or either of AS726XX.h, SparkFun_AS7265X.h and/or AS726X.h before including this file.
#endif


static uint8_t AS7265xBulb_Current[] = {0,
    AS7265X_LED_CURRENT_LIMIT_12_5MA,
    AS7265X_LED_CURRENT_LIMIT_25MA,
    AS7265X_LED_CURRENT_LIMIT_50MA,
    AS7265X_LED_CURRENT_LIMIT_100MA};

class AS7265xBulb
{
public:
    int getState() {
        return current_index != 0;
    }

    void setState(int onoff) {
        if (!sensor) return;
        if (onoff) {
            current_index = 1;
            sensor->setBulbCurrent(AS7265xBulb_Current[current_index], bulbtype);
            sensor->enableBulb(bulbtype);
        } else {
            sensor->disableBulb(bulbtype);
            current_index = 0;
        }
    }

    void toggle() {
        setState(!getState());
    }

    void toggleUp() {
        if (!sensor) return;
        if (current_index+1 >= current_limit_index) {
            sensor->disableBulb(bulbtype);
            current_index = 0;
        } else {
            current_index++;
            sensor->setBulbCurrent(AS7265xBulb_Current[current_index], bulbtype);
            if (current_index == 1)
                sensor->enableBulb(bulbtype);
        }
        // print_bulb();
    }

    void init(AS726XX_CLASS *sensor, uint8_t ledtype, uint8_t limit = 0b1111) {
        this->sensor = sensor;
        bulbtype = ledtype;

        if (sensor->isBulbAvailable(ledtype)) {
            // Set H/W limit
            // https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library/blob/master/examples/Example3_Settings/Example3_Settings.ino
            switch (ledtype) {
            case AS7265x_LED_WHITE:
                current_limit_index = 5; // no limit
                break;
            case AS7265x_LED_IR:
                current_limit_index = 4; // 100 mA not allowed
                break;
            case AS7265x_LED_UV:
                current_limit_index = 2; // 25 mA not allowed
                break;
            default:
                // error. you can typeguard by enum ledtype but not that serious.
                exit(1);
                break;
            }
            // Apply user-set limit if it's smaller than H/W limit
            int i;
            for (i=1; i<current_limit_index; i++) {
                if (AS7265xBulb_Current[i] == limit) {
                    current_limit_index = i+1;
                    break;
                }
            }
        }
        current_index = 0;
        this->sensor->disableBulb(bulbtype);
    }

    AS7265xBulb() {
        this->sensor = nullptr;
        this->bulbtype = AS7265x_LED_WHITE;
        this->current_index = 0;
        this->current_limit_index = 0;
    }
    
    ~AS7265xBulb() {
        // You may want turn off it. But we don't know if sensor is still valid when this destructor called. Leave it as is.
    }

    uint8_t getBulbtype() { return bulbtype; }

    const char *getBulbtypeString() {
        const char *ret;
        switch (bulbtype) {
        case AS7265x_LED_WHITE:
            ret = "White";
            break;
        case AS7265x_LED_IR:
            ret = "IR";
            break;
        case AS7265x_LED_UV:
            ret = "UV";
            break;
        }
        return ret;
    }

    int getCurrentIndex() { return current_index; }

    float getCurrent() {
        float mA = 0;
        if (getState()) {
            switch (AS7265xBulb_Current[current_index]) {
            case AS7265X_LED_CURRENT_LIMIT_12_5MA:
                mA = 12.5; break;
            case AS7265X_LED_CURRENT_LIMIT_25MA:
                mA = 25; break;
            case AS7265X_LED_CURRENT_LIMIT_50MA:
                mA = 50; break;
            case AS7265X_LED_CURRENT_LIMIT_100MA:
                mA = 100; break;
            }
        }
        return mA;
    }

    void print_bulb() {
        Serial.print(" -- bulb [");
        Serial.print(bulbtype);
        Serial.print("] state: ");
        Serial.print(getState());
        Serial.print(" (");
        Serial.print(getCurrent());
        Serial.print("mA)");
        Serial.println();
    }

protected:
    AS726XX_CLASS *sensor;
    uint8_t bulbtype;
    int current_index;
    int current_limit_index;
};

#endif
