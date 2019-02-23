#pragma once
#include "struct.h"
/*
 *����:����inode�ڵ�
 *
 *ʵ��:���i�ڵ�λͼ�Ƿ��п��е�i�ڵ���ã���֮����ϵͳ��Ϣ
 *
 *���ط����i�ڵ��
 */
int getInode()
{
	int b_index;		//���ڼ�¼inode�ڵ����ڵ����ݿ��飬������������ʱ��
	int i_index = -1;	//inode�ڵ�
	int temp, i, j;
	for (i = 0; i < BLOCKS_GROUP_NUM; i++)
	{
		for (j = 0; j < INODES_PER_GROUP; j++)
		{
			temp = (int)_block_group[i].group_desc.bg_inode_bitmap + j;
			if (inode_bitmap[temp] == NOT_USED)
			{
				b_index = i;
				i_index = temp;
				break;
			}
		}
		if (i_index != -1)
			break;
	}
	//����inodeλͼ������������������
	if (i_index != -1)
	{
		for (i = 0; i < BLOCKS_GROUP_NUM; i++)		//���³�����
			_block_group[i].super_block.s_free_inodes_count -= 1;
		_block_group[b_index].group_desc.bg_free_inodes_count -= 1;	//������������
		inode_bitmap[i_index] = USED;		//����inodeλͼ
	}
	return i_index;     // 
}

/*
 *����:�ͷ�inode�ڵ�
 *
 *�㷨����:����Ҫ�ͷŵ�inode�ţ����³����顢����������inodeλͼ
 */
void freeInode(int inode)
{
	int i;		//ѭ�����Ʊ���
	for (i = 0; i < BLOCKS_GROUP_NUM; i++)		//���³�����
		_block_group[i].super_block.s_free_inodes_count += 1;
	_block_group[inode / BLOCKS_PER_GROUP].group_desc.bg_free_inodes_count += 1;//������������(inode / BLOCKS_PER_GROUP ������ݿ����)
	inode_bitmap[inode] = NOT_USED;		//����inodeλͼ
}