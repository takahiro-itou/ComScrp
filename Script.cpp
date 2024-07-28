//////////////////////////////////////////////////////////////////////
//																	//
//		Script.cpp		CScript�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/12/13		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Script.h"

// �O���[�o���֐�

// �X�N���v�g�̂P���R�[�h����������
int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec) {
	int r;

	if (ScrRec->CodeType == CODETYPE_LOCAL_VAR) {
		// ���[�J���ϐ�
		ScrRec->CodeType = CODETYPE_CONST_NUMBER;
		ScrRec->CodeParam++;
		ScrRec->AccessVarEx = 1;
	} else if (ScrRec->CodeType == CODETYPE_LOCAL_PARAMETER) {
		// �֐��̉�����
		ScrRec->CodeType = CODETYPE_CONST_NUMBER;
		ScrRec->CodeParam++; 
		ScrRec->AccessVarEx = 2;
	}

	r = 0;
	if (ScrRec->SepCode)  { r |= 0x40000000; }
	if (ScrRec->SepParam) { r |= 0x20000000; }

	r |= ( ((ScrRec->CodeType) & 0x0f) << 24);

	if (ScrRec->VarPoint) {
		r |= 0x00200000;
		r |= ( ((ScrRec->VarPointDest) & 3) << 19);
	}
	r |= ( ((ScrRec->AccessVarEx) & 0x03) << 16);

	if (ScrRec->CodeParam > 0xffff) {
		/* �p�����[�^���Q�o�C�g�Ɏ��܂�Ȃ��ꍇ */
		r |= ((ScrRec->CodeParam) & 0xffff);
		r |= 0x00800000;
		Buf[wp] = r;
		wp++;

		r = ( ((ScrRec->CodeParam) >> 16) & 0xffff);
		Buf[wp] = r;
	} else {
		/* �p�����[�^���i�[���� */
		r |= (ScrRec->CodeParam & 0xffff);
		Buf[wp] = r;
	}

	wp++;
	return 1;
}

int ReadSingleCode(int *Buf, int &rp, LP_SCRIPT_RECORD ScrRec) {
	int r;

	r = Buf[rp++];
	memset(ScrRec, 0, sizeof(SCRIPT_RECORD));

	if (r & 0x40000000) { ScrRec->SepCode = 1; }
	if (r & 0x20000000) { ScrRec->SepParam = 1; }

	ScrRec->CodeType = ((r >> 24) & 0x0f);

	if (r & 0x00200000) {
		ScrRec->VarPoint = 1;
		ScrRec->VarPointDest = ((r >> 19) & 3);
	}

	ScrRec->AccessVarEx = ((r >> 16) & 0x03);

	if (r & 0x00800000) {
		/* �p�����[�^���Q�o�C�g�Ɏ��܂��Ă��Ȃ��ꍇ */
		ScrRec->CodeParam = (r & 0xffff);

		r = Buf[rp++];
		ScrRec->CodeParam |= ((r & 0xffff) << 16);
	} else {
		/* �p�����[�^���i�[���� */
		ScrRec->CodeParam = (r & 0xffff);
	}

	return 1;
}

//�@�R���X�g���N�^�^�f�X�g���N�^
CScript::CScript() {
}

CScript::~CScript() {
}

//�@�c�k�k�̏�����
int CScript::ScriptDLLInit(int ScriptFormat) {

	//�@�R���p�C��������������
	m_Compiler.InitScriptFormat(ScriptFormat);

	//�@����������
	return 1;
}

//�@�X�N���v�g�Ŏg�p�����R�}���h�̒�`�e�[�u�����쐬����
int CScript::ScriptCommandInit(const char *FileName, int uFlagArchive) {
	//�@�R�}���h
	gAppCommand.InitCommandTable(FileName, uFlagArchive);

	//�@OK
	return 1;
}

//�@�R���p�C�����̃f�o�b�O�I�v�V������ݒ肷��
int CScript::SetCompileOption(int uFlagOption) {
	m_Compiler.SetDebugOption(uFlagOption);
	return 1;
}

//�@�\�[�X�t�@�C�����R���p�C������
int CScript::CompileSource(const char *SourceFile) {
	int r;

	if (m_Compiler.ReadSourceFile(SourceFile) == 0) {
		return 0;
	}

	r = 0;
	m_Compiler.CompileText(NULL, r);
	return 1;
}

//�@�쐬�����o�C�i�����t�@�C���ɏ�������
int CScript::WriteScript(const char *BinaryFile, char *lpstrResult) {
	int r;

	memset(sResult, 0, sizeof(sResult));
	r = m_Compiler.WriteScriptFile(BinaryFile, sResult);
	strcpy(lpstrResult, sResult);
	return r;
}

//�@�o�C�i���X�N���v�g�����[�h���āA���s�̏���������
int CScript::ReadBinary(const char *FileName) {

	if ( !m_ScriptPlayer.ReadBinary(FileName) ) {
		sprintf(sResult, "This file is not Binary Script!\n");
		return 0;
	}
	if ( m_ScriptPlayer.InitializeVariable() ) {
		sprintf(sResult, "Shortage of Memory.\n");
		return 0;
	}
	return 1;
}

//�@�X�N���v�g�̂P�X�e�b�v�����s����
int CScript::StepCommand(char *lpstrResult) {
	int r;

	r = m_ScriptPlayer.StepCommand(lpstrResult);
	return r;
}
