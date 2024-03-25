#pragma once

#include "Include.h"

class CToolView;
class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }

	vector<TILE*>&		Get_VecTile() { return m_vecTile; }

public:
	void		Initialize();
	void		Update();
	void		Mini_Render();
	void		Render();
	void		Release();

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	CString m_strSelectedImage; // 선택된 이미지의 이름 또는 경로

public:
	void SetSelectedImage(const CString& strImage)
	{
		m_strSelectedImage = strImage;
		// 선택된 이미지를 기반으로 렌더링을 위한 추가적인 설정을 할 수 있습니다.
	}

private:
	vector<TILE*>		m_vecTile;

	CToolView*			m_pMainView;

};

