#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFMAX 1024
//0
int src_fd,dst_fd; 
//1 openfile()
int OpenFile(char *src,char *dst){
	if((src_fd=open(src,O_RDONLY))==-1){
		fprintf(stderr,"open error: %s\n",strerror(errno));
		return -1;
	}
	if((dst_fd=open(dst,O_WRONLY | O_CREAT|O_TRUNC,0644))==-1){
		fprintf(stderr,"open error: %s\n",strerror(errno));
		return -1;
	}
	return 0;
}

//2	Cpfiletofile
int CpfileTofile(int srcfd, int dstfd, char *argv[], int infoflag, int place){
	char buf[BUFMAX];
	int retsize,bufsize=0;
	while(1){
		if((retsize=read(srcfd,buf,BUFMAX))<0){
			fprintf(stderr,"read: %s\n",strerror(errno));
			return -1;
		}
		bufsize+=retsize;
		if(retsize<BUFMAX){
			break;
		}
		if(write(dstfd,buf,BUFMAX)<0){
			fprintf(stderr,"write: %s\n",strerror(errno));
			return -1;
		}
	}
//	printf("read is end\n");
	if(write(dstfd,buf,retsize)<0){
		fprintf(stderr,"write: %s\n",strerror(errno));
		return -1;
	}
	if(infoflag){
		fprintf(stdout,"Process pid = %d is executed!\n",getpid());	
		fprintf(stdout,"%d bytes is readed from file of %s\n",bufsize,argv[place]);
	}
	return 0;
}

//3	CpfileTodir
int CpfileTodir(int argc, char *argv[]){
	int dirflag=0,infoflag=0;
	int i,j;
	int startino,endino;
	char dst[256];
	
	if(*(argv[argc-1]+strlen(argv[argc-1])-1)!='/'){
		fprintf(stderr,"the component is error!\n");
		return -1;
	}
	else{
		if(!strcmp(argv[1],"-v"))
			infoflag=1;
		if(infoflag)
		{
			startino=2;
		//	printf("startino=%d\n",startino);
		}
		else
			startino=1;
		endino=argc-1;
		for(i=startino;i<endino;i++){
			//*******
			strcpy(dst,argv[endino]);	
			strcpy(dst,strcat(dst,argv[i]));

			//++++++++++++++
			for(j=0;j<3;j++){
				if(OpenFile(argv[startino],dst)==-1){
					fprintf(stderr,"OpenFile is error is there!\n");
					return -1;
				}
			}
			if(CpfileTofile(src_fd,dst_fd,argv,infoflag,i)==-1){
				fprintf(stderr,"CpfileTofile is error\n");
				return -1;
			}
		}
	}
	return 0;
}

int main(int argc, char *argv[]){
	//0
	int infoflag;
	//1
	if(argc<3){
		fprintf(stderr,"Usage: ./cp [-v] src dst ...\n");
		exit(1);
	}
	//2
	if(argc>4){
		CpfileTodir(argc,argv);
	}
	else{
		if(argc==4){	
			if(!strcmp(argv[1],"-v")){
				infoflag=1;
				if(*(argv[argc-1]+strlen(argv[argc-1])-1)=='/')
					argv[argc-1]=strcat(argv[argc-1],argv[infoflag+1]);
				OpenFile(argv[2],argv[3]);
			}else{
				fprintf(stderr,"error:the prompt of tags is error!\n");
				exit(2);
			}
		}
		else{
			if(!strcmp(argv[1],"-v"))
			{
				fprintf(stderr,"error:the prompt of tags is error!\n");
				exit(3);
			}
			infoflag=0;
			if(*(argv[argc-1]+strlen(argv[argc-1])-1)=='/')
				argv[argc-1]=strcat(argv[argc-1],argv[infoflag+1]);
			OpenFile(argv[1],argv[2]);
		}
		CpfileTofile(src_fd,dst_fd,argv,infoflag,2);
	}
	exit(0);
}
