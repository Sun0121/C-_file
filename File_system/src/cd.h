#pragma once
#include "struct.h"

/*���ܣ��ı䵱ǰ�Ĺ���Ŀ¼
 *
 *ʵ�֣�1����·����ʽΪ" ../Ŀ¼ "�����ȷ�����һ��Ŀ¼���ٴ���
 *		2����·����ʽΪ" ./Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *		3����·����ʽΪ" root/Ŀ¼ "���򷵻ظ�Ŀ¼���ٴ���
 *		4����·����ʽΪ" Ŀ¼/��һ��Ŀ¼ "����ֱ���ڱ���Ŀ¼����
 *
 *path : �û��������·��
 */
void cd(char *path)
{
	dir temp;
	if (getLastDir(path, temp))
		_current_dir = temp;
	else {
		printf("Ŀ¼�л�ʧ��!\n");
		return;
	}
	findFullPath(_current_dir);
}

