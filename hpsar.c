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


int hpsar()
{
	hpsarCpu();
	hpswapInOut();
	return 0;
}

