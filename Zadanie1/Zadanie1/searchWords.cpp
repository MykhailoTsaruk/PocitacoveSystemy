#include "searchWords.h"

char* searchWords(const char* pattern, int length, const char* filename) {
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        fprintf(stderr, "Unable to open the file: %s\n", filename);
        return NULL;
    }
    if (strlen(pattern) != length+1)
        return NULL;

    char line[256]; // ������������ ����� ������ � �����
    char word[MAX_LENGTH]; // ������������ ����� �����
    char* result = NULL; // ��������� ������

    while (fgets(line, sizeof(line), file) != NULL) {
        char* context;
        char* token = strtok_s(line, " ", &context);
        while (token != NULL) {
            // �������� ����� �����
            if (strlen(token) == (size_t)length) {
                // �������� ������� �����
                int i;
                for (i = 0; i < length; ++i) {
                    if (pattern[i] != '_' && pattern[i] != token[i]) {
                        break;
                    }
                }

                if (i == length) {
                    // ���������� �������
                    if (result == NULL) {
                        result = _strdup(token); // strdup �������� ������ � �������� ������
                    }
                    else {
                        size_t currentSize = strlen(result);
                        size_t tokenSize = length;
                        result = (char*)realloc(result, currentSize + tokenSize + 1); // +1 for '\0'
                        strcat_s(result, currentSize + tokenSize, token);
                    }
                }
            }

            token = strtok_s(NULL, " ", &context);
        }
    }

    fclose(file);
    return result;
}