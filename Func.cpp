#include "pch.h"
#include "Func.h"
#include "afxdialogex.h"

/* 登录逻辑函数实现 */

// 读取本地用户数据到链表
UserLinkList* readUserInfo() 
{
	// 打开本地用户文件
	FILE* fp = NULL;
	fopen_s(&fp, "res/data/user.txt", "r");

	if (fp == NULL) return NULL;

	// 创建用户单链表
	struct UserLinkList* link = (struct UserLinkList*)malloc(sizeof(struct UserLinkList));
	if (link == NULL) { fclose(fp);return NULL; }
	link->next = NULL;

	// 定义用户数据变量
	int ID = 0;
	char Name[9] = { "\0" }, Account[10] = { "\0" }, Password[19] = { "\0" };

	while (fscanf_s(fp, "%d %s %s %s", &ID, Name,9, Account, 10, Password, 19) != EOF) {

		// 动态申请新结点
		struct UserLinkList* node = (struct UserLinkList*)malloc(sizeof(struct UserLinkList));
		if (node != NULL) {
			node->data.ID = ID;
			strcpy_s(node->data.Name, Name);
			strcpy_s(node->data.Account, Account);
			strcpy_s(node->data.Password, Password);

			node->next = link->next;
			link->next = node;
		}
	}

	fclose(fp);
	return link;
}

// 释放用户链表内存
void freeUserLinkList(UserLinkList* link)
{
	UserLinkList* p;
	while (link) {
		p = link->next;
		free(link);
		link = p;
	}
}

/* 课程逻辑函数实现 */

// 读取本地课程数据到链表
CourseLinkList* readCourseInfo() 
{
	// 打开本地用户文件
	FILE* fp = NULL;
	fopen_s(&fp, "res/data/course.txt", "a+");

	if (fp == NULL) return NULL;

	// 创建课程双链表
	struct CourseLinkList* link = (struct CourseLinkList*)malloc(sizeof(struct CourseLinkList));
	if (link == NULL) { fclose(fp); return NULL; }
	link->prev = NULL;
	link->next = NULL;

	// 定义课程数据变量
	int Max = 0, All[5] = { 0 };
	char Name[37] = { "\0" };

	while (fscanf_s(fp, "%s %d %d %d %d %d %d", Name, 37, &Max, &All[0], &All[1], &All[2], &All[3], &All[4]) != EOF) {

		// 动态申请新结点
		struct CourseLinkList* node = (struct CourseLinkList*)malloc(sizeof(struct CourseLinkList));
		if (node != NULL) {                          
			node->data.Max = Max;
			strcpy_s(node->data.Name, Name);
			memcpy_s(node->data.All, sizeof(int) * 5, All, sizeof(int) * 5);

			if (link->prev == NULL) {
				link->prev = link->next = node;
				node->prev = node->next = NULL;
				continue;
			}
			                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
			link->next->prev = node;
			node->next = link->next;
			link->next = node;
			node->prev = NULL;
		}
	}

	fclose(fp);
	return link;
}

// 打印课程链表到本地
int printCourseLinkList(CourseLinkList* link)
{
	// 打开本地课程文件
	FILE* fp = NULL;
	fopen_s(&fp, "res/data/course.txt", "w");

	if (fp == NULL) {
		freeCourseLinkList(link);
		return 0;
	}

	CourseLinkList* p = link->prev;
	while (p) {
		fprintf_s(fp, "%s %d %d %d %d %d %d\n", p->data.Name, p->data.Max, p->data.All[0], p->data.All[1], p->data.All[2], p->data.All[3], p->data.All[4]);

		p = p->prev;
	}

	fclose(fp);

	freeCourseLinkList(link);

	return 1;
}

// 释放课程列表链表内存
void freeCourseLinkList(CourseLinkList* link)
{
	CourseLinkList* p;
	while (link) {
		p = link->next;
		free(link);
		link = p;
	}
}


/* 已选逻辑函数实现 */

