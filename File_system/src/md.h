#pragma once
#include "struct.h"


/*���ܣ�����Ŀ¼
 *
 *ʵ�֣��ȷֽ��������·�����������Ŀ¼��
 *		1����·����ʽΪ" ../Ŀ¼ "�����ȷ�����һ��Ŀ¼���ٴ���
 *		2����·����ʽΪ" ./Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *		3����·����ʽΪ" root/Ŀ¼ "���򷵻ظ�Ŀ¼���ٴ���
 *		4����·����ʽΪ" Ŀ¼/��һ��Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *
 *path : �û��������·��
 */
void md(char *path) {
	
	dir finalDir;
	char dirName[NAME_LEN];
	if (dividePathAndName(path, dirName, finalDir)) {
		makeDir(finalDir, dirName);
	}else 
		printf("Ŀ¼����ʧ��!\n");
}

/*
 *����: ����ָ���ľ���Ŀ¼temp_dir�������д�����Ŀ¼dir_name,��md��������
 *
 *temp_dir ��������Ŀ¼�ĸ�Ŀ¼
 *dir_name : ��������Ŀ¼��
 */
void makeDir(dir temp_dir, char *dirName) {

	long addr = -1;   // ����Ŀ¼�����������׵�ַ
	int _inode = -1;  // ����Ŀ¼��i�ڵ��
	int b_index;      // ��������ݿ������ݿ�λͼ����ʼλ��

	if (findSameName(temp_dir, dirName))
	{
		printf("%s Ŀ¼���Ѵ�����Ϊ %s ���ļ����ļ�����ʧ��!\n", temp_dir.dir_name, dirName);
		return;
	}
	// ����ռ�
	addr = getBlock(DIR_SIZE, &b_index);
	if (addr < 0) {
		printf("Ӳ�̿ռ䲻����Ŀ¼����ʧ��!\n");
		return;
	}
	else {
		_inode = getInode();       //  i�ڵ����
		if (_inode < 0) {
			printf("i�ڵ����ʧ�ܣ�Ŀ¼����ʧ��!\n");
			return;
		}

		// ��������ʼ��Ŀ¼(��������Ŀ¼'.'�͸���Ŀ¼'..')
		dir make_dir;
		strcpy(make_dir.dir_name, dirName);
		make_dir.inode = (unsigned int)_inode;
		make_dir.size = 2;
		make_dir.file_entry[0].inode = _inode;
		strcpy(make_dir.file_entry[0].name,".");
		make_dir.file_entry[1].inode = temp_dir.inode;
		strcpy(make_dir.file_entry[1].name, "..");
		// �޸ı�Ŀ¼��Ӧ��inode
		inode_table[_inode].i_block = addr;
		inode_table[_inode].i_blocks = DIR_SIZE;
		inode_table[_inode].i_change = false;
		inode_table[_inode].i_mode = _DIRECTORY;
		inode_table[_inode].i_size = sizeof(dir);
		inode_table[_inode].i_uid = uid;
		// �޸��丸Ŀ¼
		temp_dir.file_entry[temp_dir.size].inode = (unsigned int)_inode;
		strcpy(temp_dir.file_entry[temp_dir.size].name, dirName);
		temp_dir.size++;

		if (temp_dir.inode == _current_dir.inode)	//������ڵ�ǰĿ¼�´�����Ҫ���µ�ǰĿ¼
			_current_dir = temp_dir;

		// ���޸�д��Ӳ��
		fd = fopen("LINUX_FILE_SYSTEM", "rb+");
		if (fd == NULL)
		{
			printf("Linux file system file creat failed !\n");
			exit(0);
		}
		fseek(fd, 0, SEEK_SET);
		fwrite(&_block_group[0], sizeof(block_group), BLOCKS_GROUP_NUM, fd); // �����ݿ���д���ļ�
		fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*_inode), SEEK_SET);
		fwrite(&inode_bitmap[_inode], sizeof(bool), 1, fd);		//�Ѷ�Ӧ��inodeλͼ�е���һλд���ļ�
		fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM + sizeof(inode)*_inode), SEEK_SET);
		fwrite(&inode_table[_inode], sizeof(inode), 1, fd);		//�Ѷ�Ӧ��inode���е���һ��inodeд���ļ�
		fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM
			+ sizeof(inode)*TOTAL_INODES_NUM + sizeof(bool)*b_index), SEEK_SET);
		fwrite(&block_bitmap[b_index], sizeof(bool), DIR_SIZE, fd);        // ���޸ĵ����ݿ�λͼд���ļ�
		fseek(fd, addr, SEEK_SET);
		fwrite(&make_dir, sizeof(dir), 1, fd);		//�Ѵ�����Ŀ¼д���ļ�
		fseek(fd, inode_table[temp_dir.inode].i_block, SEEK_SET);
		fwrite(&temp_dir, sizeof(dir), 1, fd);		//�Ѹ�Ŀ¼д���ļ�
		fclose(fd);
	}
}