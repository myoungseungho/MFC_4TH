
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND		g_hWnd;

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() : m_pTerrain(nullptr)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes : 스크롤 바의 사이즈를 지정하는 함수
	// MM_TEXT : 픽셀 단위로 조정하겠다는 옵션

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));


	// AfxGetMainWnd : 현재의 메인 윈도우 정보를 반환하는 전역 함수
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : 현재 윈도우의 RECT 정보를 얻어와 기록하는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수대로 rect 정보를 기록함
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : 현재 view창의 rect 정보를 얻어오는 함수
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 새롭게 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X 좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치 지정 옵션)
	// nullptr : 순서 변경하지 않음, SWP_NOZORDER : 현재 순서를 유지함

	pMainFrm->SetWindowPos(nullptr, 0, 0, int(WINCX + fRowFrm), int(WINCY + fColFrm), SWP_NOZORDER);

	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Device Init Failed");
	}

	//m_pSingle = new CSingleTexture;
	//m_pSingle->Insert_Texture(L"../Texture/Cube.png");

	if (FAILED(CTextureMgr::Get_Instance()->
		Insert_Texture(L"../Texture/Cube.png", TEX_SINGLE, L"Cube")))
	{
		AfxMessageBox(L"Cube Img Insert Failed");
		return;
	}

	m_pTerrain = new CTerrain;
	m_pTerrain->Initialize();

	m_pTerrain->Set_MainView(this);

}


void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

		CDevice::Get_Instance()->Render_Begin();

	m_pTerrain->Render();

	CDevice::Get_Instance()->Render_End();	
}

void CToolView::OnDestroy()
{
	Safe_Delete(m_pTerrain);

	CScrollView::OnDestroy();

	CTextureMgr::Get_Instance()->Destroy_Instance();
	CDevice::Get_Instance()->Destroy_Instance();
}

#pragma region 생략
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion 생략

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool*	pMapTool = &pMyForm->m_MapTool;

	m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0), 
										(float)point.y + GetScrollPos(1),
										0.f), pMapTool->m_iDrawID);


	// AfxGetMainWnd() : 현재 동작하는 쓰레드로부터 wnd를 반환(현재 쓰레드가 메인 쓰레드인 경우 MainFrmWnd를 반환)
	//CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// GetParentFrame : 현재 view창을 구성하는 상위 Framewnd를 반환
	//CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());


	//AfxGetApp : 메인 쓰레드를 갖고 있는 현재 메인 App을 반환

	pMiniView->Invalidate(FALSE);
	
	

	// Invalidate : 호출 시 WM_PAINT와 WM_ERASEBKGND 메세지를 발생
	// 이 때 OnDraw함수를 다시 호출하면서 화면 갱신이 발생

	// FALSE : WM_PAINT만 호출	
	// TRUE : WM_PAINT와 WM_ERASEBKGND메세지를 동시테 발생

	Invalidate(FALSE);
}

// 직선의 방정식을 이용하여 마우스와 타일 충돌을 구현해와라
// 충돌 이후, 원하는 타일 이미지로 교체하라

// y = ax + b	(a의 기울기, b는 y절편)

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CScrollView::OnMouseMove(nFlags, point);

	if (GetAsyncKeyState(VK_LBUTTON))
	{

		CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
		CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

		CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		CMapTool*	pMapTool = &pMyForm->m_MapTool;

		m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0),
											(float)point.y + GetScrollPos(1),
											0.f), pMapTool->m_iDrawID);
		Invalidate(FALSE);
		
		pMiniView->Invalidate(FALSE);
	}
}


