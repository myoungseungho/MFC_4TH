
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView() : m_pTerrain(nullptr)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// SetScrollSizes : ��ũ�� ���� ����� �����ϴ� �Լ�
	// MM_TEXT : �ȼ� ������ �����ϰڴٴ� �ɼ�

	SetScrollSizes(MM_TEXT, CSize(TILEX * TILECX, TILEY * TILECY / 2));


	// AfxGetMainWnd : ������ ���� ������ ������ ��ȯ�ϴ� ���� �Լ�
	CMainFrame*		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT	rcWnd{};

	// GetWindowRect : ���� �������� RECT ������ ���� ����ϴ� �Լ�
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : ������ �Ű� ������� rect ������ �����
	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	RECT	rcMainView{};

	// GetClientRect : ���� viewâ�� rect ������ ������ �Լ�
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : �Ű� ������� ���Ӱ� ������ ��ġ�� ũ�⸦ �����ϴ� �Լ�
	// SetWindowPos(��ġ�� �������� Z������ ���� ������, X ��ǥ, Y��ǥ, ���� ũ��, ���� ũ��, ��ġ�� �������� ũ�� �� ��ġ ���� �ɼ�)
	// nullptr : ���� �������� ����, SWP_NOZORDER : ���� ������ ������

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

#pragma region ����
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����

#pragma endregion ����

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CScrollView::OnLButtonDown(nFlags, point);

	CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CMiniView*	pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	CMyForm*	pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	CMapTool*	pMapTool = &pMyForm->m_MapTool;

	m_pTerrain->Tile_Change(D3DXVECTOR3((float)point.x + GetScrollPos(0), 
										(float)point.y + GetScrollPos(1),
										0.f), pMapTool->m_iDrawID);


	// AfxGetMainWnd() : ���� �����ϴ� ������κ��� wnd�� ��ȯ(���� �����尡 ���� �������� ��� MainFrmWnd�� ��ȯ)
	//CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());

	// GetParentFrame : ���� viewâ�� �����ϴ� ���� Framewnd�� ��ȯ
	//CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());


	//AfxGetApp : ���� �����带 ���� �ִ� ���� ���� App�� ��ȯ

	pMiniView->Invalidate(FALSE);
	
	

	// Invalidate : ȣ�� �� WM_PAINT�� WM_ERASEBKGND �޼����� �߻�
	// �� �� OnDraw�Լ��� �ٽ� ȣ���ϸ鼭 ȭ�� ������ �߻�

	// FALSE : WM_PAINT�� ȣ��	
	// TRUE : WM_PAINT�� WM_ERASEBKGND�޼����� ������ �߻�

	Invalidate(FALSE);
}

// ������ �������� �̿��Ͽ� ���콺�� Ÿ�� �浹�� �����ؿͶ�
// �浹 ����, ���ϴ� Ÿ�� �̹����� ��ü�϶�

// y = ax + b	(a�� ����, b�� y����)

void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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


