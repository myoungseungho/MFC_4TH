#pragma once
#include "afxwin.h"

#include "Include.h"

class CUnitTool : public CDialog
{
	DECLARE_DYNAMIC(CUnitTool)

public:
	CUnitTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUnitTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNITTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnAddPlayer();
	afx_msg void OnListBox();
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
public:
	// value
	CString m_strTemp;
	CString m_strCopy;
	CString m_strName;
	int m_iHp;
	int m_iAttack;

	// control
	CListBox m_ListBox;
	CButton m_Radio[3];
	CButton m_Check[3];
	CButton m_Bitmap;

	// general
	map<CString, UNITDATA*>		m_MapUnitData;
	CString m_strFindName;
	afx_msg void OnFindName();
	afx_msg void OnDeleteData();
	afx_msg void OnSaveData();
	afx_msg void OnLoadData();
};

// 탐색 : 에디트 컨트롤 생성 -> 문자열을 입력했을 때 리스트 박스에 셀이 선택되도록
// 삭제 : 리스트 박스에 셀을 선택 -> 삭제 버튼을 누르면 리스트 박스에서 해당 데이터를 제거
// 저장 : 저장 버튼을 누르면 유닛툴에서 생성한 데이터를 파일로 저장
// 불러오기 : 불러오기 버튼을 누르면 저장했던 데이터를 리스트 박스로 읽어옴