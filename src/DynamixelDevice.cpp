/*
 * DynamixelDevice.cpp
 *
 * Created: 19.01.2019 17:51:51
 *  Author: potor
 */ 
#include"DynamixelDevice.h"

DynamixelDevice::DynamixelDevice(DynamixelID aID, uint8_t aVersion)
    : mVer(aVersion), mID(aID), mStatusReturnLevel(255)
{
    mStatus = DYN_STATUS_OK;
    if (mID == BROADCAST_ID)
    {
        mStatusReturnLevel = 0;
    }
}

DynamixelStatus DynamixelDevice::changeId(uint8_t id)
{
	DynamixelStatus result;
	result = write(DYN_ADDRESS_ID, id);
	if ( result == DYN_STATUS_OK ) {
		mID = id;
	}
	return result;
}

uint8_t DynamixelDevice::statusReturnLevel()
{
	if ( mStatusReturnLevel == 255 ) {
		init();
	}
	return mStatusReturnLevel;
}

void DynamixelDevice::statusReturnLevel(uint8_t aSRL)
{
	write(DYN_ADDRESS_SRL, aSRL);
	if ( status() == DYN_STATUS_OK ) {
		mStatusReturnLevel=aSRL;
	}
}

uint16_t DynamixelDevice::model()
{
	uint16_t result;
	read(DYN_ADDRESS_ID, result);
	return result;
}

uint8_t DynamixelDevice::firmware()
{
	uint8_t result;
	read(DYN_ADDRESS_FIRMWARE, result);
	return result;
}

void DynamixelDevice::communicationSpeed(uint32_t aSpeed)
{
	uint8_t value = 2000000 / aSpeed - 1;
	if (value != 0) { // forbid 2MBd rate, because it is out of spec, and can be difficult to undo
		write(DYN_ADDRESS_BAUDRATE, value);
	}
}

DynamixelStatus DynamixelDevice::init()
{
	mStatusReturnLevel = 2;
	DynamixelStatus status = ping();
	if (status != DYN_STATUS_OK) {
		return status;
	}

	uint8_t temp = 0;
	status = read(DYN_ADDRESS_SRL, temp);

	if (status == DYN_STATUS_OK && temp == 0) {
		mStatusReturnLevel = 2;
	}
	else 
		mStatusReturnLevel = temp;

	if (status & DYN_STATUS_TIMEOUT) {
		mStatusReturnLevel = 0;
	}
	
	return DYN_STATUS_OK;
}

