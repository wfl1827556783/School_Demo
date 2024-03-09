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

extern int userCount; // ��¼�û�����
extern char choose; // ȫ��switchѡ��

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

//^^^^^^^^^^^^^^^^^^^^^^^^^^����������^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void current_time() {
	time_t rawtime;
	struct tm* timeinfo;

	// ��ȡ��ǰʱ���ʱ���
	time(&rawtime);

	// ��ʱ���ת��Ϊ����ʱ��
	timeinfo = localtime(&rawtime);

	// ��ӡ����ʱ��
	printf("��ǰ����ʱ�䣺%s", asctime(timeinfo));
}