// 读取本地已选数据到链表
SelectionLinkList* readSelectionInfo()
{
	// 打开本地用户文件
	FILE* fp = NULL;
	fopen_s(&fp, "res/data/selection.txt", "a+");

	if (fp == NULL) return NULL;

	// 创建课程双链表
	struct SelectionLinkList* link = (struct SelectionLinkList*)malloc(sizeof(struct SelectionLinkList));
	if (link == NULL) { fclose(fp); return NULL; }
	link->prev = NULL;
	link->next = NULL;

	// 定义课程数据变量
	char Name[37] = { "\0" }, Account[10] = { "\0" };

	while (fscanf_s(fp, "%s %s", Name, 37, Account, 10) != EOF) {

		// 动态申请新结点
		struct SelectionLinkList* node = (struct SelectionLinkList*)malloc(sizeof(struct SelectionLinkList));
		if (node != NULL) {
			strcpy_s(node->data.Name, Name);
			strcpy_s(node->data.Account, Account);

			if (link->prev == NULL) {
				link->prev = link->next = node;
				node->prev = node->next = NULL;
				continue;
			}

			link->next->prev = node;
			node->next = link->next;
			link->next = node;
			node->prev = NULL;
		}
	}

	fclose(fp);
	return link;
}

// 打印已选链表到本地
int printSelectionLinkList(SelectionLinkList* link)
{
	// 打开本地课程文件
	FILE* fp = NULL;
	fopen_s(&fp, "res/data/selection.txt", "w");

	if (fp == NULL) {
		freeSelectionLinkList(link);
		return 0;
	}

	SelectionLinkList* p = link->prev;
	while (p) {
		fprintf_s(fp, "%s %s\n", p->data.Name, p->data.Account);

		p = p->prev;
	}

	fclose(fp);

	freeSelectionLinkList(link);

	return 1;
}

// 释放已选列表链表内存
void freeSelectionLinkList(SelectionLinkList* link)
{
	SelectionLinkList* p;
	while (link) {
		p = link->next;
		free(link);
		link = p;
	}
}

/* 其他小工具 */

// 宽字节转多字节
char* WcharToChar(const wchar_t* wp)

{

	char* m_char;

	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);

	m_char = new char[len + 1];

	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);

	m_char[len] = '\0';

	return m_char;

}

// 统计课程选课情况
int countSelectionNumber() 
{
	CourseLinkList* course = readCourseInfo();
	if (course == NULL) return 0;

	SelectionLinkList* selection = readSelectionInfo();
	if (selection == NULL) return 0;

	SelectionLinkList* ps;
	CourseLinkList* pc = course->next;

	while (pc) {
		pc->data.All[0] = 0;
		pc->data.All[1] = -(pc->data.All[1] == -1);
		pc->data.All[2] = -(pc->data.All[2] == -1);
		pc->data.All[3] = -(pc->data.All[3] == -1);
		pc->data.All[4] = -(pc->data.All[4] == -1);

		ps = selection->next;
		while (ps) {
			if (strcmp(pc->data.Name, ps->data.Name) == 0) {
				int theClass = _ttoi((CString(ps->data.Account).Right(3)).Left(1));
				if (pc->data.All[theClass] != -1) {
					pc->data.All[theClass]++;
					pc->data.All[0]++;
				}

			}

			ps = ps->next;
		}

		pc = pc->next;
	}

	freeSelectionLinkList(selection);
	return printCourseLinkList(course);
}

// 清空限选原已选数据
void clearMaxSelection(CString name,int theClass) {
	SelectionLinkList* selection = readSelectionInfo();
	SelectionLinkList* p = selection->next;
	SelectionLinkList* q;

	while (p) {
		if (name.Compare(CString(p->data.Name)) == 0 && _ttoi((CString(p->data.Account).Right(3)).Left(1)) == theClass) {

			if (p->prev) p->prev->next = p->next;
			if (p->next) p->next->prev = p->prev;
			if (selection->next == p) selection->next = p->next;
			if (selection->prev == p) selection->prev = p->prev;

			q = p->next;
			free(p);
			p = q;

			continue;
		}

		p = p->next;
	}
	
	printSelectionLinkList(selection);
}