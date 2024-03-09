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

extern int userCount; // 记录用户数量
extern char choose; // 全局switch选择

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

//^^^^^^^^^^^^^^^^^^^^^^^^^^函数声明块^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void current_time() {
	time_t rawtime;
	struct tm* timeinfo;

	// 获取当前时间的时间戳
	time(&rawtime);

	// 将时间戳转换为本地时间
	timeinfo = localtime(&rawtime);

	// 打印本地时间
	printf("当前本地时间：%s", asctime(timeinfo));
}