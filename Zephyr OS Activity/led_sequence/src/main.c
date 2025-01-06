/*
 * Copyright (c) 2024 Your Name
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Sleep time in milliseconds */
#define SLEEP_TIME_MS   1000

/* Number of LEDs in the sequence */
#define NUM_LEDS        8

/* Button configuration - already defined in base DTS */
#define BUTTON_NODE     DT_ALIAS(sw0)

/* LED configurations using our new aliases */
#define LED0_NODE DT_ALIAS(led_seq_0)
#define LED1_NODE DT_ALIAS(led_seq_1)
#define LED2_NODE DT_ALIAS(led_seq_2)
#define LED3_NODE DT_ALIAS(led_seq_3)
#define LED4_NODE DT_ALIAS(led_seq_4)
#define LED5_NODE DT_ALIAS(led_seq_5)
#define LED6_NODE DT_ALIAS(led_seq_6)
#define LED7_NODE DT_ALIAS(led_seq_7)

/* GPIO specifications for each LED */
static const struct gpio_dt_spec leds[] = {
    GPIO_DT_SPEC_GET(LED0_NODE, gpios),
    GPIO_DT_SPEC_GET(LED1_NODE, gpios),
    GPIO_DT_SPEC_GET(LED2_NODE, gpios),
    GPIO_DT_SPEC_GET(LED3_NODE, gpios),
    GPIO_DT_SPEC_GET(LED4_NODE, gpios),
    GPIO_DT_SPEC_GET(LED5_NODE, gpios),
    GPIO_DT_SPEC_GET(LED6_NODE, gpios),
    GPIO_DT_SPEC_GET(LED7_NODE, gpios),
};

/* Button specification */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);

static void configure_pins(void)
{
    int ret;

    /* Configure LEDs */
    for (int i = 0; i < NUM_LEDS; i++) {
        if (!gpio_is_ready_dt(&leds[i])) {
            printk("Error: LED%d device not ready\n", i);
            return;
        }

        ret = gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE);
        if (ret < 0) {
            printk("Error %d: Failed to configure LED%d\n", ret, i);
            return;
        }
    }

    /* Configure button */
    if (!gpio_is_ready_dt(&button)) {
        printk("Error: button device not ready\n");
        return;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        printk("Error %d: Failed to configure button\n", ret);
        return;
    }
}

static void set_led(int led_index, int state)
{
    gpio_pin_set_dt(&leds[led_index], state);
}

static void clear_all_leds(void)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        set_led(i, 0);
    }
}

static int is_button_pressed(void)
{
    return gpio_pin_get_dt(&button);
}

static void led_shift(void)
{
    int position = 0;
    int direction = 1;  // Initially set to shift right

    while (1) {
        clear_all_leds();
        set_led(position, 1);
        k_msleep(SLEEP_TIME_MS);

        // Check if the button is pressed
        if (is_button_pressed()) {
            direction = -1;  // Change direction to left on button press
        } else {
            direction = 1;  // Default back to shifting right if button isn't pressed
        }

        // Update the position
        position += direction;

        // Wrapping logic
        if (position >= NUM_LEDS) {
            position = 0;
        } else if (position < 0) {
            position = NUM_LEDS - 1;
        }
    }
}

int main(void)
{
    configure_pins();
    clear_all_leds();
    led_shift();
    return 0;
}