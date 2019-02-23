#include "struct.h"

/*
 *����:��Ⲣ�ظ��ļ�ϵͳ
 *ʵ��:���ÿ�����ݿ��������������������ݿ���inode�ļ�¼�Ƿ������
 *	   ��鳬�����е�ϵͳʹ�ü�¼�Ƿ���ʵ�����(�����ݿ�λͼ��i�ڵ�λͼ���Ա�)
 */
void check()
{
	unsigned int f_b_num = 0;	//�������ݿ���Ŀ
	unsigned int f_i_num = 0;	//����inode��Ŀ
	unsigned int total_f_b_num=0;//�ܵĿ������ݿ���Ŀ
	unsigned int total_f_i_num=0;//�ܵĿ���inode��Ŀ

	int i,j;			//ѭ�����Ʊ���
	int start;			//��¼���ݿ�λͼ��inodeλͼ����ʼλ��
	bool change = false;//�����Ƿ����޸Ĺ�
	printf("����ļ�ϵͳ��...\n");

	for(i=0;i<BLOCKS_GROUP_NUM;i++)
	{
		f_b_num = 0;
		f_i_num = 0;
		start = i*BLOCKS_PER_GROUP;
		for(j=0;j<BLOCKS_PER_GROUP;j++)
		{
			if(block_bitmap[start+j] == NOT_USED)	//ͳ��ÿ���ݿ���������ݿ����Ŀ
				f_b_num++;
			if(inode_bitmap[start+j] == NOT_USED)	//ͳ��ÿ���ݿ������inode����Ŀ
				f_i_num++;
		}
		if(_block_group[i].group_desc.bg_free_blocks_count != f_b_num)	//��������޸���������
		{
			_block_group[i].group_desc.bg_free_blocks_count = f_b_num;
			change = true;
		}
		if(_block_group[i].group_desc.bg_free_inodes_count != f_i_num)	//��������޸���������
		{
			_block_group[i].group_desc.bg_free_inodes_count = f_i_num;
			change = true;
		}
		total_f_b_num += _block_group[i].group_desc.bg_free_blocks_count;	//ͳ���ܵĿ������ݿ���
		total_f_i_num += _block_group[i].group_desc.bg_free_inodes_count;	//ͳ���ܵĿ���inode��
	}
	if(_block_group[0].super_block.s_free_blocks_count != total_f_b_num)	//��������޸ĳ�����
	{
		for(i=0;i<BLOCKS_PER_GROUP;i++)
			_block_group[0].super_block.s_free_blocks_count = total_f_b_num;
		change = true;
	}
	if(_block_group[0].super_block.s_free_inodes_count != total_f_i_num)	//��������޸ĳ�����
	{
		for(i=0;i<BLOCKS_PER_GROUP;i++)
			_block_group[0].super_block.s_free_inodes_count = total_f_i_num;
		change = true;
	}
	if(!change)
		printf("ϵͳû���쳣\n");
	else	//���޸�д��
	{
		printf("ϵͳ�����쳣�������޸���...\n");
		fd = fopen("LINUX_FILE_SYSTEM","rb+");
		if(fd == NULL)
		{
			printf("linux_file_system file create failed !\n");
			system("pause");
		}
		fseek(fd,0,SEEK_SET);
		fwrite(&_block_group[0],sizeof(block_group),BLOCKS_GROUP_NUM,fd);	//�����ݿ���д���ļ���
		fwrite(&inode_bitmap[0],sizeof(bool),TOTAL_INODES_NUM,fd);			//��inode λͼд���ļ���
		fwrite(&inode_table[0],sizeof(inode),TOTAL_INODES_NUM,fd);			//��inode ��д���ļ���
		fwrite(&block_bitmap[0],sizeof(bool),TOTAL_BLOCKS_NUM,fd);			//�����ݿ�λͼд���ļ���
		printf("�޸��ɹ�!\n");
	}
}