#pragma once
#include "afxdialogex.h"


// AddEdit 对话框

class AddEdit : public CDialogEx
{
	DECLARE_DYNAMIC(AddEdit)

public:
	AddEdit(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddEdit();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AddEdit };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	// 添加或修改模式
	int mode; 

	// 限选下拉框操作对象
	CComboBox m_Max; // 课程限选
	CString Name;   // 课程名称
	CString Max;   // 课程限选
	int All[5];   // 课程人数

	// 班级复选框对象
	CButton m_one;
	CButton m_two;
	CButton m_three;
	CButton m_four;

	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
};
