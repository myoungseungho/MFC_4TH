#include "stdafx.h"
#include "SingleTexture.h"
#include "Device.h"


CSingleTexture::CSingleTexture()
	: m_pTexInfo(nullptr)
{
}


CSingleTexture::~CSingleTexture()
{
	Release();
}


HRESULT CSingleTexture::Insert_Texture(const TCHAR * pFilePath, 
									   const TCHAR * pStateKey, 
									   const int & iCount)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	// D3DXGetImageInfoFromFile : 지정한 이미지 파일에 관한 정보를 얻어와 구조체에 값을 채워주는 함수
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		ERR_MSG(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL : 메모리 POOL(dx 리소스 자원들을 어떤 메모리에 보관할 것인지에 대한 옵션)

	// D3DPOOL_DEFAULT : 가장 적합한 메모리에 보관, 보통 그래픽 메모리를 말함
	// D3DPOOL_MANAGED : dx에 의해 자원을 관리하며, 그래픽 메모리를 사용하지만 이를 RAM에 백업
	// 
	// D3DPOOL_SYSTEMMEM : RAM에 저장함
	// D3DPOOL_SCRATCH : RAM에 저장하지만 DX 장치로 접근 못함


	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,	// 생성한 텍스처 방식(0인 경우 일반적인 Texture) -> D3DUSAGE_RENDERTARGET : 렌더 타겟용 텍스처
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,		// 이미지 필터링 방식
		D3DX_DEFAULT,		// 밉맵 필터링 방식
		0,					// 제거할 색상, 0을 주면 컬러키 무효화
		nullptr,			// 이미지 정보를 구조체에 저장(앞선 함수를 통해서 수행했기 때문에 null)
		nullptr,			// 팔레트 이미지 정보
		&(m_pTexInfo->pTexture))))

	{
		Safe_Delete(m_pTexInfo);
		ERR_MSG(L"single texture Create Failed");
		return E_FAIL;
	}



	return S_OK;
}

void CSingleTexture::Release()
{
	// m_pTexInfo->pTexture->Release();

	Safe_Release(m_pTexInfo->pTexture);
	Safe_Delete(m_pTexInfo);
}
