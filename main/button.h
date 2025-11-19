#ifndef BUTTON_H
#define BUTTON_H

#define GPIO_BUTTON_1 GPIO_NUM_15
#define GPIO_BUTTON_2 GPIO_NUM_16

typedef enum {BUTTON_PRESSED, BUTTON_RELEASED} button_state_t;
void button_init(void);
button_state_t button_get1(void);
button_state_t button_get2(void);

#endif