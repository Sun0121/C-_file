#include "struct.h"

/*
 *����:�����ļ�
 *
 *�㷨����:���ȼ�����������·���Ƿ���ȷ��
 *         �����ȷ���ҵ���Ӧ���ļ����������ݵ����ݻ������У��ٽ����ݻ�����������д��Ŀ��·����
 *         ������ʾ������Ϣ
 *src_path : Դ�ļ�����·��
 *des_path �� �����ļ�����·����
 */
void copy_file(char*src_path,char*des_path)
{
	dir temp_dir;				//����Ŀ��Ŀ¼
	char file_name[NAME_LEN];	//�ļ���
	char *buf;					//�ļ����ݻ�����
	long len = 0;				//�ļ�����
	char divide;				//����ָ���

	if(isHostFile(src_path))		//��host�ļ�ϵͳ������ģ��Linux�ļ�ϵͳ
	{
		fd = fopen(src_path,"rb");
		if(fd == NULL)
		{
			printf("�ļ�������,�ļ�����ʧ��!\n");
			return;
		}
		fseek(fd,0,SEEK_END);
		len = ftell(fd);           // ��ȡfdָ��ĵ�ǰλ��
		buf = new char[len];	//�����ļ����ݻ�����
		buf[len-1] = 0;
		fseek(fd,0,SEEK_SET);
		fread(buf,len-1,1,fd);	//��ȡ�ļ�������
		fclose(fd);

		divide = '\\';	//windows·���ָ���

		strcpy(file_name,strrchr(src_path,divide) +1);			//����ļ���

		if(getLastDir(des_path,temp_dir))	//�ҵ�Ŀ��Ŀ¼
		{
			write_back(temp_dir,file_name,buf,len);
			return;
		}
		else
			printf("�ļ�����ʧ��!\n");
	}
	else
	{
		if(isHostFile(des_path))		//��ģ��Linux�ļ�ϵͳ������host�ļ�ϵͳ
		{
			if(dividePathAndName(src_path,file_name,temp_dir))
			{
				int inode;              // Դ�ļ���i�ڵ��
				for(int i=2;i<temp_dir.size;i++)
					if(strcmp(temp_dir.file_entry[i].name,file_name)==0)
						inode = temp_dir.file_entry[i].inode;

				buf = new char[inode_table[inode].i_size-1];	//���仺����

				len = read_file(temp_dir,file_name,inode,buf);

				char *complete_path = new char[MAX_PATH_LEN];	//Windows�е�����·��
				sprintf(complete_path,"%s\\%s",des_path,file_name);
				fd = fopen(complete_path,"wb+");
				if(fd == NULL)
				{
					printf("�ļ�����ʧ��!\n");
					delete(buf);
					delete(complete_path);
					return;
				}
				fwrite(buf,len-1,1,fd);
				fclose(fd);
				delete(complete_path);
				delete(buf);
				return;
			}
			printf("�ļ�����ʧ��!\n");
		}
		else	//Linux�ļ�ϵͳ�ڲ�����
		{
			if(dividePathAndName(src_path,file_name, temp_dir))
			{
				int f_inode;
				for(int i=2;i<temp_dir.size;i++)
				{
					if(strcmp(temp_dir.file_entry[i].name,file_name)==0 && inode_table[temp_dir.file_entry[i].inode].i_mode == _FILE)
					{
						f_inode = temp_dir.file_entry[i].inode;
						break;
					}
				}
				buf = new char[inode_table[f_inode].i_size];	//���仺����

				len = read_file(temp_dir,file_name,f_inode,buf);

				char *complete_path = new char[MAX_PATH_LEN];	//Linux�е�����·��
				sprintf(complete_path,"%s/%s",des_path,file_name);
				if(dividePathAndName(complete_path,file_name, temp_dir))
					write_back(temp_dir,file_name,buf,len);
				else
					printf("�ļ�����ʧ��!\n");
				delete(complete_path);
				delete(buf);
			}
		}
	}
}

/*
 *����:���ݸ���Ŀ¼temp_dir�������ļ�file_name����ȡ���ݵ�buf��
 *
 *temp_dir : ����Ŀ¼
 *file_name : �ļ���
 *inode : file_name��i�ڵ��
 *buf : �����ļ����ݵĻ�����
 */
