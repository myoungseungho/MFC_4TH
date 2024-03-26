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

	// PathRelativePathTo : strFullPath에서 szCurDirPath까지 가는 상대 경로를 구해서 szRelativePath에 저장(단, 같은 드라이브에서만 가능)
	PathRelativePathTo(szRelativePath, 
		szCurDirPath, FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), FILE_ATTRIBUTE_DIRECTORY);
	
	return CString(szRelativePath);
}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	CFileFind			Find;

	// 경로 상의 모든 파일(*.*)을 찾기 위해 경로 편집
	wstring wstrFilePath = wstrPath + L"\\*.*";

	// FindFile : 주어진 경로에 파일의 유무를 판단하는 함수

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int iFileCnt(0);

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에 다음 파일을 찾아주는 함수
		//	더 찾을게 없거나 마지막 대상인 경우 0을 리턴

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
	// mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind			Find;

	// 경로 상의 모든 파일(*.*)을 찾기 위해 경로 편집
	wstring wstrFilePath = wstrPath + L"\\*.*";
	// wstrFilePath = L"D:\\유준환\\144기\\Frame144\\Texture\\Stage\\*.*"

	// FindFile : 주어진 경로에 파일의 유무를 판단하는 함수

	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		// FindNextFile : 동일 경로 안에 다음 파일을 찾아주는 함수
		//	더 찾을게 없거나 마지막 대상인 경우 0을 리턴

		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : 현재 찾은 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathlist);
		}

		else
		{
			if(Find.IsSystem())
				continue;

			IMGPATH*		pImgPath = new IMGPATH;

			TCHAR			szPathBuf[MAX_PATH] = L"";

			lstrcpy(szPathBuf, Find.GetFilePath().GetString());
			//szPathBuf = 0x00efd948 L"D:\\유준환\\144기\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack0.png"

			// 마지막 파일명 또는 폴더명을 잘라내는 함수
			PathRemoveFileSpec(szPathBuf);
			// szPathBuf = 0x00efd948 L"D:\\유준환\\144기\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack"

			pImgPath->iCount = DirFileCount(szPathBuf);

			// GetFileTitle : 파일 이름만 얻어오는 함수
			wstring	wstrTextureName = Find.GetFileTitle().GetString();
			//wstrTextureName = L"BossMultiAttack0"

			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 얻어옴
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";
			// wstrTextureName = L"BossMultiAttack%d.png"

			TCHAR szBuf[MAX_PATH] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			PathRemoveFileSpec(szBuf);

			//szBuf = 0x0045d8bc L"D:\\유준환\\144기\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack"

			// PathCombine(dst, src1, src2) : src1과 src2를 이어붙여 완성된 경로를 dst에 저장
			// src1과 scr2 사이에 \\가 자동으로 삽입
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// szBuf = 0x0045d8bc L"D:\\유준환\\144기\\Frame144\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// pImgPath->wstrPath = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\BossMultiAttack%d.png"

			PathRemoveFileSpec(szBuf);
			// szBuf = L"..\\Texture\\Stage\\Effect\\BossMultiAttack\\

			// PathFindFileName : 경로에 있는 파일명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			pImgPath->wstrStateKey = PathFindFileName(szBuf);


			PathRemoveFileSpec(szBuf);
			// szBuf = L"..\\Texture\\Stage\\Effect\\

			// PathFindFileName : 경로에 있는 파일명을 찾거나 가장 마지막 폴더명을 찾아내는 함수
			pImgPath->wstrObjKey = PathFindFileName(szBuf);

			rPathlist.push_back(pImgPath);

			bContinue = 0;
		}
	}

}
