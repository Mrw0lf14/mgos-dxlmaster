#include <stdbool.h>
#include <stdio.h>

#include "mgos.h"
#include "mgos_app.h"
#include "mgos_gpio.h"
#include "mgos_timers.h"
#include "mgos_uart.h"
#include "mgos_dxlmaster.h"

extern bool mgos_rpc_uart_init(void);

#define UART_NO 0
#define RPC_CALL_LEN 9
#define BUF_SIZE (1024)

const uint8_t rpc_call[RPC_CALL_LEN] = {0x22, 0x22, 0x22, 0x04, 0x22, 0x22,
                                        0x22, 0x0A, 0x0A};

static void rpc_check(uint8_t *data)
{
    for (uint8_t i = 0; i < RPC_CALL_LEN; i++) {
        if (data[i] != rpc_call[i]) return;
    }

    mgos_dxl_master_enable(0);
    uint8_t res = mgos_rpc_uart_init();
    LOG(LL_INFO, ("DXL disabled\n"));
    LOG(LL_INFO, ("RPC_UART_Init: %d", res));
}

static void uart_dispatcher(int uart_no, void *arg)
{
    uint16_t len = 0;
    uint16_t read = 0;
    uint8_t data[BUF_SIZE];

    len = mgos_uart_read_avail(uart_no);
    if (len == 0) return;

    read = mgos_uart_read(uart_no, data, len);

    if (read == RPC_CALL_LEN) rpc_check(data);

    mgos_dxl_uartUserCb(len, data);

    (void)arg;
}


bool mgos_mgos_dxlmaster_init(void)
{
    mgos_gpio_set_mode(5, MGOS_GPIO_MODE_OUTPUT);

    struct mgos_uart_config ucfg;
    mgos_uart_config_set_defaults(UART_NO, &ucfg);

    if (!mgos_uart_configure(UART_NO, &ucfg)) {
        LOG(LL_ERROR, ("Failed to configure UART%d", UART_NO));
    } else {
        LOG(LL_INFO, ("UART init success!\n"));
    }

    mgos_uart_set_dispatcher(UART_NO, uart_dispatcher, NULL);
    mgos_uart_set_rx_enabled(UART_NO, true);

    return true;
}
