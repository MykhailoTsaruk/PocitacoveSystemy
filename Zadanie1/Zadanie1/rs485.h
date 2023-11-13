#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>
#include <cassert>

bool createPort(HANDLE* comPort, char* portName);

bool initPort(HANDLE* comPort, char* portName, int ComRate, int ComBits, COMMTIMEOUTS timeout);

bool purgePort(HANDLE* comPort);

DWORD readPort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size);

DWORD writePort(HANDLE* comPort, LPVOID buffer, DWORD buffer_size);

bool setComParameters(HANDLE* comPort, int comRate, int comBits, COMMTIMEOUTS timeout);