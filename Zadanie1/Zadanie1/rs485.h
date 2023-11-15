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

bool readPort(HANDLE* comPort, uint8_t* buffer, uint32_t bufferSize, uint32_t* bytesRead);

int writePort(HANDLE* comPort, uint8_t* buffer, uint32_t bufferSize);

bool setComParameters(HANDLE* comPort, uint32_t comRate, int comBits, COMMTIMEOUTS timeout);