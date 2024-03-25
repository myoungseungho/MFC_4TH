#include "stdafx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture()
{
}


CMultiTexture::~CMultiTexture()
{
	Release();
}

const TEXINFO * CMultiTexture::Get_Texture(const TCHAR * pStateKey, 
											const int & iCount)
{
	auto	iter = find_if(m_mapMultiTex.begin(), m_mapMultiTex.end(), 
		[&](auto& MyPair)->bool
	{
		if (MyPair.first == pStateKey)
			return true;

		return false;

	});

	if (iter == m_mapMultiTex.end())
		return nullptr;

	return iter->second[iCount];
}

HRESULT CMultiTexture::Insert_Texture(const TCHAR * pFilePath, const TCHAR * pStateKey , const int & iCount)
{
	// 경로의 길이는 520byte를 넘어선 안 됨.
	TCHAR	szFullPath[MAX_PATH] = L"";

	for (int i = 0; i < iCount; ++i)
	{
		swprintf_s(szFullPath, pFilePath, i);

		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if (FAILED(D3DXGetImageInfoFromFile(szFullPath, &(pTexInfo->tImgInfo))))
		{
			Safe_Delete(pTexInfo);
			ERR_MSG(pFilePath);
			return E_FAIL;
		}
				
		if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
			szFullPath,
			pTexInfo->tImgInfo.Width,
			pTexInfo->tImgInfo.Height,
			pTexInfo->tImgInfo.MipLevels,
			0,	// 생성한 텍스처 방식(0인 경우 일반적인 Texture) -> D3DUSAGE_RENDERTARGET : 렌더 타겟용 텍스처
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,		// 이미지 필터링 방식
			D3DX_DEFAULT,		// 밉맵 필터링 방식
			0,					// 제거할 색상, 0을 주면 컬러키 무효화
			&(pTexInfo->tImgInfo),			// 이미지 정보를 구조체에 저장(앞선 함수를 통해서 수행했기 때문에 null)
			nullptr,			// 팔레트 이미지 정보
			&(pTexInfo->pTexture))))
		{
			Safe_Delete(pTexInfo);
			ERR_MSG(L"multi texture Create Failed");
			return E_FAIL;
		}

		m_mapMultiTex[pStateKey].push_back(pTexInfo);
	}	

	return S_OK;
}

void CMultiTexture::Release()
{
	for_each(m_mapMultiTex.begin(), m_mapMultiTex.end(), [](auto& MyPair)
	{
		for_each(MyPair.second.begin(), MyPair.second.end(), Safe_Delete<TEXINFO*>);
		MyPair.second.clear();
	});

	m_mapMultiTex.clear();

}
