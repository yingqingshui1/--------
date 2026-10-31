#pragma once

/** 
* 学生结构
*/
typedef struct Student//typedef是为了方便使用，所起的别名
{
	unsigned long long number;   //学号
	char name[32];               //姓名
	float Chinese;
	float Math;
	float English;
}Student;

/*
* 节点 Node
*/
typedef struct Node
{
	Student stu;         //学生
	struct Node* next;   //指向下一个节点的指针
}Node;

/**
*链表结构
*/
typedef struct List
{
	Node* front;         //头指针，指向第一个节点
	int size;            //当前节点数量
}List;

/**
*菜单功能
*/
typedef enum MenuOptions//菜单的选项(枚举）
{
	Quit,                //退出系统
	Entry,               //录入学生
	Print,               //打印学生
	Save,                //保存学生
	Sort,                //排序学生
	Statistics,          //统计人数
	Find,                //查找学生
	Alter,               //修改学生
	Remove               //删除学生
}MenuOptions;


/*
打印菜单,并提示用户选择功能
*/
int menu();


/**
*学生录入
*/
void entryStudent(List* list);

/**
*打印学生
*/
void printStudent(List* list);

/*
*保存学生,以二进制方式保存
*/
void saveStudent(List* list);

/*
*保存学生，以我们能看懂的方式保存
*/
void saveStudentHuman(List* list);

/*
*给学生成绩进行排序
*/
void sortStudentByScore(List* list);

/*
*统计学生均分
*/
void statisticsAvg(List* list);

/*
*查找学生
*/
Node* findStudent(List* list);

/*
*修改学生信息
*/
void alterStudent(List* list);

/*
*删除学生
*/
void removeStdent(List* list);