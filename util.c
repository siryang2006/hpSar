#include "util.h"

MYSQL *mysql = 0;
BOOL g_isCollect[1024];
double g_collectWarningMin[1024];

int init()
{
    if (0 != initIsCollectTable())
    {
        return -1;
    }

    char szServer[64]={0};
    strcpy(szServer, GetIniKeyString("mysql", "server", INI_FILE));
    
    int nPort = GetIniKeyInt("mysql", "port", INI_FILE, 3306);
    char szUserName[256]={0};
    strcpy(szUserName, GetIniKeyString("mysql", "username", INI_FILE));
    char szPassWord[256]={0};
    strcpy(szPassWord, GetIniKeyString("mysql", "password", INI_FILE));
    char szDatabase[512]={0};
    strcpy(szDatabase, GetIniKeyString("mysql", "database", INI_FILE));

    if(0!=connect_db(szServer, nPort, szDatabase, szUserName, szPassWord))
    {
        printf("connect mysql faild: server=%s,port=%d,username=%s,database=%s\n",
            szServer, nPort, szUserName, szDatabase);
        return -1;
    }
    return 0;
}

int initIsCollectTable()
{
    int i = 0;
    for(i=0; i<MAXK_PIID_NUMBER;i++)
    {
        g_isCollect[i] = FALSE;
        g_collectWarningMin[i] = -1;
    }

    char szFilePath[128]={0};
    strcpy(szFilePath, GetIniKeyString("warningconfig", "file", INI_FILE));

    FILE *file = NULL;
    file = fopen(szFilePath, "r");
    if (file == NULL)
    {
        printf("read file %s faild!!!!\n", szFilePath);
        return -1;
    }

    char szLine[512] = {0};
    while(fgets(szLine, sizeof(szLine), file) != NULL)
    {
        if(strlen(szLine)>0)
        {
            if(szLine[strlen(szLine)-1] == '\n')
            {
                szLine[strlen(szLine)-1] = '\0';
            }
        }

        if (*szLine == '#' || *szLine == '\n' || *szLine == '\0')
        {
            continue;
        }
        char *split = strrchr(szLine, ':');

        char szKpiid[20] = {0};
        char szWarningMin[20] = {0};

        if(split != NULL)
        {
            strcpy(szWarningMin, split+1);
            memcpy(szKpiid, szLine, strlen(szLine)-strlen(szWarningMin)-1);
        }
        else
        {
            strcpy(szKpiid, szLine);
        }

        int nKpiidIndex = atoi(szKpiid)-KPIID_MIN;
        if (nKpiidIndex < 0 || nKpiidIndex>=MAXK_PIID_NUMBER )
        {
            continue;
        }

        g_isCollect[nKpiidIndex] = TRUE;
        double dWarningMin = atof(szWarningMin);
        if (dWarningMin != 0)
        {
            g_collectWarningMin[nKpiidIndex] = dWarningMin;
        }
    }
    return 0;
}

int connect_db(const char* host, int nPort, const char* szDatabase, const char* user, const char* passwd)
{
    printf("connect_db ..... \n");

    mysql = mysql_init(NULL);

    if (mysql == 0)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        //exit(2000);
        return -1;
    }

    if (mysql_real_connect(mysql, host, user, passwd, szDatabase, nPort, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(mysql));
        mysql_close(mysql);
        //exit(2001);
        return -1;
    }

    printf("OK!\n");

    return (0);
}

void disconnect_db(void)
{
    if (mysql)
        mysql_close(mysql);

    printf("disconnected_db\n");
}

