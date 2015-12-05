#include <stdio.h>
#include <stdlib.h>
#include "hpother.h"
#include "util.h"

int cpuInfo()
{
char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
	char szTime[32] = {0};
char *pTmp = NULL;
#ifdef _HPUX_
		strcpy(caPSCmd,"/usr/contrib/bin/machinfo -v");
//		printf("----------------hpUX mem------------------\n");    
#else
    printf("not phux\n");
	exit(0);
#endif
	currentTime(szTime);
    do
    {
signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd, "r");
        if (NULL == fp)
        {
            printf( "machinfo -v error :call popen is failed!!!! \n" );
            fclose(fp);
            return -1;
        }
        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
if(strstr(caStdOutLine,"OS info:") != NULL)
{
#ifdef __PIPE_DEBUG__
	printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

	if(NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
	{
	 	printf("error parse machinfo result %s\n", caStdOutLine);
		fclose(fp);
		return -1;
	}
	if(NULL == (pTmp=strtok(caStdOutLine, " ")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
	insertKpidatachar(111, 1251, pTmp, szTime, "system");

	if(NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
	{
	 	printf("error parse machinfo result %s\n", caStdOutLine);
		fclose(fp);
		return -1;
	}
	if(NULL == (pTmp=strtok(caStdOutLine, ":")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
if(NULL == (pTmp=strtok(NULL, ":")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
if(pTmp[strlen(pTmp)-1] == '\n')
{
pTmp[strlen(pTmp)-1] = '\0';	
}
	insertKpidatachar(111, 1252, pTmp, szTime, "system");
}
						
					if (strstr(caStdOutLine, "CPU info") != NULL)
					{
if(NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
{
 	printf("error parse swapinfo result %s\n", caStdOutLine);
	fclose(fp);
	return -1;
}
#ifdef __PIPE_DEBUG__
						printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
						//2 Intel(R) Itanium 2 9000 series processors (1.59 GHz, 18 MB)
insertKpidatachar(111, 1006, caStdOutLine, szTime, "cpu");
						
            if(NULL == (pTmp=strtok(caStdOutLine, " ")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
            insertKpidataint(111, 1003, atof(pTmp), szTime, "cpu");
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
            insertKpidatachar(111, 1001, pTmp, szTime, "cpu");
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
            insertKpidatachar(111, 1002, pTmp, szTime, "cpu");
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
if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse swapinfo result %s\n", caStdOutLine);
                fclose(fp);
                return -1;
            }
            
            if(NULL == (pTmp=strtok(pTmp, "(")))
            {
                printf("error parse swapinfo result %s\n", pTmp);
                fclose(fp);
                return -1;
            }
#ifdef __PIPE_DEBUG__
printf("%s\n", pTmp);
#endif //__PIPE_DEBUG__
            
            insertKpidataint(111, 1005, atof(pTmp), szTime, "cpu");
					}
					
					if(strstr(caStdOutLine, "logical processors") != NULL)
					{
#ifdef __PIPE_DEBUG__
							printf("%s\n", caStdOutLine);
#endif  //__PIPE_DEBUG__
						 //4 logical processors (2 per socket)
						 char *pTmp = NULL;
             if(NULL == (pTmp=strtok(caStdOutLine, " ")))
             {
                printf("error parse swapinfo result \n");
                fclose(fp);
                return -1;
             }
             insertKpidataint(111, 1004, atof(pTmp), szTime, "cpu");
					}
				}
    }while(0);
}

int hpmem()
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
	char szTime[32] = {0};
currentTime(szTime);
#ifdef _HPUX_
		strcpy(caPSCmd,"swapinfo -tam");
		//printf("----------------hpUX mem------------------\n");    
#else
    printf("not phux\n");
	exit(0);
#endif
    do
    {
signal(SIGCHLD, SIG_IGN);
        FILE* fp = popen(caPSCmd, "r" );
        if (NULL == fp)
        {
            printf( "sar error :call popen is failed!!!! \n" );
            fclose(fp);
            return -1;
        }
        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strstr(caStdOutLine, "dev") != NULL)
		{
#ifdef __PIPE_DEBUG__
			printf("%s\n", caStdOutLine);
	//hp-ux TYPE      AVAIL    USED    FREE  USED
		    printf("1:%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
		    char *pTmp = NULL;
		    if(NULL == (pTmp=strtok(caStdOutLine, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		    
		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
	
	
		   if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        sleep(10);
		        fclose(fp);
		        return -1;
		    }
		    if (pTmp[strlen(pTmp)-1] == '\n')
		    {
		        pTmp[strlen(pTmp)-1] = '\0';
		    }
		//insertKpidataint(111, 1052, atof(pTmp), szTime, "memory");


		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		if (pTmp[strlen(pTmp)-1] == '\n')
		    {
		        pTmp[strlen(pTmp)-1] = '\0';
		    }
		insertKpidataint(111, 1081, atof(pTmp), szTime, "memory");

		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		    if (pTmp[strlen(pTmp)-1] == '\n')
		    {
		        pTmp[strlen(pTmp)-1] = '\0';
		    }
		    insertKpidataint(111, 1082, atof(pTmp), szTime, "memory");
		}
		
		if(strstr(caStdOutLine, "Memory") != NULL || strstr(caStdOutLine, "memory") != NULL)
		{
//TYPE      AVAIL    USED    FREE  USED   LIMIT RESERVE  PRI  NAME
#ifdef __PIPE_DEBUG__
		     printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
		    char *pTmp = NULL;
		    if(NULL == (pTmp=strtok(caStdOutLine, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
	            if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		   insertKpidataint(111, 1052, atof(pTmp), szTime, "memory");
		   if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		    insertKpidataint(111, 1051, atof(pTmp), szTime, "memory");
if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse swapinfo result \n");
		        fclose(fp);
		        return -1;
		    }
		    insertKpidataint(111, 1055, atof(pTmp), szTime, "memory");
		}
	}
        pclose(fp);
    }while (0);
    return 0;
}

int hpother()
{
	hpmem();
return 0;
}
