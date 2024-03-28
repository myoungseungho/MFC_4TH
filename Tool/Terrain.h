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

	vector<TILE*>& Get_VecTile() { return m_vecTile; }
	vector<TILE*>& Get_VecTree() { return m_vecTree; }

public:
	void		Initialize();
	void		Update();
	void		Mini_Render();
	void		Render();
	void		Release();
	void		TileRelease();

public:
	void		Tile_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int			Get_TileIndex(const D3DXVECTOR3& vPos);
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	bool		Picking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

	void		Obj_Change(const D3DXVECTOR3& vPos, const int& iDrawID);
	int			Get_ObjIndex(const D3DXVECTOR3& vPos);
	bool		ObjPicking_Dot(const D3DXVECTOR3& vPos, const int& iIndex);

private:
	void		Set_Ratio(D3DXMATRIX* pOut, float fRatioX, float fRatioY);

private:
	CString m_strSelectedImage; // ���õ� �̹����� �̸� �Ǵ� ���
	BYTE m_strNumberByte;
public:
	void SetSelectedImage(CString strImage, BYTE strByte)
	{
		m_strSelectedImage = strImage;
		m_strNumberByte = strByte;
		// ���õ� �̹����� ������� �������� ���� �߰����� ������ �� �� �ֽ��ϴ�.
	}

private:
	vector<TILE*>		m_vecTile;
	vector<TILE*>		m_vecTree;

	CToolView* m_pMainView;

};

