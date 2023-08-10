let DynamixelDevice = {
    _create: ffi('void *mgos_dxl_module_create(int, int)'),
    _init: ffi('int mgos_dxl_init(void *)'),
    _speed: ffi('void mgos_dxl_communicationSpeed(void *, int)'),
    _status: ffi('int mgos_dxl_status(void *)'),
    _ping: ffi('int mgos_dxl_ping(void *)'),
    _reset: ffi('int mgos_dxl_reset(void *)'),
    _read: ffi('int mgos_dxl_read(void *, int)'),
    _read16: ffi('int mgos_dxl_read16(void *, int)'),
    _write: ffi('int mgos_dxl_write(void *, int, int )'),
    _write16: ffi('int mgos_dxl_write16(void *, int, int )'),

    _proto: {

        // Initialize module
        init: function () {
            return DynamixelDevice._init(this.dxl);
        },

        // Change baud
        communicationSpeed: function (baud) {
            DynamixelDevice._speed(this.dxl, baud);
        },

        // Status of last operation
        status: function () {
            return DynamixelDevice._status(this.dxl);
        },
        
        // Ping
        ping: function () {
            return DynamixelDevice._ping(this.dxl);
        },

        // Reset
        reset: function () {
            return DynamixelDevice._reset(this.dxl);
        },

        // Read from module by adr
        read: function (adr) {
            return DynamixelDevice._read(this.dxl, adr);
        },

        // Read from module by adr
        read16: function (adr) {
            return DynamixelDevice._read16(this.dxl, adr);
        },

        // Write to module by reg
        write: function (adr, wrdata) {
            return DynamixelDevice._write(this.dxl, adr, wrdata);
        },

        // Write to module by reg
        write16: function (adr, wrdata) {
            return DynamixelDevice._write16(this.dxl, adr, wrdata);
        },

    },

    // Create an DynamixelDevice module instance * given id
    create: function (id, ver) {
        let obj = Object.create(DynamixelDevice._proto);
        obj.dxl = DynamixelDevice._create(id, ver);
        return obj;
    }
};

let DxlMaster = {
    _begin:  ffi ('void mgos_dxl_master_begin(int)'),
    _enable: ffi ('void mgos_dxl_master_enable(int)'),
    
    begin: function(baud) {
        DxlMaster._begin(baud);
    }, 

    disable: function() {
        DxlMaster._enable(0);
    }, 

    enabled: function() {
        DxlMaster._enable(1);
    }, 
  
    set_uart_callback: ffi('void mgos_dxl_setUserUartCb( void(*)(int, void *, userdata), userdata)')
};

let DynamixelConsole = {
    create: ffi('void *mgos_dxl_console_create(void)'),
    loop: ffi('void mgos_dxl_console_loop(void *)')
};

let Dynamixelmotor = {
    _create: ffi('void *mgos_dxl_motor_create(int, int)'),
    _init: ffi('int mgos_dxl_motor_init(void *)'),
    _wheelMode: ffi('void mgos_dxl_motor_wheelMode(void *)'),
    _jointMode: ffi('void mgos_dxl_motor_jointMode(void *, int, int)'),
    _enableTorque: ffi('void mgos_dxl_motor_enableTorque(void *, int)'),
    _speed: ffi('void mgos_dxl_motor_speed(void *, int)'),
    _goalPosition: ffi('void mgos_dxl_motor_goalPosition(void *, int)'),
    _led: ffi('void mgos_dxl_motor_led(void *, int)'),
    _currentPosition: ffi('int mgos_dxl_motor_currentPosition(void *)'),
    _getCurrentPosition: ffi('int mgos_dxl_motor_getCurrentPosition(void *, void *)'),
    _setComplianceMargins: ffi('int mgos_dxl_motor_setComplianceMargins(void *, void *, void *, void *, void *)'),
    _getComplianceMargins: ffi('int mgos_dxl_motor_getComplianceMargins(void *, void *, void *, void *, void *)'),
    _isMoving: ffi('int mgos_dxl_motor_isMoving(void *, void *)'),


    _proto: {
        // Init
        init: function () {
            return Dynamixelmotor._init(this.id);
        },

        // Wheel Mode
        wheelMode: function () {
            Dynamixelmotor._wheelMode(this.id);
        },

        // Joint Mode
        jointMode: function (aCWLimit, aCCWLimit) {
            Dynamixelmotor._jointMode(this.id, aCWLimit, aCCWLimit);
        },

        // Enable Torque 
        enableTorque: function (aTorque) {
            Dynamixelmotor._enableTorque(this.id, aTorque);
        },

        // Speed
        speed: function (aSpeed) {
            Dynamixelmotor._speed(this.id, aSpeed);
        },

        // Goal Position
        goalPosition: function (aPosition) {
            Dynamixelmotor._goalPosition(this.id, aPosition);
        },

        // Led
        led: function (aState) {
            Dynamixelmotor._led(this.id, aState);
        },

        // Current Position
        currentPosition: function () {
            return Dynamixelmotor._currentPosition(this.id);
        },

        // GetCurrentPosition
        getCurrentPosition: function (pos) {
            return Dynamixelmotor._getCurrentPosition(this.id, pos);
        },

        // Set Compliance Margins
        setComplianceMargins: function (cw_margin, ccw_margin, 
                                        cw_slope, ccw_slope) {
            return Dynamixelmotor._setComplianceMargins(this.id, 
                                                        cw_margin, ccw_margin, 
                                                        cw_slope, ccw_slope);
        },

        // Get Compliance Margins
        getComplianceMargins: function (cw_margin, ccw_margin, 
                                        cw_slope, ccw_slope) {
            return Dynamixelmotor._getComplianceMargins(this.id, 
                                                        cw_margin, ccw_margin, 
                                                        cw_slope, ccw_slope);
        },

        // Is Moving
        isMoving: function (moving) {
            return Dynamixelmotor._isMoving(this.id, moving);
        }

    },

    // Create an Dynamixelmotor module instance * given id
    create: function (id, ver) {
        let motor = Object.create(Dynamixelmotor._proto);
        motor.id = Dynamixelmotor._create(id, ver);
        return motor;
    }
};