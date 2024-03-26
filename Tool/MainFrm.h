
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "Include.h"
#include "MyForm.h"
#include "MiniView.h"

class CToolView;
class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;
public:
	CToolView* GetToolView() { return (CToolView*)m_MainSplitter.GetPane(0, 1); };
// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	CSplitterWnd		m_MainSplitter;
	CSplitterWnd		m_SecondSplitter;

};


// 1. �̴ϸ��� ����ؿͶ�
// 2. � ������ ���� ������ �� �����ؿͶ