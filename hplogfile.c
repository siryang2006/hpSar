#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>  
#include "hplogfile.h"
#include "util.h"

int g_ptToCheckExistFiles[128] = {0};
int g_ptToCheckFileSize[128] = {0};
int g_ptToCheckLastModifyTime[128] = {0};
int initToCheckEixstFiles();

int initLogFile()
{
	initToCheckEixstFiles();
	initToCheckFileSize();
	initToCheckLastModifyTime();
	return 0;
}

int initToCheckEixstFiles()
{
    static char szToCheckExistFiles[2048] = {0};
    if(0<readIniData("FileExist", INI_FILE_EX, szToCheckExistFiles))
    {
#ifdef __PIPE_DEBUG__
	printf("%s\n", szToCheckExistFiles);
#endif //__PIPE_DEBUG__
        char *p = strtok(szToCheckExistFiles, "\n");
        g_ptToCheckExistFiles[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptToCheckExistFiles[nIndex] = (int)p;
            nIndex++;
        }
        return 0;
    }
    return -1;
}

int initToCheckFileSize()
{
    static char szToCheckFileSize[2048] = {0};
    if(0<readIniData("FileSize", INI_FILE_EX, szToCheckFileSize))
    {
#ifdef __PIPE_DEBUG__
	printf("%s\n", szToCheckFileSize);
#endif //__PIPE_DEBUG__
        char *p = strtok(szToCheckFileSize, "\n");
        g_ptToCheckFileSize[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptToCheckFileSize[nIndex] = (int)p;
            nIndex++;
        }
        return 0;
    }
    return -1;
}

int initToCheckLastModifyTime()
{
    static char szToCheckLastModifyTime[2048] = {0};
    if(0<readIniData("FileLastModifyTime", INI_FILE_EX, szToCheckLastModifyTime))
    {
#ifdef __PIPE_DEBUG__
	printf("%s\n", szToCheckLastModifyTime);
#endif //__PIPE_DEBUG__
        char *p = strtok(szToCheckLastModifyTime, "\n");
        g_ptToCheckLastModifyTime[0] = (int)p;
        int nIndex = 1;
        while((p=strtok(NULL, "\n"))!=NULL)
        {
            g_ptToCheckLastModifyTime[nIndex] = (int)p;
            nIndex++;
        }
        return 0;
    }
    return -1;
}

int isFileExist()
{
	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for(i=0; i<128; i++)
	{
		char *p = g_ptToCheckExistFiles[i];
		if(p == NULL)
		{
			return 0;
		}

		if((access(p,F_OK))!=-1)  
		{
			insertKpidataint(111, 1227, 1, szTime, p);
		} 
		else
		{
			insertKpidataint(111, 1227, 0, szTime, p);
		}
	}
}

unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}

int fileSize()
{
	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for(i=0; i<128; i++)
	{
		char *p = g_ptToCheckExistFiles[i];
		if(p == NULL)
		{
			return 0;
		}

		if((access(p,F_OK))!=-1)  
		{
			insertKpidataint(111, 1228, get_file_size(p), szTime, p);
		} 
		else
		{
			insertKpidataint(111, 1228, -1, szTime, p);
		}
	}
}

char * getFileLastModifyTime(const char *szFileName, char szRet[])
{
	struct stat statbuff;  
    if(stat(szFileName, &statbuff) < 0){  
    }else{  
	struct tm *p;
    	p = localtime(&statbuff.st_mtime); /*转换为struct tm结构的UTC时间*/
        sprintf(szRet, "%d-%02d-%02d %02d:%02d:%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday,
                p->tm_hour, p->tm_min, p->tm_sec);
    }  
    return szRet;  
	
}

int lastModifyTime()
{
	char szTime[32] = {0};
        currentTime(szTime);
	int i=0;
	for(i=0; i<128; i++)
	{
		char *p = g_ptToCheckLastModifyTime[i];
		if(p == NULL)
		{
			return 0;
		}

		if((access(p,F_OK))!=-1)  
		{
char szModifyTime[32] = {0};
			insertKpidatachar(111, 1229, getFileLastModifyTime(p, szModifyTime), szTime, p);
		} 
		else
		{
			insertKpidatachar(111, 1229, "", szTime, p);
		}
	}
}

int doLogFile()
{
	isFileExist();
fileSize();
lastModifyTime();
return 0;
}