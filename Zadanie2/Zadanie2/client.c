#include "client.h"

int client() {
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

	if (operator == 's' && num1 < 0) {
		fprintf(stderr, "Error: Cannot calculate square root of a negative number\n");
	}
	else if (operator == '!' && num1 < 0) {
		fprintf(stderr, "Error: Cannot calculate factorial\n");
	}

	WriteFile(pipe, &operator, sizeof(char), NULL, NULL);
	WriteFile(pipe, &num1, sizeof(double), NULL, NULL);
	WriteFile(pipe, &num2, sizeof(double), NULL, NULL);

	ReadFile(pipe, &result, sizeof(double), NULL, NULL);

	printf("Result is: %lf\n", result);

	CloseHandle(pipe);

    return 0;
}
