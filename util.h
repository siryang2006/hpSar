#ifndef UTIL_H
#define UTIL_H
#include <mysql/mysql.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <net/if.h>

#include <arpa/inet.h>
#include <signal.h>

#ifdef __linux__
#include <unistd.h>
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#define TRUE 1
#define FALSE 0
typedef char BOOL;

#define KPIID_MIN 1000
#define MAXK_PIID_NUMBER 1024
#define INI_FILE "/etc/myprobe/config.txt"

extern MYSQL *mysql;
extern BOOL g_isCollect[1024];
extern double g_collectWarningMin[1024];

char* currentTime(char *szTime);
int init();
//int initIsCollectTable();
//从INI文件读取字符串类型数据
char *GetIniKeyString(char *title,char *key,char *filename);
//从INI文件读取整类型数据
int GetIniKeyInt(char *title, char *key, char *filename, int nDefault);
int getLocalIP();

int getDeviceID();

int insertKpidatachar(int deviceid, int kpiid, const char *value, const char *szTime, const char *instance);
int insertKpidataint(int deviceid, int kpiid, double value, const char *szTime, const char *szInstance);
int insertPolicyalarm(const char *message,
                      int deviceid,
                      const char *firstoccur,
                      const char *kpiid,
                      int alarmLevelId,
                      int icount,
                      int isclose,
                      int isopen);

//int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd);
void disconnect_db(void);
int getFrequencySecond();
int pclose(FILE *stream);

#endif // UTIL_H
