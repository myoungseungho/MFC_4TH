// UnitTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "UnitTool.h"
#include "afxdialogex.h"


// CUnitTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUnitTool, CDialog)

CUnitTool::CUnitTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_UNITTOOL, pParent)
	, m_strTemp(_T(""))
	, m_strCopy(_T(""))
	, m_strName(_T(""))
	, m_iHp(0)
	, m_iAttack(0)
	, m_strFindName(_T(""))
{

}

CUnitTool::~CUnitTool()
{
}

void CUnitTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTemp);
	DDX_Text(pDX, IDC_EDIT2, m_strCopy);
	DDX_Text(pDX, IDC_EDIT3, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_iHp);
	DDX_Text(pDX, IDC_EDIT5, m_iAttack);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_Radio[0]);
	DDX_Control(pDX, IDC_RADIO2, m_Radio[1]);
	DDX_Control(pDX, IDC_RADIO3, m_Radio[2]);
	DDX_Control(pDX, IDC_CHECK1, m_Check[0]);
	DDX_Control(pDX, IDC_CHECK2, m_Check[1]);
	DDX_Control(pDX, IDC_CHECK3, m_Check[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT6, m_strFindName);
}


BEGIN_MESSAGE_MAP(CUnitTool, CDialog)

	ON_BN_CLICKED(IDC_BUTTON1, &CUnitTool::OnBnClickedButton1)

	ON_BN_CLICKED(IDC_BUTTON2, &CUnitTool::OnAddPlayer)
	ON_LBN_SELCHANGE(IDC_LIST1, &CUnitTool::OnListBox)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT6, &CUnitTool::OnFindName)
	ON_BN_CLICKED(IDC_BUTTON5, &CUnitTool::OnDeleteData)
	ON_BN_CLICKED(IDC_BUTTON6, &CUnitTool::OnSaveData)
	ON_BN_CLICKED(IDC_BUTTON4, &CUnitTool::OnLoadData)
END_MESSAGE_MAP()


void CUnitTool::OnBnClickedButton1()
{
	//UpdateData() : ���� ���� �Լ�
	
	UpdateData(TRUE);		// ���̾�α� �ڽ��κ��� �Էµ� ������ ����

	m_strCopy = m_strTemp;

	UpdateData(FALSE);		// ������ ����� ������ ���̾�α� �ڽ��� �ݿ�
}


void CUnitTool::OnAddPlayer()
{
	UpdateData(TRUE);

	UNITDATA*	pUnit = new UNITDATA;

	pUnit->strName = m_strName;
	pUnit->iAttack = m_iAttack;
	pUnit->iHp = m_iHp;

	for (int i = 0; i < 3; ++i)
	{
		// üũ�� �Ǿ����� Ȯ��
		if (m_Radio[i].GetCheck())
		{
			pUnit->byJobIndex = i;
			break;
		}
	}

	pUnit->byItem = 0x00;

	if (m_Check[0].GetCheck())
		pUnit->byItem |= RUBY;

	if (m_Check[1].GetCheck())
		pUnit->byItem |= DIAMOND;

	if (m_Check[2].GetCheck())
		pUnit->byItem |= SAPPHIRE;


	// AddString : ����Ʈ �ڽ��� �Ű� ������ �ش��ϴ� ���ڿ��� �߰�
	m_ListBox.AddString(pUnit->strName);

	m_MapUnitData.insert({ pUnit->strName, pUnit });

	UpdateData(FALSE);
}


void CUnitTool::OnListBox()
{
	UpdateData(TRUE);

	CString	strFindName;

	// GetCurSel : ���� ����Ʈ �ڽ��� ���õ� ���� �ε����� ��ȯ
	int	iSelectIdx = m_ListBox.GetCurSel();

	if (-1 == iSelectIdx)
		return;

	// GetText : ���� �ε����� �ش��ϴ� ���ڿ��� ����Ʈ �ڽ��κ��� ����
	m_ListBox.GetText(iSelectIdx, strFindName);

	auto	iter = m_MapUnitData.find(strFindName);

	if (iter == m_MapUnitData.end())
		return;

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);	// ��� ���� ��ư�� üũ ���¸� ����
		m_Check[i].SetCheck(FALSE);
	}

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);

	UpdateData(FALSE);
}


