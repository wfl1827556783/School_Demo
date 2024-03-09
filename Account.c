#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>
// ��־�ļ�·��
#define JOURNAL_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\Journal.txt" 
// �����ļ�·��
#define FUND_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\Fund.txt" 
// �û��ļ�·��
#define USER_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\User.txt" 

typedef struct {
	int  TID;//����ID 
	char Trade_time[30];//����ʱ��
	char type[10];//��������
	int  UID;//�����û�ID
	int  FID;//����ID
} JOURNAL; // ��־�ṹ��
typedef struct {
	float price;//����۸�
	int All_number;//����������
	int buy_people;//�����������
	int buy_number;//�����������
	int FID;//����ID 
	char Name[30];//��������
	JOURNAL Journal;//Ƕ�׻���������
} FUND; // ����ṹ��
typedef struct {
	int  UID;//UID
	char name[20];//�˺�
	char pwd[20];//����
	float balance;//���
}USER; // �û��ṹ��
typedef struct {
	int uid;
	char username[20];
	char password[20];
	float balance;
}ACCOUNT; // ��ǰ�˻��ṹ��

ACCOUNT current_account; // ���ڴ��浱ǰ��¼�˺ŵ���Ϣ
FUND fund[11]; // ����10������ṹ�� Ԥ��һ��
USER user[21]; // ����20���û��ṹ�� Ԥ��һ��
JOURNAL journal[9999]; //��־�ṹ��

int userCount = 0; // ��¼�û�����
char choose; // ȫ��switchѡ��

//==========================����������==========================

// ��������

void Run(); //�������к���
void choice(); // ִ�л���ѡ�� ����

// �˻�����

void LoadingAccount(const char* filename); // ��¼�˻�����
void CreateAccount(const char* filename); // �����˻�����
int AuthLoad(const char* filename); // ��¼��֤����
void AccountWindows(); //��¼���˺Ŵ��ڴ���
void AccountChoose(); // �˻�ѡ����
void CurrentAccount(); // ��ǰ�˻��ĺ���
void UpdateAccount(const char* filename); // ���µ�ǰ�˻�����Ϣ����
int UID_count(const char* filename); // ����UID����
void FindAllAcount(const char* filename); // ��ѯ�����˻�����
void LoadAccounts(const char* filename); // ���ļ��ڵ��û����ݼ��ص�������
void WriteAccounts(const char* filename); // �������е�����д�뵽�ļ�
// ����Ա���� 

void Admin_Windows(); // ����Ա���ں���

// ������

void InsertFund(const char* filename); // ��ӻ�����
void UpdateFund(const char* filename); // ���Ļ�����Ϣ
int FID_Count(const char* filename); // ����FID��������
void Findtype(const char* filename); // ��ѯ������ຯ��
void SortByFID(const char* filename); // ��ʾ������Ϣ����FID����
void SortByRate(const char* filename); // ��ʾ������Ϣ�����Ƿ�����
void SortByPrice(const char* filename); // ��ʾ������Ϣ���Լ۸�����
void Sortprice(FUND f[], int N); // ����۸���
void Swap(FUND* a, FUND* b); // ��������

// ��־����

void FindJournal(const char* filename); // ��ѯ��־����

// ��ѡ���

void current_time(); // ��ѯ��ǰʱ�亯��

//^^^^^^^^^^^^^^^^^^^^^^^^^^����������^^^^^^^^^^^^^^^^^^^^^^^^^^

