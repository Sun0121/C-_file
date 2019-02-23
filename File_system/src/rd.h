#pragma once
#include "struct.h"

/*
 *����:ɾ��Ŀ¼
 *
 *ʵ��:�ȷֽ������Ҫɾ����Ŀ¼����·����Ȼ�������ʾ��Ϣ����ɾ��
 *
 *path:�û������·��
 *
 */
void rd(char *path)
{
	dir temp_dir;
	char dirName[NAME_LEN];
	if (dividePathAndName(path, dirName, temp_dir))
		remove_dir(temp_dir, dirName);
	else
		printf("Ŀ¼ɾ��ʧ��!\n");
}

/*
 *����:�ڸ�����Ŀ¼temp��ɾ��Ŀ¼��Ϊrd_name��Ŀ¼
 *
 *
 *ʵ��: �Ȳ���temp_dir���Ƿ������Ϊrd_name��Ŀ¼��Ȼ���ж�Ҫɾ����Ŀ¼�Ƿ��ǵ�ǰĿ¼_current_dir
 *		����_current_dir�����ȼ�Ŀ¼��Ȼ����rd_name�Ƿ�Ϊ�գ�����ֱ��ɾ����������ʾ��Ϣ��
 *      ��Ҫɾ����ݹ�ɾ��rd_name�µ��ļ�����Ŀ¼�������¸�Ŀ¼�������顢����������inodeλͼ�����ݿ�λͼ��
 *temp : �����ľ���Ŀ¼
 *rd_name : Ҫɾ����Ŀ¼
 *
 */
void remove_dir(dir temp, char *rd_name)
{
	int i;			//ѭ�����Ʊ���
	int temp_pos = 0;	//��¼�ҵ���Ŀ¼��temp�е�λ��
	int d_inode;	//Ҫɾ����Ŀ¼��inode��
	for (i = 2; i < temp.size; i++)
	{
		if (strcmp(temp.file_entry[i].name, rd_name) == 0 &&
			inode_table[temp.file_entry[i].inode].i_mode == _DIRECTORY)	//�ҵ���Ŀ¼��������inode������ѭ��
		{
			temp_pos = i;
			d_inode = temp.file_entry[i].inode;
			break;
		}
	}
	if (i == temp.size) {
		printf("��Ŀ¼�����ڣ�Ŀ¼ɾ��ʧ��!\n");
		return;
	}
	else	//�ҵ���Ŀ¼,����ɾ������
	{
		//��Ӳ���ж���Ҫɾ����Ŀ¼
		dir r_dir;		//Ҫɾ����Ŀ¼
		fd = fopen("LINUX_FILE_SYSTEM", "rb");
		if (fd == NULL)
		{
			printf("Linux_file_system create failed !\n");
			system("pause");
		}
		fseek(fd, inode_table[d_inode].i_block, SEEK_SET);
		fread(&r_dir, sizeof(dir), 1, fd);
		fclose(fd);

		if (!isDirRemove(r_dir,_current_dir))	//����ǵ�ǰĿ¼����ɾ��ʧ��
		{
			printf("��һ����������ʹ�ô�Ŀ¼�������޷�����\n");
			printf("Ŀ¼ɾ��ʧ��!\n");
			return;
		}

		if (r_dir.size > 2)//��Ŀ¼�����ļ���Ŀ¼
		{
			char select = 's';	//��ʼ��Ϊs��������
			fflush(stdin);		//�����
			do
			{
				if (select != '\n')
					printf("%s ��Ŀ¼�����ļ����Ƿ�Ҫɾ��(y/n)\n", r_dir.dir_name);
				select = getchar();
				if (select == 'y' || select == 'Y' || select == 'n' || select == 'N')
				{
					fflush(stdin);		//�����
					break;
				}
			} while (1);
			if (select == 'y' || select == 'Y')
			{
				remove_dir(temp, r_dir, temp_pos);	//�ݹ�ɾ����Ŀ¼���ļ�
				return;
			}
			else
			{
				printf("Ŀ¼ɾ��ʧ��!\n");
				return;
			}
		}
		else			//��Ŀ¼Ϊ�գ�ֱ��ɾ��
		{
			//(inode_table[d_inode].i_block-FIRST_DATA_ADDR)/BLOCK_SIZEΪr_dir���ݿ������ݿ�λͼ�е���ʼλ��
			freeBlock(inode_table[d_inode].i_blocks,
				((inode_table[d_inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE));	//�ͷ����ݿ�
			freeInode(d_inode);		//�ͷ�inode�ڵ�
			//�����丸Ŀ¼,���������ļ���Ŀ¼��file_entry��ǰ��һλ���ɾ����Ŀ¼�Ŀ�λ
			for (i = temp_pos; i < temp.size; i++)
			{
				temp.file_entry[i].inode = temp.file_entry[i + 1].inode;
				strcpy(temp.file_entry[i].name, temp.file_entry[i + 1].name);
			}
			temp.size--;
			if (temp.inode == _current_dir.inode)
				_current_dir = temp;

			//���Ķ�������д��Ӳ��
			fd = fopen("LINUX_FILE_SYSTEM", "rb+");
			if (fd == NULL)
			{
				printf("Linux file system file creat failed !\n");
				exit(0);
			}
			fseek(fd, 0, SEEK_SET);
			fwrite(_block_group, sizeof(block_group), BLOCKS_GROUP_NUM, fd);	//�����������������д��
			fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + d_inode), SEEK_SET);
			fwrite(inode_bitmap, sizeof(bool), 1, fd);							//��inodeλͼд��
			fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + (sizeof(bool) + sizeof(inode))*TOTAL_INODES_NUM
				+ (inode_table[d_inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE), SEEK_SET);
			fwrite(block_bitmap, sizeof(bool), inode_table[d_inode].i_blocks, fd);	//�����ݿ�λͼд��
			fseek(fd, inode_table[temp.inode].i_block, SEEK_SET);
			fwrite(&temp, sizeof(dir), 1, fd);									//����Ŀ¼д��
			fclose(fd);
		}
	}
}

