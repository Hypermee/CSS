/* 用户单链表结构体 */

// 用户数据
struct UserInfo
{
	int ID; // 数据索引，0为教务管理员
	char Name[9]; // 学生姓名
	char Account[10]; // 学生学号
	char Password[19]; // 对应密码，初始化为123456
};

// 用户链表结点
struct UserLinkList
{
	struct UserInfo data; // 用户数据
	struct UserLinkList* next; // 下一个节点
};

/* 用户登录逻辑实现 */

extern UserLinkList* readUserInfo();
extern void freeUserLinkList(UserLinkList* link);

/* 课程双链表结构体 */

// 课程数据
struct CourseInfo
{
	int Max; // 课程限选 0为无限制
	int All[5]; // 课程选课情况，[0]为总人数；
				// 后四个元素代表班级，默认人数为0，若为-1则表明该班级不可选择该课程
	char Name[37]; // 课程名称
};

// 课程链表结点
struct CourseLinkList
{
	struct CourseInfo data; // 数据指针
	struct CourseLinkList* prev; // 上一个结点指针
	struct CourseLinkList* next; // 下一个结点指针
};

extern CourseLinkList* readCourseInfo(); // 读取课程文件数据到链表
extern int printCourseLinkList(CourseLinkList* link); // 打印课程链表到文件
extern void freeCourseLinkList(CourseLinkList* link); // 释放课程链表内存

/* 选课双链表结构体 */

// 选课数据
struct SelectionInfo {
	char Name[37]; // 课程名称
	char Account[10]; // 学生学号
};

// 选课链表结点
struct SelectionLinkList
{
	struct SelectionInfo data; // 数据指针
	struct SelectionLinkList* prev; // 上一个结点指针
	struct SelectionLinkList* next; // 下一个结点指针
};

extern SelectionLinkList* readSelectionInfo(); // 读取选课文件数据到链表
extern int printSelectionLinkList(SelectionLinkList* link); // 打印选课链表到文件
extern void freeSelectionLinkList(SelectionLinkList* link); // 释放课程链表内存

/* 其他小工具 */
extern int countSelectionNumber(); // 统计选课人数到课程数据文件
extern char* WcharToChar(const wchar_t* wp); // 宽字节转多字节，用于输出字符串
extern void clearMaxSelection(CString name,int theClass); // 清空限选课程存在的已选数据