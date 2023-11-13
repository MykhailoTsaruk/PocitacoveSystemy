#include "rs485.cpp"

void sendRequest() {

}

void getRequest() {

}

void sendResponse() {

}

int main() {
	int port_number;
	printf("Select serial port: COM");
	
	if (scanf("%d", &port_number) != 1) {
		fprintf(stderr, "Not a number!\n");
		return 1;
	}
	if (port_number < 0 || port_number > 255) {
		fprintf(stderr, "Invalid port number\n");
		return 2;
	}

	char portName[6];
	HANDLE comPort;
	const int bufferSize = 256;
	const int baudRate = 9600;
	const int bitsPerByte = 8;
	COMMTIMEOUTS timeout;

	std::sprintf(portName, "COM%d\0", port_number);

	initPort(&comPort, portName, baudRate, bitsPerByte, timeout);
	Sleep(1000);

	return 0;
}