#include "rs485.h"

void sendRequest() {

}

void getRequest() {

}

void sendResponse() {

}

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
	const int baudRate = 9600;
	const int bitsPerByte = 8;
	COMMTIMEOUTS timeout = { 0 };

	sprintf_s(portName, "COM%d\0", port_number);

	if (initPort(&comPort, portName, baudRate, bitsPerByte, timeout) == 0)
		return 0;
	Sleep(250);
	printf("%s\n", argv[1]);
	if (strcmp(argv[1], "rx") == 0) {
		printf("Enter you pattern(use _ to represent any letter) and lenght: ");
		DWORD bytesWrite;
		char word[bufferSize];
		int* wordLenght = 0;
		if (scanf_s("%s %d", word, wordLenght) == 0) {
			fprintf(stderr, "Do word was entered\n");
			return 1;
		}
		bytesWrite = writePort(&comPort, word, *wordLenght);
		assert(bytesWrite == 0);
	} else if (strcmp(argv[1], "tx") == 0) {
		DWORD bytesRead;
		char readData[bufferSize];
		bytesRead = readPort(&comPort, readData, bufferSize);
		assert(bytesRead == 0);
	} else {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 2;
	}

	return 0;
}