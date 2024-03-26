// PathFind.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "PathFind.h"
#include "afxdialogex.h"
#include "FileInfo.h"


// CPathFind ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPathFind, CDialog)

CPathFind::CPathFind(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHFIND, pParent)
{

}

CPathFind::~CPathFind()
{
}

void CPathFind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
}


BEGIN_MESSAGE_MAP(CPathFind, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPathFind::OnListBox)
	ON_BN_CLICKED(IDC_BUTTON1, &CPathFind::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON7, &CPathFind::OnLoadData)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CPathFind::OnListBox()
{
	
}

void CPathFind::OnSaveData()
{
	wofstream		fout;

	fout.open(L"../Data/ImgPath.txt", ios::out);

	if (!fout.fail())	// ���� ����
	{
		for (auto& pImgPath : m_Pathlist)
		{
			fout << pImgPath->wstrObjKey << L"|" << pImgPath->wstrStateKey << L"|" << pImgPath->iCount << L"|" << pImgPath->wstrPath << endl;
		}

		fout.close();
	}

	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);

}

void CPathFind::OnLoadData()
{
	wifstream		fin;

	UpdateData(TRUE);
		
	fin.open(L"../Data/ImgPath.txt", ios::in);

	if (!fin.fail())	// ���� ����
	{
		TCHAR		szObjKey[MAX_STR] = L"";
		TCHAR		szStateKey[MAX_STR] = L"";
		TCHAR		szCount[MAX_STR] = L"";
		TCHAR		szPath[MAX_PATH] = L"";

		wstring wstrCombined = L"";

		m_ListBox.ResetContent();

		while (true)
		{
			fin.getline(szObjKey, MAX_STR, '|');
			fin.getline(szStateKey, MAX_STR, '|');
			fin.getline(szCount, MAX_STR, '|');
			fin.getline(szPath, MAX_PATH);

			if (fin.eof())
				break;

			wstrCombined = wstring(szObjKey) + L"|" + szStateKey + L"|" + szCount + L"|" + szPath;
			m_ListBox.AddString(wstrCombined.c_str());
		}

		fin.close();
	}

	UpdateData(FALSE);
	WinExec("notepad.exe ../Data/ImgPath.txt", SW_SHOW);
}

void CPathFind::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_Pathlist.begin(), m_Pathlist.end(), Safe_Delete<IMGPATH*>);
	m_Pathlist.clear();
}

void CPathFind::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	UpdateData(TRUE);

	CDialog::OnDropFiles(hDropInfo);

	TCHAR	szFullPath[MAX_PATH] = L"";

	int iFileCnt = DragQueryFile(hDropInfo, -1, nullptr, 0);

	for (int i = 0; i < iFileCnt; ++i)
	{
		DragQueryFile(hDropInfo, i, szFullPath, MAX_PATH);
		CFileInfo::DirInfoExtraction(szFullPath, m_Pathlist);
	}

	m_ListBox.ResetContent();

	wstring wstrCombined = L"";
	TCHAR	szBuf[MAX_STR] = L"";

	for (auto& pImgPath: m_Pathlist)
	{
		// _itow_s : ������ �����ڵ� ���ڿ��� ��ȯ(10������ ��ȯ�ϰڴٴ� �ǹ�)
		_itow_s(pImgPath->iCount, szBuf, 10);

		wstrCombined = pImgPath->wstrObjKey + L"|" + pImgPath->wstrStateKey + L"|" + szBuf + L"|" + pImgPath->wstrPath;

		m_ListBox.AddString(wstrCombined.c_str());
	}

	UpdateData(FALSE);
}
