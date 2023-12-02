#pragma once
#include <stdio.h>
#include <math.h>

void calculate(double operand1, char mathOperator, double operand2, double* result) {
	switch (mathOperator) {
		case '+':
			*result = operand1 + operand2; 
			break;
		
		case '-':
			*result = operand1 - operand2;
			break;
		
		case '*':
			*result = operand1 * operand2;
			break;
		
		case '/':
			*result = operand1 / operand2;
			break;
		
		case '^':
			*result = pow(operand1, operand2);
			break;
		
		case 's':
			if (operand1 >= 0) {
				*result = sqrt(operand1);
			} else{
				fprintf(stderr, "Error: Cannot calculate square root of a negative number\n");
				*result = 0.0;
			}
			break;
		
		case '!':
			if (operand1 >= 0 || operand1 - (long long int)operand1 == 0) {
				*result = 1.0;
				for (int i = 1; i <= (long long int)operand1; i++) {
					*result *= i;
				}
			} else {
				fprintf(stderr, "Error: Cannot calculate factorial\n");
				*result = 0.0;
			}
			break;

		case 'e':
			*result = exp(operand1);
			break;

		default:
			fprintf(stderr, "Error: Unsupported operator\n");
			*result = 0.0;
	}
}