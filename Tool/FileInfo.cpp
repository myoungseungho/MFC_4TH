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
