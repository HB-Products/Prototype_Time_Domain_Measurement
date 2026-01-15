#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/logging/log.h>
#include <stdbool.h>

#ifndef USER_NODE
#define USER_NODE DT_PATH(zephyr_user)
#endif

static const struct gpio_dt_spec mux_a0 = GPIO_DT_SPEC_GET(USER_NODE, mux_a0_gpios);
static const struct gpio_dt_spec mux_a1 = GPIO_DT_SPEC_GET(USER_NODE, mux_a1_gpios);
static const struct gpio_dt_spec n_step_enable = GPIO_DT_SPEC_GET(USER_NODE, n_step_enable_gpios);
static const struct gpio_dt_spec step_generate = GPIO_DT_SPEC_GET(USER_NODE, step_generate_gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define SPIOP      SPI_WORD_SET(8) | SPI_TRANSFER_MSB

struct spi_dt_spec spispec = SPI_DT_SPEC_GET(DT_NODELABEL(gendev), SPIOP, 0);


bool sensor_init(void)
{
    int ret;

    // Initialize 
    if (!gpio_is_ready_dt(&mux_a0)) {
        return false;
    }
    
    ret = gpio_pin_configure_dt(&mux_a0, GPIO_OUTPUT_INACTIVE);

    if (ret < 0) {
        return false;
    }

    if (!gpio_is_ready_dt(&mux_a1)) {
        return false;
    }
    
    ret = gpio_pin_configure_dt(&mux_a1, GPIO_OUTPUT_INACTIVE);

    if (ret < 0) {
        return false;
    }

    if (!gpio_is_ready_dt(&n_step_enable)) {
        return false;
    }
    
    ret = gpio_pin_configure_dt(&n_step_enable, GPIO_OUTPUT_INACTIVE);

    if (ret < 0) {
        return false;
    }

        if (!gpio_is_ready_dt(&step_generate)) {
        return false;
    }
    
    ret = gpio_pin_configure_dt(&step_generate, GPIO_OUTPUT_INACTIVE);

    if (ret < 0) {
        return false;
    }

    return true;
}

int main(void)
{
    sensor_init();
    
    int err = spi_is_ready_dt(&spispec);
    if (!err) {
	    LOG_ERR("Error: SPI device is not ready, err: %d", err);
        return 0;
    }

    gpio_pin_set_dt(&n_step_enable, 1); //Set HIGH
    k_msleep(200);

    while (1) {
        gpio_pin_set_dt(&step_generate, 1); //Set HIGH
        k_msleep(200);
        gpio_pin_set_dt(&step_generate, 0); //Set LOW
        k_msleep(1000);
        gpio_pin_set_dt(&mux_a0, 1); //Set High
        gpio_pin_set_dt(&mux_a1, 1); //Set High
        k_msleep(200);
        gpio_pin_set_dt(&step_generate, 1); //Set HIGH
        k_msleep(200);
        gpio_pin_set_dt(&step_generate, 0); //Set LOW
        k_msleep(1000);
        gpio_pin_set_dt(&mux_a0, 0); //Set High
        gpio_pin_set_dt(&mux_a1, 0); //Set High
        k_msleep(200);
    }



}

