    #include <stdio.h>
    #include <stdlib.h>
    #include "pico/stdlib.h"  // Biblioteca do Raspberry Pi Pico para funções básicas
    #include "hardware/adc.h" // Biblioteca para controle do ADC (Conversor Analógico-Digital)
    #include "hardware/timer.h"
    #include "hardware/i2c.h" // Biblioteca para comunicação via I2C
    #include "include/ssd1306.h"
    #include "include/font.h" // Biblioteca de fontes para o display
    #include "define.h"       // Arquivo de definições
    #include "inicia.h"       // Arquivo de inicialização dos periféricos
    #include "func.h"

    extern int a;
    extern float velocimulti;

    void gpio_irq_handler(uint gpio, uint32_t events)
    {
        // Obtém o tempo atual em microssegundos
        uint32_t current_time = to_us_since_boot(get_absolute_time());
        static volatile uint32_t last_timeA = 0; // Armazena o tempo do último evento (em microssegundos)

        // Verifica se passou tempo suficiente desde o último evento

        if (current_time - last_timeA > 200000)
        { // Debouncing de 200ms
            last_timeA = current_time;

            a = 1;
        }
    }

    void menu()
    {

        exibir_menu("9", 60, 15);

        exibir_menu("PONG GAME", 28, 30);

        exibir_menu("PRESS A", 36, 50);

        gpio_set_irq_enabled_with_callback(Botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

        velocimulti = 1.0;
    }