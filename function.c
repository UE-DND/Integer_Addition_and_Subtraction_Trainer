void create_directory(const char* path) {
#ifdef _WIN32
    if (_mkdir(path) != 0) {  // Windows 下使用 _mkdir 创建目录
        if (errno != EEXIST) {  // 如果目录不存在
            printf("无法创建目录 %s\n", path);
        }
    }
#elif defined(__APPLE__)
    if (mkdir(path, 0700) != 0) {  // macOS 下使用 mkdir 创建目录
        if (errno != EEXIST) {
            printf("无法创建目录 %s\n", path);
        }
    }
#endif
}


void print_separator() {
    printf("\n===================================================\n");
}


void get_username() {
    int valid_input = 0;
    char temp_username[MAX_USERNAME_LEN];
    int input_length;

    while (!valid_input) {
        printf("请输入用户名（最大长度 10 个字符）：");

        // 读取输入
        if (fgets(temp_username, sizeof(temp_username), stdin) == NULL) {
            printf("无效输入，请重新输入用户名！\n");
            print_separator();
            fflush(stdin);
            continue;
        }

        // 计算输入长度（包括换行符）
        input_length = strlen(temp_username);

        // 如果输入没有换行符，说明输入过长
        if (temp_username[input_length - 1] != '\n') {
            // 清空多余的输入
            while (getchar() != '\n');
            printf("用户名过长，请重新输入（最大长度 10 个字符）。\n");
            print_separator();
            continue;
        }

        // 去除换行符
        temp_username[input_length - 1] = '\0';

        // 检查用户名是否为空
        if (temp_username[0] == '\0') {
            printf("用户名不能为空，请重新输入。\n");
            print_separator();
            continue;
        }

        // 检查用户名是否包含空格
        if (strchr(temp_username, ' ') != NULL) {
            printf("用户名不能包含空格，请重新输入。\n");
            print_separator();
            continue;
        }

        // 检查最终用户名长度是否符合要求
        if (strlen(temp_username) > 10) {
            printf("用户名过长，请重新输入（最大长度 10 个字符）。\n");
            print_separator();
            continue;
        }

        // 所有检查通过，将用户名复制到目标变量
        strncpy(username, temp_username, MAX_USERNAME_LEN);
        valid_input = 1;
    }
}


void get_user_input() {
    int valid_input = 0;
    char total_question_count_string[MAX_QUESTION_COUNT];

    while (!valid_input) {
        printf("请输入要做的题目数量（最多100题，至少1题）:");

        if (fgets(total_question_count_string, sizeof(total_question_count_string), stdin) == NULL) {
            // 如果输入不是一个数字
            printf("无效输入，请输入一个有效的数字！\n");
            print_separator();
            fflush(stdin);
        }

        // 将输入的字符串转换为整数，并检查是否转换为有效数字（转换失败时返回 0）
        total_question_count = atoi(total_question_count_string);
        if (total_question_count == 0 && total_question_count_string[0] != '0') {
            printf("无效输入，请输入一个有效的数字！\n");
            print_separator();
            continue;
        }

        // 检查数字是否在有效范围内
        if (total_question_count < 1 || total_question_count > 100) {
            printf("输入的题目数量必须在 1 到 100 之间，请重新输入。\n");
            print_separator();
        }
        else {
            valid_input = 1;
        }
    }
}


void generate_random_num() {
    for (int i = 0; i < total_question_count; i++) {
        // 30%的概率从错题中选择
        if (wrong_question_count > 0 && (rand() % 100 < 30)) {
            int random_index = rand() % wrong_question_count;
            Question wrong_q = wrong_questions[random_index];

            // 验证错题的有效性
            int result1 = wrong_q.operator1 == '+' ?
                wrong_q.num1 + wrong_q.num2 :
                wrong_q.num1 - wrong_q.num2;
            int result2 = wrong_q.operator2 == '+' ?
                result1 + wrong_q.num3 :
                result1 - wrong_q.num3;

            // 检查计算结果是否与存储的答案匹配
            if (result2 == wrong_q.correct_answer &&
                result2 >= 0 && result2 <= 100) {
                questions[i] = wrong_q;
                questions[i].user_answer = -1;  // 重置用户答案
                continue;
            }
        }

        // 生成新的随机题目
        int num1 = rand() % 100 + 1;
        int num2 = rand() % 100 + 1;
        int num3 = rand() % 100 + 1;
        char operator1 = rand() % 2 == 0 ? '+' : '-';
        char operator2 = rand() % 2 == 0 ? '+' : '-';

        int result1 = operator1 == '+' ? num1 + num2 : num1 - num2;
        int result2 = operator2 == '+' ? result1 + num3 : result1 - num3;

        if (result2 >= 0 && result2 <= 100) {
            // 检查是否与之前的题目重复
            int is_duplicate = 0;
            for (int j = 0; j < i; j++) {
                if (questions[j].num1 == num1 &&
                    questions[j].num2 == num2 &&
                    questions[j].num3 == num3 &&
                    questions[j].operator1 == operator1 &&
                    questions[j].operator2 == operator2) {
                    is_duplicate = 1;
                    break;
                }
            }

            if (!is_duplicate) {
                questions[i] = (Question){ num1, num2, num3, operator1, operator2, result2, -1 };
            }
            else {
                i--;  // 重复则重新生成
            }
        }
        else {
            i--;  // 结果超出范围则重新生成
        }
    }
}


