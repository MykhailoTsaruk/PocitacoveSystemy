#include "rs485.cpp"

void sendRequest() {

}

void getRequest() {

}

void sendResponse() {

}

int main(int argc, char** argv) {
	if (argv[1] == NULL) {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 1;
	}

	int port_number;
	printf("Select serial port: COM");
	
	if (scanf("%d", &port_number) != 1) {
		fprintf(stderr, "Not a number!\n");
		return 1;
	}
	if (port_number < 0 || port_number > 255) {
		fprintf(stderr, "Invalid port number\n");
		return 1;
	}

	char portName[6];
	HANDLE comPort;
	const int bufferSize = 256;
	const int baudRate = 9600;
	const int bitsPerByte = 8;
	COMMTIMEOUTS timeout;

	sprintf(portName, "COM%d\0", port_number);

	initPort(&comPort, portName, baudRate, bitsPerByte, timeout);
	Sleep(1000);

	if (argv[1] == "rx") {
		printf("Enter you pattern(use _ to represent any letter) and lenght: ");
		char* word;
		int* wordLenght;
		if (scanf("%s %d", word, &wordLenght) == 0) {
			fprintf(stderr, "Do word was entered\n");
			return 1;
		}
		assert(writePort(&comPort, word, *wordLenght) == 0);
	} else if (argv[1] == "tx") {
		DWORD bytesRead;
		char* readData;
		bytesRead = readPort(&comPort, readData, sizeof(readData));
		assert(bytesRead == 0);
	} else {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 1;
	}

	return 0;
}