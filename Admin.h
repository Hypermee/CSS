#pragma once
#include "afxdialogex.h"


// Admin 对话框

class Admin : public CDialogEx
{
	DECLARE_DYNAMIC(Admin)

public:
	Admin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Admin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Admin };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 教务选课版列表
	CListCtrl m_list;
private:
	// 刷新教务课程管理列表
	int RefreshList(struct CourseLinkList* course);
public:
	// 要查询的课程内容
	CComboBox m_find;

	afx_msg void OnClose();
	afx_msg void OnBnClickedAdminAdd();
	afx_msg void OnBnClickedAdminDelete();
	afx_msg void OnBnClickedAdminEdit();

	afx_msg void OnDClickListAdmin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickListAdmin(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedAdminFind();

	afx_msg void OnCbnEditchangeComboAdmin();
};
