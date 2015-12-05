#include <stdio.h>
#include <stdlib.h>
#include "hpnetwork.h"
#include "util.h"
int netAdaptor()
{
char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
		strcpy(caPSCmd,"lanscan");
		//printf("----------------hpUX lanscan ------------------\n");    
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
            return -1;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        int nProcessNumber = 0;
        char szTime[32] = {0};
        currentTime(szTime);

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
            //hp-ux Filesystem          kbytes    used   avail %used Mounted on
#ifdef __PIPE_DEBUG__
 	printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

		if(strstr(caStdOutLine, "Path") != NULL || strstr(caStdOutLine, "path") != NULL)
		{
			char szName[128] = {0};
#ifdef __PIPE_DEBUG__
			    printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__
			    char *pTmp = NULL;
			    
			while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
			{
if(strcmp(caStdOutLine, "\n") ==0)
{
#ifdef __PIPE_DEBUG__
	printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

	continue;
}
				if(NULL == (pTmp=strtok(caStdOutLine, " ")))
			    {
				printf("error parse lanscan aux result %s\n", caStdOutLine);
				fclose(fp);
				return -1;
			    }
			    int i=0;
			    for(i=0; i<3;i++)
			    {
					if(NULL == (pTmp=strtok(NULL, " ")))
					    {
						printf("error parse lanscan result %s\n", caStdOutLine);
						fclose(fp);
						return -1;
					    }
			    }
					char szStatus[20] = {0};
					strcpy(szStatus, pTmp);
		
					if(NULL == (pTmp=strtok(NULL, " ")))
					    {
						printf("error parse lanscan result %s\n", caStdOutLine);
						fclose(fp);
						return -1;
					    }
				insertKpidatachar(111, 1151, pTmp, szTime, pTmp);
				insertKpidatachar(111, 1152, szStatus, szTime, pTmp);
			}
		break;
		}
	}       
 	pclose(fp);
    }while (0);
}

int inout()
{
	char caStdOutLine[1024] = {0};
    char caPSCmd[24] = {0};
#ifdef _HPUX_
		strcpy(caPSCmd,"netstat -i");
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
            return -1;
        }

        memset(caStdOutLine, 0, sizeof(caStdOutLine));
        char szTime[32] = {0};
        currentTime(szTime);
char *pTmp = NULL;

        while (NULL != fgets(caStdOutLine, sizeof(caStdOutLine), fp))
        {
		if(strstr(caStdOutLine, "name") != NULL || strstr(caStdOutLine, "Name") != NULL)
		{
#ifdef __PIPE_DEBUG__
	printf("%s\n", caStdOutLine);
#endif //__PIPE_DEBUG__

continue;
		}
char szName[20] = {0};
		if(NULL == (pTmp=strtok(caStdOutLine, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
strcpy(szName, pTmp);
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
insertKpidataint(111, 1153, atof(pTmp), szTime, pTmp);
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
if(NULL == (pTmp=strtok(NULL, " ")))
		{
			printf("error parse lanscan aux result %s\n", caStdOutLine);
			fclose(fp);
			return -1;
		}
insertKpidataint(111, 1154, atof(pTmp), szTime, pTmp);

	}
   }while(0);
}

int ping(const char *szIP)
{
    char szCmd[128] = {0};
    strcpy(szCmd, "ping ");
    strcat(szCmd, szIP);
strcat(szCmd, " -n 1");
signal(SIGCHLD, SIG_IGN);
    FILE* fp = popen(szCmd, // 一个指向以 NULL 结束的 shell 命令字符串的指针，
                     // 这行命令将被传到 bin/sh 并使用 -c 标志，
                     // 那么 shell 将执行这个命令从这个字符串中读取。
                     "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        sleep(10);
        return -1;
    }

    char szTime[32] = {0};
    currentTime(szTime);

    char szLine[1024] = {0};
    while(fgets(szLine, 1024, fp) != NULL)
    {
#ifdef __PIPE_DEBUG__
	printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

        if((strstr(szLine, "Statistics")||strstr(szLine, "statistics")) && fgets(szLine, 1024, fp) != NULL)
        {
#ifdef __PIPE_DEBUG__
	printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

            //1 packets transmitted, 1 received, 0% packet loss, time 0ms
            char *pData = NULL;
            pData = strtok(szLine, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            if(pData != NULL && pData[strlen(pData)-1] == '%')
            {
                pData[strlen(pData)-1] = '\0';
            }
            insertKpidataint(111, 1161, 100.0-atof(pData), szTime, "socket");//ping,包成功率
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");

            
            //return 0;
        }
	else if(strstr(szLine, "from"))
        {
		char *pData = NULL;
            pData = strtok(szLine, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");
            pData = strtok(NULL, " ");

		double dTime = -1;
            if(pData != NULL && strstr(pData, "time="))
            {
                char szMs[20] = {0};
                strcpy(szMs, pData+5);
#ifdef __PIPE_DEBUG__
	printf("time=%s\n", szMs);
#endif //__PIPE_DEBUG__

                dTime = atol(szMs);
		insertKpidataint(111, 1162, dTime, szTime, "socket");
            }
            
	}
    }
    return -1;
}

int socketConnectedNumber()
{
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -n|grep tcp|wc -l", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return -1;
    }
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
	printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

	if(strcmp(szLine, "\n") == 0)
	{
		continue;
	}
	if(szLine[strlen(szLine)-1] == '\n')
	{
		szLine[strlen(szLine)-1] = '\0';	
	}
	insertKpidataint(111, 1155, atof(szLine), szTime, "socket");
break;
	}

	fclose(fp);	
}

int currentSocketConnected()
{
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -n|grep SYN|wc -l", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return -1;
    }
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
		printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

		if(strcmp(szLine, "\n") == 0)
		{
			continue;
		}
		if(szLine[strlen(szLine)-1] == '\n')
		{
			szLine[strlen(szLine)-1] = '\0';	
		}
		insertKpidataint(111, 1156, atof(szLine), szTime, "socket");
break;
	}

	fclose(fp);	
}

