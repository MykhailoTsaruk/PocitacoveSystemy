#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <string.h>
#include <conio.h>
#include <thread>
#include <cassert>
#include <iostream>

bool createPort(HANDLE* comPort, char* portName);

bool initPort(HANDLE* comPort, char* portName, uint32_t ComRate, int ComBits, COMMTIMEOUTS timeout);

bool purgePort(HANDLE* comPort);

bool readPort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesRead, bool receive);

bool writePort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesWritten, bool receive);

bool setComParameters(HANDLE* comPort, uint32_t comRate, int comBits, COMMTIMEOUTS timeout);