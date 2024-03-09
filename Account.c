#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<stdbool.h>
// 日志文件路径
#define JOURNAL_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\Journal.txt" 
// 基金文件路径
#define FUND_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\Fund.txt" 
// 用户文件路径
#define USER_FILE_PATH "D:\\Code\\C\\School_Demo\\SourceFile\\User.txt" 

typedef struct {
	int  TID;//交易ID 
	char Trade_time[30];//交易时间
	char type[10];//交易类型
	int  UID;//交易用户ID
	int  FID;//基金ID
} JOURNAL; // 日志结构体
typedef struct {
	float price;//基金价格
	int All_number;//基金总数量
	int buy_people;//购买基金人数
	int buy_number;//购买基金数量
	int FID;//基金ID 
	char Name[30];//基金名称
	JOURNAL Journal;//嵌套基金交易数据
} FUND; // 基金结构体
typedef struct {
	int  UID;//UID
	char name[20];//账号
	char pwd[20];//密码
	float balance;//余额
}USER; // 用户结构体
typedef struct {
	int uid;
	char username[20];
	char password[20];
	float balance;
}ACCOUNT; // 当前账户结构体

ACCOUNT current_account; // 用于储存当前登录账号的信息
FUND fund[11]; // 申请10个基金结构体 预留一个
USER user[21]; // 申请20个用户结构体 预留一个
JOURNAL journal[9999]; //日志结构体

int userCount = 0; // 记录用户数量
char choose; // 全局switch选择

//==========================函数声明块==========================

// 基础函数

void Run(); //基础运行函数
void choice(); // 执行基础选择 函数

// 账户函数

void LoadingAccount(const char* filename); // 登录账户函数
void CreateAccount(const char* filename); // 创建账户函数
int AuthLoad(const char* filename); // 登录验证函数
void AccountWindows(); //登录后账号窗口代码
void AccountChoose(); // 账户选择函数
void CurrentAccount(); // 当前账户的函数
void UpdateAccount(const char* filename); // 更新当前账户的信息函数
int UID_count(const char* filename); // 计算UID函数
void FindAllAcount(const char* filename); // 查询所有账户函数
void LoadAccounts(const char* filename); // 将文件内的用户数据加载到数组中
void WriteAccounts(const char* filename); // 将数组中的数据写入到文件
// 管理员函数 

void Admin_Windows(); // 管理员窗口函数

// 基金函数

void InsertFund(const char* filename); // 添加基金函数
void UpdateFund(const char* filename); // 更改基金信息
int FID_Count(const char* filename); // 计算FID数量函数
void Findtype(const char* filename); // 查询基金分类函数
void SortByFID(const char* filename); // 显示基金信息（以FID排序）
void SortByRate(const char* filename); // 显示基金信息（以涨幅排序）
void SortByPrice(const char* filename); // 显示基金信息（以价格排序）
void Sortprice(FUND f[], int N); // 排序价格函数
void Swap(FUND* a, FUND* b); // 交换函数

// 日志函数

void FindJournal(const char* filename); // 查询日志函数

// 可选项函数

void current_time(); // 查询当前时间函数

//^^^^^^^^^^^^^^^^^^^^^^^^^^函数声明块^^^^^^^^^^^^^^^^^^^^^^^^^^

