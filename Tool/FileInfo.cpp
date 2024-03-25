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
