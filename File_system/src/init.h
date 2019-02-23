#pragma once
#include "struct.h"

void load()
{
	//��ʼ������ 
	strcpy(cmd[0].order, "init");
	strcpy(cmd[1].order, "info");
	strcpy(cmd[2].order, "cd");
	strcpy(cmd[3].order, "dir");
	strcpy(cmd[4].order, "md");
	strcpy(cmd[5].order, "rd");
	strcpy(cmd[6].order, "newfile");
	strcpy(cmd[7].order, "cat");
	strcpy(cmd[8].order, "copy");
	strcpy(cmd[9].order, "del");
	strcpy(cmd[10].order, "check");
	strcpy(cmd[11].order, "exit");
	strcpy(cmd[12].order, "help");

	//��ʼ��·��
	strcpy(current_path, "root>");

	//ע���û�
	_user[0].mode = ADMIN;
	_user[0].uid = 201630599319;
	strcpy(_user[0].password, "599319");

	fd = fopen("LINUX_FILE_SYSTEM", "rb");
	if (fd == NULL)
	{
		fflush(stdin);
		initialize();
		return;
	}

	// �����ļ�ϵͳ�����ݵ��ڴ�
	fread(_block_group, sizeof(block_group), BLOCKS_GROUP_NUM, fd);		//���볬�������������
	fread(inode_bitmap, sizeof(bool), TOTAL_INODES_NUM, fd);				//����inodeλͼ
	fread(inode_table, sizeof(inode), TOTAL_INODES_NUM, fd);				//����inode��
	fread(block_bitmap, sizeof(bool), TOTAL_BLOCKS_NUM, fd);				//�������ݿ�λͼ
	fread(&_current_dir, sizeof(dir), 1, fd);								//�����Ŀ¼

}

