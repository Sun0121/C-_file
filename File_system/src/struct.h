#pragma once
#ifndef _STRUCT_H                 // ��ֹͷ�ļ��ظ�����
#define _STRUCT_H

#define MAX_PATH_LEN        1000    // ���·������

/*
 * �ļ�ϵͳ�Ļ�����Ϣ
 */
#define BLOCK_SIZE			1024	//�̿��СΪ 1KB
#define BLOCKS_GROUP_NUM	100		//���ݿ������Ŀ
#define BLOCKS_PER_GROUP	1024	//ÿ����̿���
#define INODES_PER_GROUP	1024	//ÿ���inode��
#define TOTAL_BLOCKS_NUM	(BLOCKS_PER_GROUP*BLOCKS_GROUP_NUM)		//�ܵ��̿���
#define TOTAL_INODES_NUM	(INODES_PER_GROUP*BLOCKS_GROUP_NUM)		//�ܵ�inode��

/* FIRST_DATA_ADDR : ��һ�����ݿ���׵�ַ
 * ���ļ�ϵͳ�в�����ÿ�����ݿ��������ţ�����ÿ�������������������һ������
 * ������˳����  (������ + ��������) + i�ڵ�λͼ + �����̿�λͼ + i�ڵ� + ������
 */
#define FIRST_DATA_ADDR		(sizeof(block_group)*BLOCKS_GROUP_NUM + sizeof(bool)*(TOTAL_INODES_NUM + TOTAL_BLOCKS_NUM) + sizeof(inode)*TOTAL_INODES_NUM)

#define DIR_SIZE			(sizeof(dir)/BLOCK_SIZE+1)	  //һ��Ŀ¼ռ���̿���

#define NAME_LEN			128		//�ļ�������󳤶�
#define FILE_NUM			256		//һ��Ŀ¼�¿���ӵ�е��ļ�(����Ŀ¼)�������Ŀ
#define MAX_USER_NUM		8		//���ļ�ϵͳ�����û���Ŀ

#define READ_ONLY			1		//ֻ��
#define WRITE_ONLY			2		//ֻд
#define RW					3		//��д

#define NOT_USED			false	//δ��ʹ��
#define USED				true	//��ʹ��

#define ADMIN				1		//ϵͳ����Ա
#define DEFULT				0		//��ͨ�û�

#define _FILE				1		//�ļ�
#define _DIRECTORY			2		//Ŀ¼

/*
 *������
 */
struct super_block
{
	unsigned int s_inodes_count;		//inode ������
	unsigned int s_blocks_count;		//�̿������
	unsigned int s_log_block_size;		//�̿�Ĵ�С
	unsigned int s_free_blocks_count;	//���п������
	unsigned int s_free_inodes_count;	//����inode������
	unsigned int s_first_data_block;	//��һ�����ݿ�
	unsigned int s_blocks_per_group;	//ÿ����̿���
	unsigned int s_inodes_per_group;	//ÿ���inode��
};

/*
 *��������
 */
struct group_desc
{
	unsigned long bg_block_addr;		//�������ݿ������������׵�ַ
	unsigned int bg_block_bitmap;		//�������ݿ�λͼ���ڵĿ��
	unsigned int bg_inode_bitmap;		//����inode λͼ���ڵĿ��
	unsigned int bg_inode_table;		//����inode �����ڵĿ��
	unsigned int bg_free_blocks_count;	//���п��п����Ŀ
	unsigned int bg_free_inodes_count;	//���п���inode ����Ŀ
};


/*
 *inode
 */
struct inode
{
	unsigned short i_mode;				//ģʽ
	unsigned int i_uid;					//�ļ����û�ID
	unsigned int i_size;				//�ļ��Ĵ�С
	unsigned int i_blocks;				//��������ļ��Ĵ��̿����Ŀ
	long i_block;						//ָ����̿����ʼ��ַ
	bool i_change;						//��ʾ���ļ��Ƿ��޸Ĺ�,true��ʾ�޸Ĺ���false��ʾû��
};


/*
 * ���ݿ���
 */
struct block_group
{
	struct super_block super_block;		//������
	struct group_desc group_desc;		//��������
};

/*
 *�ļ�,�����ļ���(��ͨ�ļ�����Ŀ¼)��inode��
 */
struct file_entry
{
	char name[NAME_LEN];				//�ļ���
	int inode;							//inode��,����inode_table�е������±�
};


/*
 *Ŀ¼,���ڴ洢ָ������Ŀ¼�������ļ�(����Ŀ¼)����Ŀ�;�����ļ�����(��dir_entry)
 */
struct dir
{
	char dir_name[NAME_LEN];					//Ŀ¼��
	int inode;									//inode��
	int size;									//��¼��Ŀ¼�°������ٸ��ļ�(����Ŀ¼)
	file_entry file_entry[FILE_NUM];		//������ļ�����
};


/*
 *�û�
 */
struct user
{
	unsigned int mode;					//�û���ģʽ(Ȩ��)
	int uid;							//�û���ID
	char password[20];					//����
};


/*
 *����
 */
struct command
{
	char order[10];
};

extern struct block_group _block_group[BLOCKS_GROUP_NUM];		//���ݿ���
extern struct inode inode_table[TOTAL_INODES_NUM];				//inode��
extern struct user _user[MAX_USER_NUM];							//�û�
extern struct dir _current_dir;									//Ŀ¼
extern bool block_bitmap[TOTAL_BLOCKS_NUM];						//���ݿ�λͼ
extern bool inode_bitmap[TOTAL_INODES_NUM];						//inode λͼ
extern FILE *fd;												//ϵͳ�ļ�
extern unsigned int uid;										//ʹ���ļ�ϵͳ���û�ID
extern char pwd[20];											//����
extern command cmd[13];											//����
extern char current_path[1000];									//���浱ǰ·��

extern void initialize();
extern void login();
extern void help();
extern void info();

extern void display(dir _current_dir);

extern void remove_dir(dir temp,char*rd_name);
extern void remove_dir(dir father_dir,dir r_dir,int index);
extern void delete_file(char*path_and_file_name,int path_len);
extern void delete_file(dir _dir,char *file_name);
extern void create_file(dir temp_dir,char *file_name);
extern void open_file(dir temp_dir,char*file_name);
extern void copy_file(char*src_path,int src_len,char*des_path,int des_len);
extern void write_back(dir temp,char*file_name,char *buf,int len);
extern long read_file(dir temp_dir,char*file_name,int inode,char*buf);
extern void check();

// �޸ĺ��� pathAnalysis
extern bool getLastDir(char *path, dir &temp_dir);
extern void findFullPath(dir temp_dir);
extern bool getDirName(char *path, int pos, char *dirName);
extern bool findFinalDir(char *path, int &pos, char *dirName, dir &temp_dir);
extern bool dividePathAndName(char *path, char *dirName, dir &temp_dir);
//  md
extern void makeDir(dir temp_dir, char *dirName);

// findSameName
extern bool findSameName(dir temp_dir, char*dir_name);

// blockManager
extern long getBlock(int len, int *b_index);
extern void freeBlock(int len, int pos);

// inodeManage
extern int getInode();
extern void freeInode(int inode);

// rd
extern void rd(char*path_and_dir_name);
extern bool isDirRemove(dir rdDir, dir currentDir);

// copy
extern bool isHostFile(char*path);

#endif