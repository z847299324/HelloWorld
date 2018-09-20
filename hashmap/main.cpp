#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	char ch;
	int flag = 0;//���ñ�־λ
	char *infilename ="in", *outfilename = "out";	// ��ʼ������������ļ���
	FILE *infile, *outfile;//�ļ�ָ��
	if (argc == 2)
		strcpy(infilename, argv[1]);
	else if (argc == 3)	// ָ����������ļ���
	{
		if (!strcmp(argv[1], argv[2]))//�������������Ĳ���
		{
			printf("error:file is same\n");
			exit(0);
		}
		strcpy(infilename, argv[1]);
		strcpy(outfilename, argv[2]);
	}
	else if (argc > 3)
	{
		printf("error:�����������\n");
		exit(0);
	}
	if ((infile = fopen(infilename, "r")) == NULL)//���ļ��Ĵ�����
	{
		printf("can't open input file!\n");
		exit(0);
	}
	if ((outfile = fopen(outfilename, "w")) == NULL)//������ļ��ļ��
	{
		printf("can't open output file!\n");
		exit(0);
	}
	while ((ch = fgetc(infile)) != EOF)	// �������ļ��е��ַ�
	{
		if (flag == 0)
		{
			if (ch == '/')	// ����"/", flag = 1;
			{
				flag = 1;
			}
		}
		else if (flag == 1)
		{
			if (ch == '/')	// ����"//", flag = 2;
			{
				flag = 2;
				continue;
			}
			else if (ch == '*')	// ����"/*", flag = 3;
			{
				flag = 3;
				continue;
			}
			else	// ������"/"��ע�ͷ�
			{
				flag = 0;
				fputc('/', outfile);
			}
		}
		else if (flag == 2)//����˫//ע��
		{
			if (ch == '\n')	// ����"//"ע�ͽ�β�Ļ��з�
				flag = 0;
			else	// ����"//"��ע�Ͳ���
				continue;
		}
		else if (flag == 3)
		{
			if (ch == '*')	// ����"/*"ע���е�"*", flag = 4;
				flag = 4;
			continue;
		}
		else if (flag == 4)
		{
			if (ch == '/')	// ����"/*"ע�ͽ�β��"*/"
				flag = 0;
			else if (ch != '*')	// ����"/*"��ע�Ͳ���
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