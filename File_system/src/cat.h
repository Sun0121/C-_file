#pragma once
#include "struct.h"
/*
 *����:���ļ�
 *
 *ʵ��:���·�����ļ��Ƿ���ȷ����ȷ��ȡ���ļ���inode��������ļ�һ����С�Ļ�������
 *         ���ļ����ݶ�����������������ɣ�������ʾ������Ϣ
 *
 *path : �û������·��
 */
void cat(char*path)
{
	dir temp_dir;
	char file_name[NAME_LEN];
	if (dividePathAndName(path, file_name, temp_dir))
		open_file(temp_dir, file_name);
	else
		printf("�ļ���ʧ��!\n");
}

/*
 *����:���ļ�
 *ʵ��:����ļ��Ƿ���ڣ�Ȼ�������������
 *
 *temp_dir : Ҫ�򿪵��ļ��ľ������ڵ�Ŀ¼
 *file_name : ����Ҫ�򿪵��ļ����ļ���
 */
void open_file(dir temp_dir, char *file_name)
{
	int i;			//ѭ�����Ʊ���
	int inode;		//Ҫ���ļ���inode
	for (i = 2; i < temp_dir.size; i++)		//��temp_dir���Ҹ��ļ�
	{
		//����Ƿ�Ϊ��ͬ���ļ�������Ϊ�ļ�
		if (strcmp(temp_dir.file_entry[i].name, file_name) == 0 &&
			inode_table[temp_dir.file_entry[i].inode].i_mode == _FILE)
		{
			inode = temp_dir.file_entry[i].inode;
			break;
		}
	}
	if (i == temp_dir.size)
		printf("Ŀ¼ %s �в������ļ� %s ���ļ���ʧ��!\n", temp_dir.dir_name, file_name);
	else
	{
		//���ļ����ݶ���������buffer
		char *buffer = new char[inode_table[inode].i_size];		//�ļ����ݻ�����
		fd = fopen("LINUX_FILE_SYSTEM", "rb");
		if (fd == NULL)
		{
			printf("Linux_file_system create failed !\n");
			system("pause");
		}
		fseek(fd, inode_table[inode].i_block, SEEK_SET);
		fread(buffer, inode_table[inode].i_size, 1, fd);
		fclose(fd);
		buffer[inode_table[inode].i_size - 1] = 0;
		printf("�ļ� %s ����������:\n\n", file_name);
		printf("%s\n", buffer);
		delete(buffer);
	}
}
