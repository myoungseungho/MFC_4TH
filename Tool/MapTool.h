#pragma once
#include "afxwin.h"
#include <atlimage.h>


// CMapTool 대화 상자입니다.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	void LoadImagesFromDirectory(const CString& strDirectory);

private:
	void	Horizontal_Scroll();

public:
	//control
	CListBox m_ListBox;
	CStatic m_Picture;

	map<CString, CImage*>		m_mapPngImg;
	int							m_iDrawID = 0;
	afx_msg void OnSaveData();
	afx_msg void OnStnClickedPicture();
};
