#include "struct.h"
/*
 *��½�ļ�ϵͳ
 */
void login()
{

	printf("-----�༶: 16�ƿ�2��-----\n");
	printf("-----����: ���----------\n");
	printf("-----ѧ��: 201630599319--\n\n");
	printf("���¼ϵͳ...\n");

	bool isLogin = false;
	while (!isLogin) {
		int loginID;
		char loginPassword[100];
		printf("�������û�ID: ");
		fflush(stdin);
		scanf("%dL", &loginID);
		printf("����������: ");
		fflush(stdin);
		scanf("%s", loginPassword);
		fflush(stdin);
		for (int i = 0; i < MAX_USER_NUM; i++)
		{
			if (loginID == _user[i].uid && strcmp(loginPassword, _user[i].password) == 0)
			{
				uid = loginID;
				isLogin = true;
				return;
			}
		}
		printf("�û����������������������\n\n");
	}
}
