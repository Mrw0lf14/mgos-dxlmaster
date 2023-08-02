#ifndef DYNAMIXEL_CONSOLE_H
#define DYNAMIXEL_CONSOLE_H

#include "mgos.h"
#include "DynamixelInterface2.h"

#define UART_0 0

extern HardwareDynamixelInterface DxlMaster;

class DynamixelConsole;

struct DynamixelCommand
{
	typedef DynamixelStatus (DynamixelConsole::*FunPtr)(int, char **);
	const char *mName;
	//const char *mDescription;
	FunPtr mCallback;
};

class Stream
{
	public:
	size_t available(void){ return mgos_uart_read_avail(UART_0); }
	char read() 
	{
		char c;
		if (mgos_uart_read(UART_0, &c, 1) != 1) {
			return -1;
		}
		return c;	
	}

	size_t write(char c) 
	{ 
		return mgos_uart_write(UART_0, &c, 1); 
	}

	size_t write(const char *str) 
	{
		if(str == NULL) {
            return 0;
        }
		return mgos_uart_write(UART_0, str, strlen(str)); 
	}
	void print(uint8_t c) { mgos_uart_write(UART_0, &c, 1); }
	void print(const char *str) { write(str); }
  

};

class Stream;
class DynamixelConsole
{
	public:
	DynamixelConsole();
	void loop();
	
	private:
	void run();
	int parseCmd(char **argv);
	void printStatus(DynamixelStatus aStatus);
	void printData(const uint8_t *data, uint8_t length);
	void printHelp();
	
	DynamixelStatus ping(int argc, char **argv);
	DynamixelStatus read(int argc, char **argv);
	DynamixelStatus write(int argc, char **argv);
	DynamixelStatus reset(int argc, char **argv);
	DynamixelStatus reg_write(int argc, char **argv);
	DynamixelStatus action(int argc, char **argv);
	DynamixelStatus sync_write(int argc, char **argv);
	DynamixelStatus baudrate(int argc, char **argv);
	
	const static size_t sLineBufSize = 255;
	char mLineBuf[sLineBufSize + 1];
	char *mLinePtr;
	Stream mConsole;
	uint8_t mVer;
	
	const static DynamixelCommand sCommand[];
};

#endif
