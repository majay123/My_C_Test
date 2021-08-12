#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "common.h"

int main(int argc, char const *argv[])
{
    char buf[] = "你好小智";
    char buf1[] = {0xe4, 0xbd, 0xa0, 0xe5, 0xa5, 0xbd, 0xe5, 0xb0, 0x8f, 0xe6, 0x99, 0xba, 0x00};
    char temp_buf[24] = {0};
    for(int i = 0; i < sizeof(buf); i++)
        printf(" %02x", buf[i]);
    printf("\n");
    snprintf(temp_buf, sizeof(buf), "%s", buf);
    print_mcd("%d, %s, %s, %s", sizeof(buf), buf, buf1, temp_buf);

    FILE *file;
    FILE *file1;

    file = fopen("test.txt", "w+");
    if(file)
    {
        fwrite(temp_buf, sizeof(char), sizeof(temp_buf), file);
        rewind(file);
        // sleep(1);
        char buf2[24] = {0};
        size_t len = 0;
        fclose(file);
        len = fread(buf2, sizeof(char), sizeof(buf2), file);
        print_mcd("%d", len);
        fclose(file);
        print_mcd("%s", buf2);
    }
    else 
        print_mcd("error");
    
    return 0;
}
