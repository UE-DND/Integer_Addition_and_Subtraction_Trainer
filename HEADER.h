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
#define RESULT_REPORT_DIR "结果报告"

#ifdef _WIN32
#include <windows.h>  // Windows平台支持
#elif defined(__APPLE__)
#include <unistd.h>   // macOS平台支持
#endif

typedef struct {
    int num1;
    int num2;
    int num3;
    char operator1;
    char operator2;
    int correct_answer;
    int user_answer;  // 添加用户输入的答案
} Question;
Question questions[MAX_QUESTION_COUNT]; // 存储题目
Question wrong_questions[MAX_QUESTION_COUNT]; // 存储错题
Question current_wrong_questions[MAX_QUESTION_COUNT];  // 存储本次练习的错题

int wrong_question_count = 0;
int total_question_count = 0;
int correct_count = 0;
int wrong_count = 0;
int current_wrong_count = 0;
int total_score = 0;
char username[MAX_USERNAME_LEN];
double spend_time = 0.0;   // 练习用时
time_t start_time, end_time; // 记录练习开始和结束的时间

void reset_wrong_questions();  // 初始化错题数组
void load_previous_wrong_questions();  // 加载错题
void print_separator();  // 分割线打印
void create_directory(const char* path);  // 文件夹创建模块
void get_username();  // 用户名获取模块
void get_user_input();  // 题目数量获取模块
void generate_random_num();  // 随机数生成模块
void load_previous_wrong_questions();  // 用户文件读取模块
void ask_question(int question_number);  // 显示题目并检查答案模块
void save_previous_wrong_questions();  // 错题保存模块
void display_results();  // 练习结果显示模块
void save_results();  // 结果报告导出模块

#endif