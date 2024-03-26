#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	TCHAR		szCurDirPath[MAX_PATH] = L"";
	TCHAR		szRelativePath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : strFullPath���� szCurDirPath���� ���� ��� ��θ� ���ؼ� szRelativePath�� ����(��, ���� ����̺꿡���� ����)
	PathRelativePathTo(szRelativePath, 
		szCurDirPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	CFileFind			Find;

	// ��� ���� ��� ����(*.*)�� ã�� ���� ��� ����
	wstring wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : �־��� ��ο� ������ ������ �Ǵ��ϴ� �Լ�

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt(0);

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ� ���� ������ ã���ִ� �Լ�
		//	�� ã���� ���ų� ������ ����� ��� 0�� ����

		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;

		if (Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<IMGPATH*>& rPathlist)
{
	// mfc���� �����ϴ� ���� �� ��� ���� ���� Ŭ����
	CFileFind			Find;

	// ��� ���� ��� ����(*.*)�� ã�� ���� ��� ����
	wstring wstrFilePath = wstrPath + L"\\*.*";
	// wstrFilePath = L"D:\\����ȯ\\144��\\Frame144\\Texture\\Stage\\*.*"

	// FindFile : �־��� ��ο� ������ ������ �Ǵ��ϴ� �Լ�

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : ���� ��� �ȿ� ���� ������ ã���ִ� �Լ�
		//	�� ã���� ���ų� ������ ����� ��� 0�� ����

		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : ���� ã�� ��θ� ������ �Լ�
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathlist);
		}

		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;

			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x00efd948 L"D:\\����ȯ\\144��\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			// ������ ���ϸ� �Ǵ� �������� �߶󳻴� �Լ�
			PathRemoveFileSpec(szPathBuf);
			// szPathBuf = 0x00efd948 L"D:\\����ȯ\\144��\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : ���� �̸��� ������ �Լ�
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			//wstrTextureName = L"BossMultiAttack0"

			// substr(����, ��) : ���ۿ��� ���� �ش��ϴ� ���ڿ��� ����
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			TCHAR szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			//szBuf = 0x0045d8bc L"D:\\����ȯ\\144��\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack"

			// PathCombine(dst, src1, src2) : src1�� src2�� �̾�ٿ� �ϼ��� ��θ� dst�� ����
			// src1�� scr2 ���̿� \\�� �ڵ����� ����
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// szBuf = 0x0045d8bc L"D:\\����ȯ\\144��\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			// szBuf = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : ��ο� �ִ� ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			pImgPath->wstrStateKey = PathFindFileName(szBuf);


			PathRemoveFileSpec(szBuf);
			// szBuf = L"..\\Texture\\Stage\\Effect\\

			// PathFindFileName : ��ο� �ִ� ���ϸ��� ã�ų� ���� ������ �������� ã�Ƴ��� �Լ�
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathlist.push_back(pImgPath);

			bContinue = 0;
		}
	}

}
