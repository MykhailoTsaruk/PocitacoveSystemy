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
#include <mutex>

/// <summary>
/// Opens a serial port and creates a handle for communication.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable to store the created port handle.</param>
/// <param name="portName">The name of the serial port to be opened (e.g., "COM1").</param>
/// <returns>Returns true (1) if the port is successfully opened, false (0) otherwise.</returns>
bool createPort(HANDLE* comPort, char* portName);

/// <summary>
/// Initializes a serial port by opening, purging outstanding requests, and setting parameters.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable to store the created port handle.</param>
/// <param name="portName">The name of the serial port to be opened (e.g., "COM1").</param>
/// <param name="ComRate">The baud rate for communication.</param>
/// <param name="ComBits">The number of data bits in each byte.</param>
/// <param name="timeout">COMMTIMEOUTS structure specifying the communication timeouts.</param>
/// <returns>Returns true (1) if the serial port is successfully initialized, false (0) otherwise.</returns>
bool initPort(HANDLE* comPort, char* portName, uint32_t ComRate, int ComBits, COMMTIMEOUTS timeout);

/// <summary>
/// Purges any outstanding requests on the specified serial port.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable representing the open serial port.</param>
/// <returns>Returns true (1) if the outstanding requests are successfully purged, false (0) otherwise.</returns>
bool purgePort(HANDLE* comPort);

/// <summary>
/// Reads data from the specified serial port into a buffer.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable representing the open serial port.</param>
/// <param name="buffer">Pointer to an array to store the read data.</param>
/// <param name="bufferSize">The size of the buffer in bytes.</param>
/// <param name="bytesRead">Pointer to a variable to store the number of bytes actually read.</param>
/// <param name="receive">If true, the function reads from the receive buffer; if false, it reads from the transmit buffer.</param>
/// <returns>Returns true (1) if the data is successfully read, false (0) otherwise.</returns>
bool readPort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesRead, bool receive);

/// <summary>
/// Writes data to the specified serial port from a buffer.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable representing the open serial port.</param>
/// <param name="buffer">Pointer to an array containing the data to be written.</param>
/// <param name="bufferSize">The size of the buffer in bytes.</param>
/// <param name="bytesWritten">Pointer to a variable to store the number of bytes actually written.</param>
/// <param name="receive">If true, the function writes to the receive buffer; if false, it writes to the transmit buffer.</param>
/// <returns>Returns true (1) if the data is successfully written, false (0) otherwise.</returns>
bool writePort(HANDLE* comPort, char* buffer[], uint32_t bufferSize, uint32_t* bytesWritten, bool receive);

/// <summary>
/// Configures communication parameters for the specified serial port.
/// </summary>
/// <param name="comPort">Pointer to a HANDLE variable representing the open serial port.</param>
/// <param name="comRate">The baud rate for communication.</param>
/// <param name="comBits">The number of data bits in each byte.</param>
/// <param name="timeout">COMMTIMEOUTS structure specifying the communication timeouts.</param>
/// <returns> Returns true (1) if the communication parameters are successfully set, false (0) otherwise.</returns>
bool setComParameters(HANDLE* comPort, uint32_t comRate, int comBits, COMMTIMEOUTS timeout);

