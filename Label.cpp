//////////////////////////////////////////////////////////////////////
//																	//
//		Label.cpp		CLabel�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2003/01/24 - 2003/01/24		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Label.h"

CLabel::CLabel() {
}

CLabel::~CLabel() {
}

int CLabel::Initialize(CCommand *lpCmdTable) {
	theLocalCommand = lpCmdTable;
	return 1;
}

/* �֐���o�^���� */
