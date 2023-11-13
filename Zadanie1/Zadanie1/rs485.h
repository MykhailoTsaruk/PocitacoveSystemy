#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <iostream>


/*
struct rs485_obj {
	HANDLE port;
	char port_name[16];
	uint32_t baudrate;
};

bool rs485_open(struct rs485_obj* port, uint8_t port_number, uint8_t baudrate);

bool rs485_close(struct rs485_obj* port);

bool rs485_read(struct rs485_obj* port, uint8_t* buffer, uint32_t buffer_size, uint32_t* bytes_read);

bool rs232_write(struct rs232_obj* port, uint8_t* buffer, uint32_t buffer_size);
*/
bool createPort(HANDLE* comPort, char* portName);

bool initPort(HANDLE* comPort, char* portName, int ComRate, int ComBits, COMMTIMEOUTS timeout);

bool purgePort(HANDLE* comPort);

DWORD readPort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size);

DWORD writePort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size);

bool setComParameters(HANDLE* comPort, int comRate, int comBits, COMMTIMEOUTS timeout);