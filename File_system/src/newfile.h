#pragma once
#include "struct.h"

/*
 *����:�����ļ�
 *
 *ʵ��:ͬ���ȼ��·����Ȼ�����ϵ���ܷ񴴽��ļ����жϣ��ɽ����޸���Ӧ�Ĵ�����Ϣ
 *
 *path : �û������·�� 
 */
void newfile(char *path)
{
	dir temp_dir;
	char dirName[NAME_LEN];
	if (dividePathAndName(path, dirName, temp_dir))
		create_file(temp_dir, dirName);
	else
		printf("�ļ�����ʧ��!\n");
}

/*
*����:�ڸ�����Ŀ¼temp_dir�´����ļ���Ϊfile_name���ļ�
*
*�㷨����:���temp_dir���Ƿ��Ѿ�������Ϊfile_name���ļ���������򴴽�ʧ��
*         ���������ļ����ݣ�ֱ������س����������ݿ��inode���������ɹ����޸ĳ����顢
*         �������������ݿ�λͼ��inodeλͼ���ļ����ڵ�Ŀ¼���������ȣ�������ʾ������Ϣ
*temp_dir: �ڴ�Ŀ¼�½����ļ�
*file_name : �ļ�������
*/
void create_file(dir temp_dir, char *file_name)
{
	//����Ƿ���ͬ���ļ�
	if (findSameName(temp_dir, file_name))
	{
		printf("%s Ŀ¼���Ѵ�����Ϊ %s ���ļ����ļ�����ʧ��!\n", temp_dir.dir_name, file_name);
		return;
	}

	//��ȡ�ļ�����
	char content;		//�ļ������еĵ����ַ�
	int i, size = 5;		//iΪѭ�����Ʊ�����sizeΪ�ļ�����buffer�Ĵ�С�����������sizeʱ��size�ɱ�����
	char *buffer = new char[size];		//���ڴ�ȡ�ļ����ݣ���size���Ӷ�����
	char *temp;			//���ڵ�buffer���ʱ��������ʱ��ȡbuffer������
	int count = 0;		//����ͳ�ƶ�ȡ���ļ����ݵĴ�С

	for (i = 0; i < size; i++)
		buffer[i] = 0;
	printf("�������ļ����ݣ��� (# + Enter) ��������:\n\n");
	while ((content = getchar()) != '#')	//ֱ������س�����
	{
		buffer[count] = content;
		count++;
		if (count >= size - 1)			//���������ݳ�����������Сʱ����������С����
		{
			temp = new char[size];
			strcpy(temp, buffer);	//��ԭ�����������ݱ��浽temp��
			delete(buffer);
			size = size * 2;			//�ļ����ݻ������ɱ�����
			buffer = new char[size];//���·��仺������С
			for (i = 0; i < size; i++)
				buffer[i] = 0;
			strcpy(buffer, temp);	//��ԭ�������ݷŻػ�����
			delete(temp);
		}
	}

	//�����ļ����ݿ�ռ��inode
	int block_num = 0;		//�洢�ļ���Ҫ�����ݿ���Ŀ
	long addr = -1;			//�洢�ļ������ݿ��׵�ַ
	int b_bitmap_index = -1;//�洢�ļ������ݿ������ݿ�λͼ�е��׵�ַ
	int f_inode = -1;

	if ((count + 1) % BLOCK_SIZE == 0)
		block_num = (count + 1) / BLOCK_SIZE;
	else
		block_num = (count + 1) / BLOCK_SIZE + 1;
	addr = getBlock(block_num, &b_bitmap_index);
	if (addr < 0)
	{
		printf("�ļ�̫��Ӳ�̿ռ䲻�����ļ�����ʧ��!\n");
		return;
	}
	f_inode = getInode();

	//�����ļ�inode����
	inode_table[f_inode].i_block = addr;
	inode_table[f_inode].i_blocks = block_num;
	inode_table[f_inode].i_change = false;
	inode_table[f_inode].i_mode = _FILE;
	inode_table[f_inode].i_size = count + 1;
	inode_table[f_inode].i_uid = uid;
	//�����ļ�����Ŀ¼������
	temp_dir.file_entry[temp_dir.size].inode = f_inode;
	strcpy(temp_dir.file_entry[temp_dir.size].name, file_name);
	temp_dir.size++;
	if (temp_dir.inode == _current_dir.inode)
		_current_dir = temp_dir;
	//д��Ӳ��
	fd = fopen("LINUX_FILE_SYSTEM", "rb+");
	if (fd == NULL)
	{
		printf("Linux file system file creat failed !\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(&_block_group[0], sizeof(block_group), BLOCKS_GROUP_NUM, fd);	//�����ݿ���д���ļ�
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*f_inode), SEEK_SET);
	fwrite(&inode_bitmap[f_inode], sizeof(bool), 1, fd);					//��inode λͼд���ļ�
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM
		+ sizeof(inode)*f_inode), SEEK_SET);
	fwrite(&inode_table[f_inode], sizeof(inode), 1, fd);					//��inode ��д���ļ�
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM
		+ sizeof(inode)*TOTAL_INODES_NUM + sizeof(bool)*b_bitmap_index), SEEK_SET);
	fwrite(&block_bitmap[b_bitmap_index], sizeof(bool), block_num, fd);	//���޸ĵ����ݿ�λͼд���ļ�
	fseek(fd, addr, SEEK_SET);
	fwrite(buffer, 1, count, fd);				//���ļ�д��Ӳ��
	fseek(fd, inode_table[temp_dir.inode].i_block, SEEK_SET);
	fwrite(&temp_dir, sizeof(dir), 1, fd);		//�Ѹ�Ŀ¼д���ļ�
	fclose(fd);

	delete(buffer);
}