// Student.cpp: 实现文件
//

#include "pch.h"
#include "CSS.h"
#include "Func.h"
#include "Login.h"
#include "Student.h"
#include "afxdialogex.h"


// Student 对话框

IMPLEMENT_DYNAMIC(Student, CDialogEx)

Student::Student(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Student, pParent)
{
	Account = _T("");
	theClass = 0;
}

Student::~Student()
{
}

void Student::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STUDENT_COURSE, m_course);
	DDX_Control(pDX, IDC_LIST_STUDENT_SELECTION, m_selection);
	DDX_Control(pDX, IDC_COMBO_STUDENT, m_choose);
}


BEGIN_MESSAGE_MAP(Student, CDialogEx)

	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_ADMIN_CANCEL, &Student::OnBnClickedAdminCancel)
	ON_BN_CLICKED(IDC_STUDENT_SELECT, &Student::OnBnClickedStudentSelect)
	ON_CBN_EDITCHANGE(IDC_COMBO_STUDENT, &Student::OnCbnEditchangeComboStudent)

	ON_NOTIFY(NM_CLICK, IDC_LIST_STUDENT_COURSE, &Student::OnNMClickListStudentCourse)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STUDENT_COURSE, &Student::OnNMRClickListStudentCourse)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STUDENT_COURSE, &Student::OnNMDblclkListStudentCourse)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STUDENT_SELECTION, &Student::OnNMClickListStudentSelection)

	ON_COMMAND(ID_32774, &Student::OnBnClickedStudentSelect)
	ON_COMMAND(ID_32775, &Student::OnBnClickedAdminCancel)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STUDENT_SELECTION, &Student::OnNMDblclkListStudentSelection)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Student 消息处理程序

// Student对话框初始化
BOOL Student::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(title);

	// 课程列表初始化
	m_course.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_course.InsertColumn(0, L"课 程", LVCFMT_LEFT, 425);
	m_course.InsertColumn(1, L"限 选", LVCFMT_CENTER, 74);
	m_course.InsertColumn(2, L"已 选", LVCFMT_CENTER, 74);
	
	CourseLinkList* course = readCourseInfo();
	RefreshCourseList(course);

	// 已选列表初始化
	m_selection.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); 
	m_selection.InsertColumn(0, L"课 程", LVCFMT_LEFT, 425);
	m_selection.InsertColumn(1, L"限 选", LVCFMT_CENTER, 74);
	m_selection.InsertColumn(2, L"已 选", LVCFMT_CENTER, 74);

	SelectionLinkList* selection = readSelectionInfo();
	RefreshSelectionList(selection);


	return TRUE;
}


// 刷新课程列表
int Student::RefreshCourseList(CourseLinkList* course)
{
	m_course.DeleteAllItems();

	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		return 0;
	}

	int count = 0;
	CString Max, All[5];
	CourseLinkList* p = course->prev;

	while (p != NULL) {
		// 判断是否限选
		if (p->data.All[theClass] != -1) {
			p->data.Max == 0 ? Max = L"无" : Max.Format(_T("%d"), p->data.Max);
			All[0].Format(_T("%d"), p->data.All[0]);

			m_course.InsertItem(count, CString(p->data.Name));
			m_course.SetItemText(count, 2, CString(All[0]));
			m_course.SetItemText(count, 1, CString(Max));

			count++;
		}

		p = p->prev;
	}

	freeCourseLinkList(course);

	return 1;
}

