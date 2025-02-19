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
float speed_multiplier = 1.0;

// Função de interrupção para aumentar a velocidade
bool repeating_timer_callback(struct repeating_timer *t) {
    speed_multiplier += 0.1;
    printf("oi");
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

void reset_bola() {
    ball_x = 64;
    ball_y = 32;
    ball_dx = (rand() % 2) ? 1 : -1; // Direção aleatória para reinício
    ball_dy = 0; // Mantém trajetória inicial reta
    speed_multiplier = 1.0; // Reseta a velocidade
}

void func() {
    // Leitura do joystick nos eixos X e Y
    adc_select_input(0);
    uint16_t vrxvalue_you = adc_read();
    int pos_y = 28 - ((vrxvalue_you - 2048) * 28) / 2048;
    if (pos_y < 8) pos_y = 8;
    if (pos_y > 48) pos_y = 48;
    
    // Movimento da bola
    ball_x += (int)(ball_dx * speed_multiplier);
    ball_y += (int)(ball_dy * speed_multiplier);
    
    // Colisão com as laterais
    if (ball_y <= 3 || ball_y >= 58) {
        ball_dy = -ball_dy;
    }
    
    // Colisão com a barra do jogador
    if (ball_x <= 12 && ball_y + 2 >= pos_y && ball_y <= pos_y + 8) {
        ball_x = 12;
        ball_dx = -ball_dx;
        int offset = ball_y - (pos_y + 4);
        ball_dy = offset / 3; // Ajuste baseado na posição da colisão na barra
    }
    
    // Movimento do oponente baseado na posição da bola
    int opponent_y = ball_y - 4;
    if (opponent_y < 8) opponent_y = 8;
    if (opponent_y > 48) opponent_y = 48;
    
    // Colisão com a barra do oponente
    if (ball_x >= 107 && ball_y + 2 >= opponent_y && ball_y <= opponent_y + 8) {
        ball_x = 107;
        ball_dx = -ball_dx;
        int offset = ball_y - (opponent_y + 4);
        ball_dy = (offset / 3) + (rand() % 3 - 1); // Pequena variação para imprevisibilidade
    }
    
    // Se a bola passar do jogador ou do oponente, reinicia
    if (ball_x < 2 || ball_x > 122) {
        reset_bola();
    }
    
    // Atualiza o display
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(&ssd, "Z", 5, pos_y);
    ssd1306_draw_string(&ssd, "Y", 116, opponent_y);
    ssd1306_draw_string(&ssd, "X", ball_x, ball_y);
    ssd1306_send_data(&ssd);
}
