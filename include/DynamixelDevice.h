/*
 * DynamixelDevice.h
 *
 * Created: 19.01.2019 17:51:17
 *  Author: potor
 */ 


#ifndef DYNAMIXELDEVICE_H_
#define DYNAMIXELDEVICE_H_

#include "DynamixelInterface2.h"
//#include <HardwareSerial.h>



extern HardwareDynamixelInterface DxlMaster;

class DynamixelDevice
{
	public:
		DynamixelDevice(DynamixelID aId, uint8_t aVersion = DYN_PROTOCOL_V1);
		
		DynamixelStatus init();
		DynamixelStatus status() {	return mStatus;	}
		
		DynamixelID id() { return mID; }
		DynamixelStatus changeId(uint8_t id);
		
		void setStatus(uint8_t new_status) { mStatus = new_status; }

	    void protocolVersion(uint8_t version) { mVer = version; };
    	uint8_t protocolVersion(void) {	return mVer; }

		uint8_t statusReturnLevel();
		void statusReturnLevel(uint8_t aSRL);

		uint16_t model();
		uint8_t firmware();
		
		void communicationSpeed(uint32_t aSpeed);
	
		template<class T>
		inline DynamixelStatus read(uint8_t aAddress, T& aData)
		{
			return mStatus = DxlMaster.read<T>(mVer, mID, aAddress, aData, mStatusReturnLevel);
		}
		
		inline DynamixelStatus read(uint8_t aAddress, uint8_t size, uint8_t *ptr)
		{
			return mStatus = DxlMaster.read(mVer, mID, aAddress, size, ptr, mStatusReturnLevel);
		}
		
		template<class T>
		inline DynamixelStatus write(uint8_t aAddress, const T& aData)
		{
			return mStatus = DxlMaster.write<T>(mVer, mID, aAddress, aData, mStatusReturnLevel);
		}
		
		inline DynamixelStatus write(uint8_t aAddress, uint8_t size, const uint8_t *ptr)
		{
			return mStatus = DxlMaster.write(mVer, mID, aAddress, size, ptr, mStatusReturnLevel);
		}
		
		template<class T>
		inline DynamixelStatus regWrite(uint8_t aAddress, const T& aData)
		{
			return mStatus = DxlMaster.regWrite<T>(mID, aAddress, aData, mStatusReturnLevel);
		}
		
		inline DynamixelStatus regWrite(uint8_t aAddress, uint8_t size, const uint8_t *ptr)
		{
			return mStatus = DxlMaster.regWrite(mID, aAddress, size, ptr, mStatusReturnLevel);
		}
		
		inline DynamixelStatus syncRead(uint8_t nID, const uint8_t *aID, uint16_t aAddress, uint16_t aLen, uint8_t *rxPtr)
	    {
	        return mStatus = DxlMaster.syncRead(mVer, nID, aID, aAddress, aLen, rxPtr);
	    }

	    inline DynamixelStatus syncWrite(uint8_t nID, const uint8_t *aID, uint16_t aAddress, uint16_t aSize, const uint8_t *ptr)
	    {
	        return mStatus = DxlMaster.syncWrite(mVer, nID, aID, aAddress, aSize, ptr);
	    }

	    inline DynamixelStatus fastSyncRead(uint8_t nID, const uint8_t *aID, uint16_t aAddress, uint16_t aLen, uint8_t *rxPtr)
	    {
	        return mStatus = DxlMaster.fastSyncRead(mVer, nID, aID, aAddress, aLen, rxPtr);
	    }

	    inline DynamixelStatus bulkRead(uint8_t nID, const uint8_t *aTxBuf, uint16_t aTxBufLen, uint8_t *rxPtr)
	    {
	        return mStatus = DxlMaster.bulkRead(mVer, nID, aTxBuf, aTxBufLen, rxPtr);
	    }

	    inline DynamixelStatus bulkWrite(uint8_t nID, const uint8_t *aTxBuf, uint16_t aSize)
	    {
	        return mStatus = DxlMaster.bulkWrite(mVer, nID, aSize, aTxBuf);
	    }


		DynamixelStatus ping(uint8_t *buf = NULL)
		{
			return mStatus = DxlMaster.ping(mVer, mID, buf);
		}
		
		DynamixelStatus action()
		{
			return mStatus = DxlMaster.action(mID, mStatusReturnLevel);
		}
		
		DynamixelStatus reset()
		{
			return mStatus = DxlMaster.reset(mID, mStatusReturnLevel);
		}

	    DynamixelStatus reboot(void)
	    {
	        return mStatus = DxlMaster.reboot(mVer, mID, mStatusReturnLevel);
	    }

	    DynamixelStatus clear(uint16_t aSize, const uint8_t *aPtr)
	    {
	        return mStatus = DxlMaster.clear(mVer, mID, aSize, aPtr, mStatusReturnLevel);
	    }

	    DynamixelStatus backup(uint16_t size, const uint8_t *ptr)
	    {
	        return mStatus = DxlMaster.backup(mVer, mID, size, ptr, mStatusReturnLevel);
	    }
	    
		uint8_t mVer;

	private:
		DynamixelID mID;
		uint8_t mStatusReturnLevel;
		DynamixelStatus mStatus;	
};





#endif /* DYNAMIXELDEVICE_H_ */
