#include "ruuvi_driver_error.h"
#include "ruuvi_interface_gpio.h"
#include "ruuvi_interface_yield.h"
#include "task_button.h"
#include <stddef.h>

static task_button_fp_t button_callback = NULL;

// Wrapper to gpio interrupt function
static void on_button(ruuvi_interface_gpio_evt_t event)
{
  if(NULL != button_callback) { button_callback(); }

  // Poor man's debounce
  ruuvi_platform_delay_ms(50);
}

ruuvi_driver_status_t task_button_init(ruuvi_interface_gpio_slope_t slope, task_button_fp_t action)
{
  ruuvi_driver_status_t err_code = RUUVI_DRIVER_SUCCESS;
  button_callback = action;
  err_code |= ruuvi_platform_pin_interrupt_init();
  err_code |= ruuvi_platform_gpio_interrupt_enable(RUUVI_BOARD_BUTTON_1, slope, RUUVI_INTERFACE_GPIO_MODE_INPUT_PULLUP, on_button);
  return err_code;
}