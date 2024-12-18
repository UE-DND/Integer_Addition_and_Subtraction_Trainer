#include "HEADER.h"

int main() {
skip:
    srand(time(NULL));
    current_wrong_count = 0;  // 重置当前错题计数
    get_username();
    reset_wrong_questions();
    load_previous_wrong_questions();
    get_user_input();
    generate_random_num();

    time(&start_time);  // 记录开始时间
    for (int i = 0; i < total_question_count; i++) {
        ask_question(i);
    }
    time(&end_time);
    spend_time = difftime(end_time, start_time);  // 计算时长

    display_results();

    printf("是否导出练习报告? (y/n): ");
    char choice1;
    scanf(" %c", &choice1);
    if (choice1 == 'y' || choice1 == 'Y') {
        save_results();
    }
    save_previous_wrong_questions();

    printf("是否重新练习？(y/n): ");
    char choice2;
    scanf(" %c", &choice2);
    if (choice2 == 'y' || choice2 == 'Y') {
        goto skip;  // 继续作答
    } else {
        printf("\n程序已退出。\n");
        return 0;
    }
}