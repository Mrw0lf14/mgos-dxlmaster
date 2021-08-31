let DynamixelDevice = {
    _create: ffi('void *mgos_dxl_module_create(int)'),
    _init: ffi('int mgos_dxl_init(void *)'),
    _status: ffi('int mgos_dxl_status(void *)'),
    _ping: ffi('int mgos_dxl_ping(void *)'),
    _read: ffi('int mgos_dxl_read(void *, int)'),
    _write: ffi('int mgos_dxl_write(void *, int, int )'),

    _proto: {
        // Initialize module
        init: function () {
            return DynamixelDevice._init(this.dxl);
        },

        // Status of last operation
        status: function () {
            return DynamixelDevice._status(this.dxl);
        },

        // Read from module by adr
        read: function (adr) {
            return DynamixelDevice._read(this.dxl, adr);
        },

        // Write to module by reg
        write: function (adr, wrdata) {
            return DynamixelDevice._write(this.dxl, adr, wrdata);
        },

    },

    // Create an DynamixelDevice module instance * given id
    create: function (id) {
        let obj = Object.create(DynamixelDevice._proto);
        obj.dxl = DynamixelDevice._create(id);
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