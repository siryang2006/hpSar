#include <stdio.h>
#include <stdlib.h>
#include "hpprocess.h"
#include "util.h"

int g_ptProcessCpuUsed[128] = {0};
int g_ptProcessNumber[128] = {0};
int g_ptIsProcessExists[128] = {0};

typedef struct truPS
{
    char szUSER[512];
    int PID;
    float fCPU;
    float fMEM;
    long lVSZ;
    long lRSS;
    char szTTY[32];
    char szSTAT[32];
    char szSTART[32];
    char szTIME[32];
    char szCOMMAND[128];
}truPS;

int getProcessNumber(const char *szProcessName);
int getProcessCPU(const char *szProcessName);

int initProcess()
{
    static char szProcessCpuUsed[2048] = {0};
    static char szProcessNumber[2048] = {0};
    static char szIsProcessExists[2048] = {0};
    if(0<readIniData("ProcessCpuUsed", INI_FILE_EX, szProcessCpuUsed))
    {
        char *p = strtok(szProcessCpuUsed, "\n");
        g_ptProcessCpuUsed[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptProcessCpuUsed[nIndex] = (int)p;
            nIndex++;
        }
    }

    if(0<readIniData("ProcessNumber", INI_FILE_EX, szProcessNumber))
    {
        char *p = strtok(szProcessNumber, "\n");
        g_ptProcessNumber[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptProcessNumber[nIndex] = (int)p;
            nIndex++;
        }
    }
    if(0<readIniData("IsProcessExists", INI_FILE_EX, szIsProcessExists))
    {
        char *p = strtok(szIsProcessExists, "\n");
        g_ptIsProcessExists[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptIsProcessExists[nIndex] = (int)p;
            nIndex++;
        }
    }
}

int processNumber()
{
 	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for (i=0; i<128; i++)
	{
		char *p = (char*)g_ptProcessNumber[i];
		if (p == NULL)
		{
			return 0;
		}
		int nNumber = getProcessNumber(p);
		insertKpidataint(111, 1263, nNumber, szTime, p);
	}
}

int getProcessCPUUsed()
{
	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for (i=0; i<128; i++)
	{
		char *p = (char*)g_ptIsProcessExists[i];
		if (p == NULL)
		{
			return 0;
		}
		float f = getProcessCPU(p);
		insertKpidataint(111, 1262, f, szTime, p);
	}
}

int processExists()
{
 	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for (i=0; i<128; i++)
	{
		char *p = (char*)g_ptIsProcessExists[i];
		if (p == NULL)
		{
			return 0;
		}
		int nNumber = getProcessNumber(p);
		if(nNumber <= 0)
		{
			insertKpidatachar(111, 1264, "down", szTime, p);
		}
		else
		{
			insertKpidatachar(111, 1264, "up", szTime, p);
		}
	}
}

int processInfo()
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
		strcpy(caPSCmd,"UNIX95= ps -e -o ruser,pid,pcpu,sz,vsz,sz,sid,state,stime,time,args");
		//printf("----------------hpUX process------------------\n");    
#else
    strcpy(caPSCmd,"ps -aux  --sort -pcpu");
#endif
    do
    {
signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd,  "r" );
        if (NULL == fp)
        {
            printf( "ps error :call popen is failed!!!! \n" );
            fclose(fp);
            sleep(10);
            continue;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        if (NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            printf("error: read ps command head!!!! \n");
            sleep(10);
            fclose(fp);
            continue;
        }

        int nProcessNumber = 0;
        char szTime[32] = {0};
        currentTime(szTime);

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            //      USER  PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
            //hp-ux RUSER PID %CPU  SZ     VSZ   SZ  SID       S   STIME   TIME COMMAND
            
            truPS ps;
#ifdef __PIPE_DEBUG__
            printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

            char *pTmp = NULL;
            if(NULL == (pTmp=strtok(caStdOutLine, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szUSER, pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
#ifndef _HPUX_ 
	    if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.PID = 0;//atoi(pTmp);
#else
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.PID = atoi(pTmp);
#endif
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.fCPU = atof(pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.fMEM = atof(pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.lVSZ = atol(pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            ps.lRSS = atol(pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szTTY, pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                return -1;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szSTAT, pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szSTART, pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szTIME, pTmp);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            strcpy(ps.szCOMMAND, pTmp);
            insertKpidataint(111, 1262, ps.fCPU, szTime, ps.szCOMMAND);
            nProcessNumber++;
        }
        insertKpidataint(111, 1261, nProcessNumber, szTime, "process");
        pclose(fp);
    }while (0);
}

int getProcessNumber(const char *szProcessName)
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[64] = {0};
    int nProcessNumber = 0;

    strcpy(caPSCmd,"ps -e|grep ");
    strcat(caPSCmd, szProcessName);
#ifdef __PIPE_DEBUG__
    printf("----------------hpUX process Number------------------\n");
    printf("%s\n", caPSCmd);
#endif //__PIPE_DEBUG__
    do
    {
        signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd,  "r" );
        if (NULL == fp)
        {
            printf( "ps error :call popen is failed!!!! \n" );
            return -1;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            if(strcmp(caStdOutLine, "\n") == 0)
            {
                continue;
            }
#ifdef __PIPE_DEBUG__
	    printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
            nProcessNumber++;
        }
        fclose(fp);
    }while(0);
    return nProcessNumber;
}

int getProcessCPU(const char *szProcessName)
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[64] = {0};
    float fUsed = 0;

    strcpy(caPSCmd,"ps -el|grep ");
    strcat(caPSCmd, szProcessName);
#ifdef __PIPE_DEBUG__
    printf("----------------hpUX getProcessCPU------------------\n");
    printf("%s\n", caPSCmd);
#endif //__PIPE_DEBUG__
    do
    {
        signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd,  "r" );
        if (NULL == fp)
        {
            printf( "ps error :call popen is failed!!!! \n" );
            return 0;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            if(strcmp(caStdOutLine, "\n") == 0)
            {
                continue;
            }
#ifdef __PIPE_DEBUG__
	    printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
            char *p = strtok(caStdOutLine, " ");
	    p = strtok(NULL, " ");
	    p = strtok(NULL, " ");
	    p = strtok(NULL, " ");
	    p = strtok(NULL, " ");
	    p = strtok(NULL, " ");
	    if (p!= NULL)
	    {
	        fUsed += atof(p);
	    }
        }
        fclose(fp);
    }while(0);
    return fUsed;
}

int process()
{
	processInfo();
	processNumber();
	processExists();
	getProcessCPUUsed();
}
