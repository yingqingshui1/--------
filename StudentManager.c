#include "StudentManager.h"// 引入自定义头文件，包含List、Node、Student结构体声明

#include<stdio.h>          //fopen也靠这个引出
#include<stdlib.h>         // 标准库，malloc、free、perror
#include<string.h>         // 字符串处理库，strcmp、sscanf

int menu()// 菜单函数：打印操作菜单，返回用户输入的功能选择
{
	printf("************************************\\n");
	printf("**   欢迎使用学生成绩管理系统     **\\n");
	printf("************************************\\n");
	printf("**          请选择功能            **\\n");
	printf("************************************\\n");
	printf("**        1.录入学生成绩          **\\n");
	printf("**        2.打印学生成绩          **\\n");
	printf("**        3.保存学生成绩          **\\n");
	printf("**        4.排序学生成绩          **\\n");
	printf("**        5.统计学生成绩          **\\n");
	printf("**        6.查找学生成绩          **\\n");
	printf("**        7.修改学生成绩          **\\n");
	printf("**        8.删除学生成绩          **\\n");
	printf("**        0.退出系统               **\\n");
	printf("************************************\\n");
	printf("select>");// 提示用户输入选择
	int select = -1;// 定义变量保存用户选择，初始值-1
	scanf("%d", &select);// 读取用户输入数字
	return select;
}
// 录入学生信息：向链表添加一个新学生节点
void entryStudent(List* list)
{
	//创建节点
	Node* node = malloc(sizeof(Node));// 在堆内存分配一个Node大小的空间，得到新节点指针
	if (!node){                       // 判断malloc是否分配失败，指针为NULL代表失败
		printf("malloc failed\\n");
		return;
	}
	node->next = NULL;                 // 新节点的next置空，暂时不连接其他节点

	//输入学生信息
	printf("输入学生学号>");
	scanf("%llu", &node->stu.number);// 读取学号存入节点内Student结构体的number成员

	printf("输入学生姓名>");
	scanf("%s", node->stu.name);    // 读取学号存入节点内Student结构体的number成员

	printf("输入学生语文成绩>");
	scanf("%f", &node->stu.Chinese);// 提示语文成绩

	printf("输入学生数学成绩>");
	scanf("%f", &node->stu.Math );

	printf("输入学生英语成绩>");
	scanf("%f", &node->stu.English );

	//插入到链表中
	node->next = list->front;       // 新节点next指向原来链表的第一个节点
	list->front = node;             // 链表头指针更新为新节点，新节点变成链表第一个元素
	list->size++;                   // 链表学生数量计数器+1
}
// 打印全部学生成绩：遍历链表输出所有学生信息
void printStudent(List* list)
{
	printf("************************************\\n");
	printf("**   欢迎使用学生成绩管理系统     **\\n");
	printf("************************************\\n");
	printf("* 学号 * 姓名 * 语文 * 数学 * 英语 *\\n");
	printf("************************************\\n");

	//遍历链表
	Node* curNode = list->front;                   // 定义遍历指针curNode，从链表头节点开始
	while (curNode != NULL)                        // curNode不为NULL说明还有节点没有遍历
	{
		printf("* %llu * %s * %.1f * %.1f * %.1f *\\n", curNode->stu.number, curNode ->stu.name,
			curNode->stu.Chinese, curNode->stu.Math, curNode->stu.English );
		curNode = curNode->next;                    // 遍历指针移动到下一个节点
	}
}
// 二进制保存学生数据到students.data文件
void saveStudent(List* list) 
{
	//打开文件，wb模式：二进制写，文件不存在创建，存在就覆盖原有内容
	FILE* fp = fopen("students.data", "wb");// 判断文件打开是否失败，fp为NULL代表失败
	if (!fp) {                              // 打印系统层面的错误原因
		perror("file open failed");
		return;
	}

	//写入学生信息
	Node* curNode = list->front;              // 遍历指针从链表头开始
	while (curNode != NULL)                   // 循环直到遍历完所有节点
	{		// fwrite：把curNode->stu这个Student结构体二进制写入文件，写1个Student大小单元
		fwrite(&curNode->stu, sizeof(Student), 1, fp);
		curNode = curNode->next;             // 移动到下一个学生节点
	}

	//关闭文件
	fclose(fp);
}