/*
 *���ܣ��ж�Ҫɾ����Ŀ¼�Ƿ��ǵ�ǰ���ڹ�����Ŀ¼��ǰ���ڹ���Ŀ¼�����ȼ�Ŀ¼
 *
 *ʵ�֣��ݹ����ϼ����ң����Ƿ��е�ǰҪ��ɾ��Ŀ¼�����ȼ�Ŀ¼
 *
 */
bool isDirRemove(dir rdDir,dir currentDir) {
	if (rdDir.inode == 0)
		return false;                 // ��Ŀ¼����ɾ��
	if (currentDir.inode != 0)        // 0�Ǹ�Ŀ¼��i�ڵ��
	{
		if (currentDir.inode == rdDir.inode)
			return false;                     // ����ɾ��
		else {
			dir fatherDir;
			fd = fopen("LINUX_FILE_SYSTEM", "rb");
			if (fd == NULL)
			{
				printf("Linux_file_system create failed !\n");
				system("pause");
			}
			fseek(fd, inode_table[currentDir.file_entry[1].inode].i_block, SEEK_SET);
			fread(&fatherDir, sizeof(dir), 1, fd);
			fclose(fd);
			return isDirRemove(rdDir, fatherDir);
		}
	}
	return true;
}


/*
 *����:�ݹ�ɾ��Ŀ¼�����µ��ļ�
 *
 *father_dir : ��ɾ��Ŀ¼�ĸ���Ŀ¼
 *r_dir : ��ǰҪ��ɾ����Ŀ¼
 *index : ��ɾ����Ŀ¼�ڸ���Ŀ¼�е�λ��
 */
void remove_dir(dir father_dir, dir r_dir, int index)
{
	int i;		//ѭ�����Ʊ���
	for (i = 2; i < r_dir.size; i++)
	{
		if (inode_table[r_dir.file_entry[i].inode].i_mode == _DIRECTORY)
		{
			//��Ӳ�̶�������Ŀ¼
			dir sub_dir;	//Ҫɾ��Ŀ¼����Ŀ¼
			fd = fopen("LINUX_FILE_SYSTEM", "rb");
			if (fd == NULL)
			{
				printf("Linux file system file creat failed !\n");
				exit(0);
			}
			fseek(fd, inode_table[r_dir.file_entry[i].inode].i_block, SEEK_SET);
			fread(&sub_dir, sizeof(dir), 1, fd);
			fclose(fd);

			remove_dir(r_dir, sub_dir, i);//�ݹ�ɾ����Ŀ¼
		}
		else
			delete_file(r_dir, r_dir.file_entry[i].name);		//ɾ���ļ�
	}
	freeBlock(inode_table[r_dir.inode].i_blocks,
		((inode_table[r_dir.inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE));	//�ͷ����ݿ�
	freeInode(r_dir.inode);				//�ͷ�inode�ڵ�
	//�����丸Ŀ¼,���������ļ���Ŀ¼��file_entry��ǰ��һλ���ɾ����Ŀ¼�Ŀ�λ
	for (i = index; i < father_dir.size; i++)
	{
		father_dir.file_entry[i].inode = father_dir.file_entry[i + 1].inode;
		strcpy(father_dir.file_entry[i].name, father_dir.file_entry[i + 1].name);
	}
	father_dir.size--;
	if (father_dir.inode == _current_dir.inode)
		_current_dir = father_dir;

	//���Ķ�������д��Ӳ��
	fd = fopen("LINUX_FILE_SYSTEM", "rb+");
	if (fd == NULL)
	{
		printf("Linux file system file creat failed !\n");
		exit(0);
	}
	fseek(fd, 0, SEEK_SET);
	fwrite(_block_group, sizeof(block_group), BLOCKS_GROUP_NUM, fd);	//�����������������д��
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + r_dir.inode), SEEK_SET);
	fwrite(inode_bitmap, sizeof(bool), 1, fd);							//��inodeλͼд��
	fseek(fd, (sizeof(block_group)*BLOCKS_GROUP_NUM + (sizeof(bool) + sizeof(inode))*TOTAL_INODES_NUM
		+ (inode_table[r_dir.inode].i_block - FIRST_DATA_ADDR) / BLOCK_SIZE), SEEK_SET);
	fwrite(block_bitmap, sizeof(bool), inode_table[r_dir.inode].i_blocks, fd);	//�����ݿ�λͼд��
	fseek(fd, inode_table[father_dir.inode].i_block, SEEK_SET);
	fwrite(&father_dir, sizeof(dir), 1, fd);									//����Ŀ¼д��
	fclose(fd);
}
