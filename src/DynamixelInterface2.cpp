#include "DynamixelInterface2.h"
#include "mgos.h"
#include "mgos_uart.h"

void DynamixelInterface::transaction(bool aExpectStatus, uint8_t answerSize)
{
	if (prepareTransaction() != 0) {
		mPacket.mStatus = DYN_STATUS_SOFT_ERROR;
		return;
	}
	sendPacket(mPacket);
	
	if (aExpectStatus) {
		receivePacket(mPacket, answerSize);
	} else {
		mPacket.mStatus = DYN_STATUS_OK;
	}

	endTransaction(mPacket.mStatus);
}

void DynamixelInterface::transaction2(bool aExpectStatus, uint16_t answerSize)
{
    if (prepareTransaction() != 0) {
		mPacket2.mStatus = DYN_STATUS_SOFT_ERROR;
		return;
	}
    sendPacket2(mPacket2);

    if (aExpectStatus)
        receivePacket2(mPacket2, answerSize);
    else
        mPacket2.mStatus = DYN2_STATUS_OK;

    endTransaction(mPacket.mStatus);
}

/**
 *  Read (0x02)
 */
DynamixelStatus DynamixelInterface::read(
    uint8_t aVer, 
    uint8_t aID, 
    uint16_t aAddress, 
    uint16_t aRxSize, 
    uint8_t *aRxBuf, 
    uint8_t aStatusReturnLevel)
{
    if (aVer == DYN_PROTOCOL_V1) 
    {
        mPacket = DynamixelPacket(aID, DYN_READ, 4, aRxBuf, (uint8_t)aAddress, (uint8_t)aRxSize);
        transaction(aStatusReturnLevel > 0 && aID != BROADCAST_ID, aRxSize);
        return mPacket.mStatus;
    }
    else
    {
        uint8_t *params = (uint8_t *) malloc(READ_TX_PARAMS_LEN);
        params[0] = (DXL_LOBYTE(aAddress));
        params[1] = (DXL_HIBYTE(aAddress));
        params[2] = (DXL_LOBYTE(aRxSize));
        params[3] = (DXL_HIBYTE(aRxSize));

        mPacket2 = DynamixelPacket2(aID, READ_TX_LENGTH, INST_READ, params, READ_TX_PARAMS_LEN, /*aAddress,*/ aRxBuf, aRxSize);
        transaction2(aStatusReturnLevel > 0 && aID != BROADCAST_ID, READ_RX_LENGTH + aRxSize);
        free(params);
        return mPacket2.mStatus;
    }
}

/**
 *  Write (0x03)
 */
DynamixelStatus DynamixelInterface::write(
    uint8_t aVer, 
    uint8_t aID, 
    uint16_t aAddress,
    uint16_t aTxSize,
    const uint8_t *aTxBuf,
    uint8_t aStatusReturnLevel)
{
    if (aVer == DYN_PROTOCOL_V1)
    {
        mPacket = DynamixelPacket(aID, DYN_WRITE, (uint8_t)aTxSize + 3, aTxBuf, (uint8_t)aAddress);
        transaction(aStatusReturnLevel > 1 && aID != BROADCAST_ID);
        return mPacket.mStatus;
    }
    else
    {
        uint16_t params_size = WRITE_TX_PARAMS_LEN + aTxSize;
        uint8_t *params = (uint8_t *)malloc(params_size);
        params[0] = (DXL_LOBYTE(aAddress));
        params[1] = (DXL_HIBYTE(aAddress));
        
        memcpy(&params[WRITE_TX_PARAMS_LEN], aTxBuf, aTxSize);

        mPacket2 = DynamixelPacket2(aID, WRITE_TX_LENGTH + aTxSize, INST_WRITE, params, params_size/*, aAddress*/);

        transaction2(aStatusReturnLevel > 0 && aID != BROADCAST_ID, WRITE_RX_LENGTH);
        free(params);
        return mPacket2.mStatus;
    }
}

DynamixelStatus DynamixelInterface::regWrite(uint8_t aID, uint8_t aAddress, uint8_t aSize, const uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_REG_WRITE, aSize+3, aPtr, aAddress);
	transaction(aStatusReturnLevel > 1 && aID != BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::syncWrite(uint8_t nID, const uint8_t *aID, uint8_t aAddress, uint8_t aSize, const uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(BROADCAST_ID, DYN_SYNC_WRITE, (aSize+1)*nID+4, aPtr, aAddress, aSize, nID, aID);
	transaction(false);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::ping(uint8_t aID)
{
	mPacket = DynamixelPacket(aID, DYN_PING, 2, NULL);
	transaction(true);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::action(uint8_t aID, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_ACTION, 2, NULL);
	transaction(aStatusReturnLevel > 1 && aID != BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::reset(uint8_t aID, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_RESET, 2, NULL);
	transaction(aStatusReturnLevel > 1 && aID != BROADCAST_ID);
	return mPacket.mStatus;
}


