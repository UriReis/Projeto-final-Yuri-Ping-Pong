#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define c 3



// Matriz dos números de 0 a 9:
bool led_buffer[2][25] = {
    
    {
    1, 0, 0, 0, 1, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 0, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,},

    {
    0, 1, 1, 1, 0, 
    1, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0,},

    
};


// Envia um valor de pixel para o PIO (Programmable I/O) state machine.
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Converte valores de cor RGB (Red, Green, Blue) em um único valor de 32 bits no formato GRB.
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Função para "Pintar" os leds:
void set_one_led(uint8_t r, uint8_t g, uint8_t b, int iterador)
{
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {

        if (led_buffer[iterador][i])
        {
            put_pixel(color); // Liga o LED com um no buffer
        }
        else
        {
            put_pixel(0);  // Desliga os LEDs com zero no buffer
        }
    }
}

void limpa_led(uint8_t r, uint8_t g, uint8_t b){
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++)
    {

      put_pixel(0);  // Desliga os LEDs com zero no buffer
    }

}
// Inicialização do pio:
void pio(){
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}