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
	vector<TILE*>		m_vecTile;

	CToolView*			m_pMainView;

};

