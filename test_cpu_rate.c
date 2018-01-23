#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
int main(int argc, char* argv[])
{
    int num = sysconf(_SC_NPROCESSORS_CONF);
    int created_thread = 0;
    int myid;
    int percentage;
    int i;
    int j = 0;
    struct timeval tv;
    long long start_time,end_time;
    cpu_set_t mask;
    cpu_set_t get;

    printf("===test_cpu_rate V1.0===\n");
    printf("usage: ./test_cpu_rate cpuid rate\n");
    printf("\tcpuid : this process is running in cpuid\n");
    printf("\trate: percentage\n");
    printf("example: ./test_cpu_rate 0 20\n");
    printf("\t ./test_cpu_rate is running in cpuid 0, 20% occupied!\n");

    if (argc != 3)
    {
            printf("error format\n");
            exit(1);
    }


    myid = atoi(argv[1]);/*绑定到的cpu*/
    percentage = atoi(argv[2]);

    printf("system has %i processor(s). \n", num);

    CPU_ZERO(&mask);
    CPU_SET(myid, &mask); /*掩码*/

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)/*0指当前进程*/
    {
            printf("warning: could not set CPU affinity, continuing...\n");
    }

    CPU_ZERO(&get);
    if (sched_getaffinity(0, sizeof(get), &get) == -1)
    {
            printf("warning: cound not get cpu affinity, continuing...\n");
    }

    for (i = 0; i < num; i++)
    {
        if (CPU_ISSET(i, &get))
        {
            printf("this process %d is running processor : %d, rate : %d\%\n",getpid(), i,percentage);
        }
    }
    
    while(1)
    {
       gettimeofday(&tv,NULL);
       start_time = tv.tv_sec*1000000 + tv.tv_usec;
       end_time = start_time;
       while((end_time - start_time) < 60000)
       {
            gettimeofday(&tv,NULL);
            end_time = tv.tv_sec*1000000 + tv.tv_usec;
       }
       //usleep(60000);
       usleep(60000*100/percentage-60000);
    }
    // }
    return 0;
}
