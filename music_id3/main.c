#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// ID3v2 标签解析的函数声明
void parse_mp3_info(const char *filepath);
// WAV 文件信息解析函数声明
void parse_wav_info(const char *filepath);
// FLAC 文件信息解析函数声明
void parse_flac_info(const char *filepath);

// 检测音频文件格式
const char *detect_audio_format(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    char header[12] = {0}; // 读取文件头的前 12 字节
    fread(header, 1, sizeof(header), file);
    fclose(file);

    // 检测 MP3 文件（ID3 标志或 MPEG 帧头）
    if (memcmp(header, "ID3", 3) == 0) {
        return "MP3";
    }

    // 检测 WAV 文件（RIFF + WAVE）
    if (memcmp(header, "RIFF", 4) == 0 && memcmp(header + 8, "WAVE", 4) == 0) {
        return "WAV";
    }

    // 检测 FLAC 文件（fLaC 标志）
    if (memcmp(header, "fLaC", 4) == 0) {
        return "FLAC";
    }

    // 未知格式
    return "UNKNOWN";
}

// 解析音频文件信息
void parse_audio_file(const char *filepath) {
    const char *format = detect_audio_format(filepath);
    if (!format) {
        printf("Error: Failed to detect audio format.\n");
        return;
    }

    printf("Detected audio format: %s\n", format);

    if (strcmp(format, "MP3") == 0) {
        parse_mp3_info(filepath);
    } else if (strcmp(format, "WAV") == 0) {
        parse_wav_info(filepath);
    } else if (strcmp(format, "FLAC") == 0) {
        parse_flac_info(filepath);
    } else {
        printf("Unsupported audio format: %s\n", format);
    }
}

// MP3 信息解析
void parse_mp3_info(const char *filepath) {
    printf("Parsing MP3 file: %s\n", filepath);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Failed to open MP3 file");
        return;
    }

    // 检查 ID3v2 标签
    char id3_header[10];
    fread(id3_header, 1, 10, file);
    if (memcmp(id3_header, "ID3", 3) == 0) {
        int tag_size = ((id3_header[6] & 0x7F) << 21) | ((id3_header[7] & 0x7F) << 14) |
                       ((id3_header[8] & 0x7F) << 7) | (id3_header[9] & 0x7F);

        char *tag_data = (char *)malloc(tag_size);
        fread(tag_data, 1, tag_size, file);

        // 解析 ID3v2 标签帧
        int pos = 0;
        while (pos < tag_size - 10) {
            char frame_id[5] = {0};
            memcpy(frame_id, tag_data + pos, 4);
            int frame_size = (tag_data[pos + 4] << 24) | (tag_data[pos + 5] << 16) |
                             (tag_data[pos + 6] << 8) | tag_data[pos + 7];
            if (frame_size <= 0 || frame_size > tag_size - pos - 10) {
                break;
            }
            if (strcmp(frame_id, "TIT2") == 0) {
                printf("Title: %.*s\n", frame_size, tag_data + pos + 10);
            } else if (strcmp(frame_id, "TPE1") == 0) {
                printf("Artist: %.*s\n", frame_size, tag_data + pos + 10);
            } else if (strcmp(frame_id, "TALB") == 0) {
                printf("Album: %.*s\n", frame_size, tag_data + pos + 10);
            }
            pos += 10 + frame_size;
        }

        free(tag_data);
    }

    fclose(file);
}

// WAV 信息解析
void parse_wav_info(const char *filepath) {
    printf("Parsing WAV file: %s\n", filepath);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Failed to open WAV file");
        return;
    }

    char header[44];
    fread(header, 1, 44, file);

    // 检查 RIFF 标志
    if (memcmp(header, "RIFF", 4) == 0 && memcmp(header + 8, "WAVE", 4) == 0) {
        printf("Valid WAV file detected.\n");

        // 解析时长
        int byte_rate = *(int *)(header + 28);
        int data_size = *(int *)(header + 40);
        int duration = data_size / byte_rate;
        printf("Duration: %d seconds\n", duration);
    } else {
        printf("Invalid WAV file.\n");
    }

    fclose(file);
}

// FLAC 信息解析
void parse_flac_info(const char *filepath) {
    printf("Parsing FLAC file: %s\n", filepath);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Failed to open FLAC file");
        return;
    }

    char header[4];
    fread(header, 1, 4, file);

    // 检查 fLaC 标志
    if (memcmp(header, "fLaC", 4) == 0) {
        printf("Valid FLAC file detected.\n");
        // 完整解析需要读取 METADATA 块 (如 STREAMINFO)
    } else {
        printf("Invalid FLAC file.\n");
    }

    fclose(file);
}

// 主函数
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <audio_file_path>\n", argv[0]);
        return 1;
    }

    const char *audio_file_path = argv[1];
    parse_audio_file(audio_file_path);

    return 0;
}