char *getCurrentDate(char *szDate)
{
    time_t timep;
    struct tm *p;

    time(&timep); /*获得time_t结构的时间，UTC时间*/
    p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
    sprintf(szDate, "%d-%02d-%02d ", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
    //    sprintf(szDate, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
    //               p->tm_hour, p->tm_min, p->tm_sec);

    return szDate;
}

//const char *message, icount int, deviceid int, firstoccur varchar(100),
//kpiid varchar(100), isclose tinyint, isopen tinyint, alarmLevelId int);
int insertPolicyalarm(const char *message,
                      int deviceid,
                      const char *firstoccur,
                      const char *kpiid,
                      int alarmLevelId,
                      int icount,
                      int isclose,
                      int isopen)
{
    char szSql[512] = {0};
    int nDeviceID = getDeviceID();
    sprintf(szSql, "insert into policyalarm(message, icount,\
            deviceid, firstoccur,\
            kpiid, isclose, isopen, alarmLevelId) \
            values('%s', %d, %d, str_to_date('%s', '%%Y-%%m-%%d %%H:%%i:%%s'), %d, %d,%d,%d)",
            message, icount, nDeviceID, firstoccur, kpiid, isclose, isopen, alarmLevelId);

    printf("[policyalarm]:%s\n", szSql);
    if(0 != mysql_query(mysql, szSql))
    {
        const char *szError = mysql_error(mysql);
        printf("[error]  %s\n", szError);
        return -1;
    }
    return 0;
}

/*
kpidataint
(性能表 数值)	deviceid	int	11	设备id	默认5为10xxx，xxx：IP地址尾数不足3为以0补齐，0：用以区分不通网段的相同IP地址尾数，位数相同时此位+1。
如：10.10.1.15 ，id为10015；网络中有多个尾数15的IP第二位加一，如10015、11015、12015
kpiid	int	11	指标id	详见‘监控指标’页表格中KPIID
value	double	　	采集值	采集值，如：cpu利用率40%，写入 40
ydate	datetime	　	采集时间	YYYY-MM-DD hh：mm：ss
instance	varchar	40	采集对象	采集对象名称，如：cpu/核心名称、eth0、/dev
*/
int insertKpidataint(int deviceid, int kpiid, double value, const char *szTime, const char *szInstance)
{
    int index = kpiid - KPIID_MIN;
    if(index < 0 || index>=MAXK_PIID_NUMBER
            || g_isCollect[index] == FALSE
            || strcmp(szTime, "Average:") == 0
            || strcmp(szTime, "Summary:") == 0
            || strstr(szTime, ":") == NULL)
    {
        return 0;
    }

    char szSql[512] = {0};
    char szCurrentTime[64] = {0};
    getCurrentDate(szCurrentTime);
    strcat(szCurrentTime, szTime);

    int nDeviceID = getDeviceID();
    sprintf(szSql, "insert into kpidataint(deviceid, kpiid, value, \
            ydate, instance) values(%d, %d, %.8lf, str_to_date('%s', '%%Y-%%m-%%d %%H:%%i:%%s'), '%s')", nDeviceID, kpiid,
            value, szCurrentTime, szInstance);
    printf("%s\n", szSql);

    if(0 != mysql_query(mysql, szSql))
    {
        const char *szError = mysql_error(mysql);
        printf("[error]  %s\n", szError);
        return -1;
    }

    if (g_collectWarningMin[index] > 0 &&
            g_collectWarningMin[index] <= value)
    {
        char szKpiid[20] = {0};
        sprintf(szKpiid, "%d", kpiid);
        insertPolicyalarm(szInstance, deviceid, szCurrentTime, kpiid, 4, 1, 0, 0);
    }

    return 0;
}

/*
kpidatachar
(性能表 字符)	deviceid	int	11	设备id	默认5为10xxx，xxx：IP地址尾数不足3为以0补齐，0：用以区分不通网段的相同IP地址尾数，位数相同时此位+1。
如：10.10.1.15 ，id为10015；网络中有多个尾数15的IP第二位加一，如10015、11015、12015
kpiid	int	11	指标id	详见‘监控指标’页表格中KPIID
value	varchar	100	采集值	字符类型采集值，如：CPU厂商、型号，系统描述、文件修改时间等
ydate	datetime	　	采集时间	YYYY-MM-DD hh：mm：ss
instance	varchar	100	采集对象	采集对象名称，如：cpu/核心名称、eth0、/dev
*/
int insertKpidatachar(int deviceid, int kpiid, const char *value, const char *szTime, const char *instance)
{
    int index = kpiid - KPIID_MIN;
    if(index < 0 || g_isCollect[index] == FALSE
            || strcmp(szTime, "Average:") == 0
            || strcmp(szTime, "Summary:") == 0
            || strstr(szTime, ":") == NULL)
    {
        return 0;
    }

    char szSql[512] = {0};
    char szCurrentTime[64] = {0};
    getCurrentDate(szCurrentTime);
    strcat(szCurrentTime, szTime);
    int nDeviceID = getDeviceID();
    sprintf(szSql, "insert into kpidatachar(deviceid, kpiid, value, ydate, instance) \
            values(%d, %d, '%s', str_to_date('%s','%%Y-%%m-%%d %%H:%%i:%%s'), '%s')", nDeviceID, kpiid, value, szCurrentTime, instance);

            printf("%s\n", szSql);
            if(0 != mysql_query(mysql, szSql))
    {
        const char *szError = mysql_error(mysql);
        printf("[error]  %s\n", szError);
        return -1;
    }
    return 0;
}

char* currentTime(char *szTime)
{
    time_t timep;
    struct tm *p;

    time(&timep); /*获得time_t结构的时间，UTC时间*/
    p = gmtime(&timep); /*转换为struct tm结构的UTC时间*/
    //    sprintf(szTime, "%d-%02d-%02d %d:%d:%d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
    //            p->tm_hour, p->tm_min, p->tm_sec);
    sprintf(szTime, "%02d:%02d:%02d", p->tm_hour, p->tm_min, p->tm_sec);

    return szTime;
}

int getDeviceID()
{
    static int nID = -1;
    if (nID == -1)
    {
        nID = GetIniKeyInt("section", "deviceID", INI_FILE, 10111);
    }
    return nID;
}

int getLocalIP()
{
    char ETH_NAME[10] = {"eth0"};
    int sock;
    struct sockaddr_in sin;
    struct ifreq ifr;

    static int nRet = -1;
    if(nRet != -1)
    {
        return nRet;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return -1;
    }

    strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(sock, SIOCGIFADDR, &ifr) < 0)
    {
        perror("ioctl");
        return -1;
    }

    memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
    char *szIP = inet_ntoa(sin.sin_addr);
    fprintf(stdout, "eth0: %s\n", szIP);

    char *last = strrchr(szIP, '.');
    if (last+1)
    {
        int nLast = atoi(last+1);
        nRet = 10000+nLast;
    }

    return nRet;
}

