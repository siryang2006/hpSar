#include <stdio.h>
#include <stdlib.h>
#include "hpsar.h"
#include "util.h"

int hpsarCpu()
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
	strcpy(caPSCmd,"sar -u 1 1");
	//printf("----------------hpUX sar------------------\n");    
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
        if (NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            printf("error: read ps command head!!!! \n");
            fclose(fp);
            return -1;
        }
	//printf("%s\n", caStdOutLine);

        int nProcessNumber = 0;
        char szTime[32] = {0};

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strstr(caStdOutLine, "usr") == NULL)
		{
			continue;
		}
		if(NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
		{
			printf("read sar data error\n");
			fclose(fp);
            		return -1;
		}
            //hp-ux %usr    %sys    %wio   %idle
#ifdef __PIPE_DEBUG__ 
            printf("1:%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

            char *pTmp = NULL;
            if(NULL == (pTmp=strtok(caStdOutLine, " ")))
            {
                printf("error parse sar result \n");
                fclose(fp);
                return -1;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
	    strcpy(szTime, pTmp);
            
            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse sar result \n");
                fclose(fp);
                return -1;
            }
	if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
	insertKpidataint(111, 1016, atof(pTmp), szTime, "cpu");
	
           if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                sleep(10);
                fclose(fp);
                return -1;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
	insertKpidataint(111, 1017, atof(pTmp), szTime, "cpu");


            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                fclose(fp);
                return -1;
            }

            if(NULL == (pTmp=strtok(NULL, " ")))
            {
                printf("error parse ps aux result \n");
                fclose(fp);
                return -1;
            }
            if (pTmp[strlen(pTmp)-1] == '\n')
            {
                pTmp[strlen(pTmp)-1] = '\0';
            }
            insertKpidataint(111, 1018, atof(pTmp), szTime, "cpu");
	insertKpidataint(111, 1020, 100-atof(pTmp), szTime, "cpu");
	}
        pclose(fp);
    }while (0);
    return 0;
}

int hpswapInOut()
{
    char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
	strcpy(caPSCmd,"sar -w 1 1"); 
#else
    printf("not phux\n");
    exit(0);
	return -1;
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
        if (NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            printf("error: read ps command head!!!! \n");
            fclose(fp);
            return -1;
        }
	//printf("%s\n", caStdOutLine);

        int nProcessNumber = 0;
        char szTime[32] = {0};

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strstr(caStdOutLine, "swpin/s") != NULL)
		{
			if(NULL == fgets(caStdOutLine, sizeof(caStdOutLine), fp))
			{
				printf("read sar data error\n");
				fclose(fp);
            			return -1;
			}
#ifdef __PIPE_DEBUG__ 
            printf("1:%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
		char *pTmp = NULL;
			    if(NULL == (pTmp=strtok(caStdOutLine, " ")))
			    {
				printf("error parse sar result \n");
				fclose(fp);
				return -1;
			    }
			strcpy(szTime, pTmp);

            
		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse sar result \n");
		        fclose(fp);
		        return -1;
		    }
	
			insertKpidataint(111, 1053, atof(pTmp), szTime, "memory");
	
		   	if(NULL == (pTmp=strtok(NULL, " ")))
		    	{
		        	printf("error parse ps aux result \n");
		        	sleep(10);
		        	fclose(fp);
		        	return -1;
			}
			insertKpidataint(111, 1104, atof(pTmp), szTime, "disk");


		    if(NULL == (pTmp=strtok(NULL, " ")))
		    {
		        printf("error parse ps aux result \n");
		        fclose(fp);
		        return -1;
		    }

			insertKpidataint(111, 1054, atof(pTmp), szTime, "memory");

 			if(NULL == (pTmp=strtok(NULL, " ")))
		    	{
		       	 printf("error parse ps aux result \n");
		       	 fclose(fp);
		        	return -1;
		    	}
  
            	insertKpidataint(111, 1105, atof(pTmp), szTime, "disk");
		}
      }
      pclose(fp);
    }while (0);
    return 0;
}

int multiCoreCPUUsed()
{
	char caStdOutLine[1024] = {"cpu-"};
	char szTime[32] = {0};
	currentTime(szTime);
	char *pTmp = NULL;

	signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("top -d 1", "r" );
        if (NULL == fp)
        {
            printf( "sar error :call popen is failed!!!! \n" );
            return -1;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strcmp(caStdOutLine, "\n") ==0)
		{
			continue;
		}
		if(strcmp(caStdOutLine, "\r\n") ==0)
		{
			continue;
		}
		char *pStart = NULL;
		char *pEnd = NULL;

		char szValue1[20] = {0};
		char szValue2[20] = {0};
		char szValue3[20] = {0};
		char szValue4[20] = {0};
		char szValue5[20] = {0};
		char szValue6[20] = {0};
		if((pStart = strstr(caStdOutLine, "SSYS")) != NULL && (pEnd=strstr(caStdOutLine, "---")) != NULL)
		{
			pStart = pStart+4;
			char szData[1024] = {0};
			memcpy(szData, pStart, (size_t)(pEnd-pStart));

			pTmp=strtok(szData, "\r\n");
			while(NULL!=(pTmp=strtok(NULL, "\r\n")))
			{
				char szName[128]={"cpu-"};
				char sz[128] = {0};
				strcpy(sz, pTmp);
				trim(sz);
				if(strlen(sz)<=5)
				{
					continue;
				}

				sscanf(sz,"%s    %s    %s    %s    %s    %s", szValue1, szValue2, szValue3, szValue4, szValue5, szValue6);
				strcat(szName, szValue1);
				
				double dValue = 100.0-atof(szValue6);
				insertKpidataint(111, 1021, dValue, szTime, szName);
			}

		}
	}
	fclose(fp);
}


int hpsar()
{
	hpsarCpu();
	hpswapInOut();
	multiCoreCPUUsed();
	return 0;
}

