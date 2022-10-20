// AddEdit.cpp: 实现文件
//

#include "pch.h"
#include "CSS.h"
#include "Func.h"
#include "Admin.h"
#include "AddEdit.h"
#include "afxdialogex.h"


// AddEdit 对话框

IMPLEMENT_DYNAMIC(AddEdit, CDialogEx)

AddEdit::AddEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AddEdit, pParent)
{

	mode = 0;
	All[0] = 0;
	Name = _T("");
	Max = _T("");
	//  OldName = _T("");
}

AddEdit::~AddEdit()
{
}

void AddEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ADDEDIT_MAX, m_Max);
	DDX_Control(pDX, IDC_CHECK_ADDEDIT_ONE, m_one);
	DDX_Control(pDX, IDC_CHECK_ADDEDIT_TWO, m_two);
	DDX_Control(pDX, IDC_CHECK_ADDEDIT_THREE, m_three);
	DDX_Control(pDX, IDC_CHECK_ADDEDIT_FOUR, m_four);
}


BEGIN_MESSAGE_MAP(AddEdit, CDialogEx)
	ON_BN_CLICKED(IDOK_AddEdit, &AddEdit::OnBnClickedYes)
	ON_BN_CLICKED(IDCANCEL_AddEdit, &AddEdit::OnBnClickedNo)
END_MESSAGE_MAP()

// AddEdit 消息处理程序

// 添加修改对话框初始化
BOOL AddEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	// 0为添加，1为修改
	if (mode == 0) {
		// 设置对话框标题
		SetWindowText(L"添加课程界面");

		// 限选下拉框默认
		m_Max.SetCurSel(m_Max.FindStringExact(-1, _T("无")));

		// 复选框默认全选
		((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_ONE))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_TWO))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_FOUR))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_THREE))->SetCheck(TRUE);
	}
	else {
		// 设置对话框标题
		SetWindowText(L"修改课程界面");

		// 设置编辑框
		GetDlgItem(IDC_EDIT_ADDEDIT_COURSE)->SetWindowText(Name);

		// 限选下拉框
		m_Max.SetCurSel(m_Max.FindStringExact(-1, Max) == -1 ? 0 : m_Max.FindStringExact(-1, Max));

		// 复选框选中
		if (All[1] >= 0) ((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_ONE))->SetCheck(TRUE);
		if (All[2] >= 0) ((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_TWO))->SetCheck(TRUE);
		if (All[4] >= 0) ((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_FOUR))->SetCheck(TRUE);
		if (All[3] >= 0) ((CButton*)GetDlgItem(IDC_CHECK_ADDEDIT_THREE))->SetCheck(TRUE);
	}

	return TRUE;
}

// 确认执行添加或修改功能
void AddEdit::OnBnClickedYes()
{
	CString name;
	GetDlgItem(IDC_EDIT_ADDEDIT_COURSE)->GetWindowText(name);

	m_Max.GetCurSel() == 0 ? Max.Format(L"%d", 0) : m_Max.GetWindowText(Max);

	if (_ttoi(Max) < 0 || _ttoi(Max) > 999) {
		MessageBox(L"限选人数不合法！");
		return;
	}

	if (m_one.GetCheck() + m_two.GetCheck() + m_three.GetCheck() + m_four.GetCheck() == 0) {
		MessageBox(L"请至少选择一个班级！");
		return;
	}

	if (name.GetLength() < 2 || name.GetLength() > 18) {
		MessageBox(L"课程名称请保持在2-18个字符！");
		return;
	}

	CourseLinkList* course = readCourseInfo();
	if (course == NULL) {
		MessageBox(L"本地课程文件打开失败！");
		return;
	}

	CourseLinkList* p = course->next;

	// 0为添加，1为修改
	if (mode == 0) {
		All[1] = m_one.GetCheck() - 1;
		All[2] = m_two.GetCheck() - 1;
		All[4] = m_four.GetCheck() - 1;
		All[3] = m_three.GetCheck() - 1;

		while (p) {
			if (name.Compare(CString(p->data.Name)) == 0)
			{
				MessageBox(L"课程已存在，请勿重复添加！");
				freeCourseLinkList(course);
				return;
			}
			p = p->next;
		}

		// 打开本地用户文件
		FILE* fp = NULL;
		fopen_s(&fp, "res/data/course.txt", "a");

		if (fp == NULL) {
			MessageBox(L"本地课程文件打开失败！");
			return;
		}

		// 添加课程数据
		fprintf_s(fp, "\r%s %d %d %d %d %d %d", WcharToChar(name), _ttoi(Max), All[0], All[1], All[2], All[3], All[4]);

		fclose(fp);

		CDialog::OnOK();
	}
	else {
		int all[5] = { 0 };

		// 修改课程
		all[1] = m_one.GetCheck() == 0 ? -1 : All[1] == -1 ? 0 : All[1];
		all[2] = m_two.GetCheck() == 0 ? -1 : All[2] == -1 ? 0 : All[2];
		all[4] = m_four.GetCheck() == 0 ? -1 : All[4] == -1 ? 0 : All[4];
		all[3] = m_three.GetCheck() == 0 ? -1 : All[3] == -1 ? 0 : All[3];
		all[0] = (all[1] == -1) + (all[2] == -1) + (all[3] == -1) + (all[4] == -1) + all[1] + all[2] + all[3] + all[4];

		while (p) {
			if (name.Compare(CString(p->data.Name)) == 0)
			{
				MessageBox(L"该课程已存在，请重新命名！");
				freeCourseLinkList(course);
				return;
			}
			p = p->next;
		}

		p = course->next;
		while (p) {
			if (Name.Compare(CString(p->data.Name)) == 0)
			{
				p->data.Max = _ttoi(Max);
				strcpy_s(p->data.Name, WcharToChar(name));
				memcpy_s(p->data.All, sizeof(int) * 5, all, sizeof(int) * 5);

				if (!printCourseLinkList(course)) {
					MessageBox(L"修改失败！");
					return;
				}

				// 清空限选数据
				for (int i = 1; i < 5; i++) {
					if (all[i] == -1 && All[i] > 0) {
						clearMaxSelection(Name, i);
					}
				}
	
				if (name.Compare(Name) != 0) {
					SelectionLinkList* selection = readSelectionInfo();
					SelectionLinkList* ps = selection->next;

					while (ps) {
						if (Name.Compare(CString(ps->data.Name)) == 0)
						{
							strcpy_s(ps->data.Name, WcharToChar(name));
						}

						ps = ps->next;
					}

					printSelectionLinkList(selection);
				}

				CDialog::OnOK();

				return;
			}
			p = p->next;
		}

		MessageBox(L"课程不存在，请重新选择！");
	}

	return freeCourseLinkList(course);
}

// 取消返回教务选课管理界面
void AddEdit::OnBnClickedNo()
{
	CDialog::OnOK();
}




