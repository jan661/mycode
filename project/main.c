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
        fprintf(file, "사용자: %s\n챗봇: %s\n\n", user_input, response);
        fclose(file);
    } else {
        printf("로그 파일을 열 수 없습니다.\n");
    }
}

void getResponse(const char *input, char *response) {
    for (int i = 0; i < qa_count; i++) {
        if (strstr(input, qa_list[i].question) != NULL) {
            strcpy(response, qa_list[i].response);
            return;
        }
    }
    strcpy(response, "죄송합니다, 잘 이해하지 못했습니다.");
}

void learnResponse(const char *question, const char *response) {
    if (qa_count < MAX_RESPONSES) {
        strcpy(qa_list[qa_count].question, question);
        strcpy(qa_list[qa_count].response, response);
        qa_count++;
        printf("새로운 응답을 학습했습니다!\n");
    } else {
        printf("더 이상 학습할 수 없습니다.\n");
    }
}

void loadResponses() {
    FILE *file = fopen(INIT_FILE, "r");
    if (file != NULL) {
        while (qa_count < MAX_RESPONSES &&
               fgets(qa_list[qa_count].question, MAX_INPUT, file) &&
               fgets(qa_list[qa_count].response, MAX_INPUT, file)) {
            qa_list[qa_count].question[strcspn(qa_list[qa_count].question, "\n")] = 0; // 개행 문자 제거
            qa_list[qa_count].response[strcspn(qa_list[qa_count].response, "\n")] = 0; // 개행 문자 제거
            qa_count++;
        }
        fclose(file);
    } else {
        printf("파일을 열 수 없습니다.\n");
    }
}

int main() {
    char input[MAX_INPUT];
    char response[MAX_INPUT];

    loadResponses();

    printf("챗봇에 오신 것을 환영합니다! (종료하려면 '종료' 입력)\n");

    while (1) {
        printf("사용자: ");
        fgets(input, MAX_INPUT, stdin);
        input[strcspn(input, "\n")] = 0; // 개행 문자 제거

        if (strcmp(input, "종료") == 0) {
            printf("챗봇: 안녕히 가세요!\n");
            break;
        }

        getResponse(input, response);
        printf("챗봇: %s\n", response);

        if (strcmp(response, "죄송합니다, 잘 이해하지 못했습니다.") == 0) {
            printf("이 질문에 대한 응답을 입력해 주세요: ");
            fgets(response, MAX_INPUT, stdin);
            response[strcspn(response, "\n")] = 0; // 개행 문자 제거
            learnResponse(input, response);
            logResponse(input, response);
        } else {
            logResponse(input, response);
        }
    }

    return 0;
}
