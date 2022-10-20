#pragma once
#include "afxdialogex.h"


// Student 对话框

class Student : public CDialogEx
{
	DECLARE_DYNAMIC(Student)

public:
	Student(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Student();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Student };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString title;
	// 课程列表
	CListCtrl m_course;
	// 已选列表
	CListCtrl m_selection;
private:
	// 刷新课程列表
	int RefreshCourseList(CourseLinkList* course);
	int RefreshSelectionList( SelectionLinkList* selection);
public:
	CString Account;
	int theClass;
	afx_msg void OnClose();
	afx_msg void OnBnClickedAdminCancel();
	afx_msg void OnCbnEditchangeComboStudent();
	CComboBox m_choose;

	afx_msg void OnBnClickedStudentSelect();
	afx_msg void OnNMClickListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickListStudentCourse(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListStudentSelection(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListStudentSelection(NMHDR* pNMHDR, LRESULT* pResult);
};
