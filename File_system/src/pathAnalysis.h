#pragma once
#include"struct.h"


/*���ܣ����������յ��Ǹ�·��("root/Ŀ¼/��һ��Ŀ¼"ͨ���˺��������õ�"��һ��Ŀ¼")
 *
 *path : ��������·��
 *temp_dir �� �����Ϊ����ֵ������������Ŀ��·����ֵ��temp_dir
 */
bool getLastDir(char *path, dir &temp_dir)
{
	temp_dir = _current_dir;
	if (strcmp(path, ".") == 0) {                  // ��ǰĿ¼
		return true;

	}else if (strcmp(path, "..") == 0)            //  ��һ��Ŀ¼
	{
		fd = fopen("LINUX_FILE_SYSTEM", "rb");
		if (fd == NULL)
		{
			printf("Linux_file_system create failed !\n");
			system("pause");
		}
		fseek(fd, inode_table[temp_dir.file_entry[1].inode].i_block, SEEK_SET);
		fread(&temp_dir, sizeof(dir), 1, fd);
		fclose(fd);
		return true;
	}
	else {
		int pos = 1;           // ָ��·���еĵڼ���Ŀ¼
		char dirName[NAME_LEN];   // ����ֽ����Ŀ¼��
		getDirName(path, pos, dirName);     // ����ĳһ�㼶��Ŀ¼��
		fd = fopen("LINUX_FILE_SYSTEM", "rb");
		if (fd == NULL)
		{
			printf("Linux_file_system create failed !\n");
			system("pause");
		}

		if (strcmp(dirName, "root") == 0)
		{
			fseek(fd, inode_table[0].i_block, SEEK_SET);
			fread(&temp_dir, sizeof(dir), 1, fd);		//����ת����Ŀ¼
			pos++;
			if (findFinalDir(path, pos, dirName, temp_dir)) {
				fclose(fd);
				return true;
			}
			else {
				fclose(fd);
				return false;
			}
		}
		else if (strcmp(dirName, "..") == 0) {
			fseek(fd, inode_table[temp_dir.file_entry[1].inode].i_block, SEEK_SET);
			fread(&temp_dir, sizeof(dir), 1, fd);         // ��ת����һ��Ŀ¼
			pos++;
			if (findFinalDir(path, pos, dirName, temp_dir)) {
				fclose(fd);
				return true;
			}
			else {
				fclose(fd);
				return false;
			}
		}
		else if (strcmp(dirName, ".") == 0) {
			pos++;
			if (findFinalDir(path, pos, dirName, temp_dir)) {
				fclose(fd);
				return true;
			}
			else {
				fclose(fd);
				return false;
			}
		}
		else {
			if (findFinalDir(path, pos, dirName, temp_dir)) {
				fclose(fd);
				return true;
			}
			else {
				fclose(fd);
				return false;
			}
		}
	}
}

/*
 *����:���ҳ�������·�� (������� "/��һ��Ŀ¼" ���ҳ� "root/Ŀ¼/��һ��Ŀ¼ ")
 *
 *�㷨����:�ݹ������һ��Ŀ¼��ֱ���ҵ���Ŀ¼Ϊֹ
 *
 *temp_dir : ��Ҫ�����ҵ�Ŀ¼
 */
void findFullPath(dir temp_dir) {
	dir temp = temp_dir;
	 
	if (temp_dir.inode != 0) {          // 0Ϊ��Ŀ¼��i�ڵ�
		fd = fopen("LINUX_FILE_SYSTEM", "rb");
		if (fd == NULL)
		{
			printf("Linux_file_system create failed !\n");
			system("pause");
		}
		fseek(fd, inode_table[temp_dir.file_entry[1].inode].i_block, SEEK_SET);
		fread(&temp_dir, sizeof(dir), 1, fd);
		fclose(fd);
		findFullPath(temp_dir);
	}
	if (temp.inode == 0) {
		strcpy(current_path, temp.dir_name);
		if(temp.inode == _current_dir.inode)  // ��ʱ_current_dir��Ŀ��Ŀ¼
			sprintf(current_path, "%s>", current_path);
	}
	else
	{
		sprintf(current_path, "%s/%s", current_path, temp.dir_name);
		if (temp.inode == _current_dir.inode)
			sprintf(current_path, "%s>", current_path);
	}
}

