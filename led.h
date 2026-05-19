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
CRGB *leds1;

uint16_t autoBright = 50;
uint16_t isUpBright = 1;

CRGB policeColors[3] = {CRGB(0, 0, 0),CRGB(255, 0, 0),CRGB(0, 0, 255)};
int policeArr[10][2] = {{1,0},{2,0},{0,1},{0,2},{1,0},{2,0},{0,1},{0,2},{1,2},{2,1}};


void led_init() {
    leds = (CRGB *)malloc(sizeof(CRGB));
    leds1 = (CRGB *)malloc(sizeof(CRGB));
    cled = &FastLED.addLeds<LED_CHIP, LED_PIN, LED_ORDER>(leds, 0);
    cled1 = &FastLED.addLeds<LED_CHIP, LED_PIN1, LED_ORDER>(leds1, 0);
}

void led_change() {
    leds = (CRGB *)realloc(leds, sizeof(CRGB) * data.led_amount);
    leds1 = (CRGB *)realloc(leds1, sizeof(CRGB) * data.led_amount);
    cled->setLeds(leds, data.led_amount);
    cled1->setLeds(leds1, data.led_amount);
}

void led_tick() {
    static GHtimer tmr(LED_PRD);
    if (tmr) {
        static uint16_t pos = 0;
        pos += data.max_spd * LED_PRD / 1000;  // (mm / s) * (ms / 1000) == mm
        if (data.max_spd == 0 and data.isPolice == false) {
            pos = 0;
        }

        uint16_t idx = pos * data.ledm / 1000;  // mm * (leds / m / 1000) == leds

        switch (data.mode) {
            case 0: {

                for (uint16_t i = 0; i < data.led_amount; i++) {
                    leds[i] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                    leds1[i] = leds[i];
                }
                cled->showLeds(data.led_bri);
                cled1->showLeds(data.led_bri);
            } break;

            
            case 1: {
                for (uint16_t i = 0; i < data.led_amount; i++) {
                    if (i > data.led_amount / 2) {
                      leds[data.led_amount - i - 1] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                      leds1[data.led_amount - i - 1] = leds[data.led_amount - i - 1];
                      continue;
                    }
                    leds[i] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                    leds1[i] = leds[i];
                }
                cled->showLeds(data.led_bri);
                cled1->showLeds(data.led_bri);
            }break;
            
            case 2: {
                cled->showColor(CHSV(255, 0, 0), data.led_bri);
                cled1->showColor(CHSV(255, 0, 0), data.led_bri);
            } break;

            case 3: {
              if (data.isPolice) {
                if (autoBright >= 9) {
                  autoBright = 0;
                }
                cled->showColor(policeColors[policeArr[autoBright][0]], data.led_bri);
                cled1->showColor(policeColors[policeArr[autoBright][1]], data.led_bri);
                
                if (autoBright >= 9) {
                  isUpBright = 0;
                  autoBright = 0;
                } else {
                  if (isUpBright >= 5) {
                    autoBright = autoBright + 1;
                    isUpBright = 0;
                  } else {
                    isUpBright = isUpBright + 1;
                  }
                }
              } else {
                if (data.max_spd == 0) {
                  static uint16_t pos = 0;
                  uint16_t idx = pos * data.ledm / 1000;  // mm * (leds / m / 1000) == leds
                  for (uint16_t i = 0; i < data.led_amount; i++) {
                    if (i < 5 or i > data.led_amount - 5){
                      leds[i] =  ColorFromPalette(paletteArr[31], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                      leds1[i] = leds[i];
                    } else {
                      leds[i] =  ColorFromPalette(paletteArr[32], 0, 255, LINEARBLEND);
                      leds1[i] = leds[i];
                    }
                  }
                  if (isUpBright == 1) {
                    autoBright = autoBright + 1;
                    if (autoBright == 255) {
                      isUpBright = 0;
                    }
                  }
                  if (isUpBright == 0) {
                    autoBright = autoBright - 1;
                    if (autoBright == 50) {
                      isUpBright = 1;
                    }
                  }
                  cled->showLeds(autoBright);
                  cled1->showLeds(autoBright);
                } else {
                  isUpBright = 0;
                  autoBright = 255;
                  for (uint16_t i = 0; i < data.led_amount; i++) {
                    leds[i] = ColorFromPalette(paletteArr[data.pal], 255 - (idx + i) * 255 / data.pal_len, 255, LINEARBLEND);
                    leds1[i] = leds[i];
                  }
                  cled->showLeds(data.led_bri);
                  cled1->showLeds(data.led_bri);
                }
              }
            }
            break;

        }
    }
}
