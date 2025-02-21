#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include "define.h"

ssd1306_t ssd; // Estrutura para controlar o display OLED

int ball_x = 64, ball_y = 32;
int ball_dx = 1, ball_dy = 1;
float velocimulti = 1.0;
int player_pontos = 0;
int cpu_pontos = 0;
const int max_pontos = 3;
int cpu_rebatidas = 0; // Contador de rebatidas da CPU

// Função de interrupção para aumentar a velocidade
bool repeating_timer_callback(struct repeating_timer *t) {
    velocimulti += 0.1;
    return true; // Continua a repetição do timer
}

void inii2() {
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o I2C com frequência de 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);
    
    // Configura um timer para chamar a função a cada 1 segundo
    static struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
}

void exibir_mensagem(const char *msg) {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, msg, 30, 30);
    ssd1306_send_data(&ssd);
    sleep_ms(3000);
}

void reset_bola() {
    ball_x = 64;
    ball_y = 32;
    ball_dx = (rand() % 2) ? 1 : -1; // Direção aleatória para reinício
    ball_dy = 0; // Mantém trajetória inicial reta
    velocimulti = 1.0; // Reseta a velocidade
    cpu_rebatidas = 0; // Reseta o contador de rebatidas da CPU
}

void verificar_vencedor() {
    if (player_pontos >= max_pontos) {
        exibir_mensagem("PLAYER WINS");
        player_pontos = 0;
        cpu_pontos = 0;
        reset_bola();
    } else if (cpu_pontos >= max_pontos) {
        exibir_mensagem("CPU WINS");
        exibir_mensagem("GAME OVER");
        player_pontos = 0;
        cpu_pontos = 0;
        reset_bola();
    }
}

void func() {
    // Leitura do joystick nos eixos X e Y
    adc_select_input(0);
    uint16_t vrxvalue_you = adc_read();
    int pos_y = 28 - ((vrxvalue_you - 2048) * 28) / 2048;
    if (pos_y < 8) pos_y = 8;
    if (pos_y > 48) pos_y = 48;
    
    // Movimento da bola
    ball_x += (int)(ball_dx * velocimulti);
    ball_y += (int)(ball_dy * velocimulti);
    
    // Colisão com as bordas superior e inferior
    if (ball_y <= 6) {
        ball_y = 6;
        ball_dy = -ball_dy;
    }
    if (ball_y >= 52) {
        ball_y = 52;
        ball_dy = -ball_dy;
    }
    
    // Colisão com a barra do jogador
    if (ball_x <= 12 && ball_y + 2 >= pos_y && ball_y <= pos_y + 8) {
        ball_x = 12;
        ball_dx = -ball_dx;
        int offset = ball_y - (pos_y + 4);
        ball_dy = offset / 3;
    }
    
    // Movimento do oponente baseado na posição da bola
    int opponent_y = ball_y - 4;
    if (opponent_y < 8) opponent_y = 8;
    if (opponent_y > 48) opponent_y = 48;
    
    // Colisão com a barra do oponente
    if (ball_x >= 107 && ball_y + 2 >= opponent_y && ball_y <= opponent_y + 8) {
        if (cpu_rebatidas < 6) {
            ball_x = 107;
            ball_dx = -ball_dx;
            int offset = ball_y - (opponent_y + 4);
            ball_dy = (offset / 3) + (rand() % 3 - 1);
            cpu_rebatidas++;
        } else {
           
            player_pontos++;
            exibir_mensagem("CPU ERROU!");
            reset_bola();
        }
    }
    
    // Se a bola passar do jogador ou do oponente, adiciona ponto e reinicia
    if (ball_x < 2) {
        cpu_pontos++;
        exibir_mensagem("PONTO CPU");
        reset_bola();
    }
    if (ball_x > 122) {
        player_pontos++;
        exibir_mensagem("PONTO P1");
        reset_bola();
    }
    verificar_vencedor();
    
    // Atualiza o display com o placar
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    char placar[20];
    sprintf(placar, "%d - %d", player_pontos, cpu_pontos);
    ssd1306_draw_string(&ssd, placar, 50, 0);
    ssd1306_draw_string(&ssd, "Z", 5, pos_y);
    ssd1306_draw_string(&ssd, "V", 116, opponent_y);
    ssd1306_draw_string(&ssd, "X", ball_x, ball_y);
    ssd1306_send_data(&ssd);
}
