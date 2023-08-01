#ifndef DYNAMIXEL_INTERFACE_IMPL_H
#define DYNAMIXEL_INTERFACE_IMPL_H

#include "DynamixelInterface2.h"
#include <stdio.h>

#include "mgos.h"
#include "mgos_app.h"
#include "mgos_gpio.h"
#include "mgos_timers.h"
#include "mgos_uart.h"

#include "mgos_uart_hal.h"
#include "soc/dport_access.h"
#include "soc/uart_reg.h"

#include "driver/periph_ctrl.h"
#include "driver/uart.h"
#include "esp32/rom/uart.h"
#include "esp_attr.h"
#include "esp_intr_alloc.h"

#include "common/cs_dbg.h"
#include "common/cs_rbuf.h"
#include "mgos_gpio.h"
#include "mgos_uart_hal.h"


typedef void (*userUartCb_t)(uint16_t, uint8_t *,  void *);


class DynamixelInterfaceImpl:public DynamixelInterface
{    
	private:
		/** \brief Switch stream to read (receive)) mode */
		void readMode();
		
		/** \brief Switch stream to write (send) mode */
		void writeMode();

		size_t dxlWrite(uint8_t byte);
		size_t dxlWrite(uint8_t *data, uint8_t length);
		size_t dxlRead(uint8_t *buffer, uint8_t length);

		IRAM void getRxAdd(int uart_no, uint32_t *rd, uint32_t *wr);
		uint8_t dxlRxByte(void);

		struct mgos_uart_config mUartCfgDxl;
		struct mgos_uart_config mUartCfgSaved;
		
		uint8_t mUARTno;
		
		
	public:
		/**
		 * \brief Constructor
		 * \param[in] aUARTno : number of the UART
		*/
		DynamixelInterfaceImpl(uint8_t aUARTno);
		
		/**
		 * \brief Destructor
		 * Delete stream if it is owned by the instance
		 */
		~DynamixelInterfaceImpl();

		/**
		 * \brief Start interface
		 * \param[in] aBaud : Baudrate
		 *
		 * Start the interface with desired baudrate, call once before using 
		 * the interface
		*/
		void begin(long unsigned int aBaud);

		/**
	     * \brief Change timeOut
	     * \param[in] timeOut : ms
	     */
	    void setTimeOut(uint16_t timeOut);

		/**
		 * \brief Hardware specific preparations before transaction
		 * \param[in] aBaud : Baudrate
		 *
		 * Reinit interface for dynamixel (flush, set baudrate) for case when 
		 * interface shared with other functional
		*/
		uint8_t prepareTransaction();

		/**
		 * \brief Hardware specific cleaning after transaction
		 * \param[in] aBaud : Baudrate
		 *
		 * Reinit interface to its original state (flush, set baudrate) for 
		 * case when interface shared with other functional
		*/
		void endTransaction(DynamixelStatus status);
		
		/**
		 * \brief Send a packet on bus
		 * \param[in] aPacket : Packet to send
		 *
		 * The function wait for the packet to be completly sent 
		 * (using Stream.flush)
		*/
		void sendPacket(const DynamixelPacket &aPacket);

        /**
	     * \brief Send a packet on bus
	     * \param[in] aPacket : Packet to send
	     *
	     * The function wait for the packet to be completly sent (using Stream.flush)
	     */
	    void sendPacket2(DynamixelPacket2 &aPacket);

		/**
		 * \brief Receive a packet on bus
		 * \param[out] aPacket : Received packet. mData field must 
		 * 						 be previously allocated
		 * \param[in] answerSize : the size of the memory allocated to 
		 * 						   the mData field
		 *
		 * The function wait for a new packet on the bus. 
		 * Timeout depends of timeout of the underlying stream.
		 * Return error code in case of communication error (timeout, 
		 * checksum error, ...)
		*/
		void receivePacket(DynamixelPacket &aPacket, uint8_t answerSize = 0);

        /**
	     * \brief Receive a packet on bus
	     * \param[out] aPacket : Received packet. mData field must be previously allocated
	     * \param[in] answerSize : the size of the memory allocated to the mData field
	     *
	     * The function wait for a new packet on the bus.
	     * Timeout depends of timeout of the underlying stream.
	     * Return error code in case of communication error (timeout, checksum error, ...)
	     */
	    void receivePacket2(DynamixelPacket2 &aPacket, uint16_t answerSize = 0 , uint8_t mode = RECEIVE_NORMAL);

		/**
         * \brief Stop interface
        */
        void end();

		/**
		 * \brief Set user uart dispatcher
		*/
		void setUserUartDispatcherCB(userUartCb_t callback, void *user_data);
        void uartUserCb(uint16_t len, uint8_t *data);

		userUartCb_t mUserUartDispatcherCb = NULL;
        bool mUartUserCb_enabled = true;
        void *mUserData_p = NULL;
};

class HardwareDynamixelInterface:public DynamixelInterfaceImpl
{
	private:
		uint8_t mInterfaceEnable = true;
	
	public:
		HardwareDynamixelInterface(uint8_t aUARTno);
		~HardwareDynamixelInterface();

		void setInterfaceEnable(uint8_t state) { mInterfaceEnable = state; }
		uint8_t getInterfaceEnable(void) {return mInterfaceEnable; }

};


#endif