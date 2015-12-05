#include <stdio.h>
#include <stdlib.h>
#include "hpprocess.h"
#include "util.h"

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

int process()
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
