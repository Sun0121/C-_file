#pragma once
#include "struct.h"
/*
 *����: �������ݿ�ռ�
 *
 *ʵ��: �ҵ�һ�������Ŀռ��ܹ�����������ĳ���
 *
 *len : ����������ݿռ�ĳ���(�̿���Ŀ)
 *b_index : ��������ݿ������ݿ�λͼ����ʼλ��(����ʱ��ֵ)
 * return : ʵ�ʷ���������������ʼ��ַ(�����ַ)
 */
long getBlock(int len, int *b_index) {
	long addr = -1;  // ���ڷ����̿���׵�ַ

	if ((int)_block_group[0].super_block.s_free_blocks_count < len) {   // �����ó������е���Ϣ�жϿ��п���Ŀ
		return addr;
	}
	else
	{
		int count = 0;	//���ڼ��������Ŀ����̿���
		int addr_block_bitmap_index = 0;		//���ڼ�¼��һ������õ����ݿ����ڵ����ݿ�λͼ��λ��
		bool block_group[BLOCKS_GROUP_NUM];	//���ڼ�¼���õ������ݿ��飬�������ݿ���������ʱ�õ�
		int block_group_count[BLOCKS_GROUP_NUM];	//���ڼ�¼���õ������ݿ�������ݿ���Ŀ(ÿ�����ݿ������˼������ݿ�)

		int block_group_index = 0;			//���ڼ�¼�õ��ĵ�һ�����ݿ����λ��

		int i, j;		//ѭ�����Ʊ���

		for (i = 0; i < BLOCKS_GROUP_NUM; i++)		//��ʼ��block_group��block_group_count
		{
			block_group[i] = false;
			block_group_count[i] = 0;
		}
		for (i = 0; i < BLOCKS_GROUP_NUM; i++) // i��ʾ���ڶ���һ�����ݿ�����ж�
		{
			if (count == 0)	//����Ϊ0ʱ��ʾǰһ�����ĩβ�����ݿ�����ã�������������һ��������ݿ�һ��ʹ�ã����Բ��ü��
				if ((int)_block_group[i].group_desc.bg_free_blocks_count < len)
					continue;
			for (j = 0; j < BLOCKS_PER_GROUP; j++)		//����Ƿ������������ݿ������
			{
				// �жϵ�i��ĵ�j�����ݿ���û�б�ʹ��
				if (block_bitmap[(_block_group[i].group_desc.bg_block_bitmap + j)] == NOT_USED)
				{
					count++;
					block_group[i] = true;		//����������������������Ҫ�޸�
					block_group_count[i]++;		//ͳ��������ݿ������˶��ٿ飬������������ʱ��
					if (count == 1)	//��һ�����õ����ݿ�
					{
						addr = _block_group[i].group_desc.bg_block_addr + j * BLOCK_SIZE;	//�����ݿ����ʼ��ַ
						addr_block_bitmap_index = i * BLOCKS_PER_GROUP + j;	//�����ݿ�λͼ��λ�ã��޸����ݿ�λͼʱ��
						*b_index = addr_block_bitmap_index;    // ��b_index��ָ��ĵ�ַ�и�ֵ
						block_group_index = i;	//block_group�����е���ʼλ�ã����ڵ��¸�����������ʱ�ҵ�һ����ʼҪ�޸ĵ�λ��
					}
				}
				else	//�м��в����������ݿ������¿�ʼ��
				{
					count = 0;
					//����Ǹջ�������һ���µ����ݿ��飬��Ҫ���ǰһ�����ݿ����Ƿ���������Ϊʹ��,i-1��Ϊ�˱��⵱�����ݿ����ǵ�һ�����ݿ���ʱ�����±�Ϊ��
					if (j == 0 && (i - 1) >= 0 && block_group[i - 1])
					{	// ����һ���µ����ݿ��鲢��ͷ��ʼ
						block_group[i - 1] = false;
						block_group_count[i - 1] = 0;
					}
					block_group[i] = false;
					block_group_count[i] = 0;
				}
				if (count == len)	//�����Ҫ���̿���������������ѭ������Ҫ���������ѭ�����������������һ�����
					break;
			}
			if (count == len)			//���Ͻ���
				break;
		}
		if (count != len)
		{
			addr = -1;
			return addr;
		}
		//���¾ݿ�λͼ�������顢��������
		for (i = 0; i < BLOCKS_GROUP_NUM; i++)		//���³�����
			_block_group[i].super_block.s_free_blocks_count -= len;
		j = addr_block_bitmap_index + len;
		for (i = addr_block_bitmap_index; i < j; i++)		//�������ݿ�λͼ
			block_bitmap[i] = USED;
		for (i = block_group_index; i < BLOCKS_GROUP_NUM; i++)		//������������
			if (block_group[i])
				_block_group[i].group_desc.bg_free_blocks_count -= block_group_count[i];
	}
	return addr;
}

/*
 *����:���ݸ�������ʼ��ַ�������ݿ���Ŀ���ͷ��ⲿ�����ݿ�
 *
 *len : ���õ����ݿ���Ŀ
 *pos : posΪҪɾ�������ݿ������ݿ�λͼ����ʼλ��
 */
void freeBlock(int len, int pos)
{
	int i;					//ѭ�����Ʊ���
	int blk_end = len + pos;	//�������ݿ�λͼ��ĩβ
	unsigned int blk_group[BLOCKS_GROUP_NUM];	//���ڼ�¼���ݿ�λͼ����Ӧ�����ݿ�������Ҫ���µ����ݿ���Ŀ 
	for (i = 0; i < BLOCKS_GROUP_NUM; i++)	//���³�����
	{
		_block_group[i].super_block.s_free_blocks_count += len;
		blk_group[i] = 0;	//��ʼ��blk_group
	}
	for (i = pos; i < blk_end; i++)			//�������ݿ�λͼ
	{
		block_bitmap[i] = NOT_USED;
		blk_group[i / BLOCKS_PER_GROUP]++;	//(i/BLOCKS_PER_GROUP)Ϊ��i�����ݿ�����Ӧ�����ݿ���
	}
	for (i = pos / BLOCKS_PER_GROUP; i < BLOCKS_PER_GROUP; i++)	//������������
	{
		if (blk_group[i] != 0)	//��Ϊ���ʾ�����ݿ��������ݿ�Ҫ�ͷ�
			_block_group[i].group_desc.bg_free_blocks_count += blk_group[i];
		else	//Ϊ���ʾ�����ݿ�Ҫ�ͷţ���Ϊ�����ݿ�����������ģ����Ժ�������ݿ���Ҳû�����ݿ��ͷ�
			break;
	}
}