int getFrequencySecond()
{
    return GetIniKeyInt("section", "frequencySecond", INI_FILE, 1);
}

//从INI文件读取整类型数据
int GetIniKeyInt(char *title,char *key, char *filename, int nDefault)
{
    char *sz = GetIniKeyString(title,key,filename);
    if (strcmp(sz, "") == 0)
    {
        return nDefault;
    }
    return atoi(sz);
}

char* GetIniKeyString(char* Section,char* Item, char* FileName)
{
    char *value,Sect[30],c;
    char linebuf[1024*2]={};
    FILE *inifp;
    strcpy(Sect,"[");
    strcat(Sect,Section);
    strcat(Sect,"]");
    /*strcpy(Ite,"[");strcat(Ite,Item);strcat(Ite,"]");*/
    if((inifp=fopen(FileName,"rb"))==NULL)
    {
	printf("read file %s error \n", FileName);
        return "";
    }
    /*printf("Sect = %s,Item = %s /n",Sect,Item);*/
    while((c=fgetc(inifp))!=EOF)
    {
        if(c=='[')
        {
            ungetc(c,inifp);
            fgets(linebuf,sizeof(linebuf),inifp);
            if(strstr(linebuf,Sect))
            {
                while((c=fgetc(inifp))!='[' && c!=EOF)
                {
                    ungetc(c,inifp);
                    fgets(linebuf,sizeof(linebuf),inifp);
                    if(strstr(linebuf,Item))
                    {
                        if(value=strchr(linebuf,'='))
                        {
                            value++;
                            /*printf("value = %s /n",value);*/
                            fclose(inifp);
                            if(*value=='/n')
                                return "";

                            char *p = strstr(value, "\n");
                            if (p != NULL)
                            {
                                *p = '\0';
                            }
                            return value;
                        }
                    }
                }
                if(c==EOF)
                {
                    break;
                }
                ungetc(c,inifp);
            }
        }
        else
        {
            ungetc(c,inifp);
            fgets(linebuf,sizeof(linebuf),inifp);
        }
    }
    fclose(inifp);
    return "";
}

int sig_exit(int sig_no)
{return 0;

}
