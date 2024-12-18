#pragma once
#pragma GCC optimize(3,"Ofast","inline")

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#define _CRT_SECURE_NO_WARNINGS
#define MAX_QUESTION_COUNT 100
#define MAX_USERNAME_LEN 10
#define USER_DATA_DIR "user_data"
#define RESULT_REPORT_DIR "�������"

#ifdef _WIN32
#include <windows.h>  // Windowsƽ̨֧��
#elif defined(__APPLE__)
#include <unistd.h>   // macOSƽ̨֧��
#endif

typedef struct {
    int num1;
    int num2;
    int num3;
    char operator1;
    char operator2;
    int correct_answer;
    int user_answer;  // ����û�����Ĵ�
} Question;
Question questions[MAX_QUESTION_COUNT]; // �洢��Ŀ
Question wrong_questions[MAX_QUESTION_COUNT]; // �洢����
Question current_wrong_questions[MAX_QUESTION_COUNT];  // �洢������ϰ�Ĵ���

int wrong_question_count = 0;
int total_question_count = 0;
int correct_count = 0;
int wrong_count = 0;
int current_wrong_count = 0;
int total_score = 0;
char username[MAX_USERNAME_LEN];
double spend_time = 0.0;   // ��ϰ��ʱ
time_t start_time, end_time; // ��¼��ϰ��ʼ�ͽ�����ʱ��

void reset_wrong_questions();  // ��ʼ����������
void load_previous_wrong_questions();  // ���ش���
void print_separator();  // �ָ��ߴ�ӡ
void create_directory(const char* path);  // �ļ��д���ģ��
void get_username();  // �û�����ȡģ��
void get_user_input();  // ��Ŀ������ȡģ��
void generate_random_num();  // ���������ģ��
void load_previous_wrong_questions();  // �û��ļ���ȡģ��
void ask_question(int question_number);  // ��ʾ��Ŀ������ģ��
void save_previous_wrong_questions();  // ���Ᵽ��ģ��
void display_results();  // ��ϰ�����ʾģ��
void save_results();  // ������浼��ģ��

#endif