void saveStudentHuman(List* list)            // 关闭文件流，刷新缓冲区，保存真正落盘
{
	//打开文件 w文本写模式，覆盖旧文件
	FILE* fp = fopen("students.txt", "w");
	if (!fp) {                                 // 判断打开失败
		perror("file open failed");
		return;
	}

	//写入学生信息
	Node* curNode = list->front;             // 遍历指针指向链表头部
	while (curNode != NULL)                  // 遍历全部节点
	{// fprintf向文件输出格式化文本，和printf用法一样，输出到文件而非控制台
		fprintf(fp, "* %llu * %s * %.1f * %.1f * %.1f *\\n", curNode->stu.number, curNode->stu.name,
			curNode->stu.Chinese, curNode->stu.Math, curNode->stu.English);
		curNode = curNode->next;             // 指针后移
	}

	//关闭文件
	fclose(fp);
}
// 按三科总分降序排序，冒泡排序，交换结构体内容，不改动节点链接
void sortStudentByScore(List* list)//按三科总分降序排序
{
	if (list->size <= 1)            // 如果学生数量小于等于1不需要排序
	{
		printf("学生数量不足，无需排序\\n");
		return;
	}

	Node* p;               // 冒泡外层遍历指针
	Node* q;               // p的下一个节点，用来比较
	Student temp; //交换学生信息的临时结构体
	int i, j;
	//冒泡排序 链表长度list->size
	for (i = 0; i < list->size - 1; i++) // 外层冒泡循环，一共size?1轮
	{
		p = list->front;                  // 每一轮开始，p重置为链表头部
		for (j = 0; j < list->size - 1 - i; j++) // 内层循环，每轮减少i次比较（末尾i个已经排好序）
		{
			q = p->next;                 // q取p的后继节点，用来和p对比
			//计算两个学生的三科总分
			float sum1 = p->stu.Chinese + p->stu.Math + p->stu.English; // p指向学生总分
			float sum2 = q->stu.Chinese + q->stu.Math + q->stu.English; // q指向学生总分
			                                 // 如果p总分小于q，需要交换两者学生信息实现降序
			if (sum1 < sum2)
			{
				//交换两个节点里面学生结构体内容，节点地址不变，只交换里面存的学生数据
				temp = p->stu;
				p->stu = q->stu;
				q->stu = temp;
			}
			p = p->next;                           // p向后移动一个节点
		}
	}
	printf("按三科总分降序排序完成！\\n");
}

