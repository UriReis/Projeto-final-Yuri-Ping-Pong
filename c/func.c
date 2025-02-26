#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "include/ssd1306.h"
#include "include/font.h"
#include "define.h"
#include "bibliopio.h"

#define BUZZER_PIN 21 // Porta associada ao Buzzer

ssd1306_t ssd; // Estrutura para controlar o display OLED

int ball_x = 64, ball_y = 32;
int ball_dx = 1, ball_dy = 1;
volatile float velocimulti = 1.0;
int player_pontos = 0;
int cpu_pontos = 0; 
const int max_pontos = 3;
int cpu_rebatidas = 0; // Contador de rebatidas da CPU
bool cpu_deixando_passar = false;





extern int a ;

// Inicializa o PWM para o buzzer
void buzzer_init() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    
}

// Toca um tom no buzzer
void buzz(uint freq, uint tempo) {
  
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint channel = pwm_gpio_to_channel(BUZZER_PIN);

    // Configurando a frequência
    uint32_t clock_freq = 125000000; 
    uint32_t divider = clock_freq / freq / 65536 + 1; 
    uint32_t top = clock_freq / (divider * freq);

    // Configurando as repetições
    pwm_set_clkdiv(slice_num, divider); 
    pwm_set_wrap(slice_num, top - 1);
    pwm_set_chan_level(slice_num, channel, top / 2);
    pwm_set_enabled(slice_num, true);

    sleep_ms(tempo);

    pwm_set_enabled(slice_num, false);
}



// Função de interrupção para aumentar a velocidade
bool repeating_timer_callback(struct repeating_timer *t) {
    velocimulti += 0.1;
    return true;
}

void inii2() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    buzzer_init(); // Inicializa o buzzer
    
    static struct repeating_timer timer;
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
}

void exibir_mensagem(const char *msg, int x, int y) {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, msg, x, y);
    ssd1306_send_data(&ssd);
    sleep_ms(3000);
}

void exibir_menu(const char *msg, int x, int y){
    ssd1306_draw_string(&ssd, msg, x, y);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_send_data(&ssd);

    
}


void reset_bola() {
    ball_x = 64;
    ball_y = 32;
    ball_dx = (rand() % 2) ? 1 : -1; // Direção aleatória para reinício
    ball_dy = 0; // Mantém trajetória inicial reta
    velocimulti = 1.0; // Reseta a velocidade
    cpu_rebatidas = 0; // Reseta o contador de rebatidas da CPU
    cpu_deixando_passar = false;
}

void verificar_vencedor() {
    if (player_pontos >= max_pontos) {
        set_one_led(0, 0, 200, 1);
        exibir_mensagem("PLAYER WINS",30,30);
        limpa_led(0, 0, 0);

        player_pontos = 0;
        cpu_pontos = 0;

        ssd1306_fill(&ssd, false);
        a = 0;

        reset_bola();
    } else if (cpu_pontos >= max_pontos) {
        set_one_led(200, 0, 0, 0);
        exibir_mensagem("CPU WINS",30,30);
        
        
        exibir_mensagem("GAME OVER",30,30);
        
        buzz(300,500);
        limpa_led(0, 0, 0);
        
        buzz(250,500);
        buzz(100,250);
        player_pontos = 0;
        cpu_pontos = 0;
        reset_bola();
        ssd1306_fill(&ssd, false);
        a = 0;
        
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
    
    // CPU segue a bola normalmente
    int opponent_y = ball_y - 4;
    if (opponent_y < 8) opponent_y = 8;
    if (opponent_y > 48) opponent_y = 48;
    
    // CPU decide errar na terceira rebatida
    if (cpu_rebatidas >= 8 && ball_x >= 85) {
        cpu_deixando_passar = true;
        opponent_y = (ball_y > 30) ? 48 : 8; // Move para longe ANTES da colisão
    }
    
    if (!cpu_deixando_passar) {
        // Colisão com a barra do oponente
        if (ball_x >= 107 && ball_y + 2 >= opponent_y && ball_y <= opponent_y + 8) {
            ball_x = 107;
            ball_dx = -ball_dx;
            int offset = ball_y - (opponent_y + 4);
            ball_dy = (offset / 3) + (rand() % 3 - 1);
            cpu_rebatidas++;
        }
    }
    
    // Se a bola passar do jogador ou do oponente, adiciona ponto e reinicia
    if (ball_x < 2) {
        cpu_pontos++;
        gpio_put(pinR, true);
        set_one_led(200, 0, 0, 0);
        exibir_mensagem("PONTO CPU",30,30);
        gpio_put(pinR, false);
        limpa_led(0, 0, 0);
        reset_bola();
    }
    if (ball_x > 122) {
        player_pontos++;
        gpio_put(pinB, true);
        set_one_led(0, 0, 200, 1);
        exibir_mensagem("PONTO P1",30,30);
        limpa_led(0, 0, 0);
        gpio_put(pinB, false);
        reset_bola();
    }
    
    
    // Atualiza o display com o placar
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    char placar[20];
    sprintf(placar, "%d - %d", player_pontos, cpu_pontos);
    ssd1306_draw_string(&ssd, placar, 50, 0);
    ssd1306_draw_string(&ssd, "Z", 5, pos_y);
    ssd1306_draw_string(&ssd, "H", 116, opponent_y);
    ssd1306_draw_string(&ssd, "X", ball_x, ball_y);
    ssd1306_send_data(&ssd);

    verificar_vencedor();
}
