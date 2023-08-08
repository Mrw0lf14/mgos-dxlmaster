#include "DynamixelInterfaceMGOSImpl.h"

#define DXL_DIR_TXD_PIN 5
#define DXL_DIR_RXD_PIN 5
#define UART_0 0
#define READ_TIMEOUT 100
#define READ_TIMEOUT_SLEEP 100

HardwareDynamixelInterface DxlMaster(UART_0);

HardwareDynamixelInterface::HardwareDynamixelInterface(uint8_t aUART_no):
	DynamixelInterfaceImpl(aUART_no) {}

HardwareDynamixelInterface::~HardwareDynamixelInterface() {}


DynamixelInterfaceImpl::DynamixelInterfaceImpl(uint8_t aUART_no):
	mUARTno(aUART_no)
{
	readMode();
	mgos_gpio_set_mode(DXL_DIR_TXD_PIN, MGOS_GPIO_MODE_OUTPUT);
	mgos_gpio_set_mode(DXL_DIR_RXD_PIN, MGOS_GPIO_MODE_OUTPUT);
	mUserUartDispatcherCb = NULL;
}

DynamixelInterfaceImpl::~DynamixelInterfaceImpl() 
{}

void DynamixelInterfaceImpl::begin(long unsigned int aBaud)
{
	/* Defaults: 8 bit, parity = none, sopt bit = 1, buf_size = 256 */
	mgos_uart_config_set_defaults(mUARTno, &mUartCfgDxl);
	mUartCfgDxl.baud_rate = aBaud; 
	mUartCfgDxl.rx_linger_micros = 50;

	readMode();
}

void DynamixelInterfaceImpl::end() 
{
	mUserUartDispatcherCb = NULL;
}


uint8_t DynamixelInterfaceImpl::prepareTransaction()
{
	if (DxlMaster.getInterfaceEnable() == false) return 1;

    mUartUserCb_enabled = false;

	mgos_uart_flush(mUARTno);

	if (!mgos_uart_config_get(mUARTno, &mUartCfgSaved)) {
  		LOG(LL_ERROR, ("Failed to get cofg UART%d", mUARTno));
		return 1;
	}

	if (!mgos_uart_configure(mUARTno, &mUartCfgDxl)) {
		endTransaction(99);
		return 1;
	} 
	return 0;
}


void DynamixelInterfaceImpl::endTransaction(DynamixelStatus status)
{
	// if (status == 99) mgos_msleep(10);
	// else if (status != DYN_STATUS_OK) mgos_msleep(2);
	// writeMode(); // sometimes after uart recongf it send trash byte
    mUartUserCb_enabled = true;
	mgos_uart_configure(mUARTno, &mUartCfgSaved); // ...

}


void DynamixelInterfaceImpl::sendPacket(const DynamixelPacket &aPacket)
{
	// empty receive buffer, in case of a error in previous transaction
	uint8_t dummy;
	while (mgos_uart_read_avail(mUARTno)) {
		mgos_uart_read(mUARTno, (void *) &dummy, 1);
	}

	writeMode();

	dxlWrite(0xFF);
	dxlWrite(0xFF);
	dxlWrite(aPacket.mID);
	dxlWrite(aPacket.mLength);
	dxlWrite(aPacket.mInstruction);

	uint8_t n = 0;
	if (aPacket.mAddress != 255) {
		dxlWrite(aPacket.mAddress);
		++n;
	}
	if (aPacket.mDataLength != 255)	{
		dxlWrite(aPacket.mDataLength);
		++n;
	}
	if (aPacket.mLength > (2 + n)) {
		if (aPacket.mIDListSize == 0) {
			dxlWrite(aPacket.mData, aPacket.mLength - 2 - n);
		} else {
			uint8_t *ptr = aPacket.mData;
			for (uint8_t i = 0; i < aPacket.mIDListSize; ++i) {
				dxlWrite(aPacket.mIDList[i]);
				dxlWrite(ptr, aPacket.mDataLength);
				ptr += aPacket.mDataLength;
			}
		}
	}

	dxlWrite(aPacket.mCheckSum);

	mgos_uart_flush(mUARTno);
	readMode();
}

void DynamixelInterfaceImpl::sendPacket2(DynamixelPacket2 &aPacket)
{
	// empty receive buffer, in case of a error in previous transaction
	uint8_t dummy;
	while (mgos_uart_read_avail(mUARTno)) {
		mgos_uart_read(mUARTno, (void *) &dummy, 1);
	}

	writeMode();
	dxlWrite(aPacket.mHead, HEAD_LEN);
	dxlWrite(aPacket.mParams, aPacket.mParamSize);
	dxlWrite(DXL_LOBYTE(aPacket.mCheckSum));
	dxlWrite(DXL_HIBYTE(aPacket.mCheckSum));

	mgos_uart_flush(mUARTno);

	readMode();
}

void DynamixelInterfaceImpl::receivePacket(DynamixelPacket &aPacket, 
											uint8_t answerSize)
{
	uint8_t buffer[5] = {0};

	aPacket.mIDListSize = 0;
	aPacket.mAddress = 255;
	aPacket.mDataLength = 255;

	if (dxlRead(buffer, 5) < 5) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}
	/* Check headers (0xFF) */
	if (buffer[0] != 255 || buffer[1] != 255) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}

	/* Check ID */
	if (aPacket.mID != buffer[2]) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}
	/* Check Length */
	aPacket.mLength = buffer[3];
	if ((aPacket.mLength - 2) != answerSize) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR;
		return;
	}

	aPacket.mStatus = buffer[4];
	
	if ((aPacket.mLength > 2) 
		&& (int)dxlRead(aPacket.mData, aPacket.mLength - 2) 
						< (aPacket.mLength - 2)) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	}

	if (dxlRead((&(aPacket.mCheckSum)), 1) < 1) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
		return;
	} 
	

	if (aPacket.checkSum() != aPacket.mCheckSum) {
		aPacket.mStatus = DYN_STATUS_COM_ERROR | DYN_STATUS_CHECKSUM_ERROR;
	}
}

