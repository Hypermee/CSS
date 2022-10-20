#include "pch.h"
#include "CSS.h"
#include "Func.h"
#include "Login.h"
#include "Admin.h"
#include "AddEdit.h"
#include "afxdialogex.h"

// Admin 对话框

IMPLEMENT_DYNAMIC(Admin, CDialogEx)

Admin::Admin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Admin, pParent)
{

}

Admin::~Admin()
{
}

void Admin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADMIN, m_list);
	DDX_Control(pDX, IDC_COMBO_ADMIN, m_find);
}

BEGIN_MESSAGE_MAP(Admin, CDialogEx)

	ON_WM_CLOSE()

	ON_BN_CLICKED(IDC_ADMIN_ADD, &Admin::OnBnClickedAdminAdd)
	ON_BN_CLICKED(IDC_ADMIN_EDIT, &Admin::OnBnClickedAdminEdit)
	ON_BN_CLICKED(IDC_ADMIN_FIND, &Admin::OnBnClickedAdminFind)
	ON_BN_CLICKED(IDC_ADMIN_DELETE, &Admin::OnBnClickedAdminDelete)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADMIN, &Admin::OnDClickListAdmin)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ADMIN, &Admin::OnRClickListAdmin)

	ON_COMMAND(ID_32771, &Admin::OnBnClickedAdminAdd)
	ON_COMMAND(ID_32772, &Admin::OnBnClickedAdminEdit)
	ON_COMMAND(ID_32773, &Admin::OnBnClickedAdminDelete)

	ON_CBN_EDITCHANGE(IDC_COMBO_ADMIN, &Admin::OnCbnEditchangeComboAdmin)
END_MESSAGE_MAP()

// Admin 消息处理程序

// 教务界面初始化
BOOL Admin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 课程管理列表初始化
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT); // 设置扩展风格，LVS_EX_GRIDLINES显示网格线、LVS_EX_FULLROWSELECT整行选中风格
	m_list.InsertColumn(0, L"课 程", LVCFMT_LEFT, 125);
	m_list.InsertColumn(1, L"一 班", LVCFMT_CENTER, 75);
	m_list.InsertColumn(2, L"二 班", LVCFMT_CENTER, 75);
	m_list.InsertColumn(3, L"三 班", LVCFMT_CENTER, 75);
	m_list.InsertColumn(4, L"四 班", LVCFMT_CENTER, 75);
	m_list.InsertColumn(5, L"限 选", LVCFMT_CENTER, 74);
	m_list.InsertColumn(6, L"已 选", LVCFMT_CENTER, 74);

	CourseLinkList* course = readCourseInfo();
	RefreshList(course);

	return TRUE;
}

// 刷新课程列表
int Admin::RefreshList(struct CourseLinkList* course)
{
	m_list.DeleteAllItems();

	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		return 0;
	}

	int count = 0;
	CString Max, All[5];
	CourseLinkList* p = course->prev;

	while (p != NULL) {
		p->data.Max == 0 ? Max = L"无" : Max.Format(_T("%d"), p->data.Max);
		All[0].Format(_T("%d"), p->data.All[0]);
		All[1].Format(_T("%d"), p->data.All[1]);
		All[2].Format(_T("%d"), p->data.All[2]);
		All[3].Format(_T("%d"), p->data.All[3]);
		All[4].Format(_T("%d"), p->data.All[4]);

		m_list.InsertItem(count, CString(p->data.Name));
		m_list.SetItemText(count, 1, CString(All[1]));
		m_list.SetItemText(count, 2, CString(All[2]));
		m_list.SetItemText(count, 3, CString(All[3]));
		m_list.SetItemText(count, 4, CString(All[4]));
		m_list.SetItemText(count, 6, CString(All[0]));
		m_list.SetItemText(count, 5, CString(Max));

		p = p->prev;
		count += 1;
	}

	freeCourseLinkList(course);

	return 1;
}

// 关闭返回事件
void Admin::OnClose()
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

// 添加按钮事件
void Admin::OnBnClickedAdminAdd()
{
	AddEdit dlg;
	dlg.mode = 0;

	dlg.DoModal();
	
	CourseLinkList* course = readCourseInfo();
	RefreshList(course);
}

