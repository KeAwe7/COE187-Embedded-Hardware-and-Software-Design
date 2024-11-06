#include <stdio.h>
#include "gpio.h"
#include "mxc_delay.h"
#include "mxc_device.h"

#define NUM_LEDS 8
#define DELAY_MS 1000
#define BUTTON_PORT MXC_GPIO4  // Port 4 for PB0
#define BUTTON_PIN MXC_GPIO_PIN_0  // Pin 0 for PB0 (P4.0)

// GPIO pin configuration structure
const mxc_gpio_cfg_t gpio_cfg_leds[NUM_LEDS] = {
    {MXC_GPIO1, MXC_GPIO_PIN_7, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO1, MXC_GPIO_PIN_8, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO1, MXC_GPIO_PIN_9, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO1, MXC_GPIO_PIN_10, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO0, MXC_GPIO_PIN_7, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO0, MXC_GPIO_PIN_8, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO0, MXC_GPIO_PIN_9, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH},
    {MXC_GPIO0, MXC_GPIO_PIN_14, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH}
};

// Button configuration for PB0 on P4.0
const mxc_gpio_cfg_t button_cfg = {BUTTON_PORT, BUTTON_PIN, MXC_GPIO_FUNC_IN, MXC_GPIO_PAD_PULL_UP, MXC_GPIO_VSSEL_VDDIOH};

void configure_gpio_pins() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (MXC_GPIO_Config(&gpio_cfg_leds[i]) != E_NO_ERROR) {
            printf("Error configuring LED pin %d\n", i);
        }
    }
    if (MXC_GPIO_Config(&button_cfg) != E_NO_ERROR) {
        printf("Error configuring button pin\n");
    }
}

void set_led(int led_index, int state) {
    mxc_gpio_cfg_t cfg = gpio_cfg_leds[led_index];
    if (state) {
        MXC_GPIO_OutSet(cfg.port, cfg.mask);
    } else {
        MXC_GPIO_OutClr(cfg.port, cfg.mask);
    }
}

void clear_all_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        set_led(i, 0);
    }
}

int is_button_pressed() {
    // Button is active low, so return 1 if pressed, 0 if not pressed
    return MXC_GPIO_InGet(BUTTON_PORT, BUTTON_PIN) == 0;
}

void led_shift() {
    int position = 0;
    int direction = 1;  // Initially set to shift right

    while (1) {
        clear_all_leds();
        set_led(position, 1);

        MXC_Delay(MXC_DELAY_MSEC(DELAY_MS));

        // Check if the button is pressed
        if (is_button_pressed()) {
            direction = -1;  // Change direction to left on button press
        } else {
            direction = 1;  // Default back to shifting right if button isn't pressed
        }

        // Update the position
        position += direction;

        // **Wrapping logic**: If we go out of bounds, wrap back around
        if (position >= NUM_LEDS) {
            position = 0;  // Wrap around to the first LED if we move right out of bounds
        } else if (position < 0) {
            position = NUM_LEDS - 1;  // Wrap around to the last LED if we move left out of bounds
        }
    }
}

int main() {
    // Initialize GPIO for LED ports (GPIO0 and GPIO1) and button port (GPIO4)
    if (MXC_GPIO_Init(0x13) != E_NO_ERROR) {  // 0x13 = 0b10011 (ports 0, 1, and 4)
        printf("Error initializing GPIO\n");
        return -1;
    }

    configure_gpio_pins();
    clear_all_leds();
    led_shift();

    return 0;
}
