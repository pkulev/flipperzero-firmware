#include <u8g2.h>
#include <assert.h>
#include <furi-hal.h>
#include <stm32wbxx_ll_utils.h>

static FuriHalSpiDevice* u8g2_periphery_display = NULL;

uint8_t u8g2_gpio_and_delay_stm32(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) {
    switch(msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
        /* HAL initialization contains all what we need so we can skip this part. */
        break;

    case U8X8_MSG_DELAY_MILLI:
        LL_mDelay(arg_int);
        break;

    case U8X8_MSG_DELAY_10MICRO:
        LL_mDelay(1);
        break;

    case U8X8_MSG_DELAY_100NANO:
        asm("nop");
        break;

    case U8X8_MSG_GPIO_RESET:
        hal_gpio_write(&gpio_display_rst, arg_int);
        break;

    default:
        return 0;
    }

    return 1;
}

uint8_t u8x8_hw_spi_stm32(u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr) {
    switch(msg) {
    case U8X8_MSG_BYTE_SEND:
        furi_hal_spi_bus_tx(u8g2_periphery_display->bus, (uint8_t*)arg_ptr, arg_int, 10000);
        break;

    case U8X8_MSG_BYTE_SET_DC:
        hal_gpio_write(&gpio_display_di, arg_int);
        break;

    case U8X8_MSG_BYTE_INIT:
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        assert(u8g2_periphery_display == NULL);
        u8g2_periphery_display =
            (FuriHalSpiDevice*)furi_hal_spi_device_get(FuriHalSpiDeviceIdDisplay);
        hal_gpio_write(u8g2_periphery_display->chip_select, false);
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        assert(u8g2_periphery_display);
        hal_gpio_write(u8g2_periphery_display->chip_select, true);
        furi_hal_spi_device_return(u8g2_periphery_display);
        u8g2_periphery_display = NULL;
        break;

    default:
        return 0;
    }

    return 1;
}
