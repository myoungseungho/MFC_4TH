#pragma once
#include "afxwin.h"
#include <atlimage.h>


// CMapTool ��ȭ �����Դϴ�.

class CMapTool : public CDialog
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnListBox();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
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
};
