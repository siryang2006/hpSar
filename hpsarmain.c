#include "hpprocess.h"
#include "hpsar.h"
#include "util.h"
#include "hpnetwork.h"
int main(int argc, char **argv)
{
int interval = getFrequencySecond();
		if (0 != init())
    {
        printf("probe init faild!!!\n");
        return -1;
    }
    
	cpuInfo();

	while(1)
	{
		hpother();
		process();
		hpsar();
		filesystem();
		network();
		sleep(interval);	
	}
	disconnect_db();
	return 0;	
}