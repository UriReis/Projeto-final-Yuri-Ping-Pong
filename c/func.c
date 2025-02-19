#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include "define.h"

ssd1306_t ssd; // Estrutura para controlar o display OLED

int ball_x = 64, ball_y = 32;
int ball_dx = 1, ball_dy = 1;

void inii2()
{
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o I2C com frequência de 400 kHz

    // Configura os pinos SDA e SCL para função I2C com pull-up
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false); // Limpa o display
    ssd1306_send_data(&ssd);
}



void func()
{
    // Leitura do joystick nos eixos X e Y
    adc_select_input(0);
    uint16_t vrxvalue_you = adc_read();
    

    // Converte os valores do joystick para coordenadas no display
    
    int pos_y = 28 - ((vrxvalue_you - 2048) * 28) / 2048;

    // Limita os valores para manter o quadrado dentro da tela
   
    if (pos_y < 8) pos_y = 8;
    if (pos_y > 48) pos_y = 48;

    // Movimento da bola
    ball_x += ball_dx;
    ball_y += ball_dy;
    
    // Colisão com as laterais
    if (ball_x <= 3 || ball_x >= 122) ball_dx = -ball_dx;
    if (ball_y <= 3 || ball_y >= 58) ball_dy = -ball_dy;

    // Colisão com a barra do jogador
    if (ball_x <= 6 && ball_y >= pos_y && ball_y <= pos_y + 10) {
        ball_dx = -ball_dx;
    }
    
    // Movimento do oponente baseado na posição da bola
    int opponent_y = ball_y - 5;
    if (opponent_y < 8) opponent_y = 8;
    if (opponent_y > 48) opponent_y = 48;

    // Colisão com a barra do oponente
    if (ball_x >= 116 && ball_y >= opponent_y && ball_y <= opponent_y + 10) {
        ball_dx = -ball_dx;
    }
    
    // Se a bola passar do jogador, reinicia
    if (ball_x < 2) {
        ball_x = 64;
        ball_y = 32;
        ball_dx = 1;
        ball_dy = 1;
    }
    

    // Atualiza o display
    ssd1306_fill(&ssd, false); 
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false); // Desenha a borda

    // Se o botão foi pressionado, ativa modo alternativo
    
    

    // Calcula nova posição para o quadrado alternativo
        
    int pos_yPB = 27 - ((vrxvalue_you - 2048) * 28) / 2048;

    
        
    if (pos_yPB < 8) pos_yPB = 8;
    if (pos_yPB > 48) pos_yPB = 48;

    

    ssd1306_draw_string(&ssd, "Z", 5, pos_yPB);
    
    ssd1306_draw_string(&ssd, "Y", 116, opponent_y);
    ssd1306_draw_string(&ssd, "X", ball_x, ball_y);
    
    
    
    
 

    ssd1306_send_data(&ssd); // Atualiza o display
}
