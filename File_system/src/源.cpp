#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "pathAnalysis.h"
#include "blockManage.h"
#include "inodeManage.h"
#include "findSameName.h"
#include "struct.h"
#include "init.h"
#include "login.h"
#include "help.h"
#include "dir.h"
#include "copy.h"
#include "info.h"
#include "check.h"
#include "exit.h"
#include "cd.h"
#include "md.h"
#include "rd.h"
#include "newfile.h"
#include "cat.h"
#include "del.h"

unsigned int uid;										//ʹ���ļ�ϵͳ���û�ID
char pwd[20];											//����
command cmd[13];										//����
block_group _block_group[BLOCKS_GROUP_NUM];		//���ݿ���(������ + ��������)
inode inode_table[TOTAL_INODES_NUM];			//inode��
user _user[MAX_USER_NUM];						//�û�
dir _current_dir;								//Ŀ¼
bool block_bitmap[TOTAL_BLOCKS_NUM];					//���ݿ�λͼ
bool inode_bitmap[TOTAL_INODES_NUM];					//inode λͼ
char current_path[MAX_PATH_LEN];								//���浱ǰ·��
FILE *fd;												//ϵͳ�ļ�

void main()
{
	load();login(); 
	printf("\n*****************************************************************************\n");
	printf("*                  ��ӭʹ��ģ��Linux�ļ�ϵͳ                                *\n");
	printf("*****************************************************************************\n");
	while(1)
	{
		char control[100];					//������������
		char path1[MAX_PATH_LEN],path2[MAX_PATH_LEN];		//����·������������
		int i=0;							//ѭ�����Ʊ���
		fflush(stdin);						//�����
		printf("\n%s",current_path);
		scanf("%s",control);
		for(i=0;i<13;i++)
			if(strcmp(cmd[i].order,control)==0)
				break;
		switch(i)
		{
		case 0:		//init
			initialize();
			break;
		case 1:		//info
			info();
			break;
		case 2:		//cd
			scanf("%s",path1);
			cd(path1);
			break;
		case 3:		//dir
			scanf("%s",path1);
			dirDisplay(path1);
			break;
		case 4:		//md
			scanf("%s",path1);
			md(path1);
			break;
		case 5:		//rd
			scanf("%s",path1);
			rd(path1);
			break;
		case 6:		//newfile
			scanf("%s",path1);
			newfile(path1);
			break;
		case 7:		//cat
			scanf("%s",path1);
			cat(path1);
			break;
		case 8:		//copy
			scanf("%s",path1);
			scanf("%s",path2);
			copy_file(path1,path2);
			break;
		case 9:		//del
			scanf("%s",path1);
			del(path1);
			break;
		case 10:	//check
			check();
			break;
		case 11:	//exit
			exit();
			break;
		case 12:	//help
			help();
			break;
		default:
			printf("û�д�����!������������� hlep\n");
		}
	}
}