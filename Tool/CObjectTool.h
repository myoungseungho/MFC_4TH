#pragma once
#include "afxdialogex.h"


// CObjectTool 대화 상자

class CObjectTool : public CDialog
{
	DECLARE_DYNAMIC(CObjectTool)

public:
	CObjectTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObjectTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();

private:
	virtual BOOL OnInitDialog();
	void	Horizontal_Scroll();
	void LoadImagesFromDirectory(const CString& strDirectory);
public:
	CListBox m_ListBox;
	int							m_iDrawID = 0;
	CStatic m_Picture;
	afx_msg void OnSaveData();
	map<CString, CImage*>		m_mapPngImg;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
