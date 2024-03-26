// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialog)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MAPTOOL, pParent)
{

}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_PICTURE, m_Picture);
}

BOOL CMapTool::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 여기에 초기화 코드 추가
	LoadImagesFromDirectory(L"C:\\Users\\명승호\\Desktop\\쥬신\\4차팀과제\\Texture\\Stage\\BackGround\\BackGround"); // 경로는 예시입니다. 실제 경로로 변경하세요.
	return TRUE;
}

void CMapTool::LoadImagesFromDirectory(const CString& strDirectory)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDirectory + L"\\*.png"); // PNG 파일만 찾습니다.

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() || finder.IsDirectory())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();

		// 파일의 확장자 명을 잘라내는 함수
		PathRemoveExtension(strFileName.GetBuffer());
		strFileName.ReleaseBuffer();

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;

			pPngImg->Load(strFilePath); // 해당 경로의 이미지를 로드

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(strFileName);
		}
	}
}

BEGIN_MESSAGE_MAP(CMapTool, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMapTool::OnListBox)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnSaveData)
	ON_BN_CLICKED(IDOK, &CMapTool::OnBnClickedOk)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


void CMapTool::OnListBox()
{
	UpdateData(TRUE);


	CString		strSelectName;

	int	iSelectIdx = m_ListBox.GetCurSel();

	if (LB_ERR == iSelectIdx)
		return;

	m_ListBox.GetText(iSelectIdx, strSelectName);

	auto	iter = m_mapPngImg.find(strSelectName);

	if (iter == m_mapPngImg.end())
		return;

	m_Picture.SetBitmap(*(iter->second));

	int i(0);

	for (; i < strSelectName.GetLength(); ++i)
	{
		// isdigit : 매개 변수로 전달받은 글자가 문자 형태의 글자인지 숫자 형태의 글자인지 판별
		// 숫자 형태의 글자라 판명되면 0이 아닌 값을 반환

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	strSelectName.Delete(0, i);

	//_tstoi : 문자를 정수형으로 변환하는 함수
	m_iDrawID = _tstoi(strSelectName);
	
	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : 드롭된 파일의 정보를 얻어오는 함수(절대 경로를 얻어옴)
	// 두 번째 매개 변수의 값이 -1(0xffffffff)인 경우 드롭된 파일의 개수를 반환

	int		iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR	szFileName[MAX_STR] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		// 드롭된 파일의 경로를 추출함
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
		
		// 경로 중 파일의 이름만 잘라내는 함수
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		// 파일의 확장자 명을 잘라내는 함수
		PathRemoveExtension(szFileName);

		// 확장자명이 잘린 파일 이름을 CString에 대입
		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelativePath);	// 해당 경로의 이미지를 로드

			m_mapPngImg.insert({ strFileName, pPngImg });
			m_ListBox.AddString(strFileName);
		}
	}

	Horizontal_Scroll();

	UpdateData(FALSE);
}


void CMapTool::OnDestroy()
{

	for_each(m_mapPngImg.begin(), m_mapPngImg.end(), [](auto& MyPair)
	{
		MyPair.second->Destroy();
		Safe_Delete(MyPair.second);
	});

	m_mapPngImg.clear();

	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMapTool::Horizontal_Scroll()
{
	CString	StrName;
	CSize	Size;

	int iWidth(0);

	CDC*	pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		// GetText : 인덱스에 해당하는 목록의 문자열을 얻어옴
		m_ListBox.GetText(i, StrName);

		// GetTextExtent : 현재 문자열의 길이를 픽셀 단위로 변환
		Size = pDC->GetTextExtent(StrName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : 리스트 박스가 가로로 스크롤 할 수 있는 최대 범위를 얻어오는 함수
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}


void CMapTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,		// FALSE(다른 이름으로 저장), TRUE(불러오기)
		L"dat",		// default 파일 확장자명
		L"*.dat",	// 대화 상자에 표시될 최초 파일명
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,  // OFN_HIDEREADONLY(읽기 전용 체크 박스 숨김 옵션) | OFN_OVERWRITEPROMPT(중복 파일을 덮어쓰기 저장 시, 경고 메세지 띄우는 옵션)
		L"Data Files(*.dat) | *.dat||", // 대화 상자에 표시될 파일 형식 "콤보 박스에 출력될 문자열 | 실제 사용할 필터링 문자열 ||"
		this);	// 부모 윈도우 주소

	TCHAR	szPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szPath);

	PathRemoveFileSpec(szPath);


	lstrcat(szPath, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szPath;

	if (IDOK == Dlg.DoModal())
	{
		CString			str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE	hFile = CreateFile(pGetPath,
			GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD	dwByte(0);

		CMainFrame*	pMainFrm = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		CToolView*	pMainView = dynamic_cast<CToolView*>(pMainFrm->m_MainSplitter.GetPane(0, 1));

		CTerrain*	pTerrain = pMainView->m_pTerrain;

		vector<TILE*>&	vecTile = pTerrain->Get_VecTile();

		if (vecTile.empty())
			return;

		for (auto& pTile : vecTile)
			WriteFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}

void CMapTool::OnBnClickedOk()
{
	// 리스트 박스에서 선택된 아이템의 인덱스를 얻습니다.
	int iSelectIdx = m_ListBox.GetCurSel();
	if (LB_ERR == iSelectIdx)
		return;

	CString strSelectName;
	m_ListBox.GetText(iSelectIdx, strSelectName);

	// 메인 프레임에 접근합니다.
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame != nullptr)
	{
		// ToolView에 접근합니다.
		CToolView* pToolView = pMainFrame->GetToolView();
		if (pToolView != nullptr)
		{
			// Terrain에 선택된 이미지 설정
			CTerrain* pTerrain = pToolView->m_pTerrain;
			if (pTerrain != nullptr)
			{
				pTerrain->SetSelectedImage(strSelectName);
			}
		}
	}

	CDialog::OnOK(); // 기본 OK 처리를 실행합니다.
}