long read_file(dir temp_dir,char *file_name,int inode,char *buf)
{
	fd = fopen("LINUX_FILE_SYSTEM","rb");
	if (fd == NULL)
	{
		printf("Linux_file_system create failed !\n");
		system("pause");
	}
	fseek(fd,inode_table[inode].i_block,SEEK_SET);
	fread(buf,inode_table[inode].i_size-1,1,fd);
	fclose(fd);
	buf[inode_table[inode].i_size-1] = 0;
	return inode_table[inode].i_size;
}

/*
 *����:���������ļ����²�д��Ӳ��
 * 
 *ʵ�֣��ȼ��Ŀ��Ŀ¼���Ƿ����ͬ���ļ���Ȼ�����ϵͳ��Ϣ
 *
 *temp : �������ļ����ڵ�Ŀ¼
 *file_name:���������ļ���
 *buf : �м仺�棬�洢�ļ�����
 *len : �ļ��ĳ���(�ֽ���)
 */
void write_back(dir temp,char *file_name,char *buf,int len)
{
	int b_index = -1;		//���ļ���������ݿ������ݿ�λͼ�е�λ��
	long addr = -1;			//���ļ���������ݿ��׵�ַ
	int blocks_num = 0;		//���ļ���ռ�����ݿ���Ŀ
	int f_inode = -1;		//���ļ���inode

	blocks_num = len/BLOCK_SIZE;	//�����ļ���ռ�����ݿ���Ŀ
	if(len % BLOCK_SIZE != 0)
		blocks_num += 1;

	if (findSameName(temp, file_name))
	{
		printf("%s Ŀ¼���Ѵ�����Ϊ %s ���ļ�������ʧ��!\n", temp.dir_name, file_name);
		return;
	}

	addr = getBlock(blocks_num,&b_index);	//Ϊ�ļ��������ݿ�ռ�		
	if(addr<0)
	{
		printf("�ļ�̫��Ӳ�̿ռ䲻�����ļ�����ʧ��!\n");
		return;
	}
	f_inode = getInode();						//Ϊ�ļ�����inode
	//�����ļ�inode
	inode_table[f_inode].i_block = addr;
	inode_table[f_inode].i_blocks = blocks_num;
	inode_table[f_inode].i_change = false;
	inode_table[f_inode].i_mode = _FILE;
	inode_table[f_inode].i_size = len ;
	inode_table[f_inode].i_uid = uid;
	//����������Ŀ¼
	temp.file_entry[temp.size].inode = f_inode;
	strcpy(temp.file_entry[temp.size].name,file_name);
	temp.size++;
	if(temp.inode == _current_dir.inode)
		_current_dir = temp;
	//д��Ӳ��
	fd = fopen("LINUX_FILE_SYSTEM","rb+");
	if (fd == NULL)
	{
		printf("Linux_file_system create failed !\n");
		system("pause");
	}
	fseek(fd,0,SEEK_SET);
	fwrite(&_block_group[0],sizeof(block_group),BLOCKS_GROUP_NUM,fd);	//�����ݿ���д���ļ�
	fseek(fd,(sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*f_inode),SEEK_SET);
	fwrite(&inode_bitmap[f_inode],sizeof(bool),1,fd);					//��inode λͼд���ļ�
	fseek(fd,(sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM
		+ sizeof(inode)*f_inode),SEEK_SET);
	fwrite(&inode_table[f_inode],sizeof(inode),1,fd);					//��inode ��д���ļ�
	fseek(fd,(sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*TOTAL_INODES_NUM
		+ sizeof(inode)*TOTAL_INODES_NUM + sizeof(bool)*b_index),SEEK_SET);
	fwrite(&block_bitmap[b_index],sizeof(bool),blocks_num,fd);	//���޸ĵ����ݿ�λͼд���ļ�
	fseek(fd,addr,SEEK_SET);
	fwrite(buf,1,len,fd);				//���ļ�д��Ӳ��
	fseek(fd,inode_table[temp.inode].i_block,SEEK_SET);
	fwrite(&temp,sizeof(dir),1,fd);		//�Ѹ�Ŀ¼д���ļ�
	fclose(fd);
}

/*
 *����:����Ƿ��host�ļ�ϵͳ�п���
 *
 *�㷨����:���·���ĵ�һ���ֶ��Ƿ�Ϊ<host>,�����·����path����ȡ��������������path��
 *         ���򷵻�false
 */
bool isHostFile(char*path)
{
	char host[] = "<host>";
	char *divide;
	divide = strstr(path, host);
	if (divide == path)
	{
		strcpy(path, divide + sizeof(host) - 1); // ��ֵ��Դ��ַ��ʼ��������'\0'����
		return true;
	}
	return false;
}