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

//^^^^^^^^^^^^^^^^^^^^^^^^^^函数声明块^^^^^^^^^^^^^^^^^^^^^^^^^^

void InsertFund() {
	FILE* fp = fopen("D:\\Code\\C\\School_Demo\\SourceFile\\Fund.txt", "a"); 
	if (fp == NULL) {
		printf("无法打开文件\n");
		return;
	}

	FUND fund;
	printf("请依次输入基金名称、价格、基金总数量、购买人数、购买总数量：\n");
	scanf("%s %f %d %d %d", fund.Name, &fund.price, &fund.All_number, &fund.buy_people, &fund.buy_number);

	fund.FID = FID_Count(FUND_FILE_PATH); // 生成唯一的基金ID

	// 写入基金信息到文件中
	fprintf(fp, "%d %.2f %d %d %d %s\n", fund.FID, fund.price, fund.All_number, fund.buy_people, fund.buy_number, fund.Name);
	printf("添加基金信息成功、详细信息如下:\n");
	printf("FID:%d 基金名：%s 价格:%.2f 总数:%d 购买人数：%d 已购买数量：%d \n", fund.FID, fund.Name, fund.price, fund.All_number, fund.buy_people, fund.buy_number);
	printf("==============================================================================================\n");
	fclose(fp);
}
int FID_Count(const char* filename) {
	FILE* fp;
	int fid = 0;
	char line[100];

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("无法打开文件。\n");
		return -1; // 返回-1表示出错
	}

	// 逐行读取文件并计算 UID 数量
	while (fgets(line, sizeof(line), fp) != NULL) {
		int currentFID;
		sscanf(line, "%d", &currentFID);
		if (currentFID > fid) {
			fid = currentFID;
		}
	}

	fclose(fp);

	return fid + 1;
}
void UpdateFund(const char* filename) {
	FILE* fp = fopen(filename, "r+");
	if (fp == NULL) {
		printf("无法打开文件\n");
		return;
	}

	FILE* temp_fp = fopen("temp.txt", "w"); // 创建临时文件
	if (temp_fp == NULL) {
		printf("无法创建临时文件\n");
		fclose(fp);
		return;
	}

	int targetFID;
	printf("请输入要修改的基金ID: ");
	scanf("%d", &targetFID);

	FUND fund;
	int found = 0;

	while (fscanf(fp, "%d %f %d %d %d %[^\n]", &fund.FID, &fund.price, &fund.All_number, &fund.buy_people, &fund.buy_number, fund.Name) != EOF) {
		if (fund.FID == targetFID) {
			found = 1;
			break;
		}
		// 将非目标基金写入临时文件
		fprintf(temp_fp, "%d %.2f %d %d %d %s\n", fund.FID, fund.price, fund.All_number, fund.buy_people, fund.buy_number, fund.Name);
	}

	if (!found) {
		printf("未找到对应基金ID的记录\n");
		fclose(fp);
		fclose(temp_fp);
		remove("temp.txt"); // 删除临时文件
		return;
	}

	printf("找到基金信息如下：\n");
	printf("==============================================================================================\n");
	printf("\n基金ID: %d ", fund.FID);
	printf("基金名称: %s ", fund.Name);
	printf("价格: %.2f ", fund.price);
	printf("总数量: %d ", fund.All_number);
	printf("购买人数: %d ", fund.buy_people);
	printf("购买数量: %d \n\n", fund.buy_number);
	printf("==============================================================================================\n");

	printf("请选择要修改的内容:\n");
	printf("1.价格\n2.总数量\n3.基金名称\n");
	int option;
	scanf("%d", &option);
	fflush(stdin); // 清除输入缓冲区

	switch (option) {
	case 1:
		printf("请输入新的价格: ");
		scanf("%f", &fund.price);
		break;
	case 2:
		printf("请输入新的总数量: ");
		scanf("%d", &fund.All_number);
		break;
	case 3:
		printf("请输入新的基金名称: ");
		scanf("%s", fund.Name);
		break;
	default:
		printf("无效选项\n");
		fclose(fp);
		fclose(temp_fp);
		remove("temp.txt"); // 删除临时文件
		return;
	}

	// 写入更新后的数据到临时文件
	fprintf(temp_fp, "%d %.2f %d %d %d %s\n", fund.FID, fund.price, fund.All_number, fund.buy_people, fund.buy_number, fund.Name);

	// 复制剩余数据到临时文件
	while (fscanf(fp, "%d %f %d %d %d %[^\n]", &fund.FID, &fund.price, &fund.All_number, &fund.buy_people, &fund.buy_number, fund.Name) != EOF) {
		fprintf(temp_fp, "%d %.2f %d %d %d %s\n", fund.FID, fund.price, fund.All_number, fund.buy_people, fund.buy_number, fund.Name);
	}

	fclose(fp);
	fclose(temp_fp);

	remove(filename); // 删除原始文件
	rename("temp.txt", filename); // 将临时文件重命名为原始文件名称

	printf("基金信息修改成功\n");
}
void Findtype(const char* filename) {
	printf("请选择要查询的方式：\n");
	printf("1.单体查询  2.集体分类查询\n");
	scanf("%d", &choose);
	switch (choose) {
	case 1:
		printf("正在执行单体查询:\n");
		printf("请输入要查询的基金FId:\n");
		scanf("%d", &choose);
		//写一个基金结构体ID输出整个基金信息

		break;
	case 2:
		printf("请选择输出的排序方式：\n");
		printf("1.FID排序\n2.Price排序\n3.Rate排序\n");
		scanf("%d", &choose);
		switch (choose) {
		case 1:
			SortByFID(filename);
			break;
		case 2:
			SortByPrice(filename);
			break;
		case 3:
			SortByRate(filename);
			break;
		default:
			printf("输出错误！\n");

			break;
		}
	default:
		break;
	}
}
void SortByFID() {


}
void SortByPrice() {

}
void Sortprice(FUND f[], int N) {

}
void Swap(FUND *a, FUND *b) {

}
void SortByRate() {
	
}
