#include "DynamixelMotor.h"


DynamixelMotor::DynamixelMotor(DynamixelID aId):
DynamixelDevice(aId)
{}


void DynamixelMotor::wheelMode()
{
	jointMode(0,0);
}

void DynamixelMotor::jointMode(uint16_t aCWLimit, uint16_t aCCWLimit)
{
	uint32_t data= (aCWLimit | (uint32_t(aCCWLimit)<<16));
	write(DYN_ADDRESS_CW_LIMIT, data);
}

void DynamixelMotor::enableTorque(bool aTorque)
{
	write(DYN_ADDRESS_ENABLE_TORQUE, uint8_t(aTorque?1:0));
}

void DynamixelMotor::speed(int16_t aSpeed)
{
	if(aSpeed<0) 
	{
		aSpeed=-aSpeed | 1024;
	}
	write(DYN_ADDRESS_GOAL_SPEED, aSpeed);
}

void DynamixelMotor::goalPosition(uint16_t aPosition)
{
	write(DYN_ADDRESS_GOAL_POSITION, aPosition);
}

void DynamixelMotor::led(uint8_t aState)
{
	write(DYN_ADDRESS_LED, aState);
}

uint16_t DynamixelMotor::currentPosition()
{
	uint16_t currentPosition;
	read(DYN_ADDRESS_CURRENT_POSITION, currentPosition);
	return currentPosition;
}

DynamixelStatus DynamixelMotor::getCurrentPosition(uint16_t &pos)
{
	return read(DYN_ADDRESS_CURRENT_POSITION, pos);
}

DynamixelStatus DynamixelMotor::setComplianceMargins(uint8_t cw_margin, uint8_t ccw_margin, uint8_t cw_slope, uint8_t ccw_slope)
{
	DynamixelStatus status;

	status = write(DYN_ADDRESS_CW_COMP_MARGIN, cw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = write(DYN_ADDRESS_CCW_COMP_MARGIN, ccw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = write(DYN_ADDRESS_CW_COMP_SLOPE, cw_slope);
	if (DYN_STATUS_OK != status) { return status; }

	return write(DYN_ADDRESS_CCW_COMP_SLOPE, ccw_slope);
}
	
DynamixelStatus DynamixelMotor::getComplianceMargins(uint8_t &cw_margin, uint8_t &ccw_margin, uint8_t &cw_slope, uint8_t &ccw_slope)
{
	DynamixelStatus status;

	status = read(DYN_ADDRESS_CW_COMP_MARGIN, cw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = read(DYN_ADDRESS_CCW_COMP_MARGIN, ccw_margin);
	if (DYN_STATUS_OK != status) { return status; }

	status = read(DYN_ADDRESS_CW_COMP_SLOPE, cw_slope);
	if (DYN_STATUS_OK != status) { return status; }
	
	return read(DYN_ADDRESS_CCW_COMP_SLOPE, ccw_slope);
}

DynamixelStatus DynamixelMotor::isMoving(uint8_t &moving) 
{
	return read(DYN_ADDRESS_MOVING, moving);
}
