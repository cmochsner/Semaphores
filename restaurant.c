#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
 
sem_t mutex;

struct cust
{
	char customers[100][100];
	int delay[100];
	int service[100];
	int x;
	int i;
	int c;
	bool done;
	int clerks;
	bool finish;
	int wait;
	int serviced;
};
struct cust s;
void* thread(void* arg)
{
	printf(s.customers[s.i]);
	printf(" arrives at the restaurant.\n");
	int index=s.i;
	s.i++;	
	s.done=true;
	int value;
	sem_getvalue(&mutex, &value);
	if(value<1)
	{
		s.wait++;
	}
	sem_wait(&mutex); 
	printf(s.customers[index]);
	printf(" is getting helped.\n");
	s.serviced++;
	sleep(s.service[index]);
	printf(s.customers[index]);
	printf(" leaves the restaurant.\n");
	sem_post(&mutex);
	if(index==s.c)
	{
		s.finish=true;
	}
}
 
int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		printf("You have supplied too many arguments in your command line.\n");
		return 0;
	}		
	else if (argc < 2)
	{
		printf("You need to list the # of clerks you want in your command line.\n");
		return 0;
	}	
	sscanf(argv[1], "%d", &s.clerks);
	if(s.clerks>16)
	{
		printf("The maximum number of clerks for this restaurant is 16.\n");
		return 0;
	}
	FILE *fp;
	char name[100];
	s.c=0;
  	printf("What is your file name?\n");
	scanf("%s",&name);
	fp = fopen(name, "r");
	if (!fopen(name,"r"))
 	{
		printf( "Unable to open file.\n"); 
		return 0;
	}
  	else 
  	{
		
		while ( !feof(fp)) 
		{
			fscanf(fp,"%s",s.customers[s.c]);
			fscanf(fp,"%d",&s.delay[s.c]);
			fscanf(fp,"%d",&s.service[s.c]); 
			if( feof(fp) ) break;
			s.c++;
		}
		fclose(fp);
	}	
    sem_init(&mutex, 1, s.clerks);    
    pthread_t t[100];
	s.x=0;
	s.i=0;
	s.done=false;
	s.finish=false;
	s.serviced=0;
	s.wait=0;
	int h=0;
	while((s.i<s.c+1)||s.finish==false)
	{
		if((s.done||h==0)&&h<s.c+1)
		{	
			s.done=false;
			h++;
			sleep(s.delay[s.i]);
			pthread_create(&t[s.i],NULL,thread,NULL);
		}
	}
	int o=0;
	while(o<3)
	{
		pthread_join(t[o],NULL);
		o++;
	}
	sem_destroy(&mutex);
	printf("\nTotal number of customers that were serviced: ");
	printf("%d\n",s.serviced);
	int nowait=s.serviced-s.wait;
	printf("Total number of customers that did not have to wait: ");
	printf("%d\n",nowait);
	printf("Total number of customers that had to wait: ");
	printf("%d\n",s.wait);

	return 0;
}