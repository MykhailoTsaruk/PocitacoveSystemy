#include "rs485.h"
#include "searchWords.h"

// Create mutex
std::mutex mtx;

// Function to be executed in a separate thread for the server
void serverThread(HANDLE comPort, int bytes, char* sendData[]) {
	// Lock the mutex to ensure exclusive access to shared resources
	std::lock_guard<std::mutex> lock(mtx);

	// Write data to the serial port
	uint32_t bytesSend;
	if (writePort(&comPort, sendData, bytes, &bytesSend, true) == 0) {
		printf("Error writing to serial port\n");
		return;
	}
	else {
		printf("Success writing %d bytes to serial port\n\n", bytes);
	}
}

int main(int argc, char* argv[]) {
	// Check if an argument is provided
	if (argv[1] == NULL) {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 1;
	}

	int portNumber;
	printf("Select serial port: COM");

	// Get the serial port number from user input
	if (scanf_s("%d", &portNumber) == 0) {
		fprintf(stderr, "Not a number!\n");
		return 1;
	}
	if (portNumber < 0 || portNumber > 255) {
		fprintf(stderr, "Invalid port number\n");
		return 1;
	}
	
	// Variables for serial port communication
	char portName[6];
	HANDLE comPort;
	const int bufferSize = 32;
	const uint32_t baudRate = 9600;
	const int bitsPerByte = 8;
	COMMTIMEOUTS timeout = { 0 };
	
	// Format the serial port name
	sprintf_s(portName, "COM%d\0", portNumber);
	
	// Start client
	if (strcmp(argv[1], "rx") == 0) {
		// Prompt the user to enter a pattern and length
		printf("Enter you pattern(use _ to represent any letter) and lenght: ");
		char word[bufferSize];
		char* buffer[2];
		uint8_t wordLength;
		uint32_t bytes;

		// Read user input for the word and its length
		if (scanf_s("%31s %hhu", word, sizeof(word), &wordLength) == 2) {
			// Check if the entered word length matches the actual length
			if (strlen(word) != wordLength) {
				fprintf(stderr, "Wrong word length\n");
				return 1;
			}
			// Initialize the serial port
			if (initPort(&comPort, portName, baudRate, bitsPerByte, timeout) == 0)
				return 0;
			Sleep(200);
			printf("**Word successful entered**\n");

			// Transform the word to lower case
			for (char* ptr = word; *ptr; ptr++) {
				*ptr = tolower(*ptr);
			}

			// Allocate memory for buffer
			buffer[0] = (char*)malloc(strlen(word) + 1);
			buffer[1] = (char*)malloc(2);
			if (buffer[0] == NULL || buffer[1] == NULL) {
				printf("Memory allocation failed\n");
				return 13;
			}
			// Transforming a string from input into a two-dimensional buffer
			strcpy_s(buffer[0], strlen(word) + 1, word);
			buffer[0][strlen(word)] = '\0';
			buffer[1][0] = (char)wordLength;
			buffer[1][1] = '\0';

			// Write data to the serial port for receiving
			if (writePort(&comPort, buffer, bufferSize, &bytes, false) == 0) {
				printf("Error writing to serial port\n");
				free(buffer[0]);
				free(buffer[1]);
				return 10;
			}
			else {
				printf("**Success writing to serial port**\n");
			}
			// Freeing memory
			free(buffer[0]);
			free(buffer[1]);

			// Second part || getting a response
			Sleep(50);
			printf("**Buffer clear successfully**\n");

			// Allocate memory for response
			char* resultWord[1];
			resultWord[0] = (char*)malloc(wordLength);
			// Read data from the serial port for response
			if (readPort(&comPort, resultWord, wordLength, &bytes, true) == 0) {
				printf("Error reading from serial port\n");
				free(resultWord[0]);
				return 10;
			}
			else {
				printf("You word: %s\n", resultWord[0]);
			}
		}
		else {
			fprintf(stderr, "No word was entered\n");
			return 1;
		}
	}
	// Start server
	else if (strcmp(argv[1], "tx") == 0) {
		// Initialize the serial port for the server
		if (initPort(&comPort, portName, baudRate, bitsPerByte, timeout) == 0)
			return 0;
		Sleep(100);

		while (true) {
			// Allocate memory for data to be read from the serial port
			char* readData[2];
			readData[0] = (char*)malloc(bufferSize);
			readData[1] = (char*)malloc(2);
			uint32_t bytes;

			// Read data from the serial port for transmission
			if (readPort(&comPort, readData, bufferSize, &bytes, false) == 0) {
				free(readData[0]);
				free(readData[1]);
				printf("Error reading from serial port\n");
				return 11;
			}
			else {
				printf("\nSuccess reading from serial port\n");
			}

			// Second part || sending a response
			Sleep(100);

			// Allocate memory for data to be sent to the serial port
			char* sendData[1];
			sendData[0] = (char*)malloc(bytes);
			// Copy data to be sent, excluding the null terminator
			memcpy(sendData[0], readData[0], bytes-1);
			sendData[0][bytes-1] = '\0';

			// Perform word search and update sendData
			const char* fileName = "D:\\Programming\\PocitacoveSystemy\\Zadanie1\\Zadanie1\\text.txt";
			sendData[0] = searchWords(readData[0], (int)readData[1][0], fileName);
			// Freeing memory
			free(readData[0]);
			free(readData[1]);

			// Create a thread for server communication
			std::thread server(serverThread, comPort, bytes, sendData);
			server.join();

			// Wait for some time before freeing memory
			Sleep(400);
			free(sendData[0]);
		}
	}
	// If incorrect arguments are given
	else {
		printf("Please reopen file and set argument to 'rx/tx'\n");
		return 2;
	}
	// Close the serial port
	CloseHandle(comPort);

	return 0;
}