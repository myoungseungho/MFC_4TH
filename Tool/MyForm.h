#pragma once

#include "UnitTool.h"
#include "MapTool.h"
#include "PathFind.h"
#include "CObjectTool.h"

// CMyForm �� ���Դϴ�.

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUnitTool();
	virtual void OnInitialUpdate();
	afx_msg void OnMapTool();
	afx_msg void OnPathFind();
	afx_msg void OnBnClickedButton3();

	CFont		m_Font;		// mfc ���� �����ϴ� ��Ʈ ��ü
	CUnitTool	m_UnitTool;
	CMapTool	m_MapTool;
	CPathFind	m_PathFind;
	CObjectTool m_ObjTool;

};


