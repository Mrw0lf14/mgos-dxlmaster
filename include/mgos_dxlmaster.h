#ifndef MGOS_DXLMASTER_H
#define MGOS_DXLMASTER_H

#include "mgos.h"

#ifdef __cplusplus
#include "DxlMaster.h"
#include "DynamixelConsole.h"
#else
typedef struct DynamixelDeviceTag DynamixelDevice;
typedef struct DynamixelConsoleTag DynamixelConsole;
typedef void (*userUartCb_t)(uint16_t, uint8_t *,  void *);
#endif

#ifdef __cplusplus
extern "C" {
#endif

DynamixelConsole *mgos_dxl_console_create(void);

void mgos_dxl_console_loop(DynamixelConsole *console);

DynamixelDevice *mgos_dxl_module_create(int model_id);

void mgos_dxl_master_begin(uint32_t baud);

void mgos_dxl_master_enable(uint8_t state);

uint8_t mgos_dxl_init(DynamixelDevice *module);

void mgos_dxl_communicationSpeed(DynamixelDevice *module, uint32_t baud);

uint8_t mgos_dxl_status(DynamixelDevice *module); 

uint8_t mgos_dxl_ping(DynamixelDevice *module);

uint8_t mgos_dxl_read(DynamixelDevice *module, uint8_t reg);

uint8_t mgos_dxl_write(DynamixelDevice *module, uint8_t reg, 
                       uint8_t var);

void mgos_dxl_setUserUartCb(userUartCb_t cb, void *arg);

void mgos_dxl_uartUserCb(uint16_t len, uint8_t *data);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif // MGOS_DXLMASTER_H