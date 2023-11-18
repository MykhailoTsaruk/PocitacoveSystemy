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

bool initPort(HANDLE* comPort, char* portName, uint32_t ComRate, int ComBits, COMMTIMEOUTS timeout) {
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
	timeout.ReadIntervalTimeout = MAXWORD;			// Maximum time to wait between bytes in milliseconds.
	timeout.ReadTotalTimeoutMultiplier = 0;		// The multiplier used to calculate the total time-out period for read operations in milliseconds.
	timeout.ReadTotalTimeoutConstant = 100;	// The constant used to calculate the total time-out period for read operations in milliseconds.
	timeout.WriteTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(*comPort, &timeout);

	return 1;
}

bool readPort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesRead, bool receive) {
	DWORD read;
	DWORD numberOfBytesRead;

	while (true) {
		// Waiting until data is available
		if (WaitForSingleObject(*comPort, INFINITE) != WAIT_OBJECT_0) {
			printf("Failed to wait for serial port\nError code: 0x%x\n", GetLastError());
			return 0;
		}

		// Read the data from the serial port.
		read = ReadFile(
			*comPort,           // pointer to COM port
			buffer[0],          // buffer
			bufferSize,         // size of buffer
			&numberOfBytesRead, // number of bytes read
			NULL                // not overlapped
		);

		// Check for errors.
		if (read == 0) {
			printf("Read error: 0x%x\n", GetLastError());
			ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
			return 0;
		}

		// If data was read, break out of the loop.
		if (numberOfBytesRead > 0) {
			*bytesRead = (uint32_t)numberOfBytesRead;
			if (!receive) {
				buffer[1][0] = buffer[0][numberOfBytesRead - 1];
				buffer[1][1] = '\0';
			}
			buffer[0][numberOfBytesRead] = '\0';

			printf("String: %s\n", buffer[0]);
			if (!receive)
				printf("Length word: %d\n", buffer[1][0]);

			printf("Bytes read: %ld\n", numberOfBytesRead);
			printf("Successful read %ld bytes\n", numberOfBytesRead);

			return 1;
		}
	}

	return 0;
}

/*
bool readPort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesRead) {
	DWORD read;
	DWORD numberOfBytesRead;

	while (true) {
		// Waiting until data is available
		if (WaitForSingleObject(*comPort, INFINITE) != WAIT_OBJECT_0) {
			printf("Failed to wait for serial port\nError code: 0x%x\n", GetLastError());
			return 0;
		}

		// Read the data from the serial port.
		read = ReadFile(
			*comPort,           // pointer to COM port
			buffer[0],          // buffer
			bufferSize,         // size of buffer
			&numberOfBytesRead, // number of bytes read
			NULL                // not overlapped
		);

		// Check for errors.
		if (read == 0) {
			printf("Read error: 0x%x\n", GetLastError());
			ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
			return 0;
		}

		// If data was read, break out of the loop.
		if (numberOfBytesRead > 0) {
			buffer[0][numberOfBytesRead] = '\0'; // Null-terminate the received data.
			printf("String: %s\n", buffer[0]);
			printf("Bytes read: %ld\n", numberOfBytesRead);
			printf("Successful read %ld bytes\n", numberOfBytesRead);

			// Read the second part of the data (assuming it's a single byte).
			read = ReadFile(
				*comPort,           // pointer to COM port
				buffer[1],          // buffer for the single byte
				1,                  // size of buffer for the single byte
				&numberOfBytesRead, // number of bytes read
				NULL                // not overlapped
			);

			if (read == 0) {
				printf("Read error for the second part: 0x%x\n", GetLastError());
				ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
				return 0;
			}

			printf("Length word: %d\n", buffer[1][0]);
			printf("Bytes read: %ld\n", numberOfBytesRead);
			printf("Successful read %ld bytes\n", numberOfBytesRead);

			*bytesRead = (uint32_t)(numberOfBytesRead + bufferSize);
			return 1;
		}
	}

	return 0;
}
*/


bool writePort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesWritten, bool receive) {

	DWORD written;
	DWORD numberOfBytesWritten;

	// Write the data to the serial port.
	written = WriteFile(
		*comPort,             // pointer to COM port
		buffer[0],            // buffer for the string
		strlen(buffer[0]),    // size of buffer for the string
		&numberOfBytesWritten,// number of bytes written
		NULL                  // not overlapped
	);

	// Check for errors.
	if (written == 0) {
		printf("Write error for the string: 0x%x\n", GetLastError());
		ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
		return 0;
	}

	printf("String: %s\n", buffer[0]);
	if (!receive) {
		*bytesWritten = (uint32_t)numberOfBytesWritten;
		// Write the second part of the data (assuming it's a single byte).
		written = WriteFile(
			*comPort,             // pointer to COM port
			buffer[1],            // buffer for the single byte
			1,                    // size of buffer for the single byte
			&numberOfBytesWritten,// number of bytes written
			NULL                  // not overlapped
		);

		// Check for errors.
		if (written == 0) {
			printf("Write error for the second part: 0x%x\n", GetLastError());
			ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
			return 0;
		}

		printf("Byte: %d\n", (int)buffer[1][0]);

		*bytesWritten += (uint32_t)numberOfBytesWritten;
	}

	return 1;
}

/*
bool readPort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesRead) {

	DWORD read;
	DWORD numberOfBytesRead;
	//buffer[0] = (char*)malloc(bufferSize);
	//buffer[1] = (char*)malloc(bufferSize);
	//LPDWORD lpErrors = 0;
	//LPCOMSTAT lpStat = 0;

	while (true) {
		// Waiting until data is available
		if (WaitForSingleObject(*comPort, INFINITE) != WAIT_OBJECT_0) {
			printf("Failed to wait for serial port\nError code: 0x%x\n", GetLastError());
			return 0;
		}
		// Read the data from the serial port.
		read = ReadFile(
			*comPort,           // pointer to COM port
			buffer,             // buffer
			buffer,         // size of buffer
			&numberOfBytesRead, // number of bytes read
			NULL                // not overlapped
		);
		// Check for errors.
		if (read == 0) {
			printf("Read error: 0x%x\n", GetLastError());
			ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
			return 0;
		}
		// If data was read, break out of the loop.
		if (numberOfBytesRead > 0) {
			printf("Word: %s + %d\n", buffer[0], buffer[1][0]);
			printf("Bytes read: %ld\n", numberOfBytesRead);
			printf("Successful read %ld bytes\n", numberOfBytesRead);
			*bytesRead = (uint32_t)numberOfBytesRead;
			return 1;
		}
	}

	return 1;
}
*/
/*
bool writePort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesWritten) {

	DWORD written;
	DWORD numberOfBytesWritten;
	// Write the data to the serial port.
	written = WriteFile(
		*comPort,				// pointer to COM port
		buffer,					// buffer
		bufferSize,				// size of buffer
		&numberOfBytesWritten,	// number of bytes written
		NULL					// not overlapped
	);
	// Check for errors.
	if (written == 0) {
		printf("Write error: 0x%x\n", GetLastError());
		ClearCommError(*comPort, (LPDWORD)0, (LPCOMSTAT)0);
		return 0;
	}

	printf("strlen: %d\n", strlen((char*)buffer[0]));
	printf("WORD: %s + %d\n", buffer[0], buffer[1][0]);

	//printf("Successfully wrote %ld bytes\n", numberOfBytesWritten);
	*bytesWritten = (uint32_t)numberOfBytesWritten;
	return 1;
}
*/	