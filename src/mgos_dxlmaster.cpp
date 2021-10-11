#include "mgos_dxlmaster.h"
#include "mgos_rpc.h"

DynamixelConsole *mgos_dxl_console_create(void)
{
    return new DynamixelConsole();
}

void mgos_dxl_console_loop(DynamixelConsole *console)
{
    if (console == nullptr) {
        return;
    }
    console->loop();
}


DynamixelDevice *mgos_dxl_module_create(int model_id)
{
    return new DynamixelDevice(model_id);
}

void mgos_dxl_master_begin(uint32_t baud)
{
    DxlMaster.begin(baud);
}

void mgos_dxl_master_enable(uint8_t state)
{
    DxlMaster.setInterfaceEnable(state);
}

uint8_t mgos_dxl_init(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->init();
}

uint8_t mgos_dxl_status(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->status();
}

uint8_t mgos_dxl_ping(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->ping();
}

uint8_t mgos_dxl_read(DynamixelDevice *module, uint8_t reg)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    uint8_t data = 0;
    module->read(reg, 1, (uint8_t *)&data);
    return data;
}

uint8_t mgos_dxl_write(DynamixelDevice *module, uint8_t reg, uint8_t var)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->write(reg, 1, &var);
}


void mgos_dxl_setUserUartCb(userUartCb_t cb, void *arg)
{
  DxlMaster.setUserUartDispatcherCB(cb, arg);
}


void mgos_dxl_uartUserCb(uint16_t len, uint8_t *data)
{
   DxlMaster.uartUserCb(len, data);
}