void CUnitTool::OnDestroy()
{
	CDialog::OnDestroy();

	for_each(m_MapUnitData.begin(), m_MapUnitData.end(), [&](auto& MyPair)
	{
		if (MyPair.second)
		{
			delete MyPair.second;
			MyPair.second = nullptr;
		}
	});

	m_MapUnitData.clear();
}


BOOL CUnitTool::OnInitDialog()
{
	CDialog::OnInitDialog();


	HBITMAP	hBitmap = (HBITMAP)LoadImage(nullptr, L"../Texture/JusinLogo1.bmp", IMAGE_BITMAP, 100, 50, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_Bitmap.SetBitmap(hBitmap);

	return TRUE;  
				  
}


void CUnitTool::OnFindName()
{
	UpdateData(TRUE);

	auto& iter = m_MapUnitData.find(m_strFindName);

	if (iter == m_MapUnitData.end())
		return;

	int iIndex = m_ListBox.FindString(0, m_strFindName);

	if (LB_ERR == iIndex)
		return;

	// �Ű� ������ �ش��ϴ� ���� �����ϴ� �Լ�
	m_ListBox.SetCurSel(iIndex);

	m_strName = iter->second->strName;
	m_iHp = iter->second->iHp;
	m_iAttack = iter->second->iAttack;

	for (int i = 0; i < 3; ++i)
	{
		m_Radio[i].SetCheck(FALSE);	// ��� ���� ��ư�� üũ ���¸� ����
		m_Check[i].SetCheck(FALSE);
	}

	if (iter->second->byItem & RUBY)
		m_Check[0].SetCheck(TRUE);

	if (iter->second->byItem & DIAMOND)
		m_Check[1].SetCheck(TRUE);

	if (iter->second->byItem & SAPPHIRE)
		m_Check[2].SetCheck(TRUE);

	m_Radio[iter->second->byJobIndex].SetCheck(TRUE);


	UpdateData(FALSE);
}


void CUnitTool::OnDeleteData()
{
	UpdateData(TRUE);

	CString		strFindName = L"";

	int iSelectIdx = m_ListBox.GetCurSel();

	if (LB_ERR == iSelectIdx)
		return;

	m_ListBox.GetText(iSelectIdx, strFindName);

	auto& iter = m_MapUnitData.find(strFindName);

	if (iter == m_MapUnitData.end())
		return;

	Safe_Delete(iter->second);
	m_MapUnitData.erase(strFindName);

	// �Ű� ���� idx�� �ش��ϴ� ���ڿ��� ������Ű�� �Լ�
	m_ListBox.DeleteString(iSelectIdx);

	UpdateData(FALSE);
}

void CUnitTool::OnSaveData()
{
	// CFileDialog : ���� �ҷ����� �Ǵ� �ٸ� �̸����� ���� �۾��� �ʿ��� ��ȭ ���ڸ� �����ϴ� ��ü

	CFileDialog		Dlg(FALSE,		// FALSE(�ٸ� �̸����� ����), TRUE(�ҷ�����)
						L"dat",		// default ���� Ȯ���ڸ�
						L"*.dat",	// ��ȭ ���ڿ� ǥ�õ� ���� ���ϸ�
						OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,  // OFN_HIDEREADONLY(�б� ���� üũ �ڽ� ���� �ɼ�) | OFN_OVERWRITEPROMPT(�ߺ� ������ ����� ���� ��, ��� �޼��� ���� �ɼ�)
						L"Data Files(*.dat) | *.dat||", // ��ȭ ���ڿ� ǥ�õ� ���� ���� "�޺� �ڽ��� ��µ� ���ڿ� | ���� ����� ���͸� ���ڿ� ||"
						this);	// �θ� ������ �ּ�

	TCHAR	szPath[MAX_PATH] = L"";

	// GetCurrentDirectory : ���� ������Ʈ ������ �ִ� ���丮 ��θ� ����
	GetCurrentDirectory(MAX_PATH, szPath);
	// szPath = 0x0020e850 L"D:\\����ȯ\\144��\\Frame144\\Tool"

	// ��ü ��� �� ���� �̸��� �߶󳻴� �Լ�
	// ��� �� ���� �̸��� ���ٸ� ���� ������ �������� �߶�

	PathRemoveFileSpec(szPath);
	// L"D:\\����ȯ\\144��\\Frame144

	lstrcat(szPath, L"\\Data");
	// L"D:\\����ȯ\\144��\\Frame144\\Data

	Dlg.m_ofn.lpstrInitialDir = szPath;

	// DoModal : ��ȭ ���ڸ� ����. IDOK �Ǵ� IDCANCEL ��ȯ
	if (IDOK == Dlg.DoModal())
	{
		// GetPathName : ���õ� ��θ� ��ȯ�ϴ� �Լ�
		// GetString : c_str�� ���� ���� �ڷ� ������ ���� ��ȯ
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
		DWORD	dwStrByte(0);

		for (auto& MyPair : m_MapUnitData)
		{
			// Key �� ����
			dwStrByte = sizeof(TCHAR) * (MyPair.first.GetLength() + 1);

			WriteFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);
			WriteFile(hFile, MyPair.first.GetString(), dwStrByte, &dwByte, nullptr);
			
			// value �� ����
			WriteFile(hFile, &(MyPair.second->iHp), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->iAttack), sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			WriteFile(hFile, &(MyPair.second->byItem), sizeof(BYTE), &dwByte, nullptr);
		}
	
		CloseHandle(hFile);
	}

}

