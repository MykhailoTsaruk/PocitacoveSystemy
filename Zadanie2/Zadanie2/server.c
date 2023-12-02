#include "calculator.h"
#include "server.h"

#define BUFFER_SIZE 128

long long int stringToNumber(char buffer[]) {
    long long int result = 0;
    int i = 0;

    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] != '\0' && buffer[i] >= '0' && buffer[i] <= '9') {
			result = result * 10 + (buffer[i] - '0');
        }
    }

    return result;
}

int server() {
    HANDLE pipe;

    pipe = CreateNamedPipeA(
        "\\\\.\\pipe\\MyPipe",
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        sizeof(double) * 2 + sizeof(char), 
        sizeof(double),
        NMPWAIT_WAIT_FOREVER,
        NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();

        LPVOID errorMsg;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            error,
            0,
            (LPWSTR)&errorMsg,
            0,
            NULL
        );

        fprintf(stderr, "Cannot create pipe: %ls\n", errorMsg);

        LocalFree(errorMsg);
        return 1;
    }
    printf("Pipe create successful\n");
    ConnectNamedPipe(pipe, NULL);

    char operator;
    double num1 = 0, num2 = 0, result;
    
    ReadFile(pipe, &operator, sizeof(char), NULL, NULL);
    ReadFile(pipe, &num1, sizeof(double), NULL, NULL);
    ReadFile(pipe, &num2, sizeof(double), NULL, NULL);

    printf("%c %f %f\n", operator, num1, num2);

    calculate(num1, operator, num2, &result);

    printf("%f\n", result);
    WriteFile(pipe, &result, sizeof(double), NULL, NULL);
    DisconnectNamedPipe(pipe);

	return 0;
}