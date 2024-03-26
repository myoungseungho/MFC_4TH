#include "stdafx.h"
#include "Terrain.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ToolView.h"

CTerrain::CTerrain() : m_pMainView(nullptr)
{
	m_vecTile.reserve(TILEX * TILEY);
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::Get_Instance()->
		Insert_Texture(L"../Texture/Stage/Terrain/Tile/Tile%d.png", TEX_MULTI, L"Terrain", L"Tile", 38)))
	{
		AfxMessageBox(L"Tile Img Insert Failed");
		return;
	}

	if (FAILED(CTextureMgr::Get_Instance()->
		Insert_Texture(L"../Texture/Stage/BackGround/BackGround/BackGound%d.png", TEX_MULTI, L"BackGround", L"BackGround", 9)))
	{
		AfxMessageBox(L"Tile Img Insert Failed");
		return;
	}


	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE*	pTile = new TILE;

			float	fY = i * (TILECY / 2.f);
			float	fX = (j * TILECX) + (i % 2) * (TILECX / 2.f);

			pTile->vPos = {fX, fY , 0.f};
			pTile->vSize = { (float)TILECX, (float)TILECY, 0.f };
			pTile->byOption = 0;
			pTile->byDrawID = 36;

			m_vecTile.push_back(pTile);			
		}
	}
}

void CTerrain::Update()
{
	
}

void CTerrain::Mini_Render()
{
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

		Set_Ratio(&matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO*	pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", pTile->byDrawID);

		if (nullptr == pTexInfo)
			return;

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
			nullptr,  
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), 
			nullptr,  
			D3DCOLOR_ARGB(255, 255, 255, 255));	
	}
}

void CTerrain::Render()
{
	RECT	rc{};

	// GetClientRect : 현재 클라이언트 영역의 RECT 정보를 얻어옴
	::GetClientRect(m_pMainView->m_hWnd, &rc);

	float		fX = WINCX / float(rc.right - rc.left);
	float		fY = WINCY / float(rc.bottom - rc.top);

	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x - m_pMainView->GetScrollPos(0),	// 가로 스크롤
			pTile->vPos.y - m_pMainView->GetScrollPos(1),	// 세로 스크롤
			pTile->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(&matWorld, fX, fY);
		
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

	// 선택된 이미지 렌더링
	if (!m_strSelectedImage.IsEmpty())
	{
		// 선택된 이미지의 텍스처 정보 조회
		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_strSelectedImage);
		if (pTexInfo != nullptr)
		{
			D3DXMATRIX matWorld, matScale, matTrans;

			// 이미지 위치와 크기를 조절할 수 있는 트랜스폼 설정
			D3DXMatrixScaling(&matScale, 1.0f, 1.0f, 1.0f); // 필요에 따라 스케일 조정
			D3DXMatrixTranslation(&matTrans,
				WINCX / 2.0f - (pTexInfo->tImgInfo.Width / 2.0f),
				WINCY / 2.0f - (pTexInfo->tImgInfo.Height / 2.0f),
				0.0f);

			matWorld = matScale * matTrans;
			Set_Ratio(&matWorld, fX, fY); // 화면 비율에 맞게 조정

			CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

			// 이미지 그리기
			CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
				nullptr,
				&D3DXVECTOR3(pTexInfo->tImgInfo.Width / 2.0f, pTexInfo->tImgInfo.Height / 2.0f, 0.0f),
				nullptr,
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::Tile_Change(const D3DXVECTOR3 & vPos, const int & iDrawID)
{
	int		iIndex = Get_TileIndex(vPos);

	if (-1 == iIndex)
		return;
	
	m_vecTile[iIndex]->byDrawID = (BYTE)iDrawID;
	m_vecTile[iIndex]->byOption = 1;
}

int CTerrain::Get_TileIndex(const D3DXVECTOR3 & vPos)
{
	for (size_t iIndex = 0; iIndex < m_vecTile.size(); ++iIndex)
	{
		if (Picking_Dot(vPos, iIndex))
			return iIndex;
	}

	return -1;
}

bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// 12->3->6->9

	float		fGradient[4]  
	{
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f),
		(TILECY / 2.f) / (TILECX / 2.f) * -1.f,
		(TILECY / 2.f) / (TILECX / 2.f)
	};

	// 12, 3, 6, 9

	D3DXVECTOR3		vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f } ,
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f } ,
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f } ,
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	// y = ax + b; -> b = y - ax

	float fB[4]
	{
		vPoint[0].y - fGradient[0] * vPoint[0].x,
		vPoint[1].y - fGradient[1] * vPoint[1].x,
		vPoint[2].y - fGradient[2] * vPoint[2].x,
		vPoint[3].y - fGradient[3] * vPoint[3].x
	};

	bool	bCheck[4] {false};

	// 12->3
	// 0 = ax + b - y  (선 상에 점이 존재)
	// 0 > ax + b - y  (선 보다 위)
	// 0 < ax + b - y  (선 보다 아래)

	if (0 < fGradient[0] * vPos.x + fB[0] - vPos.y)
		bCheck[0] = true;

	// 3->6

	if (0 >= fGradient[1] * vPos.x + fB[1] - vPos.y)
		bCheck[1] = true;

	// 6->9
	if (0 >= fGradient[2] * vPos.x + fB[2] - vPos.y)
		bCheck[2] = true;

	// 9->12
	if (0 < fGradient[3] * vPos.x + fB[3] - vPos.y)
		bCheck[3] = true;
	
	return bCheck[0] && bCheck[1] && bCheck[2] && bCheck[3];
}

bool CTerrain::Picking_Dot(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	// 12, 3, 6, 9

	D3DXVECTOR3		vPoint[4]
	{
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY / 2.f), 0.f } ,
		{ m_vecTile[iIndex]->vPos.x + (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f } ,
		{ m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY / 2.f), 0.f } ,
		{ m_vecTile[iIndex]->vPos.x - (TILECX / 2.f), m_vecTile[iIndex]->vPos.y, 0.f }
	};

	D3DXVECTOR3		vDir[4]
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3	vNormal[4]
	{
		{ -vDir[0].y, vDir[0].x, 0.f},
		{ -vDir[1].y, vDir[1].x, 0.f },
		{ -vDir[2].y, vDir[2].x, 0.f },
		{ -vDir[3].y, vDir[3].x, 0.f }
	};

	D3DXVECTOR3	vMouseDir[4]
	{
		vPos - vPoint[0],
		vPos - vPoint[1],
		vPos - vPoint[2],
		vPos - vPoint[3]
	};

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
	}

	for (int i = 0; i < 4; ++i)
	{
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}

void CTerrain::Set_Ratio(D3DXMATRIX * pOut, float fRatioX, float fRatioY)
{
	pOut->_11 *= fRatioX;
	pOut->_21 *= fRatioX;
	pOut->_31 *= fRatioX;
	pOut->_41 *= fRatioX;

	pOut->_12 *= fRatioY;
	pOut->_22 *= fRatioY;
	pOut->_32 *= fRatioY;
	pOut->_42 *= fRatioY;
}
