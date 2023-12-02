#include "client.h"

#define BUFFER_SIZE 128

void addToBuffer(char buffer[], long long int num,	int* index) {
	long long int temp = num;
	int i = *index;

	do {
		buffer[*index] = temp % 10 + '0';
		temp /= 10;
		*index += 1;
	} while (temp >= 1);

	for (int j = *index - 1; i < j; i++, j--) {
		temp = buffer[j];
		buffer[j] = buffer[i];
		buffer[i] = temp;
	}

	buffer[*index] = '|';
	*index += 1;
}

int client() {
   /*
	HANDLE pipe;
	pipe = CreateFileA(
		"\\\\.\\pipe\\MyPipe",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (pipe == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Cannot open pipe: 0x%x\n", GetLastError());
		return 1;
	}
	
	char operator;
	char buf[5];

	printf("Enter operator (+, -, *, /, ^, sqrt, !, exp): ");
	scanf_s("%4s", &buf, sizeof(buf));
	operator = buf[0];

	char buffer[BUFFER_SIZE];
	int index = 0;
	if (operator == 's' || operator == '!' || operator == 'e') {
		long long int operand1 = 0;
		printf("Enter 1 integer operand (e.g. 7): ");
		scanf_s("%lld", &operand1);

		buffer[index++] = operator;
		buffer[index++] = '|';
		addToBuffer(buffer, operand1, &index);
		buffer[index++] = '\0';
	} 
	else if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '^') {
		long long int operand1 = 0, operand2 = 0;
		printf("Enter 2 integer operands (e.g. 7 9): ");
		scanf_s("%lld %lld", &operand1, &operand2);
		
		buffer[index++] = operator;
		buffer[index++] = '|';
		addToBuffer(buffer, operand1, &index);
		addToBuffer(buffer, operand2, &index);
		buffer[index++] = '\0';
	}
	else {
		printf("Wrong operator\n");
		return 1;
	}
	int bytesWritten = 0;
	WriteFile(pipe, buffer, sizeof(buffer), &bytesWritten, NULL);

	Sleep(50);
	int bytesRead = 0;
	char requestData[BUFFER_SIZE];
	ReadFile(pipe, requestData, sizeof(requestData), &bytesRead, NULL);

	printf("Result = %s\n", requestData);
   */

	HANDLE pipe;

	// Rest remains same  
	pipe = CreateFileA(
		"\\\\.\\pipe\\MyPipe",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL
	);

	char operator;
	char buffer[5];
	printf("Enter operation (+, -, *, /, ^, sqrt, !, exp): ");
	scanf_s("%4s", &buffer, sizeof(buffer));
	operator = buffer[0];

	double num1 = 0, num2 = 0, result = 0;
	if (operator == 's' || operator == '!' || operator == 'e') {
		printf("Enter one operand (e.g. 7): ");
		scanf_s("%lf", &num1);
	}
	else if (operator == '+' || operator == '-' || operator == '*' || operator == '/' || operator == '^') {
		printf("Enter two operands (e.g. 7 9): ");
		scanf_s("%lf %lf", &num1, &num2);
	}
	else {
		printf("Wrong operator\n");
		return 1;
	}

	WriteFile(pipe, &operator, sizeof(char), NULL, NULL);
	WriteFile(pipe, &num1, sizeof(double), NULL, NULL);
	WriteFile(pipe, &num2, sizeof(double), NULL, NULL);

	ReadFile(pipe, &result, sizeof(double), NULL, NULL);

	printf("Result is: %lf\n", result);

	CloseHandle(pipe);

    return 0;
}
