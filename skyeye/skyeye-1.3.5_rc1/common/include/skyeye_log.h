#ifndef __SKYEYE_LOG_H__
#define __SKYEYE_LOG_H__
#include "skyeye_types.h"
#include "skyeye_options.h"
#include <stdio.h>
#include <assert.h>
#ifdef __cplusplus
 extern "C" {
#endif

#ifdef DEBUG
#define DBG(fmt, ...) do { fprintf(stderr, fmt, ## __VA_ARGS__); } while (0)
#else
#define DBG(fmt, ...) do { } while (0)
#endif
typedef enum{
	Quiet_log = 0, /* Only output the necessary information for the user */
	Info_log , /* Some addtional information to see how about the system */
	Debug_log, /* Debug information for programmer */
	Warning_log, /* Something warning information, maybe something wrong */
	Error_log, /* Something really error, but maybe still work or can be recover */
	Critical_log, /* Can not recover and the system should be terminated immediately */
	MAX_LOG_LEVEL
}log_level_t;

typedef enum {
	RED,
	LIGHT_RED,
	BLACK,
	DARK_GRAY,
	BLUE,
	LIGHT_BLUE,
	GREEN,
	LIGHT_GREEN,
	CYAN,
	LIGHT_CYAN,
	PURPLE,
	LIGHT_PURPLE,
	BROWN,
	YELLOW,
	LIGHT_GRAY,
	WHITE
} COLOR_TYPE;

#define _debug_in_red(fmt,...)		printf("\033[0;31m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_red(fmt,...)	printf("\033[1;31m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_black(fmt,...)	printf("\033[0;30m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_dark_gray(fmt,...)	printf("\033[1;30m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_blue(fmt,...)		printf("\033[0;34m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_blue(fmt,...)	printf("\033[1;34m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_green(fmt,...)	printf("\033[0;32m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_green(fmt,...)	printf("\033[1;32m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_cyan(fmt,...)		printf("\033[0;36m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_cyan(fmt,...)	printf("\033[1;36m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_purple(fmt,...)	printf("\033[0;35m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_purple(fmt,...)	printf("\033[1;35m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_brown(fmt,...)	printf("\033[0;33m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_yellow(fmt,...)	printf("\033[1;33m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_light_gray(fmt,...)	printf("\033[0;37m"fmt"\033[0m", ## __VA_ARGS__)
#define _debug_in_white(fmt,...)	printf("\033[1;37m"fmt"\033[0m", ## __VA_ARGS__)

void skyeye_printf_in_color(COLOR_TYPE type, char *format, ...);
void skyeye_log_in_color(log_level_t log_level,const char* func_name, char* format, ...);
void skyeye_log(log_level_t log_level, const char* func_name, char* format,...);
char* get_front_message(void);

char* get_exp_str(exception_t exp);
void skyeye_info(char* format, ...);
void skyeye_debug(char* format, ...);
void skyeye_error(char* format, ...);

void skyeye_backtrace();

int
do_log_option (skyeye_option_t * this_option, int num_params,
	       const char *params[]);

#ifdef __cplusplus
}
#endif

#endif