/*
 *���ܣ�����pos����ĳһ�㼶��Ŀ¼��(����"root/һ��/����" ��pos = 1,���ҵ�root,��posΪ2,���ҵ�һ�㣬����...)
 *
 *�㷨������strtok���������ַ����ָ�
 *
 *path : ���ֽ��·��
 *pos :  �ֽ⼶��
 *dirName : �����Ϊ����ֵ�������ҵ��Ľ����ֵ��dirName
 */
bool getDirName(char *path, int pos, char *dirName) {
	char *pathCopy = new char[MAX_PATH_LEN];
	strcpy(pathCopy, path);

	char dir_name[NAME_LEN];

	char *singleDir = strtok(pathCopy, "/");
	strcpy(dir_name, singleDir);

	singleDir = strtok(NULL, "/");
	for (int i = 1; i < pos; ++i) {
		if (singleDir) {
			
			strcpy(dir_name, singleDir);
			singleDir = strtok(NULL, "/");
		}
		else {
			return false;
		}
	}
	strcpy(dirName, dir_name);
	return true;
}

/*
 *���ܣ����ϲ���·���и�����pos��Ŀ¼����֤����ȷ�ԣ�ֱ�����ҵ����һ��,������Ŀ¼
 *
 *path : ������·��
 *pos : ��־��ǰ���ڵڼ���Ŀ¼
 *dirName : ���ҵ�pos�����Ŀ¼����ֵ��dirName
 *temp_dir �� ���ҵ���·���е����һ��Ŀ¼��ֵ��temp_dir
 */
bool findFinalDir(char *path, int &pos, char *dirName, dir &temp_dir) {
	while (getDirName(path, pos, dirName)) {
		int temp_dir_size = temp_dir.size;         // ��ʱtemp_dir��dirName�ĸ�Ŀ¼
		int i;
		for (i = 2; i < temp_dir_size; ++i) {     // temp_dir�ĵ�һ��Ŀ¼�ֱ���.��..
			if (strcmp(dirName, temp_dir.file_entry[i].name) == 0 &&
				inode_table[temp_dir.file_entry[i].inode].i_mode == _DIRECTORY) {
				fseek(fd, inode_table[temp_dir.file_entry[i].inode].i_block, SEEK_SET);
				fread(&temp_dir, sizeof(dir), 1, fd);		//temp_dir�Ѿ��ı�!!!
				break;
			}
		}
		if (i < temp_dir_size) {
			pos++;
		}
		else {
			printf("������·������!\n");
			return false;
		}
	}
	return true;
}

/*
 *���ܣ������·����Ŀ¼(���� "root/a/b/c" �����"root>a>b" �� "c" ,c�Ǵ�������Ŀ¼���ļ�)
 *
 *ʵ�֣�����strrchr����
 *
 *path : ���ֽ��·��
 *dirName : �ֽ�õ������ֽ����ֵ��dirName
 *temp_dir���ֽ�õ���Ŀ¼�����ֵ��temp_dir
 */
bool dividePathAndName(char *path,char *dirName, dir &temp_dir) {
	char *str;
	temp_dir = _current_dir;

	char *copyPath = new char[MAX_PATH_LEN];
	strcpy(copyPath, path);

	str = strrchr(path, '/');
	if (str) {
		//����������·����Ŀ¼��
		int divide_pos = str - path;	//���һ�� '/' ���ڵ�λ��
		char *dividePath = new char[divide_pos + 1];			//Ҫ�򿪡�ɾ���򴴽����ļ���Ŀ¼���ڵ�·��
		int i;		//ѭ������
		for (i = divide_pos + 1; i < strlen(copyPath) + 1; ++i)				//�ֽ��Ҫ�������ļ���
			dirName[i - (divide_pos + 1)] = copyPath[i];
		dirName[i] = 0;
		for (i = 0; i < divide_pos; i++)						//�ֽ��Ҫ�򿪡�ɾ���򴴽����ļ���Ŀ¼���ڵ�·��
			dividePath[i] = copyPath[i];
		dividePath[i] = 0;
		
		if (!getLastDir(dividePath, temp_dir))
			return false;

		delete(dividePath);

		return true;
	}
	else {
		strcpy(dirName, path);
		return true;
	}
}