#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	char ch;
	int flag = 0;//设置标志位
	char *infilename ="in", *outfilename = "out";	// 初始化的输入输出文件名
	FILE *infile, *outfile;//文件指针
	if (argc == 2)
		strcpy(infilename, argv[1]);
	else if (argc == 3)	// 指定输入输出文件名
	{
		if (!strcmp(argv[1], argv[2]))//检测来两个传入的参数
		{
			printf("error:file is same\n");
			exit(0);
		}
		strcpy(infilename, argv[1]);
		strcpy(outfilename, argv[2]);
	}
	else if (argc > 3)
	{
		printf("error:传入参数过多\n");
		exit(0);
	}
	if ((infile = fopen(infilename, "r")) == NULL)//打开文件的错误检测
	{
		printf("can't open input file!\n");
		exit(0);
	}
	if ((outfile = fopen(outfilename, "w")) == NULL)//打开输出文件的检测
	{
		printf("can't open output file!\n");
		exit(0);
	}
	while ((ch = fgetc(infile)) != EOF)	// 读输入文件中的字符
	{
		if (flag == 0)
		{
			if (ch == '/')	// 遇到"/", flag = 1;
			{
				flag = 1;
			}
		}
		else if (flag == 1)
		{
			if (ch == '/')	// 遇到"//", flag = 2;
			{
				flag = 2;
				continue;
			}
			else if (ch == '*')	// 遇到"/*", flag = 3;
			{
				flag = 3;
				continue;
			}
			else	// 遇到的"/"非注释符
			{
				flag = 0;
				fputc('/', outfile);
			}
		}
		else if (flag == 2)//遇到双//注释
		{
			if (ch == '\n')	// 遇到"//"注释结尾的换行符
				flag = 0;
			else	// 遇到"//"的注释部分
				continue;
		}
		else if (flag == 3)
		{
			if (ch == '*')	// 遇到"/*"注释中的"*", flag = 4;
				flag = 4;
			continue;
		}
		else if (flag == 4)
		{
			if (ch == '/')	// 遇到"/*"注释结尾的"*/"
				flag = 0;
			else if (ch != '*')	// 遇到"/*"的注释部分
				flag = 3;
			continue;
		}
		else
		{
			if (ch == ' ')
			{
				continue;
			}
			fputc(ch, outfile);
		}
	}
	fclose(infile);	
	fclose(outfile);
}