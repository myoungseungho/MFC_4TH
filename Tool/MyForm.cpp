// MyForm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyForm.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_MYFORM)
{

}

CMyForm::~CMyForm()
{
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyForm::OnUnitTool)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyForm::OnMapTool)
	ON_BN_CLICKED(IDC_BUTTON8, &CMyForm::OnPathFind)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedButton3)
END_MESSAGE_MAP()


// CMyForm 진단입니다.

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG




void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// CreatePointFont(폰트 크기, 글씨체)
	m_Font.CreatePointFont(180, L"궁서");

	GetDlgItem(IDC_BUTTON1)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON7)->SetFont(&m_Font);
	GetDlgItem(IDC_BUTTON8)->SetFont(&m_Font);
}

void CMyForm::OnUnitTool()
{
	// GetSafeHwnd : 현재 다이얼로그 윈도우 핸들을 반환

	if(nullptr == m_UnitTool.GetSafeHwnd())
		m_UnitTool.Create(IDD_UNITTOOL);	 // 해당 id에 맞는 다이얼로그 생성

	m_UnitTool.ShowWindow(SW_SHOW);		 // 창 모양으로 출력
	
}


void CMyForm::OnMapTool()
{

	if (nullptr == m_MapTool.GetSafeHwnd())
		m_MapTool.Create(IDD_MAPTOOL);	 // 해당 id에 맞는 다이얼로그 생성

	m_MapTool.ShowWindow(SW_SHOW);
}


void CMyForm::OnPathFind()
{
	if (nullptr == m_PathFind.GetSafeHwnd())
		m_PathFind.Create(IDD_PATHFIND);	 // 해당 id에 맞는 다이얼로그 생성

	m_PathFind.ShowWindow(SW_SHOW);
}


void CMyForm::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == m_ObjTool.GetSafeHwnd())
		m_ObjTool.Create(IDD_OBJECTTOOL);	 // 해당 id에 맞는 다이얼로그 생성

	m_ObjTool.ShowWindow(SW_SHOW);
}