// 刷新已选列表
int Student::RefreshSelectionList(SelectionLinkList* selection)
{
    m_selection.DeleteAllItems();

	if (selection == NULL) {
		MessageBox(L"读取本地已选课程数据失败！");
		return 0;
	}

	int error = 0;
	int count = 0;
	SelectionLinkList* p = selection->prev;

	CourseLinkList* pc;
	CourseLinkList* course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		return 0;
	}


	while (p != NULL) {
		// 找到学号已选课程
		if (Account.Compare(CString(p->data.Account)) == 0) {
			CString name = CString(p->data.Name);

			// 查找课程数据，插入到列表
			pc = course->next;
			while (pc) {
				if (name.Compare(CString(pc->data.Name)) == 0) {

					CString Max, All[5];
					pc->data.Max == 0 ? Max = L"无" : Max.Format(_T("%d"), pc->data.Max);
					All[0].Format(_T("%d"), pc->data.All[0]);

					m_selection.InsertItem(count, CString(pc->data.Name));
					m_selection.SetItemText(count, 2, CString(All[0]));
					m_selection.SetItemText(count, 1, CString(Max));

					count++;

					break;
				}

				pc = pc->next;
			}

			// 有已选课程，但是没有找到课程数据
			if (pc == NULL) {

				if (p->prev) p->prev->next = p->next;
				if (p->next) p->next->prev = p->prev;
				if (selection->next == p) selection->next = p->next;
				if (selection->prev == p) selection->prev = p->prev;
				
				pc = (CourseLinkList*)p;
				p = ((SelectionLinkList*)pc)->prev;
				free(pc);

				error++;

				continue;
			}
		}

		p = p->prev;
	}

	freeCourseLinkList(course);
	error ? printSelectionLinkList(selection) : freeSelectionLinkList(selection);

	return 1;
}

// 关闭返回
void Student::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int ret = MessageBox(L"将返回登录界面，请问是否立即退出？", L"关闭提示", MB_YESNO);

	if (ret == IDYES)
	{
		CDialogEx::OnClose();
	}
	else
	{
		// 返回登录界面
		Login dlg;

		CDialog::OnOK();
		dlg.DoModal();
	}
}


// 选课按钮事件
void Student::OnBnClickedStudentSelect()
{

	CString name;
	m_choose.GetWindowText(name);

	if (name.GetLength() < 2 || name.GetLength() > 18)
	{
		MessageBox(L"请选中一条已选列表中的课程数据", L"提示", MB_ICONEXCLAMATION);
		return;
	}

	SelectionLinkList* selection = readSelectionInfo();
	if (selection == NULL) {
		MessageBox(L"读取本地已选课程数据失败！");
		return;
	}
	SelectionLinkList* ps = selection->next;

	CourseLinkList* course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		freeSelectionLinkList(selection);
		return;
	}

	CourseLinkList* pc = course->next;


	while (ps) {
		if (name.Compare(CString(ps->data.Name)) == 0) {
			if (Account.Compare(CString(ps->data.Account)) == 0) {
				MessageBox(L"你已选择该课程，请勿重复操作！");
				break;
			}
		}

		ps = ps->next;
	}

	pc = course->next;
	while (pc && ps == NULL) {
		// 找到指定课程，选课人数加一
		if (name.Compare(CString(pc->data.Name)) == 0) {

			if (pc->data.All[theClass] == -1) {
				MessageBox(L"你所在班级被限选！");
				break;
			}

			if (pc->data.Max != 0 && pc->data.All[0] >= pc->data.Max) {
				MessageBox(L"该课程已选满，请选择其他课程！");
				break;
			}

			// 打开本地选课文件，添加选课数据
			FILE* fp = NULL;
			fopen_s(&fp, "res/data/selection.txt", "a");

			if (fp == NULL) {
				MessageBox(L"本地选课文件打开失败！");
				break;
			}

			// 添加选课数据
			fprintf_s(fp, "\r%s %s", WcharToChar(name), WcharToChar(Account));

			fclose(fp);

			// 选课数据增加
			pc->data.All[0]++;
			pc->data.All[theClass]++;
			printCourseLinkList(course);
			
			freeSelectionLinkList(selection);

			// 刷新列表
			course = readCourseInfo();
			RefreshCourseList(course);
			selection = readSelectionInfo();
			RefreshSelectionList(selection);

			return;
		}

		pc = pc->next;
	}

	if (pc == NULL) MessageBox(L"不存在该课程，请重新选择！");

	freeCourseLinkList(course);
	freeSelectionLinkList(selection);
}

