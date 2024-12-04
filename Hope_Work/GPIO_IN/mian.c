#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define GPIO_COUNT 1
#define DEBOUNCE_TIME_MS 50
#define GPIO_BASE_PATH "/sys/class/gpio"

// GPIO 引脚配置
// int gpio_pins[GPIO_COUNT] = {20, 21, 22, 23, 24, 25, 26, 27}; // 假设使用 GPIO 20-27
int gpio_pins[GPIO_COUNT] = {110}; // 假设使用 GPIO 20-27

// 存储上次触发时间
struct timespec last_trigger[GPIO_COUNT];

// 防抖检查
int debounce_check(int gpio_index) {
    struct timespec now;
    long elapsed_time_ms;

    clock_gettime(CLOCK_MONOTONIC, &now);
    elapsed_time_ms = (now.tv_sec - last_trigger[gpio_index].tv_sec) * 1000 +
                      (now.tv_nsec - last_trigger[gpio_index].tv_nsec) / 1000000;

    if (elapsed_time_ms < DEBOUNCE_TIME_MS) {
        return 0;  // 如果时间间隔小于防抖时间，返回0表示不处理
    }

    last_trigger[gpio_index] = now;  // 更新触发时间
    return 1;  // 返回1表示可以处理该事件
}

// 读取 GPIO 电平状态
int read_gpio_value(int gpio_pin) {
    char path[64];
    char value_str[3];
    int value;
    FILE *fd;

    snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/value", gpio_pin);
    fd = fopen(path, "r");
    if (fd == NULL) {
        perror("Failed to open GPIO value");
        return -1;
    }

    fgets(value_str, sizeof(value_str), fd);
    fclose(fd);

    value = atoi(value_str);  // 将字符串转换为整型
    return value;
}

// 导出 GPIO
int export_gpio(int gpio_pin) {
    char path[64];
    FILE *fd;

    snprintf(path, sizeof(path), GPIO_BASE_PATH "/export");
    fd = fopen(path, "w");
    if (fd == NULL) {
        perror("Failed to export GPIO");
        return -1;
    }

    fprintf(fd, "%d", gpio_pin);
    fclose(fd);
    return 0;
}

// 设置 GPIO 为输入
int set_gpio_input(int gpio_pin) {
    char path[64];
    FILE *fd;

    snprintf(path, sizeof(path), GPIO_BASE_PATH "/gpio%d/direction", gpio_pin);
    fd = fopen(path, "w");
    if (fd == NULL) {
        perror("Failed to set GPIO direction");
        return -1;
    }

    fprintf(fd, "in");
    fclose(fd);
    return 0;
}

// 主函数
int main() {
    int i;
    // int prev_values[GPIO_COUNT] = {-1, -1, -1, -1, -1, -1, -1, -1};
    // 默认高电平
    int prev_values[GPIO_COUNT] = {1};
    int current_values[GPIO_COUNT];

    // 导出并设置 GPIO 为输入
    for (i = 0; i < GPIO_COUNT; i++) {
        if (export_gpio(gpio_pins[i]) != 0) {
            return -1;
        }
        if (set_gpio_input(gpio_pins[i]) != 0) {
            return -1;
        }
    }

    printf("Monitoring GPIO signals...\n");

    while (1) {
        for (i = 0; i < GPIO_COUNT; i++) {
            // 读取 GPIO 当前的电平
            current_values[i] = read_gpio_value(gpio_pins[i]);

            if (current_values[i] == -1) {
                continue;  // 如果读取失败，跳过此 GPIO
            }

            // 检查是否有电平变化，并且通过防抖检查
            if (current_values[i] != prev_values[i] && debounce_check(i)) {
                if (current_values[i] == 1) {
                    printf("===============GPIO %d is HIGH\n", gpio_pins[i]);
                } else {
                    printf("===============GPIO %d is LOW\n", gpio_pins[i]);
                }
                prev_values[i] = current_values[i];  // 更新上次状态
            }
        }

        usleep(100000);  // 每 100 毫秒检测一次
    }

    return 0;
}
