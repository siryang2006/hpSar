#include <stdio.h>
#include <stdlib.h>
#include "hpfilesystem.h"
#include "util.h"
int filesystem()
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
		strcpy(caPSCmd,"bdf");
//		printf("----------------hpUX process------------------\n");    
#else
    printf("system error ,hpux!!!!\n");
	exit(-1);
#endif
    do
    {
	signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd,  "r" );

        if (NULL == fp)
        {
            printf( "bdf error :call popen is failed!!!! \n" );
            fclose(fp);
            sleep(10);
            continue;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        if (NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            printf("error: read bdf command head!!!! \n");
            sleep(10);
            fclose(fp);
            continue;
        }

        int nProcessNumber = 0;
        char szTime[32] = {0};
        currentTime(szTime);

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            //hp-ux Filesystem          kbytes    used   avail %used Mounted on
            
            char szName[128] = {0};
#ifdef __PIPE_DEBUG__
            printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
            char *pTmp = NULL;
            if(NULL == (pTmp=strtok(caStdOutLine, " ")))
            {
                printf("error parse bdf aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
strcpy(szName, pTmp);
	insertKpidatachar(111, 1201, pTmp, szTime, szName);


            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse bdf result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
insertKpidataint(111, 1203, atof(pTmp), szTime, szName);

            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            insertKpidataint(111, 1204, atof(pTmp), szTime, szName);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            insertKpidataint(111, 1205, atof(pTmp), szTime, szName);
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                continue;
            }
            insertKpidataint(111, 1206, atof(pTmp), szTime, szName);
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
	 insertKpidatachar(111, 1202, pTmp, szTime, szName);
        }
        pclose(fp);
    }while (0);
}
