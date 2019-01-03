#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define __USE_GNU
#include <sched.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#define APIC_BASE	(0xfee00000)
#define APIC_LEN	(0x1000)
void help(void)
{
    printf("%s [cpu_id] [out_file_name],if no out_file_name arg,default name is apic.bin\n");
}
int main(int argc,char *argv[])
{
    int i = 0,ret = -1,fd_in = -1,fd_out = -1;
    char filename[32] = "apic.bin";
    void *mem = NULL;
    int cpu_id = 0,cpu = 0;
    cpu_set_t mask,get;
    
	printf("%s:%d\n",__func__,__LINE__);
    if(3 > argc)
	help();
    else
        strcpy(filename,argv[2]);
    
		printf("%s:%d\n",__func__,__LINE__);
    if(2 <= argc)
	cpu_id = atoi(argv[1]);
     cpu = get_nprocs();
     if(cpu_id >= cpu)
        cpu_id = cpu - 1;

     if(cpu_id < 0)
         cpu_id = 0;
		printf("%s:%d\n",__func__,__LINE__);
     CPU_ZERO(&mask);
     CPU_SET(cpu_id,&mask);

	ret = sched_setaffinity(0,sizeof(mask),&mask);
	if(ret < 0){
		printf("%s:%d\n",__func__,__LINE__);
	}
	CPU_ZERO(&get);
	ret = sched_getaffinity(0,sizeof(get),&get);
       if(ret < 0){
		printf("%s:%d\n",__func__,__LINE__);
	}

    for(i = 0; i < cpu;i++){
	if(CPU_ISSET(i,&get))
		printf("%s:%d i = %d\n",__func__,__LINE__,i);
   }


	
    printf("%s:%d\n",__func__,__LINE__);
    fd_out = open(filename,O_RDWR | O_CREAT);
    if(0 > fd_out){

    printf("%s:%d\n",__func__,__LINE__);
         return fd_out;
    }
    printf("%s:%d\n",__func__,__LINE__);
    fd_in = open("/dev/mem",O_RDONLY);
    if(0 > fd_in){

    printf("%s:%d\n",__func__,__LINE__);
         return fd_in;
    }

    mem = mmap(NULL,APIC_LEN,PROT_READ,MAP_SHARED,fd_in,APIC_BASE);
    if(!mem){

    printf("%s:%d\n",__func__,__LINE__);
        return -1;
     }

      ret = write(fd_out,mem,APIC_LEN);
      if(ret < 0)
	  return ret;

    printf("%s:%d\n",__func__,__LINE__);
      printf("ret = %d\n",ret);

      munmap(mem,APIC_LEN);
      close(fd_out);
      close(fd_in);
      return 0;
}










