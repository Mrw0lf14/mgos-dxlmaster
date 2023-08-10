#include "mgos_dxlmaster.h"
#include "mgos_rpc.h"

/* Dynamixel Devices **********************************************************/
/******************************************************************************/

DynamixelDevice *mgos_dxl_module_create(int model_id, int ver)
{
    return new DynamixelDevice(model_id, ver);
}

void mgos_dxl_master_begin(uint32_t baud)
{
    DxlMaster.begin(baud);
}

void mgos_dxl_master_enable(uint8_t state)
{
    DxlMaster.setInterfaceEnable(state);
}

DynamixelStatus mgos_dxl_init(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->init();
}

void mgos_dxl_communicationSpeed(DynamixelDevice *module, uint32_t baud)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return;
    }
    module->communicationSpeed(baud);
}


DynamixelStatus mgos_dxl_status(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->status();
}

DynamixelStatus mgos_dxl_ping(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->ping();
}

DynamixelStatus mgos_dxl_reset(DynamixelDevice *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->reset();
}

uint8_t mgos_dxl_read(DynamixelDevice *module, uint8_t reg)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    uint8_t data = 0;
    module->read(reg, 1, &data);
    return data;
}

uint16_t mgos_dxl_read16(DynamixelDevice *module, uint8_t reg)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    uint16_t data;
    module->read(reg, 2, (uint8_t *)&data);
    return data;
}


DynamixelStatus mgos_dxl_write(DynamixelDevice *module, uint8_t reg, uint8_t var)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->write(reg, 1, &var);
}

DynamixelStatus mgos_dxl_write16(DynamixelDevice *module, 
                                 uint8_t reg, 
                                 uint16_t var)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->write(reg, 2, (uint8_t *)&var);
}

/* Dynamixel direct operations ************************************************/
/******************************************************************************/
uint8_t mgos_dxl_direct_ping(uint8_t ver, uint8_t id)
{
    return DxlMaster.ping(ver, id);
}

void mgos_dxl_direct_begin(uint32_t baud)
{
    DxlMaster.begin(baud);
}

uint8_t mgos_dxl_direct_read(uint8_t id, uint8_t reg, 
                             uint8_t *ptr, uint8_t size) 
{
    return DxlMaster.read(id, reg, size, ptr);
}

uint8_t mgos_dxl_direct_write_byte(uint8_t id, uint8_t reg, uint8_t byte) 
{
    return DxlMaster.write(id, reg, 1, &byte);
}

uint8_t mgos_dxl_direct_write(uint8_t id, uint8_t reg, 
                              const uint8_t *ptr, uint8_t size) 
{
    return DxlMaster.write(id, reg, size, ptr);
}


/* Dynamixel Console **********************************************************/
/******************************************************************************/

DynamixelConsole *mgos_dxl_console_create(void)
{
    return new DynamixelConsole();
}

void mgos_dxl_console_loop(DynamixelConsole *console)
{
    if (console == nullptr) return;
    console->loop();
}

/* Dynamixel Motor ************************************************************/
/******************************************************************************/

DynamixelMotor *mgos_dxl_motor_create(int motor_id, int ver)
{
    return new DynamixelMotor(motor_id, ver);
}

DynamixelStatus mgos_dxl_motor_init(DynamixelMotor *module)
{
    if (module == nullptr) {
        module->setStatus(DYN_STATUS_SOFT_ERROR);
        return 0;
    }
    return module->init();
}

void mgos_dxl_motor_wheelMode(DynamixelMotor *motor)
{
    if (motor == nullptr) return;
    motor->wheelMode();
}

void mgos_dxl_motor_jointMode(DynamixelMotor *motor, 
                              uint16_t aCWLimit, 
                              uint16_t aCCWLimit)
{
    if (motor == nullptr) return;
    motor->jointMode(aCWLimit, aCCWLimit);
}

void mgos_dxl_motor_enableTorque(DynamixelMotor *motor, bool aTorque)
{
    if (motor == nullptr) return;
    motor->enableTorque(aTorque);
}

void mgos_dxl_motor_speed(DynamixelMotor *motor, int16_t aSpeed)
{
    if (motor == nullptr) return;
    motor->speed(aSpeed);
}

void mgos_dxl_motor_goalPosition(DynamixelMotor *motor, uint16_t aPosition)
{
    if (motor == nullptr) return;
    motor->goalPosition(aPosition);
}

void mgos_dxl_motor_led(DynamixelMotor *motor, uint8_t aState)
{
    if (motor == nullptr) return;
    motor->led(aState);
}

uint16_t  mgos_dxl_motor_currentPosition(DynamixelMotor *motor)
{
    if (motor == nullptr) return 0;
    return motor->currentPosition();
}

DynamixelStatus  mgos_dxl_motor_getCurrentPosition(DynamixelMotor *motor, 
                                                   uint16_t *pos)
{
    if (motor == nullptr) return 0;
    return motor->getCurrentPosition(pos);
}

DynamixelStatus  mgos_dxl_motor_setComplianceMargins(DynamixelMotor *motor, 
                                                     uint8_t cw_margin, 
                                                     uint8_t ccw_margin, 
                                                     uint8_t cw_slope, 
                                                     uint8_t ccw_slope)
{
    if (motor == nullptr) return 0;
    return motor->setComplianceMargins(cw_margin, ccw_margin, 
                                       cw_slope, ccw_slope);
}

DynamixelStatus  mgos_dxl_motor_getComplianceMargins(DynamixelMotor *motor, 
                                                     uint8_t *cw_margin, 
                                                     uint8_t *ccw_margin, 
                                                     uint8_t *cw_slope, 
                                                     uint8_t *ccw_slope)
{
    if (motor == nullptr) return 0;
    return motor->getComplianceMargins(cw_margin, ccw_margin, 
                                       cw_slope, ccw_slope);
}

DynamixelStatus  mgos_dxl_motor_isMoving(DynamixelMotor *motor, uint8_t *moving)
{
    if (motor == nullptr) return 0;
    return motor->isMoving(moving);
}


/* UART Dispatcher ************************************************************/
/******************************************************************************/

void mgos_dxl_setUserUartCb(userUartCb_t cb, void *arg)
{
  DxlMaster.setUserUartDispatcherCB(cb, arg);
}


void mgos_dxl_uartUserCb(uint16_t len, uint8_t *data)
{
   DxlMaster.uartUserCb(len, data);
}