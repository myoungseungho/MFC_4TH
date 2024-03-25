#include "stdafx.h"
#include "TextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}


CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO * CTextureMgr::Get_Texture(const TCHAR * pObjKey, const TCHAR * pStateKey, const int & iCount)
{
	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(),
	[&](auto& MyPair)->bool
	{
		if (MyPair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
		return nullptr;
	
	return iter->second->Get_Texture(pStateKey, iCount);
}

HRESULT CTextureMgr::Insert_Texture(const TCHAR * pFilePath,
	TEXTYPE eType, 
	const TCHAR * pObjKey, 
	const TCHAR * pStateKey, 
	const int & iCount)
{
	auto	iter = find_if(m_mapTex.begin(), m_mapTex.end(),
		[&](auto& MyPair)->bool
	{
		if ( MyPair.first == pObjKey)
			return true;

		return false;
	});

	if (iter == m_mapTex.end())
	{
		CTexture* pTexture = nullptr;

		switch (eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCount)))
		{
			ERR_MSG(pFilePath);
			return E_FAIL;
		}

		m_mapTex.insert({ pObjKey, pTexture });
	}
	else if (TEX_MULTI == eType)
		iter->second->Insert_Texture(pFilePath, pStateKey, iCount);

	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& MyPair)
	{
		Safe_Delete(MyPair.second);
	});

	m_mapTex.clear();
}
