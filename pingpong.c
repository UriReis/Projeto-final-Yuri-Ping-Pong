#include <stdio.h>         
#include <stdlib.h>        
#include "pico/stdlib.h"   // Biblioteca do Raspberry Pi Pico para funções básicas
#include "hardware/adc.h"  // Biblioteca para controle do ADC (Conversor Analógico-Digital)
#include "hardware/timer.h"
#include "hardware/i2c.h"  // Biblioteca para comunicação via I2C
#include "include/ssd1306.h" 
#include "include/font.h"  // Biblioteca de fontes para o display
#include "define.h"        // Arquivo de definições 
#include "inicia.h"        // Arquivo de inicialização dos periféricos
#include "func.h"




int main()
{

    stdio_init_all(); 

    inicia();  // Chama a função de inicialização geral do sistema (configuração de GPIOs, ADC, etc.)
    inii2();



    while (true) {
        
        
        func();
        sleep_ms(10);
    }
}
