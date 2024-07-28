//////////////////////////////////////////////////////////////////////
//																	//
//		IfElse.cpp		CIfElseクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "IfElse.h"

//　エラー通知用のグローバル変数
int gIfElseErrorInfo;

//　コンストラクタ、デストラクタ
CIfElse::CIfElse() {
	int i;

	//　ポインタを初期化する
	m_IfFalsePoint = NULL;
	m_ExitPointCount = 0;
	for (i = 0; i < MAX_ELSEIF; i++) {
		m_ExitPoint[i] = NULL;
	}
}

CIfElse::~CIfElse() {
	if (m_ExitPointCount) {
		//グローバル変数を通してエラーを返す
		gIfElseErrorInfo = IFELSE_ERR_NOTFINALIZE;
	}

	gIfElseErrorInfo = 0;
}

//////////////////////////////////////////////////////////////////////
//　条件不成立時のジャンプ先を書き込む処理
void CIfElse::SetFalsePoint(int *p) {
	m_IfFalsePoint = p;
}

int CIfElse::WriteFalsePoint(int value) {
	int r = 0;

	if (m_IfFalsePoint) {
		*(m_IfFalsePoint) = (value | 0x80000000);
		r = 1;
	}
	m_IfFalsePoint = NULL;
	return r;
}

//////////////////////////////////////////////////////////////////////
//　条件成立時のELSEブロックのスキップ先を書き込む処理
void CIfElse::InitExitPointList() {
	m_ExitPointCount = 0;
}

int CIfElse::AddExitPoint(int *p) {
	if (m_ExitPointCount >= MAX_ELSEIF) {
		return 0;
	}
	m_ExitPoint[m_ExitPointCount++] = p;
	return 1;
}

int CIfElse::WriteExitPoint(int value) {
	int i;
	int r = 0;

	for (i = 0; i < m_ExitPointCount; i++) {
		if (m_ExitPoint[i]) {
			*(m_ExitPoint[i]) = (value | 0x80000000);
			r++;
		}
	}

	m_ExitPointCount = 0;
	return (r);
}
