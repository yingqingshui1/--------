#include<stdio.h>
#include <stdbool.h>//使while(true)正常
#include<stdlib.h>//包含system
#include<string.h>

#include "StudentManager.h"//25行要用menu，作为头文件包含

int main(int argc,char* argv[])
{
	system("chcp 65001");//Windows 终端（cmd）默认用 GBK 编码，两边编码不一致，中文就乱码。
	//创建链表
	List list = { 0 };
	//内存设置
	memset(&list, 0, sizeof(List));

		while(true)
	{
		int op = menu();
		switch (op)
		{
		case Quit:
			printf("正在自动保存txt文件...\n");
			saveStudentHuman(&list); //自动保存成txt
			printf("保存成功，即将退出！\n");
			return 0;// 直接退出程序，解决死循环
		case Entry:
			entryStudent(&list);
			break;
		case Print:
			printStudent(&list);
			break;
		case Save:
			//saveStudent(&list);
			saveStudentHuman(&list);
			break;
        case Sort:
			sortStudentByScore(&list);
			break;
        case Statistics:
			statisticsStudentCount(&list);
			break;
		case Find: 
		{
            Node* node = findStudent(&list);
			if (!node)
			{
				printf("没有找到查询的学生\n");
			}
			else
			{
				printf(" * %llu * %s * %.1f * %.1f * %.1f * \n", node->stu.number,node ->stu.name,
					node->stu.Chinese,node->stu.Math,node->stu.English);
			}
			break;
		}
		case Alter:
			alterStudent(&list);
			break;
		case Remove:
			removeStudent(&list);
			break;
		default:
			printf("输入错误，请输入0~8之间的数字！\n");
			break;
		}
		// 每次操作完暂停，按回车继续，然后清屏回到菜单
		system("pause");//让程序暂停一下
		system("cls");//清屏
	}
	
		//自动保存数据
		saveStudent(&list);
		saveStudentHuman(&list);
	    return 0;
}