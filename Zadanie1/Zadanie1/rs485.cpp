#include "rs485.h"

bool createPort(HANDLE* comPort, char* portName) {
	// Open a handle to the serial port.
	*comPort = CreateFileA(
		portName,						// COM port number
		GENERIC_READ | GENERIC_WRITE,	// Read/write access
		NULL,							// No sharing allowed
		NULL,							// No security attributes
		OPEN_EXISTING,					// Open existing port only
		FILE_ATTRIBUTE_NORMAL,			// File without atributes
		NULL							// No template file
	);

	if (*comPort == INVALID_HANDLE_VALUE) {
		printf("Could not open COM port %s\n", portName);
		return 0;
	}

	printf("%s port was opened successfully\n", portName);
	return 1;
}

bool purgePort(HANDLE* comPort) {
	// Purge any outstanding requests on the serial port
	if (PurgeComm(*comPort, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR) == 0) {
		printf("Could not purge serial port\nError code: 0x%x\n", GetLastError());
		return 0;
	}

	return 1;
}

bool setComParameters(HANDLE* comPort, uint32_t comRate, int comBits, COMMTIMEOUTS timeout) {
	DCB dcb;
	// Clear DCB structure
	memset(&dcb, 0, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(*comPort, &dcb)) {
		printf("Could not get DCB\nError code: 0x%x\n", GetLastError());
		return 0;
	}
	// Set DCB parameters
	dcb.BaudRate = comRate;					// Baud rate
	dcb.ByteSize = (BYTE)comBits;			// Number of bits
	dcb.Parity = NOPARITY;					// No parity
	dcb.StopBits = ONESTOPBIT;				// 1 stop bit
	if (!SetCommState(*comPort, &dcb)) {
		printf("Could not set DCB\nError code: 0x%x\n", GetLastError());
		return 0;
	}

	// Set timeouts
	memset((void*)&timeout, 0, sizeof(timeout));
	timeout.ReadIntervalTimeout = 500;			// Maximum time to wait between bytes in milliseconds.
	timeout.ReadTotalTimeoutMultiplier = 1;		// The multiplier used to calculate the total time-out period for read operations in milliseconds.
	timeout.ReadTotalTimeoutConstant = 5000;	// The constant used to calculate the total time-out period for read operations in milliseconds.
	SetCommTimeouts(*comPort, &timeout);

	return 1;
}

bool readPort(HANDLE* comPort, uint8_t* buffer, uint32_t bufferSize, uint32_t* bytesRead) {
	
	while (true) {
		if (WaitForSingleObject(*comPort, INFINITE) != WAIT_OBJECT_0) {
			printf("Failed to wait for serial port\nError code: 0x%x\n", GetLastError());
			break;
		}
	}

	return 1;
}
