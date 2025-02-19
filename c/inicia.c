#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "define.h"

void inicia()
{
    // Configura o Botão A como entrada com pull-up interno
    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);

    // Configura os pinos dos LEDs (Vermelho, Verde e Azul) como saída
    gpio_init(pinR);
    gpio_set_dir(pinR, GPIO_OUT);
    gpio_init(pinG);
    gpio_set_dir(pinG, GPIO_OUT);
    gpio_init(pinB);
    gpio_set_dir(pinB, GPIO_OUT);

    // Configura o botão do joystick como entrada com pull-up interno
    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB); 

    // Inicializa o ADC para leitura dos eixos X e Y do joystick
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN); 
    adc_gpio_init(JOYSTICK_Y_PIN); 


}

