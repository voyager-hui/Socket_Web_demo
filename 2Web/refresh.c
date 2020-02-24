//名称：生成shell命令字符串
//时间：20200213
//作者：voyager
//功能：按行读取文本，然后生成shell命令，写入到服务器的文件中，等待其他程序读取
//注意：数组长度一定要分配够

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int read_data(FILE *read_file, char buf[100][100]);

int main(){
	FILE *read_file;
    int i, lines;
	char buf[100][100] = {0};
	char command[100][200] = {0};
	//获取原始数据
	lines = read_data(read_file, buf);
	//去掉字符串中的换行符
	for(i=0; i<lines; i++){
		if(strlen(buf[i]) > 1){
			if(buf[i][strlen(buf[i])-2] == '\r' || buf[i][strlen(buf[i])-2] == '\n'){
				buf[i][strlen(buf[i])-2] = 0;
			}
			else if(buf[i][strlen(buf[i])-1] == '\n' || buf[i][strlen(buf[i])-1] == '\r'){
				buf[i][strlen(buf[i])-1] = 0;
			}
		}
		else if(strlen(buf[i]) == 1){
			if(buf[i][strlen(buf[i])-1] == '\n' || buf[i][strlen(buf[i])-1] == '\r'){
				buf[i][strlen(buf[i])-1] = 0;
			}
		}
	}
	//拼装shell命令
	for(i=0; i<lines; i++){
		sprintf(command[i], "echo \"%s\" > /var/www/html/sensor/data.txt", buf[i]);
	}
	//发送shell命令，间隔10秒
	for(i=0; ;i++,i%=lines){
		printf("command[%d] = %s\n", i, command[i]);
		system(command[i]);
		sleep(10);
	}
	return 0;
}

int read_data(FILE *read_file, char buf[100][100]){
	int count = 0;
	read_file = fopen("data.csv", "r");
	if(!read_file){
		puts("Cannot read");
	}
	while(fgets(buf[count], 100, read_file) != NULL){
		count++;
	}
	fclose(read_file);
	return count;
}