void initialize()
{
	printf("���ڽ��д��̻��ֺͳ�ʼ��ϵͳ...\n");
	int i;

	//����ռ�
	long file_size = FIRST_DATA_ADDR + BLOCK_SIZE * TOTAL_BLOCKS_NUM;
	char *buf = new char[file_size];
	//д�����
	fd = fopen("LINUX_FILE_SYSTEM", "wb+");
	if (fd == NULL)
	{
		printf("Linux_file_system create failed !\n");
		system("pause");
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(buf, file_size, 1, fd);
	fclose(fd);
	delete(buf);

	//��ʼ�����ݿ���
	for (i = 0; i < BLOCKS_GROUP_NUM; i++)
	{
		//��ʼ��������
		_block_group[i].super_block.s_inodes_count = TOTAL_INODES_NUM;
		_block_group[i].super_block.s_blocks_count = TOTAL_BLOCKS_NUM;
		_block_group[i].super_block.s_log_block_size = BLOCK_SIZE;
		_block_group[i].super_block.s_free_blocks_count = TOTAL_BLOCKS_NUM - DIR_SIZE;	//һ�����ڸ�Ŀ¼
		_block_group[i].super_block.s_free_inodes_count = TOTAL_INODES_NUM - 1;			//һ�����ڸ�Ŀ¼
		_block_group[i].super_block.s_first_data_block = FIRST_DATA_ADDR;
		_block_group[i].super_block.s_blocks_per_group = BLOCKS_PER_GROUP;
		_block_group[i].super_block.s_inodes_per_group = INODES_PER_GROUP;

		//��ʼ����������
		_block_group[i].group_desc.bg_block_addr = FIRST_DATA_ADDR + i * BLOCKS_PER_GROUP*BLOCK_SIZE;
		_block_group[i].group_desc.bg_block_bitmap = i * BLOCKS_PER_GROUP;
		_block_group[i].group_desc.bg_inode_bitmap = i * INODES_PER_GROUP;
		_block_group[i].group_desc.bg_inode_table = i * INODES_PER_GROUP;
		_block_group[i].group_desc.bg_free_blocks_count = BLOCKS_PER_GROUP;
		_block_group[i].group_desc.bg_free_inodes_count = INODES_PER_GROUP;
	}

	//��ʼ�����ݿ�λͼ,�������ݿ��δ��ʹ��
	for (i = 0; i < TOTAL_BLOCKS_NUM; i++)
		block_bitmap[i] = NOT_USED;

	//��ʼ��inodeλͼ,����inode��δ��ʹ��
	for (i = 0; i < TOTAL_INODES_NUM; i++)
		inode_bitmap[i] = NOT_USED;

	//��ʼ��inode��
	for (i = 0; i < TOTAL_INODES_NUM; i++)
	{
		inode_table[i].i_mode = READ_ONLY;
		inode_table[i].i_uid = 0;
		inode_table[i].i_size = 0;
		inode_table[i].i_blocks = 0;
		inode_table[i].i_block = -1;
		inode_table[i].i_change = false;
	}

	/*���õ�0�����ݿ�Ϊ��Ŀ¼��һϵ����Ϣ*/
	//��ʼ����Ŀ¼�������ݿ������������
	_block_group[0].group_desc.bg_free_blocks_count = BLOCKS_PER_GROUP - DIR_SIZE;	//һ�����ڸ�Ŀ¼
	_block_group[0].group_desc.bg_free_inodes_count = INODES_PER_GROUP - 1;			//һ�����ڸ�Ŀ¼
	//��ʼ����Ŀ¼�����ݿ�λͼ
	for (i = 0; i < DIR_SIZE; i++)
		block_bitmap[i] = USED;
	//��ʼ����Ŀ¼��inode λͼ
	inode_bitmap[0] = USED;

	//��ʼ����Ŀ¼��inode
	inode_table[0].i_mode = _DIRECTORY;			//ģʽΪĿ¼
	inode_table[0].i_uid = 201630599319;				//����Ա��uid
	inode_table[0].i_size = sizeof(dir);		//��Ŀ¼�Ĵ�С
	inode_table[0].i_blocks = DIR_SIZE;			//һ��Ŀ¼����DIR_SIZE�����ݿ�
	inode_table[0].i_block = FIRST_DATA_ADDR;	//��Ŀ¼���ļ�����ʼλ��
	inode_table[0].i_change = false;
	
	//����Ŀ¼�����������
	strcpy(_current_dir.dir_name, "root");			//��Ŀ¼��Ϊroot
	_current_dir.inode = 0;							//��Ŀ¼��inode ��Ϊ0
	_current_dir.size = 2;							//һ����ǰĿ¼��һ����һ��Ŀ¼
	strcpy(_current_dir.file_entry[0].name, ".");	//��ǰĿ¼
	_current_dir.file_entry[0].inode = 0;			//�����Ǹ�Ŀ¼
	strcpy(_current_dir.file_entry[1].name, "..");	//��һ��Ŀ¼
	_current_dir.file_entry[1].inode = 0;			//��Ŀ¼����һ��Ŀ¼�����Լ�

	//�ڴ��е�����д���ļ�
	fd = fopen("LINUX_FILE_SYSTEM", "rb+");
	if (fd == NULL)
	{
		printf("Linux_file_system create failed !\n");
		system("pause");
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(&_block_group[0], sizeof(block_group), BLOCKS_GROUP_NUM, fd);	//�����ݿ���д���ļ���
	fwrite(&inode_bitmap[0], sizeof(bool), TOTAL_INODES_NUM, fd);			//��inode λͼд���ļ���
	fwrite(&inode_table[0], sizeof(inode), TOTAL_INODES_NUM, fd);			//��inode ��д���ļ���
	fwrite(&block_bitmap[0], sizeof(bool), TOTAL_BLOCKS_NUM, fd);			//�����ݿ�λͼд���ļ���
	fwrite(&_current_dir, sizeof(dir), 1, fd);								//�Ѹ�Ŀ¼д���ļ���
	fclose(fd);
}