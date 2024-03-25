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

	// D3DXGetImageInfoFromFile : ������ �̹��� ���Ͽ� ���� ������ ���� ����ü�� ���� ä���ִ� �Լ�
	if (FAILED(D3DXGetImageInfoFromFile(pFilePath, &(m_pTexInfo->tImgInfo))))
	{
		Safe_Delete(m_pTexInfo);
		ERR_MSG(pFilePath);
		return E_FAIL;
	}

	// D3DPOOL : �޸� POOL(dx ���ҽ� �ڿ����� � �޸𸮿� ������ �������� ���� �ɼ�)

	// D3DPOOL_DEFAULT : ���� ������ �޸𸮿� ����, ���� �׷��� �޸𸮸� ����
	// D3DPOOL_MANAGED : dx�� ���� �ڿ��� �����ϸ�, �׷��� �޸𸮸� ��������� �̸� RAM�� ���
	// 
	// D3DPOOL_SYSTEMMEM : RAM�� ������
	// D3DPOOL_SCRATCH : RAM�� ���������� DX ��ġ�� ���� ����


	if (FAILED(D3DXCreateTextureFromFileEx(CDevice::Get_Instance()->Get_Device(),
		pFilePath,
		m_pTexInfo->tImgInfo.Width,
		m_pTexInfo->tImgInfo.Height,
		m_pTexInfo->tImgInfo.MipLevels,
		0,	// ������ �ؽ�ó ���(0�� ��� �Ϲ����� Texture) -> D3DUSAGE_RENDERTARGET : ���� Ÿ�ٿ� �ؽ�ó
		m_pTexInfo->tImgInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,		// �̹��� ���͸� ���
		D3DX_DEFAULT,		// �Ӹ� ���͸� ���
		0,					// ������ ����, 0�� �ָ� �÷�Ű ��ȿȭ
		nullptr,			// �̹��� ������ ����ü�� ����(�ռ� �Լ��� ���ؼ� �����߱� ������ null)
		nullptr,			// �ȷ�Ʈ �̹��� ����
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
