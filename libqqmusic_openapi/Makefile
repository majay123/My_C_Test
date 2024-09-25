# 定义编译器
CC = gcc

# 定义编译标志
CFLAGS = -fPIC -Wall

# 定义链接标志
LDFLAGS = -lm -lcurl -lssl -lcrypto -lrt

# 定义目标共享库
TARGET = libexample.so

# 定义源文件
SRCS = qq_music_openapi.c cJSON.c

# 定义对象文件
OBJS = $(SRCS:.c=.o)

# 目标规则
all: $(TARGET)

# 生成共享库
$(TARGET): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

# 生成对象文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)