void CUnitTool::OnLoadData()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE,		// FALSE(�ٸ� �̸����� ����), TRUE(�ҷ�����)
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
		for (auto& MyPair : m_MapUnitData)
			delete MyPair.second;

		m_MapUnitData.clear();
		
		// ResetContent : ������ ����Ʈ �ڽ� ����� �ʱ�ȭ
		m_ListBox.ResetContent();
		
		CString			str = Dlg.GetPathName().GetString();
		const TCHAR*	pGetPath = str.GetString();

		HANDLE	hFile = CreateFile(pGetPath,
			GENERIC_READ, 0, 0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			0);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD		dwByte(0);
		DWORD		dwStrByte(0);
		UNITDATA	tData{};

		while (true)
		{
			// Key �� �ε�
			ReadFile(hFile, &dwStrByte, sizeof(DWORD), &dwByte, nullptr);

			TCHAR*	pName = new TCHAR[dwStrByte];
			ReadFile(hFile, pName, dwStrByte, &dwByte, nullptr);

			// value �� �ε�

			ReadFile(hFile, &(tData.iHp), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.iAttack), sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byJobIndex), sizeof(BYTE), &dwByte, nullptr);
			ReadFile(hFile, &(tData.byItem), sizeof(BYTE), &dwByte, nullptr);

			if (0 == dwByte)
			{
				delete[]pName;
				break;
			}

			UNITDATA*		pUnit = new UNITDATA;

			pUnit->strName = pName;
			delete[]pName;

			pUnit->iHp = tData.iHp;
			pUnit->iAttack = tData.iAttack;
			pUnit->byJobIndex = tData.byJobIndex;
			pUnit->byItem = tData.byItem;

			m_MapUnitData.insert({ pUnit->strName, pUnit });

			m_ListBox.AddString(pUnit->strName);

		}

		CloseHandle(hFile);

	}

	UpdateData(FALSE);
}