void statisticsStudentCount(List* list)
{
	if (list->size == 0)                                 // 判断链表没有学生
	{
		printf("暂无学生数据，无法统计平均分！\\n");
		return;
	}

	float sumChinese = 0;
	float sumMath = 0;
	float sumEnglish = 0;
	Node* cur = list->front;                                // 遍历指针从链表头部开始

	//遍历累加所有科目总分
	while (cur != NULL)                                    // 循环遍历全部学生节点
	{
		sumChinese += cur->stu.Chinese;                      //累加
		sumMath += cur->stu.Math;
		sumEnglish += cur->stu.English;
		cur = cur->next;                                    // 指针移动到下一个学生
	}

	//计算各科平均分
	float avgCh = sumChinese / list->size;
	float avgMa = sumMath / list->size;
	float avgEn = sumEnglish / list->size;
	float totalAvg = (sumChinese + sumMath + sumEnglish) / (list->size * 3.0f);

	printf("=======成绩统计=======\\n");
	printf("学生总人数：%d\\n", list->size);
	printf("语文平均分：%.2f\\n", avgCh);
	printf("数学平均分：%.2f\\n", avgMa);
	printf("英语平均分：%.2f\\n", avgEn);
	printf("三科总体平均分：%.2f\\n", totalAvg);
}
// 查找学生，输入学号或者姓名，找到返回对应节点指针，找不到返回NULL
Node* findStudent(List* list)
{
	if (list->size == 0) // 如果链表没有学生，直接返回空指针
	{
		return NULL;
	}
	char buffer[32];               // 字符数组，接收用户输入的字符串（姓名或者学号字符串）
	printf("请输入要查找的学生学号或姓名");
	scanf("%s", buffer);             // 读取输入字符串存入buffer

	//把buffer转成整型
	unsigned long long number=-1;
	sscanf(buffer, "%llu", &number);// 将buffer字符串按无符号长long long解析存入number

	Node* curNode = list->front;  // 遍历指针从链表头开始
	while (curNode != NULL)         // 循环遍历每一个节点
	{//判断：姓名匹配 或者 学号匹配，匹配成功返回当前节点地址
		if (strcmp(curNode->stu.name, buffer) == 0 || curNode->stu.number==number ) 
		{
			return curNode;            // 找到，返回节点指针
		}
		curNode = curNode->next;       // 没找到，继续下一个
	}
	return NULL;                         // 循环结束都没匹配到，返回NULL
}

void alterStudent(List* list)
{
	if (list->size == 0)                 // 判断链表为空
	{
		printf("暂无学生~\\n");
		return;
	}
	char buffer[32];                       // 接收用户输入的姓名/学号字符串
	printf("请输入要修改的学生学号或姓名>");
	scanf("%s", buffer);

	//把buffer转成整型
	unsigned long long number = -1;
	sscanf(buffer, "%llu", &number);          // 字符串解析成数字学号

	Node* curNode = list->front;                // 遍历指针指向链表头部
	while (curNode != NULL)
	{//姓名相等或者学号相等
		if (strcmp(curNode->stu.name, buffer) == 0 ||
			curNode->stu.number == number){

			printf("请输入要修改的学生 语文成绩 数学成绩 英语成绩>");
			scanf("%f %f %f",&curNode->stu.Chinese,&curNode->stu.Math,&curNode->stu.English);// 直接修改节点内部成绩
			printf("修改成功QAQ\\n");
			break;              // 修改完成跳出while循环
		}
		curNode = curNode->next; // 继续下一个节点
	}

	if (curNode == NULL)// 循环走完curNode为NULL代表没有找到
	{
		printf("没有找到该学生，修改失败~\\n");
	}

}
// 删除学生：输入姓名或学号，从链表移除并释放节点内存
void removeStudent(List* list)
{
	if (list->size == 0)// 链表无学生
	{
		printf("暂无学生~\\n");
		return;
	} 

	char buffer[32];
	printf("请输入要删除的学生学号或姓名>");
	scanf("%s", buffer);

	//把buffer转成整型
	unsigned long long number = -1;
	sscanf(buffer, "%llu", &number);

	Node* curNode = list->front;       // curNode是待查找的节点
	Node* prevNode = NULL;              // prevNode保存curNode的前一个节点，用于链表摘除操作
	while (curNode != NULL)
	{
		if (strcmp(curNode->stu.name, buffer) == 0 ||
			curNode->stu.number == number) {
			break;// 匹配到学生，跳出循环，此时curNode指向待删节点
		}
		prevNode = curNode;                     // prev跟上cur
		curNode = curNode->next;                  // cur往后移动
	}
	
	//是否找到节点，curNode不为NULL代表找到了
	if (curNode)
	{
		//要删除的就是第一个节点，prevNode是NULL
		if (prevNode == NULL)
		{
			list->front = curNode->next; // 头指针直接跳过被删节点，指向下一个节点
		}
		else
		{
			prevNode->next = curNode->next;    // 前一个节点next跳过待删除节点，把链表接上
		}	
		free(curNode);                      // 释放被删除节点堆内存，防止内存泄漏
		list->size--;                     // 链表学生数量减一
	}
}