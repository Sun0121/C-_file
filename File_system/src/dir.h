#include "struct.h"

/*
 *����:��ʾĿ¼,��ʾ��ǰĿ¼��ָ��Ŀ¼(����·�������·��)
 *
 *ʵ�֣�1����·����ʽΪ" ../Ŀ¼ "�����ȷ�����һ��Ŀ¼���ٴ���
 *		2����·����ʽΪ" ./Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *		3����·����ʽΪ" root/Ŀ¼ "���򷵻ظ�Ŀ¼���ٴ���
 *		4����·����ʽΪ" Ŀ¼/��һ��Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *
 *path : �û������Ŀ¼
 */
void dirDisplay(char *path)
{
	dir temp;
	if (getLastDir(path, temp))
	{
		printf("%8s ��Ŀ¼\n\n", path);
		display(temp);
	}
	else
		printf("Ŀ¼��ʾʧ��!\n");
}

/*
 *����:��ʾָ��Ŀ¼������ļ�����Ŀ¼�Ĳ������
 *
 *temp_dir : ����ʾ��Ŀ¼(���һ��)
 */
void display(dir temp_dir)
{
	printf("%15s%10s%15s  %-37s\n","�����ַ","����","����(�ֽ�)","����");
	for(int i=0;i< temp_dir.size;i++)
	{
		printf("%15ld",inode_table[temp_dir.file_entry[i].inode].i_block);  // �����ַ
		if(inode_table[temp_dir.file_entry[i].inode].i_mode == _DIRECTORY)  // �ļ�����(�ļ� or Ŀ¼)
		{
			printf("%10s","<DIR>");
			printf("%15s","");
		}
		else
		{
			printf("%10s","<FILE>");
			printf("%15u",inode_table[temp_dir.file_entry[i].inode].i_size);  // ��ʾ�ļ���С
		}
		printf("  %-37s\n",temp_dir.file_entry[i].name);
	}
}