void FindAllAccount() {
	userCount = UID_Count(USER_FILE_PATH);
	printf("�����˺���Ϣ����:\n");
	for (int i = 0; i <= userCount; i++) {
		printf("UID: %d\n", user[i].UID);
		printf("�˺�: %s\n", user[i].name);
		printf("����: %s\n", user[i].pwd);
		printf("���: %.2f\n", user[i].balance);
		printf("-----------------\n");
	}
}
int AuthLoad(const char* filename) {
	char username[20], password[20];
	printf("�������˺ź����룬�ÿո�ָ���\n");
	scanf("%s %s", username, password);

	FILE* fp;
	char line[100];
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("�޷����ļ���\n");
		return false;
	}

	int authenticated = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		char file_username[20], file_password[20];
		int uid;
		sscanf(line, "UID:%d �˺ţ�%s ���룺%s ���:%f", &uid, file_username, file_password, &current_account.balance);
		if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
			current_account.uid = 1;
			strcpy(current_account.username, "admin");
			strcpy(current_account.password, "admin");
			authenticated = 2;
			fclose(fp);
			return authenticated;
		}
		else if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
			// ���ƥ��ɹ������浱ǰ��¼���˺���Ϣ
			current_account.uid = uid;
			strcpy(current_account.username, file_username);
			strcpy(current_account.password, file_password);
			authenticated = 1;
			break;
		}
	}

	fclose(fp);

	return authenticated;
}
void LoadingAccount(const char* filename) {
	int a = AuthLoad(filename);
	if (a == 1) {
		printf("��¼�ɹ�!\n");
		AccountWindows();
	}else if(a ==2){
		printf("��ӭ����Ա!!!\n");
		Admin_Windows();
	}
	else {
		printf("��¼ʧ��!\n");
		//�˴���д����������֤
	}
}
void CreateAccount(const char* filename) {
	
	FILE* fp;
	 
	userCount = UID_Count(filename);

	// ��ȡ���������

	printf("��������Ҫ�������˺�,��������\n");
	scanf("%s %s %f", user[userCount].name, user[userCount].pwd , &user[userCount].balance);
	user[userCount].UID = userCount;

	// ����ȡ������д�뵽�ļ���

	fp = fopen(filename, "a");
	if (fp == NULL) {
		printf("�޷����ļ���\n");
	}
	fprintf(fp, "UID:%d �˺ţ�%s ���룺%s ���:%f \n",user[userCount].UID,user[userCount].name,user[userCount].pwd,user[userCount].balance);
	fclose(fp);

	// �����������

	printf("�����ɹ�������Ϣ���£�\n UID:%d �˺ţ�%s ���룺%s ���:%f\n", user[userCount].UID, user[userCount].name, user[userCount].pwd,user[userCount].balance);

}
int UID_Count(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("�޷����ļ���\n");
		return 0; // ���� 0 ��ʾû�� UID
	}

	int count = 0;
	char line[100]; // ����һ����� 100 ���ַ�
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (strstr(line, "UID:") != NULL) {
			count++;
		}
	}

	fclose(fp);
	return count;
}
void AccountWindows() {
	CurrentAccount();
	AccountChoose();
}
void AccountChoose() {
	do {
		printf("��������:\n");
		printf("1.��ѯ�˺���Ϣ		2.�޸��˺���Ϣ\n");
		printf("3.��ѯ������Ϣ          4.��ѯ��־��Ϣ\n");
		printf("5.�������		6.��������\n");
		printf("7.��ѯ�ѹ���Ļ���	8.�˳��˺�\n");
		printf("���������ѡ��:\n");
		scanf("%d", &choose);

		switch (choose) {
		case 1:
			CurrentAccount();
			break;
		case 2:
			UpdateAccount(USER_FILE_PATH);
			CurrentAccount();
			break;
		case 3:
			Findtype(FUND_FILE_PATH);
			break;
		case 4:
			FindJournal(JOURNAL_FILE_PATH);
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			printf("���˳��˺�!\n");
			return;
		default:
			printf("��������! �������������ѡ��:\n");
		}
	} while (choose != 7);
}
void CurrentAccount() {
	printf("��ǰ��¼�˺���Ϣ��\n");
	printf("UID: %d\n", current_account.uid);
	printf("�û���: %s\n", current_account.username);
	printf("����: %s\n", current_account.password);
	printf("���: %.2f\n", current_account.balance);

	printf("========================================\n");
}
void LoadAccounts(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("�޷����ļ���\n");
		return;
	}

	while (fscanf(fp, "UID:%d �˺ţ�%s ���룺%s ���:%f", &user[userCount].UID, user[userCount].name, user[userCount].pwd, &user[userCount].balance) == 4) {
		userCount++;
		if (userCount >= 21) {
			printf("�û������������޷����������û���Ϣ��\n");
			break;
		}
	}

	fclose(fp);
}
void UpdateAccount(const char* filename) {
	CurrentAccount(); // �����ǰ��¼�˺���Ϣ
	int uid = current_account.uid;
	printf("��ѡ��Ҫ�޸ĵ�ѡ��:\n");
	printf("1. �û���\n");
	printf("2. ����\n");
	printf("3. ���\n");

	scanf("%d", &choose);

	switch (choose) {
	case 1:
		printf("�������µ��û���:\n");
		scanf("%s", current_account.username);
		strcpy(user[uid].name , current_account.username);
		break;
	case 2:
		printf("�������µ�����:\n");
		scanf("%s", current_account.password);
		strcpy(user[uid].pwd, current_account.password);
		break;
	case 3:
		printf("�������µ����:\n");
		scanf("%f", &current_account.balance);
		strcpy(user[uid].balance, current_account.balance);
		break;
	default:
		printf("��ѡ����ȷ��ѡ��!\n");
		return;
	}

	WriteAccounts(filename);
	printf("�˺���Ϣ�Ѹ���!\n");
}
void WriteAccounts(const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("�޷����ļ���д���˻���Ϣ��\n");
		return;
	}

	for (int i = 0; i <= userCount; i++) {
		fprintf(fp, "UID:%d �˺ţ�%s ���룺%s ���:%.2f\n", user[i].UID, user[i].name, user[i].pwd, user[i].balance);
	}

	fclose(fp);
}






