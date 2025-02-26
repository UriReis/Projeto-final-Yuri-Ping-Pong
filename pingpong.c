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
#include "menu.h"
#include "bibliopio.h"

// Variável global volátil para indicar o estado do jogo
volatile int a = 0; // a = 0 → Jogo no menu | a = 1 → Jogo em execução


int main() {
    // Inicializa a comunicação serial padrão (para debugging, se necessário)
    stdio_init_all(); 

    // Chama a função de inicialização geral do sistema (configuração de GPIOs, ADC, etc.)
    inicia();  

    // Inicializa a comunicação I2C e configura o display OLED
    inii2();   

    // Inicializa a matriz de LEDs utilizando o PIO (Programmable I/O)
    pio();  

    // Loop principal do programa
    while (true) {

        // Exibe o menu inicial e aguarda o jogador iniciar o jogo
        menu(); 
        
        // Quando o jogador inicia o jogo, executa a lógica do Pong
        while (a == 1) {
            func(); // Função responsável pela lógica do jogo
            sleep_ms(10); 
        }
        
        sleep_ms(10); 
    }
}