int socketConnectedFINNumber()
{
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -n|grep FIN|wc -l", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return -1;
    }
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
		printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

		if(strcmp(szLine, "\n") == 0)
		{
			continue;
		}
		if(szLine[strlen(szLine)-1] == '\n')
		{
			szLine[strlen(szLine)-1] = '\0';	
		}
		insertKpidataint(111, 1157, atof(szLine), szTime, "socket");
break;
	}

	fclose(fp);	
}

int socketCreatedNumber()
{
int nRet = 0;
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -n|grep ESTABLISH|wc -l", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return nRet;
    }
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
		printf("%s\n", szLine);
#endif //__PIPE_DEBUG__

		if(strcmp(szLine, "\n") == 0)
		{
			continue;
		}
		if(szLine[strlen(szLine)-1] == '\n')
		{
			szLine[strlen(szLine)-1] = '\0';	
		}
		nRet = atoi(szLine);
	break;
	}

	fclose(fp);	
return nRet;
}

int socketCreatedTotalNumber()
{
int nRet = 1;
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -n|grep tcp|wc -l", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return nRet;
    }
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
		printf("%s\n", szLine);
#endif
		if(strcmp(szLine, "\n") == 0)
		{
			continue;
		}
		if(szLine[strlen(szLine)-1] == '\n')
		{
			szLine[strlen(szLine)-1] = '\0';	
		}
		nRet = atoi(szLine);
if(nRet<1)
{
nRet = 1;
}
	break;
	}

	fclose(fp);	
return nRet;
}

int socketCreateRatio()
{
char szTime[32] = {0};
   	 currentTime(szTime);
	double d = (double)(socketCreatedNumber()*100)/socketCreatedTotalNumber();
	insertKpidataint(111, 1158, d, szTime, "socket");
}

int portIsOpen()
{
	int nRet = 1;
	char szLine[1024] = {0};
	char szTime[32] = {0};
   	 currentTime(szTime);
signal(SIGCHLD, SIG_IGN);
	FILE* fp = popen("netstat -an|grep LISTEN", "r" );  // 文件指针连接到 shell 命令的标准输出
    if ( NULL == fp )
    {
        printf( "ping error :call popen is failed!!!! \n" );
        return nRet;
    }
char *pTmp = NULL;
	while(NULL!=(fgets(szLine, sizeof(szLine), fp)))
	{
#ifdef __PIPE_DEBUG__
		printf("%s\n", szLine);
#endif
		if(strcmp(szLine, "\n") == 0)
		{
			continue;
		}

  		if(NULL == (pTmp=strtok(szLine, " ")))
		{
			printf("error parse LISTEN result %s\n", szLine);
			fclose(fp);
			return -1;
	        }
		int i=0;
		for(i=0; i<3;i++)
		{
		if(NULL == (pTmp=strtok(NULL, " ")))
				{
					printf("error parse LISTEN result %s\n", szLine);
					fclose(fp);
					return -1;
				}
		}
		pTmp = strchr(pTmp, '.');
		if(pTmp != NULL && (pTmp=pTmp+1)!=NULL)
		{
			insertKpidataint(111, 1159, atof(pTmp), szTime, "socket");
		}

	}

	fclose(fp);	
return nRet;
}


int network()
{
	netAdaptor();
	inout();
	ping("127.0.0.1");
	socketConnectedNumber();
	currentSocketConnected();
socketConnectedFINNumber();
socketCreateRatio();
portIsOpen();
	return 0;    
}
