#include "stdafx.h"
#include "MyTerrain.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"


CMyTerrain::CMyTerrain()
{
	m_vecTile.reserve(TILEX * TILEY);
}


CMyTerrain::~CMyTerrain()
{
	Release();
}

HRESULT CMyTerrain::Initialize(void)
{
	if (FAILED(Load_Tile(L"../Data/Edit.dat")))
	{
		ERR_MSG(L"Map Data Load Failed");
		return E_FAIL;
	}

	return S_OK;
}

int CMyTerrain::Update(void)
{

	//if (0.f > ::Get_Mouse().x)
	//	m_vScroll.x += 5.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	//if (WINCX < ::Get_Mouse().x)
	//	m_vScroll.x -= 5.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	//if (0.f > ::Get_Mouse().y)
	//	m_vScroll.y += 5.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

	//if (WINCY < ::Get_Mouse().y)
	//	m_vScroll.y -= 5.f * CTimeMgr::Get_Instance()->Get_TimeDelta();


	return 0;
}

void CMyTerrain::Late_Update(void)
{
}

void CMyTerrain::Render(void)
{
	//D3DXMATRIX		matScale, matRotZ, matTrans;

	//TCHAR		szBuf[MIN_STR] = L"";
	//int			iIndex(0);

	//int		iCullX = int(-m_vScroll.x) / TILECX;
	//int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

	//int		iMaxX = WINCX / TILECX;
	//int		iMaxY = WINCY / (TILECY / 2);

	//for(int i = iCullY; i < iCullY + iMaxY; ++i)
	//{ 
	//	for (int j = iCullX; j < iCullX + iMaxX; ++j)
	//	{
	//		int		iIndex = i * TILEX + j;

	//		if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
	//			continue;

	//		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	//		D3DXMatrixTranslation(&matTrans,
	//			m_vecTile[iIndex]->vPos.x + m_vScroll.x,	// 0인 경우 x 스크롤 값
	//			m_vecTile[iIndex]->vPos.y + m_vScroll.y,	// 1인 경우 y 스크롤 값
	//			m_vecTile[iIndex]->vPos.z);

	//		m_tInfo.matWorld = matScale * matTrans;

	//		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

	//		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_vecTile[iIndex]->byDrawID);

	//		if (nullptr == pTexInfo)
	//			return;

	//		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	//		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	//		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
	//			nullptr,
	//			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
	//			nullptr,
	//			D3DCOLOR_ARGB(255, 255, 255, 255));

	//		swprintf_s(szBuf, L"%d", iIndex);

	//		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
	//			szBuf,
	//			lstrlen(szBuf),
	//			nullptr,
	//			0,
	//			D3DCOLOR_ARGB(255, 255, 255, 255));

	//		++iIndex;
	//	}
	//}
	
	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x,
			pTile->vPos.y,
			pTile->vPos.z);

		matWorld = matScale * matTrans;


		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,  // 출력할 이미지 영역에 대한 RECT 주소, NULL인 경우 이미지의 0, 0 기준으로 출력
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),  // 출력할 이미지 중심 위치에 대한 VEC3 구조체 주소, NULL인 경우 0, 0이 중심 좌표
			nullptr,  // 이미지를 출력할 좌표, NULL인 경우 스크린 상 0,0 좌표에 출력
			D3DCOLOR_ARGB(255, 255, 255, 255));	// 출력할 원본 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 원본 값 유지

												// 타일 인덱스 출력
		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(
			CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// 출력할 문자열
			lstrlen(szBuf), // 출력할 문자열 버퍼의 크기
			nullptr,	// 출력할 위치의 렉트 주소
			0,			// 문자 정렬 옵션
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 폰트 색상

		++iIndex;

	}
}

void CMyTerrain::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

HRESULT CMyTerrain::Load_Tile(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, 
		GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	Release();

	DWORD		dwByte(0);
	TILE*		pTile = nullptr;
	
	while (true)
	{
		pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, nullptr);

		if (0 == dwByte)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);

	return S_OK;
}
