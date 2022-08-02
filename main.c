#include <stdio.h>
#include "pico/stdlib.h"

/* Prototipos para funciones de ultrasonido */
void ultrasonico_init(uint8_t trigger, uint8_t echo);
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo);

/*
 *  @brief  Programa principal
 */
int main(void) {

    stdio_init_all();

    const uint8_t TRIGGER = 0;
    const uint8_t ECHO = 1;

    ultrasonico_init(TRIGGER, ECHO);
    gpio_init(28);
    gpio_init(27);
    gpio_init(26);
    gpio_set_dir(28,true);
    gpio_set_dir(27,true);
    gpio_set_dir(26,true);
              

    while(1) {
        float enrique = ultrasonico_get_distance_cm(TRIGGER, ECHO); 
        if(enrique < 10){
            gpio_put(28,true);
            gpio_put(27,false);
            gpio_put(26,false);
        }
        if(enrique > 10 && enrique < 50){
            gpio_put(28,false);
            gpio_put(27,true);
            gpio_put(26,false);
        }
        if(enrique > 50 && enrique < 100) {
            gpio_put(28,false);
            gpio_put(27,false);
            gpio_put(26,true);
        }
        sleep_ms(500);
    }
}

/*
 *  @brief  Inicializa los pines del ultrasonico
 *
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 */
void ultrasonico_init(uint8_t trigger, uint8_t echo) {
    /* Habilito el pin de Trigger */
    gpio_init(trigger);  
    /* Configuro el Trigger como salida */
    gpio_set_dir(trigger, true);
    /* Pongo un 0 en el pin de Trigger */
    gpio_put(trigger, false);
    /* Habilito el pin de Echo */
    gpio_init(echo);
    /* Configuro el Echo como entrada */
    gpio_set_dir(echo, false);
}

/*
 *  @brief  Obtiene la distancia en cm
 *  
 *  @param  trigger: numero de GP para usar de TRIGGER
 *  @param  echo: numero de GP para usar de ECHO
 * 
 *  @return distancia en cm
 */
float ultrasonico_get_distance_cm(uint8_t trigger, uint8_t echo) {
    /* Escribo un 1 en el Trigger */
    gpio_put(trigger, true);
    /* Espero 10 us con el pulso encendido */
    sleep_us(10);
    /* Escribo un 0 en el Trigger */
    gpio_put(trigger, false);
    /* Espero a que el pulso llegue al Echo */
    while(!gpio_get(echo));
    /* Mido el tiempo de cuando llega el pulso */
    absolute_time_t from = get_absolute_time();
    /* Espero a que el pulso del Echo baje a cero */
    while(gpio_get(echo));
    /* Mido el tiempo cuando termina el pulso */
    absolute_time_t to = get_absolute_time();
    /* Saco la diferencia de tiempo (el ancho del pulso) en us */
    int64_t pulse_width = absolute_time_diff_us(from, to);
    /* Calculo la distancia y la devuelvo */
    return pulse_width / 59.0;
}