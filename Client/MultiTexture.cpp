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
	// ����� ���̴� 520byte�� �Ѿ �� ��.
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
			0,	// ������ �ؽ�ó ���(0�� ��� �Ϲ����� Texture) -> D3DUSAGE_RENDERTARGET : ���� Ÿ�ٿ� �ؽ�ó
			pTexInfo->tImgInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT,		// �̹��� ���͸� ���
			D3DX_DEFAULT,		// �Ӹ� ���͸� ���
			0,					// ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ
			&(pTexInfo->tImgInfo),			// �̹��� ������ ����ü�� ����(�ռ� �Լ��� ���ؼ� �����߱� ������ null)
			nullptr,			// �ȷ�Ʈ �̹��� ����
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
