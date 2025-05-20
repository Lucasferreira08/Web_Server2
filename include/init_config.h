#ifndef INIT_CONFIG_H
#define INIT_CONFIG_H

#define BUZZER_PIN 21
#define BUZZER_FREQUENCY 100

#define RED_PIN 12
#define BLUE_PIN 13

#define OUT_PIN 7

#include "ssd1306.h"
#include "pio_matrix.pio.h"     // Programa PIO para controlar a matriz de LEDs

void display_init(ssd1306_t *ssd);

void buzzer_init();

void init_pins();

uint pio_init(PIO pio);

#endif