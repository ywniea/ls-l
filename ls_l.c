#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>

void mode_to_str(mode_t mt,char *str){

	strcpy(str,"----------");
	if(S_ISDIR(mt)){

		str[0]='d';
	}
	if(mt & S_IRUSR){

		str[1]='r';
	}
	if(mt & S_IWUSR){

		str[2]='w';
	}
	if(mt & S_IXUSR){

		str[3]='x';
	}
	if(mt & S_IRGRP){

		str[4]='r';
	}
	if(mt & S_IWGRP){

		str[5]='w';
	}
	if(mt & S_IXGRP){

		str[6]='x';
	}
	if(mt & S_IROTH){

		str[7]='r';
	}
	if(mt & S_IWOTH){

		str[8]='w';
	}
	if(mt & S_IXOTH){

		str[9]='x';
	}
	str[10]=0;
}

void format_time(char *time){

	while(*time){

		time++;
	}
	while(*time != ':'){

		time--;
	}
	*time=0;
}

int main(int argc,char *argv[]){

	int mysize=0;
	DIR * mydir;
	struct dirent *myent;
	struct stat mystat;
	char mode_str[11];
	char *time,*dir_name;
	char road[100];
	int w_uname=0,w_gname=0,w_dname=0;
	char myname[260];
	road[0]=0;
	if(argc==1){

		mydir=opendir(".");
		dir_name = ".";

	}else{

		mydir=opendir(argv[1]);
		dir_name = argv[1];
	}

	while( (myent=readdir(mydir))!=NULL){//readdir returns  struct dirent*

		if((strncmp(myent->d_name,".",1)!=0) && (strncmp(myent->d_name,"..",2)!=0) ){

			sprintf(road,"%s/%s",dir_name,myent->d_name);	
			stat(road ,&mystat) ;//stat requires 	struct stat*
			//attention! the type of myent and mystat.

			mysize=mysize+mystat.st_size;

			sprintf(myname,"%s",myent->d_name);
			if(w_dname<strlen(myname)){

				w_dname=strlen(myname);
			}
			sprintf(myname,"%s",getpwuid(mystat.st_uid)->pw_name);
			if(w_uname<strlen(myname)){

				w_uname=strlen(myname);
			}
			sprintf(myname,"%s",getgrgid(mystat.st_gid)->gr_name);
			if(w_gname<strlen(myname)){

				w_gname=strlen(myname);
			}

		}
	}
//	printf("uname= %d gname= %d dname= %d\n",w_uname,w_gname,w_dname);
	mysize=mysize/1024;
	printf("total %d\n",mysize);
	///////////////////////////

	if(argc==1){

		mydir=opendir(".");
		dir_name = ".";

	}else{

		mydir=opendir(argv[1]);
		dir_name = argv[1];
	}

	while( (myent=readdir(mydir))!=NULL){//readdir returns  struct dirent*

		if((strncmp(myent->d_name,".",1)!=0) && (strncmp(myent->d_name,"..",2)!=0) ){

			sprintf(road,"%s/%s",dir_name,myent->d_name);	
			stat(road ,&mystat) ;//stat requires 	struct stat*
			//attention! the type of myent and mystat.
			time=ctime(&mystat.st_atime);
			format_time(time);
			mode_to_str(mystat.st_mode,mode_str);

			printf("%10s. %2d %-*s %*s %5d %12s %-*s\n",mode_str,mystat.st_nlink,w_uname,getpwuid(mystat.st_uid)->pw_name,w_gname,getgrgid(mystat.st_gid)->gr_name,mystat.st_size,time+4,w_dname,myent->d_name);
		}
	}


	return 0;
}