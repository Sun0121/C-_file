#pragma once
#include "struct.h"
/*
 *����:ɾ���ļ�
 *ʵ�֣���������·�����ҳ�Ҫɾ�����ļ����ڵ�Ŀ¼��ɾ���ļ����޸�ϵͳ��Ϣ
 *
 *path : �û��������·��
 *         
 */
void del(char *path)
{
	dir temp_dir;
	char file_name[NAME_LEN];
	if (dividePathAndName(path, file_name, temp_dir))
		delete_file(temp_dir, file_name);
	else
		printf("�ļ�ɾ��ʧ��!\n");
}

/*
 *����:ɾ������Ŀ¼�µ�ָ���ļ�
 *ʵ��:���ݸ�����Ŀ¼temp_dir�������Ƿ������Ϊfile_name���ļ�������ɾ���ļ���
 *         �ͷ����ݿ��inode�ڵ㣬�޸�����Ŀ¼temp_dir�������顢�������������ݿ�λͼ��inodeλͼ��
 *         ����������ʾ������Ϣ
 *temp_dir : Ҫɾ�����ļ����ڵ�Ŀ¼
 *file_name : Ҫɾ�����ļ�������
 */
void delete_file(dir temp_dir, char *file_name)
{
	int i;			//ѭ�����Ʊ���
	int f_inode;	//�ļ���inode
	int temp_pos;	//��temp_dir��file_entry��λ��
	for (i = 2; i < temp_dir.size; i++)
	{
		if (strcmp(temp_dir.file_entry[i].name, file_name) == 0 &&
			inode_table[temp_dir.file_entry[i].inode].i_mode == _FILE)
		{
			f_inode = temp_dir.file_entry[i].inode;
			temp_pos = i;
			break;
		}
	}
	if (i == temp_dir.size)	//û�ҵ��ļ�
	{
		printf("Ŀ¼ %s �в������ļ� %s,�ļ�ɾ��ʧ��!\n", temp_dir.dir_name, file_name);
		return;
	}
	freeBlock(inode_table[f_inode].i_blocks,
		((inode_table[f_inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE));	//�ͷ����ݿ�
	freeInode(f_inode);	//�ͷ��ļ�inode
	//����������Ŀ¼,���������ļ���Ŀ¼��file_entry��ǰ��һλ���ɾ�����ļ��Ŀ�λ
	for (i = temp_pos; i < temp_dir.size; i++)
	{
		temp_dir.file_entry[i].inode = temp_dir.file_entry[i + 1].inode;
		strcpy(temp_dir.file_entry[i].name, temp_dir.file_entry[i + 1].name);
	}
	temp_dir.size--;
	if (temp_dir.inode == _current_dir.inode)
		_current_dir = temp_dir;

	//���Ķ�������д��Ӳ��
	fd = fopen("linux_file_system", "rb+");
	if (fd == NULL)
	{
		printf("Linux file system file creat failed !\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(_block_group, sizeof(block_group), BLOCKS_GROUP_NUM, fd);	//�����������������д��
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + f_inode), SEEK_SET);
	fwrite(inode_bitmap, sizeof(bool), 1, fd);							//��inodeλͼд��
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + (sizeof(bool) + sizeof(inode))*TOTAL_INODES_NUM
		+ (inode_table[f_inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE), SEEK_SET);
	fwrite(block_bitmap, sizeof(bool), inode_table[f_inode].i_blocks, fd);	//�����ݿ�λͼд��
	fseek(fd, inode_table[temp_dir.inode].i_block, SEEK_SET);
	fwrite(&temp_dir, sizeof(dir), 1, fd);									//����Ŀ¼д��
	fclose(fd);
}