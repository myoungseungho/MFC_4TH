#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
	LPD3DXFONT				Get_Font()	 { return m_pFont; }

public:
	HRESULT			Init_Device();
	void			Render_Begin();
	void			Render_End(HWND hWnd = NULL);
	void			Release();

private:
	void			Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private: 
	
	LPDIRECT3D9		m_pSDK;	// 사용하고자 하는 장치를 조사하고 해당 장치를 제어하기 위한 또 다른 객체를 생성해주는 모체

	LPDIRECT3DDEVICE9		m_pDevice;	// 그래픽 장치를 통한 렌더링을 제어하는 객체 포인터

	LPD3DXSPRITE			m_pSprite;	// dx상에서 2d이미지를 출력해주는 com 객체 포인터

	LPD3DXFONT				m_pFont;	// dx상에서 폰트 출력을 담당하는 com 객체 포인터
};

