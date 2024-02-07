#pragma once
#include <Arduino.h>
#include <FastLED.h>

#include "data.h"
#include "hub.h"
#include "palettes.h"
#include "tacho.h"

CLEDController *cled = nullptr;
CLEDController *cled1 = nullptr;
CRGB *leds;

void led_init() {
    leds = (CRGB *)malloc(sizeof(CRGB));
    cled = &FastLED.addLeds<LED_CHIP, LED_PIN, LED_ORDER>(leds, 0);
    cled1 = &FastLED.addLeds<LED_CHIP, LED_PIN1, LED_ORDER>(leds, 0);
}

void led_change() {
    leds = (CRGB *)realloc(leds, sizeof(CRGB) * data.led_amount);
    cled->setLeds(leds, data.led_amount);
    cled1->setLeds(leds, data.led_amount);
}

void led_tick() {
    static GHtimer tmr(LED_PRD);
    if (tmr) {
        switch (data.mode) {
            case 0: {
                static uint16_t pos = 0;
                pos += data.max_spd * LED_PRD / 1000;  // (mm / s) * (ms / 1000) == mm
                if (data.max_spd == 0) {
                  pos = 0;
                }
                
                uint16_t idx = pos * data.ledm / 1000;  // mm * (leds / m / 1000) == leds

                for (uint16_t i = 0; i < data.led_amount; i++) {
                    leds[i] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                }
                cled->showLeds(data.led_bri);
                cled1->showLeds(data.led_bri);
            } break;

            case 1: {
                uint16_t spd = min(tacho_speed(), (uint16_t)(35 * 278));
                cled->showColor(CHSV(map(data.max_spd, 0, 35 * 278, 120, 0), 255, 255), data.led_bri);
                cled1->showColor(CHSV(map(data.max_spd, 0, 35 * 278, 120, 0), 255, 255), data.led_bri);
            } break;

            
            case 2: {
               static uint16_t pos = 0;
                pos += data.max_spd * LED_PRD / 1000;  // (mm / s) * (ms / 1000) == mm
                if (data.max_spd == 0) {
                  pos = 0;
                }
                
                uint16_t idx = pos * data.ledm / 1000;  // mm * (leds / m / 1000) == leds

                for (uint16_t i = 0; i < data.led_amount; i++) {
                    if (i > data.led_amount / 2) {
                      leds[data.led_amount - i - 1] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                      continue;
                    }
                    leds[i] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                }
                cled->showLeds(data.led_bri);
                cled1->showLeds(data.led_bri);
            }break;
            
            case 3: {
                uint16_t spd = min(tacho_speed(), (uint16_t)(35 * 278));
                cled->showColor(CHSV(255, 0, 0), data.led_bri);
                cled1->showColor(CHSV(255, 0, 0), data.led_bri);
            } break;

            case4: {
              if (data.max_spd == 0) {
                for (uint16_t i = 0; i < data.led_amount; i++) {
                  static uint16_t pos = 0;
                  uint16_t idx = pos * data.ledm / 1000;  // mm * (leds / m / 1000) == leds
                  leds[i] =  ColorFromPalette(paletteArr[31], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                }
                cled->showLeds(data.led_bri);
              } break;
            }

        }
    }
}
