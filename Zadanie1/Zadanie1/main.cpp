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

	if (strcmp(argv[2], "debug")) {
#define DEBUG 1
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

#ifdef DEBUG
	printf("%s\n", argv[1]);
#endif // DEBUG

	if (strcmp(argv[1], "rx") == 0) {
		printf("Enter you pattern(use _ to represent any letter) and lenght: ");
		char word[bufferSize];
		char* buffer[2];
		uint8_t wordLength;
		uint32_t bytes;
		if (scanf_s("%31s %hhu", word, sizeof(word), &wordLength) == 2) {
#ifdef DEBUG
			printf("%s + %d\n", word, wordLength);
			printf("Word successful entered\n");
#endif // DEBUG

			buffer[0] = (char*)malloc(strlen(word) + 1);
			buffer[1] = (char*)malloc(2);
			if (buffer[0] == NULL || buffer[1] == NULL) {
#ifdef DEBUG
				printf("Memory allocation failed\n");
#endif // DEBUG
				return 13;
			}

			strcpy_s(buffer[0], strlen(word) + 1, word);
			buffer[0][strlen(word)] = '\0';
			buffer[1][0] = (char)wordLength;
			buffer[1][1] = '\0';

#ifdef DEBUG
			printf("Word in buffer: %s\n", buffer[0]);
			printf("Lenght word in buffer: %d\n", buffer[1][0]);
#endif // DEBUG

			if (writePort(&comPort, buffer, bufferSize, &bytes, false) == 0) {
				printf("Error writing to serial port\n");
				free(buffer[0]);
				free(buffer[1]);
				return 10;
			}
			else {
				printf("Success writing to serial port\n");
			}

#ifdef DEBUG
			printf("END FUNCTION buffer[0] = %s\n", buffer[0]);
			printf("END FUNCTION buffer[1] = %d\n", buffer[1][0]);
			printf("END FUNCTION bytes = %d\n", bytes);
#endif // DEBUG

			free(buffer[0]);
			free(buffer[1]);

			// Second part
			Sleep(1000);
			printf("Buffer clear successfully\n");
			char* resultWord[1];
			resultWord[0] = (char*)malloc(wordLength);
			if (readPort(&comPort, resultWord, wordLength, &bytes, true) == 0) {
				printf("Error reading from serial port\n");
				free(resultWord[0]);
				return 10;
			}
			else {
				printf("Success writing to serial port\n");
			}
		}
		else {
			fprintf(stderr, "No word was entered\n");
			return 1;
		}
	}
	else if (strcmp(argv[1], "tx") == 0) {
		char* readData[2];
		readData[0] = (char*)malloc(bufferSize);
		readData[1] = (char*)malloc(2);
		uint32_t bytes;
		if (readPort(&comPort, readData, bufferSize, &bytes, false) == 0) {
			free(readData[0]);
			free(readData[1]);
			printf("Error reading from serial port\n");
			return 11;
		}
		else {
			printf("Success reading from serial port\n");
		}

#ifdef DEBUG
		printf("END FUNCTION readData[0] = %s\n", readData[0]);
		printf("END FUNCTION readData[1] = %d\n", readData[1][0]);
		printf("END FUNCTION bytes = %d\n", bytes);
#endif // DEBUG

		free(readData[1]);

		// Second part
		Sleep(1000);
		char* sendData[1];
		sendData[0] = (char*)malloc(bytes + 1);

		for (int i = 0; i < strlen(readData[0]); i++) {
			if (readData[0][i] >= 'a' && readData[0][i] <= 'z') {
				sendData[0][i] = readData[0][i] - 32;
			}
			else if (readData[0][i] >= 'A' && readData[0][i] <= 'Z') {
				sendData[0][i] = readData[0][i] + 32;
			}
			else {
				sendData[0][i] = readData[0][i];
			}

		}
		sendData[0][strlen(readData[0])] = '\0';
		printf("SENDDATA = %s\n", sendData[0]);

		uint32_t bytesSend;
		if (writePort(&comPort, sendData, bytes, &bytesSend, true) == 0) {
			free(readData[0]);
			free(sendData[0]);
			printf("Error writing to serial port\n");
			return 12;
		}
		else {
			printf("Success writing to serial port\nBytes send: %d\n", bytesSend);
		}
		free(readData[0]);
		free(sendData[0]);


	}
	else {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 2;
	}

	CloseHandle(comPort);

	return 0;
}