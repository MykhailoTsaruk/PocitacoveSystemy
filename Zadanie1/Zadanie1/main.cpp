#include "rs485.h"

int main(int argc, char* argv[]) {
	if (argv[1] == NULL) {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 1;
	}

	int port_number;
	printf("Select serial port: COM");

	if (scanf_s("%d", &port_number) == 0) {
		fprintf(stderr, "Not a number!\n");
		return 1;
	}
	if (port_number < 0 || port_number > 255) {
		fprintf(stderr, "Invalid port number\n");
		return 1;
	}
	
	char portName[6];
	HANDLE comPort;
	const int bufferSize = 32;
	const uint32_t baudRate = 9600;
	const int bitsPerByte = 8;
	COMMTIMEOUTS timeout = { 0 };

	sprintf_s(portName, "COM%d\0", port_number);

	if (initPort(&comPort, portName, baudRate, bitsPerByte, timeout) == 0)
		return 0;
	Sleep(200);

	printf("%s\n", argv[1]);

	if (strcmp(argv[1], "rx") == 0) {
		printf("Enter you pattern(use _ to represent any letter) and lenght: ");
		char word[bufferSize];
		char buffer[2][bufferSize];
		uint8_t wordLength;
		uint32_t bytes;
		if (scanf_s("%63s %d", word, sizeof(word), &wordLength) == 2) {
			printf("Word successful entered\n");
			for (int i = 0; i < strlen(word); i++) {
				buffer[0][i] = word[i];
			}
			buffer[0][strlen(word)] = '\0';
			buffer[1][0] = (char)wordLength;
			buffer[1][1] = '\0';
			printf("this word in buffer: %d\n", buffer[1][0]);
			if (writePort(&comPort, (uint8_t*)word, wordLength, bufferSize, &bytes) == 0) {
				printf("Error writing to serial port\n");
				return 10;
			} else{
				printf("Success wirting to serial port\n");
			}
		} else {
			fprintf(stderr, "Do word was entered\n");
			return 1;
		}
	} else if (strcmp(argv[1], "tx") == 0) {
		char readData[2][bufferSize];
		uint32_t bytes;
		if (readPort(&comPort, (uint8_t**)readData, bufferSize, &bytes) == 0) {
			printf("Error reading from serial port\n");
			return 11;
		} else {
			printf("Success reading from serial port\n");
		}
	} else{
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 2;
	}

	return 0;
}