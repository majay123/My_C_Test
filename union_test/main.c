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
 * ************Copyright 2022 MCD************
 * 
 * @version      : 
 * @Company      : HOPE
 * @Author       : MCD
 * @Date         : 2022-09-07 10:17:41
 * @LastEditors  : MCD
 * @LastEditTime : 2022-09-09 08:46:57
 * @FilePath     : /My_C_Test/union_test/main.c
 * @Description  : 联合体和字段位
 * 
 * ******************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>


//GPIO OUT
#define PED_GPIO_STBY_0		13
#define PED_GPIO_STBY_1		12
#define PED_GPIO_AUDIO_SW	34

#define	BOARD_PEDESTAL_GPIO_OUT	\
	{PED_GPIO_STBY_0, 0},		\
	{PED_GPIO_STBY_1, 1},		\
	{PED_GPIO_AUDIO_SW, 1},

struct gpio_config_t {
	const uint32_t pin;
	uint32_t value;
};

static struct gpio_config_t pedestal_gpio_out_config[] = {
	BOARD_PEDESTAL_GPIO_OUT
};


struct pedestal_gpio_int_t {
	uint16_t source1_status:1;
	uint16_t source2_status:1;
};

struct pedestal_gpio_out_t {
	uint16_t pa_0:4;			/* 0---pa en, 1---pa disen */
	uint16_t pa_1:1;			/* 0---pa en, 1---pa disen  */
	uint16_t audio_sw:1;		/* 0---extern audio switch, 1---local audio switch */
};

struct pedestal_gpio_control_t {

	// uint32_t prev_in_daa;
	
	/* system output control */
	union {
		uint32_t out_data;
		struct pedestal_gpio_out_t gpio_out;
	};
};


struct flag_t {
	uint8_t stop : 1;
	uint8_t : 1;
	uint8_t sta : 1;
	uint8_t : 5;
	uint8_t dir : 3;
	uint8_t wauto : 1;
	uint8_t ven : 3;
};

struct flag1_t {
	uint8_t :8;
	uint8_t sen : 1;
	uint8_t :6;
	uint8_t rev:1;
};
struct {
	union {
		uint16_t data;
		struct flag_t flag;
	};
}flags[10];

struct {
	union {
		uint16_t data;
		struct flag1_t flag;
	};
}flags1[10];


int main(int argc, char const *argv[])
{
    
#if 0
	struct pedestal_gpio_control_t test;
    printf("sizeof = %lu\n", sizeof(flags));
    test.gpio_out.audio_sw = 1;
    test.gpio_out.pa_0 = 12;
    printf("out data = %d\n", test.out_data);
    test.gpio_out.audio_sw = 0;
    printf("out data = %d\n", test.out_data);
    test.out_data = 44;
    printf("pa_0 = %d, pa_1 = %d, audio_sw = %d\n",test.gpio_out.pa_0, test.gpio_out.pa_1, test.gpio_out.audio_sw);
#endif
	memset(&flags, 0, sizeof(flags));
	memset(&flags1, 0, sizeof(flags1));

    flags[0].data = 0x5f05;
	printf("stop = %d, sta = %d, dir = %d, wauto = %d, ven = %d\n", 
		flags[0].flag.stop,
		flags[0].flag.sta,
		flags[0].flag.dir,
		flags[0].flag.wauto,
		flags[0].flag.ven);
	flags[0].flag.stop = 0;
	flags[0].flag.dir = 3;
	printf("data = %04x\n", flags[0].data);
	printf("stop = %d, sta = %d, dir = %d, wauto = %d, ven = %d\n", 
		flags[0].flag.stop,
		flags[0].flag.sta,
		flags[0].flag.dir,
		flags[0].flag.wauto,
		flags[0].flag.ven);


	// flags1.data = 0x8100;
	flags1[0].flag.sen = 0; flags1[0].flag.rev = 1;
	printf("data = %04x\n", flags1[0].data);
	printf("sen = %d, rev = %d\n", flags1[0].flag.sen, flags1[1].flag.rev);
    return 0;
}
