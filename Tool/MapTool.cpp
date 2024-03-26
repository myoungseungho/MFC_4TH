// MapTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"


// CMapTool ��ȭ �����Դϴ�.

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

	// ���⿡ �ʱ�ȭ �ڵ� �߰�
	LoadImagesFromDirectory(L"C:\\Users\\���ȣ\\Desktop\\���\\4��������\\Texture\\Stage\\BackGround\\BackGround"); // ��δ� �����Դϴ�. ���� ��η� �����ϼ���.
	return TRUE;
}

void CMapTool::LoadImagesFromDirectory(const CString& strDirectory)
{
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDirectory + L"\\*.png"); // PNG ���ϸ� ã���ϴ�.

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		if (finder.IsDots() || finder.IsDirectory())
			continue;

		CString strFilePath = finder.GetFilePath();
		CString strFileName = finder.GetFileName();

		// ������ Ȯ���� ���� �߶󳻴� �Լ�
		PathRemoveExtension(strFileName.GetBuffer());
		strFileName.ReleaseBuffer();

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage* pPngImg = new CImage;

			pPngImg->Load(strFilePath); // �ش� ����� �̹����� �ε�

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


// CMapTool �޽��� ó�����Դϴ�.


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
		// isdigit : �Ű� ������ ���޹��� ���ڰ� ���� ������ �������� ���� ������ �������� �Ǻ�
		// ���� ������ ���ڶ� �Ǹ�Ǹ� 0�� �ƴ� ���� ��ȯ

		if (0 != isdigit(strSelectName[i]))
			break;
	}

	strSelectName.Delete(0, i);

	//_tstoi : ���ڸ� ���������� ��ȯ�ϴ� �Լ�
	m_iDrawID = _tstoi(strSelectName);
	
	UpdateData(FALSE);
}


void CMapTool::OnDropFiles(HDROP hDropInfo)
{
	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFilePath[MAX_PATH] = L"";

	// DragQueryFile : ��ӵ� ������ ������ ������ �Լ�(���� ��θ� ����)
	// �� ��° �Ű� ������ ���� -1(0xffffffff)�� ��� ��ӵ� ������ ������ ��ȯ

	int		iFileCnt = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR	szFileName[MAX_STR] = L"";

	for (int i = 0; i < iFileCnt; ++i)
	{
		// ��ӵ� ������ ��θ� ������
		DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

		CString	strRelativePath = CFileInfo::ConvertRelativePath(szFilePath);
		
		// ��� �� ������ �̸��� �߶󳻴� �Լ�
		CString	strFileName = PathFindFileName(strRelativePath);

		lstrcpy(szFileName, strFileName.GetString());

		// ������ Ȯ���� ���� �߶󳻴� �Լ�
		PathRemoveExtension(szFileName);

		// Ȯ���ڸ��� �߸� ���� �̸��� CString�� ����
		strFileName = szFileName;

		auto iter = m_mapPngImg.find(strFileName);

		if (iter == m_mapPngImg.end())
		{
			CImage*		pPngImg = new CImage;

			pPngImg->Load(strRelativePath);	// �ش� ����� �̹����� �ε�

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

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CMapTool::Horizontal_Scroll()
{
	CString	StrName;
	CSize	Size;

	int iWidth(0);

	CDC*	pDC = m_ListBox.GetDC();

	for (int i = 0; i < m_ListBox.GetCount(); ++i)
	{
		// GetText : �ε����� �ش��ϴ� ����� ���ڿ��� ����
		m_ListBox.GetText(i, StrName);

		// GetTextExtent : ���� ���ڿ��� ���̸� �ȼ� ������ ��ȯ
		Size = pDC->GetTextExtent(StrName);

		if (Size.cx > iWidth)
			iWidth = Size.cx;
	}

	m_ListBox.ReleaseDC(pDC);

	// GetHorizontalExtent : ����Ʈ �ڽ��� ���η� ��ũ�� �� �� �ִ� �ִ� ������ ������ �Լ�
	if (iWidth > m_ListBox.GetHorizontalExtent())
		m_ListBox.SetHorizontalExtent(iWidth);

}


void CMapTool::OnSaveData()
{
	CFileDialog		Dlg(FALSE,		// FALSE(�ٸ� �̸����� ����), TRUE(�ҷ�����)
		L"dat",		// default ���� Ȯ���ڸ�
		L"*.dat",	// ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,  // OFN_HIDEREADONLY(�б� ���� üũ �ڽ� ���� �ɼ�) | OFN_OVERWRITEPROMPT(�ߺ� ������ ����� ���� ��, ��� �޼��� ���� �ɼ�)
		L"Data Files(*.dat) | *.dat||", // ��ȭ ���ڿ� ǥ�õ� ���� ���� "�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
		this);	// �θ� ������ �ּ�

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
	// ����Ʈ �ڽ����� ���õ� �������� �ε����� ����ϴ�.
	int iSelectIdx = m_ListBox.GetCurSel();
	if (LB_ERR == iSelectIdx)
		return;

	CString strSelectName;
	m_ListBox.GetText(iSelectIdx, strSelectName);

	// ���� �����ӿ� �����մϴ�.
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame != nullptr)
	{
		// ToolView�� �����մϴ�.
		CToolView* pToolView = pMainFrame->GetToolView();
		if (pToolView != nullptr)
		{
			// Terrain�� ���õ� �̹��� ����
			CTerrain* pTerrain = pToolView->m_pTerrain;
			if (pTerrain != nullptr)
			{
				pTerrain->SetSelectedImage(strSelectName);
			}
		}
	}

	CDialog::OnOK(); // �⺻ OK ó���� �����մϴ�.
}