// 修改按钮事件
void Admin::OnBnClickedAdminEdit()
{
	// 先判断是否选中课程数据
	int ID = 0;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"请选中一条课程数据", L"提示", MB_ICONEXCLAMATION);
		return;
	}


	AddEdit dlg;
	ID = (int)m_list.GetNextSelectedItem(pos);

	dlg.mode = 1;

	dlg.Max = m_list.GetItemText(ID, 5);
	dlg.Name = m_list.GetItemText(ID, 0);
	dlg.All[0] = _ttoi(m_list.GetItemText(ID, 6));
	dlg.All[1] = _ttoi(m_list.GetItemText(ID, 1));
	dlg.All[2] = _ttoi(m_list.GetItemText(ID, 2));
	dlg.All[3] = _ttoi(m_list.GetItemText(ID, 3));
	dlg.All[4] = _ttoi(m_list.GetItemText(ID, 4));


	dlg.DoModal();

	CourseLinkList* course = readCourseInfo();
	RefreshList(course);
}

// 删除按钮事件
void Admin::OnBnClickedAdminDelete()
{
	int ID = 0;
	CString Name;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(L"请选中一条课程数据！", L"提示", MB_ICONEXCLAMATION);
		return;
	}

	ID = (int)m_list.GetNextSelectedItem(pos);
	Name = m_list.GetItemText(ID, 0);

	// 首先删除课程文件中的数据
	CourseLinkList* course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		return;
	}

	CourseLinkList* p = course->next;
	while (p) {
		if (Name.Compare(CString(p->data.Name)) == 0) {
			if (p->prev) p->prev->next = p->next;
			if (p->next) p->next->prev = p->prev;
			if (course->next == p) course->next = p->next;
			if (course->prev == p) course->prev = p->prev;

			free(p);

			if (!printCourseLinkList(course)) {
				MessageBox(L"删除课程失败！");
				freeCourseLinkList(course);
				return;
			}

			// 其次删除已选文件中的数据
			SelectionLinkList* selection = readSelectionInfo();
			if (selection == NULL) {
				MessageBox(L"读取本地已选课程数据失败！");
				return;
			}

			SelectionLinkList* ps = selection->next;
			while (ps) {
				if (Name.Compare(CString(ps->data.Name)) == 0) {
					if (ps->prev) ps->prev->next = ps->next;
					if (ps->next) ps->next->prev = ps->prev;
					if (selection->next == ps) selection->next = ps->next;
					if (selection->prev == ps) selection->prev = ps->prev;

					free(ps);
				}

				ps = ps->next;
			}

			printSelectionLinkList(selection);

			m_list.DeleteItem(ID);

			return;
		}

		p = p->next;
	}

}

// 查询按钮事件
void Admin::OnBnClickedAdminFind()
{
	CString name;
	CourseLinkList* course;

	course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"读取本地课程数据失败！");
		return;
	}

	m_find.GetWindowText(name);

	if (name.GetLength() < 1 || name.GetLength() > 18) {
		RefreshList(course);
		return;
	}

	CourseLinkList* p = course->next;

	while (p) {
		if (CString(p->data.Name).Find(name) == -1) {
			if (p->prev) p->prev->next = p->next;
			if (p->next) p->next->prev = p->prev;
			if (course->next == p) course->next = p->next;
			if (course->prev == p) course->prev = p->prev;

			free(p);
		}

		p = p->next;
	}

	RefreshList(course);

}

// 查询编辑框提示
void Admin::OnCbnEditchangeComboAdmin()
{
	CString name;
	CString list;

	m_find.GetWindowText(name);
	int num = m_find.GetCount();

	for (int i = 0; i < num; i++) m_find.DeleteString(num - i - 1);

	if (name.GetLength() < 1 || name.GetLength() > 18) return m_find.ShowDropDown(FALSE);

	for (int i = 0; i < m_list.GetItemCount(); i++) {
		list = m_list.GetItemText(i, 0);
		if (list.Find(name) != -1) m_find.AddString(list);
	}

	m_find.ShowDropDown(m_find.GetCount());

}

// 右击列表弹出菜单
void Admin::OnRClickListAdmin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	POINT pt;
	CMenu menu;
	GetCursorPos(&pt);
	menu.LoadMenuW(IDR_MENU_ADMIN_LIST);
	CMenu* pop = menu.GetSubMenu(0);


	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}

// 双击列表选项修改
void Admin::OnDClickListAdmin(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 先判断是否选中课程数据
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	OnBnClickedAdminEdit();
}