void reset_wrong_questions() {
    wrong_question_count = 0;  // 清空错题数量
    for (int i = 0; i < MAX_QUESTION_COUNT; i++) {
        wrong_questions[i] = (Question){ 0 };  // 重置错题数组
    }
}



void load_previous_wrong_questions() {
    char filename[100];
    sprintf(filename, "%s/%s.bin", USER_DATA_DIR, username);

    FILE* file = fopen(filename, "rb");
    if (!file) {
        wrong_question_count = 0;
        return;
    }

    size_t read_count = fread(&wrong_question_count, sizeof(int), 1, file);
    if (read_count != 1 || wrong_question_count <= 0 || wrong_question_count > MAX_QUESTION_COUNT) {
        wrong_question_count = 0;
        fclose(file);
        return;
    }

    read_count = fread(wrong_questions, sizeof(Question), wrong_question_count, file);
    if (read_count != wrong_question_count) {
        wrong_question_count = 0;
    }

    fclose(file);
}


void ask_question(int question_number) {
    Question* q = &questions[question_number];

    int user_answer;
    printf("\n题目 %d: %d %c %d %c %d = ?\n",
        question_number + 1, q->num1, q->operator1,
        q->num2, q->operator2, q->num3);
    printf("请输入答案: ");
    int flag;
    while(1) {
        flag = scanf("%d", &user_answer);
        if (flag == 1) {
            break;
        } else {
            printf("非法输入，在此重新输入一个数字：");
            while (getchar() != '\n');  // 清空输入缓冲
        }
    }

    // 存储用户答案
    q->user_answer = user_answer;

    if (user_answer == q->correct_answer) {
        printf("回答正确！\n");
        total_score += 10;
        correct_count++;
    }
    else {
        printf("回答错误，正确答案是 %d\n", q->correct_answer);
        // 同时保存到历史错题和当前错题
        wrong_questions[wrong_question_count++] = *q;
        current_wrong_questions[current_wrong_count++] = *q;
        wrong_count++;
    }

    print_separator();
}


void save_previous_wrong_questions() {
    create_directory(USER_DATA_DIR);

    char filename[100];
    sprintf(filename, "%s/%s.bin", USER_DATA_DIR, username);

    FILE* file = fopen(filename, "wb");  // 使用"wb"模式，覆盖原文件
    if (!file) {
        printf("无法保存错题文件！\n");
        return;
    }

    fwrite(&wrong_question_count, sizeof(int), 1, file);
    fwrite(wrong_questions, sizeof(Question), wrong_question_count, file);

    fclose(file);
}


void display_results() {
    printf("\n练习结束！\n");
    printf("您一共做了 %d 题，做对了 %d 题，做错了 %d 题。\n",
        total_question_count, correct_count, wrong_count);
    printf("总得分：%d\n", total_score);
    printf("完成练习用时: %.2f 秒\n", spend_time);

    if (total_score >= 90) {
        printf("\n成绩优秀！\n");
    }
    else if (total_score >= 60) {
        printf("\n成绩良好！\n");
    }
    else {
        printf("\n还需要努力！\n");
    }

    // 显示本次练习的错题
    if (current_wrong_count > 0) {
        printf("\n错误的题目及正确答案：\n");
        for (int i = 0; i < current_wrong_count; i++) {
            Question q = current_wrong_questions[i];
            printf("错误题目 %d:\n", i + 1);
            printf("%d %c %d %c %d = %d（正确答案：%d）\n",
                q.num1, q.operator1, q.num2, q.operator2,
                q.num3, q.user_answer, q.correct_answer);
        }
    }
    print_separator();
}


void save_results() {
    create_directory(RESULT_REPORT_DIR);

    char filename[100];
    sprintf(filename, "%s/%s.txt", RESULT_REPORT_DIR, username);

    // 先尝试以只读方式打开文件，检查文件是否存在
    FILE* file = fopen(filename, "r");
    if (file) {
        // 文件存在，关闭后以追加模式重新打开
        fclose(file);
        file = fopen(filename, "a");
    }
    else {
        // 文件不存在，以写入模式打开并写入标题
        file = fopen(filename, "w");
        if (file) {
            fprintf(file, "——————————— %s的练习报告 ————————————\n\n", username);
        }
    }

    if (!file) {
        return;
    }

    // 获取当前时间
    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);
    // 格式化时间为 "YYYY-MM-DD HH:MM:SS"
    char formatted_time[100];
    strftime(formatted_time, sizeof(formatted_time), "%Y年 %m月 %d日 %H:%M:%S", time_info);

    // 写入练习结果
    fprintf(file, "练习日期: %s\n", formatted_time);
    fprintf(file, "完成练习用时: %.2f 秒\n", spend_time);
    fprintf(file, "总得分: %d\n", total_score);
    fprintf(file, "错题数量: %d\n", current_wrong_count);
    for (int i = 0; i < current_wrong_count; i++) {
        Question q = current_wrong_questions[i];
        fprintf(file, "错题 %d:\n", i + 1);
        fprintf(file, "%d %c %d %c %d = %d（正确答案：%d）\n",
            q.num1, q.operator1, q.num2, q.operator2,
            q.num3, q.user_answer, q.correct_answer);
    }
    fprintf(file, "==========================================\n\n");
    fclose(file);

    printf("结果报告已保存为文件：%s, 请于本程序目录下查看\n", filename);
}