void FindAllAccount() {
	userCount = UID_Count(USER_FILE_PATH);
	printf("所有账号信息如下:\n");
	for (int i = 0; i <= userCount; i++) {
		printf("UID: %d\n", user[i].UID);
		printf("账号: %s\n", user[i].name);
		printf("密码: %s\n", user[i].pwd);
		printf("余额: %.2f\n", user[i].balance);
		printf("-----------------\n");
	}
}
int AuthLoad(const char* filename) {
	char username[20], password[20];
	printf("请输入账号和密码，用空格分隔：\n");
	scanf("%s %s", username, password);

	FILE* fp;
	char line[100];
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("无法打开文件。\n");
		return false;
	}

	int authenticated = 0;
	while (fgets(line, sizeof(line), fp) != NULL) {
		char file_username[20], file_password[20];
		int uid;
		sscanf(line, "UID:%d 账号：%s 密码：%s 余额:%f", &uid, file_username, file_password, &current_account.balance);
		if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
			current_account.uid = 1;
			strcpy(current_account.username, "admin");
			strcpy(current_account.password, "admin");
			authenticated = 2;
			fclose(fp);
			return authenticated;
		}
		else if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
			// 如果匹配成功，保存当前登录的账号信息
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
		printf("登录成功!\n");
		AccountWindows();
	}else if(a ==2){
		printf("欢迎管理员!!!\n");
		Admin_Windows();
	}
	else {
		printf("登录失败!\n");
		//此处可写三次密码验证
	}
}
void CreateAccount(const char* filename) {
	
	FILE* fp;
	 
	userCount = UID_Count(filename);

	// 获取输入的数据

	printf("请输入您要创建的账号,密码和余额\n");
	scanf("%s %s %f", user[userCount].name, user[userCount].pwd , &user[userCount].balance);
	user[userCount].UID = userCount;

	// 将获取的数据写入到文件中

	fp = fopen(filename, "a");
	if (fp == NULL) {
		printf("无法打开文件。\n");
	}
	fprintf(fp, "UID:%d 账号：%s 密码：%s 余额:%f \n",user[userCount].UID,user[userCount].name,user[userCount].pwd,user[userCount].balance);
	fclose(fp);

	// 检查输入数据

	printf("创建成功，您信息如下：\n UID:%d 账号：%s 密码：%s 余额:%f\n", user[userCount].UID, user[userCount].name, user[userCount].pwd,user[userCount].balance);

}
int UID_Count(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("无法打开文件。\n");
		return 0; // 返回 0 表示没有 UID
	}

	int count = 0;
	char line[100]; // 假设一行最多 100 个字符
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
		printf("功能如下:\n");
		printf("1.查询账号信息		2.修改账号信息\n");
		printf("3.查询基金信息          4.查询日志信息\n");
		printf("5.买入基金		6.卖出基金\n");
		printf("7.查询已购买的基金	8.退出账号\n");
		printf("请输入你的选择:\n");
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
			printf("已退出账号!\n");
			return;
		default:
			printf("输入有误! 请重新输入你的选择:\n");
		}
	} while (choose != 7);
}
void CurrentAccount() {
	printf("当前登录账号信息：\n");
	printf("UID: %d\n", current_account.uid);
	printf("用户名: %s\n", current_account.username);
	printf("密码: %s\n", current_account.password);
	printf("余额: %.2f\n", current_account.balance);

	printf("========================================\n");
}
void LoadAccounts(const char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("无法打开文件。\n");
		return;
	}

	while (fscanf(fp, "UID:%d 账号：%s 密码：%s 余额:%f", &user[userCount].UID, user[userCount].name, user[userCount].pwd, &user[userCount].balance) == 4) {
		userCount++;
		if (userCount >= 21) {
			printf("用户数组已满，无法继续加载用户信息。\n");
			break;
		}
	}

	fclose(fp);
}
void UpdateAccount(const char* filename) {
	CurrentAccount(); // 输出当前登录账号信息
	int uid = current_account.uid;
	printf("请选择要修改的选项:\n");
	printf("1. 用户名\n");
	printf("2. 密码\n");
	printf("3. 余额\n");

	scanf("%d", &choose);

	switch (choose) {
	case 1:
		printf("请输入新的用户名:\n");
		scanf("%s", current_account.username);
		strcpy(user[uid].name , current_account.username);
		break;
	case 2:
		printf("请输入新的密码:\n");
		scanf("%s", current_account.password);
		strcpy(user[uid].pwd, current_account.password);
		break;
	case 3:
		printf("请输入新的余额:\n");
		scanf("%f", &current_account.balance);
		strcpy(user[uid].balance, current_account.balance);
		break;
	default:
		printf("请选择正确的选项!\n");
		return;
	}

	WriteAccounts(filename);
	printf("账号信息已更新!\n");
}
void WriteAccounts(const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("无法打开文件以写入账户信息。\n");
		return;
	}

	for (int i = 0; i <= userCount; i++) {
		fprintf(fp, "UID:%d 账号：%s 密码：%s 余额:%.2f\n", user[i].UID, user[i].name, user[i].pwd, user[i].balance);
	}

	fclose(fp);
}






