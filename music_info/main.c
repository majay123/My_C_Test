/*
 * 
 * 　　┏┓　　　┏┓+ +
 * 　┏┛┻━━━┛┻┓ + +
 * 　┃　　　　　　　┃ 　
 * 　┃　　　━　　　┃ ++ + + +
 *  ████━████ ┃+
 * 　┃　　　　　　　┃ +
 * 　┃　　　┻　　　┃
 * 　┃　　　　　　　┃ + +
 * 　┗━┓　　　┏━┛
 * 　　　┃　　　┃　　　　　　　　　　　
 * 　　　┃　　　┃ + + + +
 * 　　　┃　　　┃
 * 　　　┃　　　┃ +  神兽保佑
 * 　　　┃　　　┃    代码无bug　　
 * 　　　┃　　　┃　　+　　　　　　　　　
 * 　　　┃　 　　┗━━━┓ + +
 * 　　　┃ 　　　　　　　┣┓
 * 　　　┃ 　　　　　　　┏┛
 * 　　　┗┓┓┏━┳┓┏┛ + + + +
 * 　　　　┃┫┫　┃┫┫
 * 　　　　┗┻┛　┗┻┛+ + + +
 * 
 * 
 * 
 * ************Copyright 2023 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2023-02-18 09:59:05
 * @LastEditors  : MCD
 * @LastEditTime : 2023-03-28 13:36:30
 * @FilePath     : /My_C_Test/music_info/main.c
 * @Description  : 
 * 
 * ******************************************
 */
#include <stdio.h>
#include <string.h>
#include <id3v2lib-2.0/id3v2lib.h>
#include "mp3_id3_tags.h"
#include "test.h"
#include "stdint.h"

// #define MP3_ID3_TAGS_USE_GENRES
// #define MP3_ID3_TAGS_IMPLEMENTATION


typedef struct {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[30];
    unsigned char genre;
} ID3v1Tag;

int get_mp3_id3(const char* file) {
    FILE* fp;
    ID3v1Tag tag;
    char filename[100];
    int size;

    printf("请输入MP3文件名: ");
    scanf("%s", filename);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("无法打开文件\n");
        return 1;
    }

    fseek(fp, -128, SEEK_END);
    fread(&tag, sizeof(ID3v1Tag), 1, fp);

    if (strncmp(tag.tag, "TAG", 3) == 0) {
        printf("歌曲名: %s\n", tag.title);
        printf("歌手: %s\n", tag.artist);
        printf("专辑: %s\n", tag.album);
        printf("年份: %s\n", tag.year);
        printf("注释: %s\n", tag.comment);
        printf("流派: %d\n", tag.genre);
    } else {
        printf("文件不含ID3v1标签\n");
    }

    size = ftell(fp);
    printf("文件大小: %d 字节\n", size);

    fclose(fp);
    return 0;
}

int get_music_info_ID3v2(char *filename)
{
    ID3v2_Tag *tag = ID3v2_read_tag(filename);
    if(tag == NULL)
        return -1;
    
    ID3v2_TextFrame *artist_frame = ID3v2_Tag_get_artist_frame(tag);
    printf("encoding: %d, artist: %s\n",artist_frame->data->encoding, artist_frame->data->text);

    return 0;
}

int main(int argc, char const *argv[])
{
    #if 0
    if (argc == 1) {
        printf("usage: %s filename\n", argv[0]);
    } else if (argc == 2) {
        FILE *f = fopen(argv[1], "rb");

        if (f) {
            mp3_id3_tags tags;
            if (mp3_id3_file_read_tags(f, &tags)) {
                printf("MP3: %s\n\n", argv[1]);

                printf("Title: %s\n", tags.title);
                printf("Artist: %s\n", tags.artist);
                printf("Album: %s\n", tags.album);
                printf("Year: %s\n", tags.year);
                printf("Comment: %s\n", tags.comment);
                // printf("Genre: %s\n", tags.genre);
            } else {
                fprintf(stderr, "error: %s\n", mp3_id3_failure_reason());
            }

            fclose(f);
        } else {
            printf("failed to open/read '%s'\n", argv[1]);
        }
    }
    #else
    // get_mp3_id3("Bone.mp3");
    // get_music_info_ID3v2("Bone.mp3");
    #endif
    // char *p = malloc(sizeof(char) * 10);
    // free(p);
    // free(p);

    // printf("get num = %d\n", get_test());
    // printf("size is = %d\n", sizeof(ACM86xxVolTable) /);

    return EXIT_SUCCESS;
}
