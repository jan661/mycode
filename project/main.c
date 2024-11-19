#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define LOG_FILE "chat_log.txt"
#define INIT_FILE "initial_responses.txt"
#define MAX_RESPONSES 100

typedef struct {
    char question[MAX_INPUT];
    char response[MAX_INPUT];
} QA;

QA qa_list[MAX_RESPONSES];
int qa_count = 0;

void logResponse(const char *user_input, const char *response) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file != NULL) {
        fprintf(file, "�����: %s\nê��: %s\n\n", user_input, response);
        fclose(file);
    } else {
        printf("�α� ������ �� �� �����ϴ�.\n");
    }
}

void getResponse(const char *input, char *response) {
    for (int i = 0; i < qa_count; i++) {
        if (strstr(input, qa_list[i].question) != NULL) {
            strcpy(response, qa_list[i].response);
            return;
        }
    }
    strcpy(response, "�˼��մϴ�, �� �������� ���߽��ϴ�.");
}

void learnResponse(const char *question, const char *response) {
    if (qa_count < MAX_RESPONSES) {
        strcpy(qa_list[qa_count].question, question);
        strcpy(qa_list[qa_count].response, response);
        qa_count++;
        printf("���ο� ������ �н��߽��ϴ�!\n");
    } else {
        printf("�� �̻� �н��� �� �����ϴ�.\n");
    }
}

void loadResponses() {
    FILE *file = fopen(INIT_FILE, "r");
    if (file != NULL) {
        while (qa_count < MAX_RESPONSES &&
               fgets(qa_list[qa_count].question, MAX_INPUT, file) &&
               fgets(qa_list[qa_count].response, MAX_INPUT, file)) {
            qa_list[qa_count].question[strcspn(qa_list[qa_count].question, "\n")] = 0; // ���� ���� ����
            qa_list[qa_count].response[strcspn(qa_list[qa_count].response, "\n")] = 0; // ���� ���� ����
            qa_count++;
        }
        fclose(file);
    } else {
        printf("������ �� �� �����ϴ�.\n");
    }
}

int main() {
    char input[MAX_INPUT];
    char response[MAX_INPUT];

    loadResponses();

    printf("ê���� ���� ���� ȯ���մϴ�! (�����Ϸ��� '����' �Է�)\n");

    while (1) {
        printf("�����: ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = 0; // ���� ���� ����

        if (strcmp(input, "����") == 0) {
            printf("ê��: �ȳ��� ������!\n");
            break;
        }

        getResponse(input, response);
        printf("ê��: %s\n", response);

        if (strcmp(response, "�˼��մϴ�, �� �������� ���߽��ϴ�.") == 0) {
            printf("�� ������ ���� ������ �Է��� �ּ���: ");
            fgets(response, MAX_INPUT, stdin);
            response[strcspn(response, "\n")] = 0; // ���� ���� ����
            learnResponse(input, response);
            logResponse(input, response);
        } else {
            logResponse(input, response);
        }
    }

    return 0;
}
