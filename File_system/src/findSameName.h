#include "struct.h"
/*
 *����:�жϸ�����Ŀ¼���Ƿ������ͬ���ļ���
 *
 *�㷨����:����Ŀ¼�Ĵ�Сѭ�����ҶԱȣ���ͬ������true�����򷵻�false
 */
bool findSameName(dir temp_dir,char*dirName)
{
	for(int i=2;i<temp_dir.size;i++)
	{
		if(strcmp(temp_dir.file_entry[i].name, dirName)==0)
			return true;
	}
	return false;
}