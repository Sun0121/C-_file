#include "struct.h"

/*
 *���ܣ��˳�ϵͳ
 */
void exit()
{
	//д���ļ�
	fd = fopen("LINUX_FILE_SYSTEM","rb+");
	if(fd == NULL)
	{
		printf("linux_file_system file creat failed !\n");
		exit(0);
	}
	fseek(fd,0,SEEK_SET);
	fwrite(&_block_group[0],sizeof(block_group),BLOCKS_GROUP_NUM,fd);	//�����ݿ���д���ļ���
	fwrite(&inode_bitmap[0],sizeof(bool),TOTAL_INODES_NUM,fd);			//��inode λͼд���ļ���
	fwrite(&inode_table[0],sizeof(inode),TOTAL_INODES_NUM,fd);			//��inode ��д���ļ���
	fwrite(&block_bitmap[0],sizeof(bool),TOTAL_BLOCKS_NUM,fd);			//�����ݿ�λͼд���ļ���
	fseek(fd,inode_table[_current_dir.inode].i_block,SEEK_SET);
	fwrite(&_current_dir,sizeof(dir),1,fd);								//�ѵ�ǰĿ¼д���ļ�
	fclose(fd);
	exit(0);
}