#include "rs485.h"

bool rs485_open(struct rs485_obj* port, uint8_t port_number, uint8_t baudrate) {
	
	if (!port)
		return false;

	port->baudrate = baudrate;
	snprintf(port->port_name, sizeof(port->port_name), "\\\\.\\COM%d", (int)port_number);
	port->port = INVALID_HANDLE_VALUE;

	// open serial port
	HANDLE com_port = CreateFileA(
		port->port_name,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (com_port == INVALID_HANDLE_VALUE) {
		return false;
	}

	DCB dcb;
	SecureZeroMemory(&dcb, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);

	if (!GetCommState(com_port, &dcb)) {
		CloseHandle(com_port);
		return false;
	}

	dcb.BaudRate = (DWORD)baudrate;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	//dcb.fDtrControl = DTR_CONTROL_ENABLE;
	//dcb.fRtsControl = RTS_CONTROL_ENABLE;

	if (!SetCommState(com_port, &dcb)) {
		CloseHandle(com_port);
		return false;
	}

	port->port = com_port;
	return true;
}

bool rs485_close(struct rs485_obj* port) {

	if (!port || port->port == INVALID_HANDLE_VALUE)
		return false;

	CloseHandle(port->port);
	port->port = INVALID_HANDLE_VALUE;

	return true;
}

bool rs485_read(struct rs485_obj* port, uint8_t* buffer, uint32_t buffer_size, uint32_t* bytes_read) {

	if (!port || port->port == INVALID_HANDLE_VALUE)
		return false;

	DWORD bytes;
	if (!ReadFile(
		port->port,
		buffer,
		buffer_size,
		&bytes_read,
		NULL
	)) {
		return false;
	}

	*bytes_read = (uint32_t)bytes;
	return true;
}

bool rs232_write(struct rs485_obj* port, uint8_t* buffer, uint32_t buffer_size) {
	
	if (!port || port->port == INVALID_HANDLE_VALUE)
		return false;

	DWORD bytes;
	if (!WriteFile(
		port->port,
		buffer,
		buffer_size,
		&bytes,
		NULL
	)) {
		return false;
	}
	
	return bytes == buffer_size;
}