// 退选按钮事件
void Student::OnBnClickedAdminCancel()
{
	int ID = 0;
	CString name;
	m_choose.GetWindowText(name);

	if (name.GetLength() < 2 || name.GetLength() > 18)
	{
		MessageBox(L"请选中一条已选列表中的课程数据", L"提示", MB_ICONEXCLAMATION);
		return;
	}

	SelectionLinkList* selection = readSelectionInfo();
	if (selection == NULL) {
		MessageBox(L"读取本地已选课程数据失败！");
		return;
	}
	SelectionLinkList* ps = selection->next;

	CourseLinkList* course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		freeSelectionLinkList(selection);
		return;
	}

	CourseLinkList* pc = course->next;

	while (ps) {
		// 找到已选课程
		if (Account.Compare(CString(ps->data.Account)) == 0 && name.Compare(CString(ps->data.Name)) == 0) {
			while (pc) {
				// 找到该课程，人数减一
				if (name.Compare(CString(pc->data.Name)) == 0) {
					if (pc->data.All[theClass] <= 0 || pc->data.All[0] <= 0) {
						MessageBox(L"数据异常代码-FailX001！");
						return;
					}

					pc->data.All[theClass]--;
					pc->data.All[0]--;

					if (ps->prev) ps->prev->next = ps->next;
					if (ps->next) ps->next->prev = ps->prev;
					if (selection->next == ps) selection->next = ps->next;
					if (selection->prev == ps) selection->prev = ps->prev;
					free(ps);

					// 保存失败
					if (printSelectionLinkList(selection) == 0 || printCourseLinkList(course) == 0) {
						countSelectionNumber() ? MessageBox(L"数据异常代码-FailX002！") : MessageBox(L"退选课程失败！");
					}

					// 刷新列表
					course = readCourseInfo();
					RefreshCourseList(course);
					selection = readSelectionInfo();
					RefreshSelectionList(selection);

					return;
				}

				pc = pc->next;
			}

			return;
		}

		ps = ps->next;
	}

	MessageBox(L"你未选择该课程！");
}


// 选课编辑框改变提示
void Student::OnCbnEditchangeComboStudent()
{
	CString name;
	CString list;

	m_choose.GetWindowText(name);
	int num = m_choose.GetCount();

	for (int i = 0; i < num; i++) m_choose.DeleteString(num - i - 1);

	if (name.GetLength() < 1 || name.GetLength() > 18) return m_choose.ShowDropDown(FALSE);

	for (int i = 0; i < m_course.GetItemCount(); i++) {
		list = m_course.GetItemText(i, 0);
		if (list.Find(name) != -1) m_choose.AddString(list);
	}

	m_choose.ShowDropDown(m_choose.GetCount());
}


// 点击课程列表选中
void Student::OnNMClickListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int ID = 0;
	CString name;
	POSITION pos = m_course.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	ID = (int)m_course.GetNextSelectedItem(pos);
	name = m_course.GetItemText(ID, 0);

	m_choose.SetWindowText(name);

}

// 点击已选列表选中
void Student::OnNMClickListStudentSelection(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int ID = 0;
	CString name;
	POSITION pos = m_selection.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	ID = (int)m_selection.GetNextSelectedItem(pos);
	name = m_selection.GetItemText(ID, 0);

	m_choose.SetWindowText(name);
}


// 双击课程列表直接选课
void Student::OnNMDblclkListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 判断是否选择选中
	POSITION pos = m_course.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	OnBnClickedStudentSelect();
}

// 右击课程列表弹出菜单
void Student::OnNMRClickListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int ID = 0;
	CString name;

	// 判断是否选择选中
	POSITION pos = m_course.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	ID = (int)m_course.GetNextSelectedItem(pos);
	name = m_course.GetItemText(ID, 0);

	m_choose.SetWindowText(name);

	POINT pt;
	CMenu menu;
	GetCursorPos(&pt);
	menu.LoadMenuW(IDR_MENU_STUDENT_LIST);
	CMenu* pop = menu.GetSubMenu(0);


	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}


// 双击已选列表直接退课
void Student::OnNMDblclkListStudentSelection(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 判断是否选择选中
	POSITION pos = m_selection.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	OnBnClickedAdminCancel();
}
