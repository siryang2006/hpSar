#include "util.h"
#define INI_FILE_REFRESH "/etc/myprobe/iostatconfig.txt"

int iostat(int nTimer)
{
	char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
sprintf(caPSCmd, "sar -d %d 1", nTimer);
		//strcpy(caPSCmd,"sar -d 2 1");
//		printf("----------------hpUX lanscan ------------------\n");    
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
            printf( "lanscan  error :call popen is failed!!!! \n" );
            fclose(fp);
            return -1;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        char szTime[32] = {0};
char szName[32] = {0};
char *pTmp = NULL;

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strstr(caStdOutLine, "%busy") != NULL)
		{
			while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
			{
				if(strcmp(caStdOutLine, "\n") == 0)
				{
					continue;
				}
				if(NULL == (pTmp=strtok(caStdOutLine, " ")))
			    {
				printf("error parse swapinfo result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
				strcpy(szTime, pTmp);

			   if(NULL == (pTmp=strtok(NULL, " ")))
			    {
				printf("error parse swapinfo result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
				strcpy(szName, pTmp);
if(strcmp(pTmp, "") == 0)
{
	continue;
}
				insertKpidatachar(111, 1101, szName, szTime, szName);

if(NULL == (pTmp=strtok(NULL, " ")))
			    {
				printf("error parse swapinfo result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
				if(strcmp(pTmp, "") == 0)
				{
					continue;
				}
				insertKpidataint(111, 1102, atof(pTmp), szTime, szName);
 if(NULL == (pTmp=strtok(NULL, " ")))
			    {
				printf("error parse swapinfo result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
 if(NULL == (pTmp=strtok(NULL, " ")))
			    {
				printf("error parse swapinfo result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
insertKpidataint(111, 1103, atof(pTmp), szTime, szName);
			
			}
		}
	}
   }while(0);
}

int main(int argc, char **argv)
{
	int interval = GetIniKeyInt("section", "frequencySecond", INI_FILE_REFRESH, 5);
	//printf("time=%d\n", interval);
    if (0 != init())
    {
        printf("probe init faild!!!\n");
        return -1;
    }

while(1)
{
	iostat(interval);
}
    
	disconnect_db();
	return 0;	
}