void DynamixelInterfaceImpl::receivePacket2(DynamixelPacket2 &aPacket, uint16_t answerSize, uint8_t mode)
{
	uint8_t buffer[RX_HEAD_BUFF] = {0};
    aPacket.mCheckSum = 0;

	if (mode != RECEIVE_FAST)
    {
		if (dxlRead(buffer, RX_HEAD_BUFF) < RX_HEAD_BUFF)
		{
			aPacket.mStatus = DYN2_STATUS_TIMEOUT;
			return;
		}

		if (buffer[0] != 0xFF || buffer[1] != 0xFF || buffer[2] != 0xFD || buffer[3] != 0x00)
        {
            aPacket.mStatus = DYN2_STATUS_HEADERS_ERR;
            return;
        }

		if (aPacket.mID != buffer[4])
        {
            aPacket.mStatus = DYN2_STATUS_PACKET_ID_ERR;
            return;
        }

		uint16_t rxLength = buffer[5] | ((uint16_t)buffer[6] << 8);
        if (rxLength != answerSize)
        {
            aPacket.mStatus = DYN2_STATUS_PACKET_LEN_ERR;
            return;
        }

        if (0x55 != buffer[7])
        {
            aPacket.mStatus = DYN2_STATUS_PACKET_INST_ERR;
            return;
        }

		aPacket.mCheckSum = aPacket.updateCRC(0, buffer, RX_HEAD_BUFF);
	}

	// Read Err field
	if (dxlRead(buffer, ERR_LEN) < ERR_LEN) 
    {
        aPacket.mStatus = DYN2_STATUS_TIMEOUT_ERR;
        return;
    }

	aPacket.mStatus = buffer[0];
    aPacket.mCheckSum = aPacket.updateCRC(aPacket.mCheckSum, buffer, ERR_LEN);

    /* Read Data fields*/
    if (dxlRead(aPacket.mRxData, aPacket.mRxDataLength) < aPacket.mRxDataLength)
    {
        aPacket.mStatus = DYN2_STATUS_TIMEOUT_DATA;
        return;
    }

    aPacket.mCheckSum = aPacket.updateCRC(aPacket.mCheckSum, aPacket.mRxData, aPacket.mRxDataLength);


    if (dxlRead(buffer, CRC_LEN) < CRC_LEN)
    {
        aPacket.mStatus = DYN2_STATUS_TIMEOUT_CRC;
        return;
    }

    uint16_t rx_crc = ((uint16_t)buffer[1] << 8) | buffer[0];
    if (aPacket.mCheckSum != rx_crc)
    {
        aPacket.mStatus = DYN2_STATUS_PACKET_CRC_ERR;
        return;
    }
}

void DynamixelInterfaceImpl::readMode()
{
	mgos_gpio_write(DXL_DIR_RXD_PIN, 1);
}
	
void DynamixelInterfaceImpl::writeMode()
{
	mgos_gpio_write(DXL_DIR_RXD_PIN, 0);
}
	

size_t DynamixelInterfaceImpl::dxlWrite(uint8_t byte)
{
	return mgos_uart_write(mUARTno, &byte, 1);
}


size_t DynamixelInterfaceImpl::dxlWrite(uint8_t *data, uint8_t length)
{
	return mgos_uart_write(mUARTno, (void *)data, length);
}


size_t DynamixelInterfaceImpl::dxlRead(uint8_t *buffer, uint8_t length)
{
	int8_t timeout = READ_TIMEOUT;
	while (esp32_uart_rx_fifo_len(UART_0) < length) {
		mgos_usleep(READ_TIMEOUT_SLEEP);
		if (timeout-- <= 0) {
			return 0;
		}
	}

	size_t count = 0;
  	while (count < length) {
    	buffer[count] = dxlRxByte();
    	count++;
  	}
	  
 	return count;
}


IRAM void DynamixelInterfaceImpl::getRxAdd(int uart_no, uint32_t *rd, 
														  uint32_t *wr) 
{
  uint32_t mrxs = DPORT_READ_PERI_REG(UART_MEM_RX_STATUS_REG(uart_no));
  if (rd) *rd = ((mrxs & UART_MEM_RX_RD_ADDR_M) >> UART_MEM_RX_RD_ADDR_S);
  if (wr) *wr = ((mrxs & UART_MEM_RX_WR_ADDR_M) >> UART_MEM_RX_WR_ADDR_S);
}


uint8_t DynamixelInterfaceImpl::dxlRxByte(void) 
{
	uint8_t byte;
	uint32_t rx_before = 0, rx_after = 0;
	getRxAdd(UART_0, &rx_before, NULL);

	do {
		byte = (uint8_t)(*((volatile uint32_t *) UART_FIFO_AHB_REG(UART_0)));
		getRxAdd(UART_0, &rx_after, NULL);

	} while (rx_after == rx_before);

	return byte;
}

void DynamixelInterfaceImpl::setUserUartDispatcherCB(userUartCb_t callback, 
                                                    void *user_data)
{
    if (callback == NULL) return;
    mUserUartDispatcherCb = callback;
    mUserData_p = user_data;
}

void DynamixelInterfaceImpl::uartUserCb(uint16_t len, uint8_t *data)
{
    if (mUserUartDispatcherCb != NULL && mUartUserCb_enabled == true) {
        mUserUartDispatcherCb(len, data, mUserData_p); 
    }
}
