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
		printf("Could not open COM port %s\nError code: 0x%x\n", portName, GetLastError());
		return 0;
	}
	
	printf("%s port was opened successfully\n", portName);
	return 1;
}

bool initPort(HANDLE* comPort, char* portName, int ComRate, int ComBits, COMMTIMEOUTS timeout) {
	if (createPort(comPort, portName) == 0) return 0;							// Open serial port
	if (purgePort(comPort) == 0) return 0;										// Purge any outstanding requests on the serial port
	if (setComParameters(comPort, ComRate, ComBits, timeout) == 0) return 0;	// Set the serial port parameters
	if (purgePort(comPort) == 0) return 0;										// Purge any outstanding requests on the serial port
	
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

DWORD readPort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size) {
	DWORD bytesRead;
	DWORD numberOfBytesRead;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpStat = 0;
	// Read the data from the serial port.
	bytesRead = ReadFile(
		*comPort,				// pointer to COM port
		buffer,					// buffer
		buffer_size,			// size of buffer
		&numberOfBytesRead,		// number of bytes read
		NULL					// not overlapped
	);
	// Check for errors.
	if (bytesRead == 0) {
		printf("Read erorr: 0x%x\n", GetLastError());
		ClearCommError(comPort, lpErrors, lpStat);
		return 0;
	}

	printf("Successful read %ld bytes\n", numberOfBytesRead);
	return numberOfBytesRead;
}

DWORD writePort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size) {
	DWORD bytesWritten;
	DWORD numberOfBytesWritten;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpStat = 0;
	// Write the data to the serial port.
	bytesWritten = WriteFile(
		*comPort,					// pointer to COM port
		buffer,						// buffer
		buffer_size,				// size of buffer
		&numberOfBytesWritten,		// number of bytes written
		NULL						// not overlapped
	);
	// Check for errors.
	if (bytesWritten == 0) {
		printf("Write erorr: 0x%x\n", GetLastError());
		ClearCommError(comPort, lpErrors, lpStat);
		return 0;
	}

	printf("Successful write %ld bytes\n", numberOfBytesWritten);
	return bytesWritten;
}

bool setComParameters(HANDLE* comPort, int comRate, int comBits, COMMTIMEOUTS timeout) {
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