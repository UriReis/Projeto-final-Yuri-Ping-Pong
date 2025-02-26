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

// Declaração de variáveis externas, definidas em outro arquivo
extern int a;               // Controla se o jogo está no menu (0) ou em execução (1)
extern float velocimulti;   // Controla a velocidade do jogo

void gpio_irq_handler(uint gpio, uint32_t events) {
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    static volatile uint32_t last_timeA = 0; // Armazena o tempo do último evento

    // Verifica se passaram pelo menos 200ms desde o último acionamento (evita múltiplos cliques acidentais)
    if (current_time - last_timeA > 200000) { 
        last_timeA = current_time; // Atualiza o tempo do último evento
        a = 1; // Altera o estado do jogo para iniciar
    }
}


void menu() {
    // Exibe os elementos do menu no display OLED
    exibir_menu("9", 60, 15);         // Ícone gráfico (opcional)
    exibir_menu("PONG GAME", 28, 30); // Nome do jogo
    exibir_menu("PRESS A", 36, 50);   // Instrução para o jogador iniciar o jogo

    // Configura uma interrupção para detectar quando o botão A for pressionado
    gpio_set_irq_enabled_with_callback(Botao_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Reinicia a velocidade do jogo ao entrar no menu
    velocimulti = 1.0;
}
