/*****************************************************************************
名称: 客户端处理程序
时间: 20200208
作者: voyager
功能: 计算地点之间的两两距离
注意:
*****************************************************************************/

#include<stdio.h>
#include<string.h>
#include<math.h>

#define PI 3.141592

void process(char buf[100][1000], double buf_result[100][1000], int lines);  //处理函数
int read_data(FILE *read_file, char buf[100][1000]);  //读取文本
void save_data(FILE *write_file, double buf_result[100][1000], int lines);  //保存文本


int main(){
	FILE *read_file, *write_file;
	char buf[100][1000];
	double buf_result[100][1000];
	int lines;

	lines = read_data(read_file, buf);
	process(buf, buf_result, lines);
	save_data(write_file, buf_result, lines);
	
	return 0;
}

/**
	处理时数据保留了小数点后6位
*/
void process(char buf[100][1000], double buf_result[100][1000], int lines){
	int i, j;
	double i0, i1, j0, j1;
	for(i=0; i<lines; i++){
		sscanf(buf[i], "%lf,%lf", &i0, &i1);
		for(j=0; j<lines; j++){
			sscanf(buf[j], "%lf,%lf", &j0, &j1);
			buf_result[i][j] = sqrt(pow(i0-j0, 2)+pow(i1-j1, 2)) / 180 * PI * 6370;
		}
	}
}


int read_data(FILE *read_file, char buf[100][1000]){
	int count = 0;
	read_file = fopen("server_data.csv", "r");
	if(!read_file){
		puts("Cannot read");
	}
	while(fgets(buf[count], 1000, read_file) != NULL){
		count++;
	}
	fclose(read_file);
	return count;
}

void save_data(FILE *write_file, double buf_result[100][1000], int lines){
	int i, j;
	int count = 0;
	write_file = fopen("server_result.csv", "w+");
	if(!write_file){
		puts("Cannot write");
	}
	for(i=0; i<lines; i++){
		for(j=0; j<lines; j++){
			if(j == (lines-1)){
				fprintf(write_file, "%lf\n", buf_result[i][j]);
			}
			else{
				fprintf(write_file, "%lf,", buf_result[i][j]);
			}
		}
	}
	fclose(write_file);
}
