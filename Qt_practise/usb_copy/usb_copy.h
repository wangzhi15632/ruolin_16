#ifndef USB_COPY_H
#define USB_COPY_H

#define BOOL int  
#define TRUE 1  
#define FALSE 0  
#define MAX_FMTSTR_LENGTH 2048  /*传递给print_message函数的格式字符串最大长度*/  
#define COPY_BUF_SIZE 4096 /*复制文件时每次读取的长度*/  
#define MAX_PATH_LENGTH (PATH_MAX + 1)  /*路径的最大长度*/  
#define GBYTES (1024 * 1024 * 1024)  
#define MBYTES (1024 * 1024)  
#define KBYTES 1024  
#define HOUR (60 * 60)  
#define MINUTE 60  
#define OPP_CONTINUE 0  
#define OPP_SKIP 1  
#define OPP_CANCEL 2 /*walk 函数终止遍历退出*/  
  
#define MSGT_PROMPT 0  
#define MSGT_WARNING 1  
#define MSGT_ERROR 2  
#define MSGT_VERBOSE 3  
#define MSGT_DEMO 4  

class CopyThread;
typedef struct sum
{
	int file;  
	int dir;  
	long long size;  
}sum_t;

typedef struct copied
{
	int file;  
	int dir;  
	long long size;  
}copied_t;

typedef int (*each_opp_t)(const char*, const char*, const char*, const struct stat* st, CopyThread* copy);
typedef void (*sig_handler_t)(int);

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif


#endif

