//////////////////////////////////////////////////////////////////////
//																	//
//		Player.h		CScriptPlayer�N���X�̃C���^�[�t�F�C�X		//
//																	//
//		�N���X�̊T�v�F�X�N���v�g�̎��s�A�t�R���p�C��(�f�o�b�O�p)	//
//				Copyright Takahiro Ito, 2003/01/14 - 2003/02/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_PLAYER_H__
#define __COMSCRP_INCLUDE_PLAYER_H__

#include "Command.h"
#include "Stream.h"

int ReadSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);

class CScriptPlayer {
public:
	CScriptPlayer();
	~CScriptPlayer();

	/* �������A�ǂݍ��� */
	int InitializeVariable();
	int ReadBinary(const char *FileName);

	/* ���s */
	int SetScriptPosition(int nIP) { m_ScriptPos = nIP; return (m_ScriptPos); }
	int StepCommand(char *lpstrResult);

	/* �A�v���P�[�V�����ւ̎w�� */
	int GetLastCommand(LP_SCRIPT_RECORD lpRecord) { memcpy(lpRecord, &m_LastCommand, sizeof(SCRIPT_RECORD)); return 1; }
	int GetParameterCount() { return (m_CmdParamCount); }
	int GetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags);

	/* �o�C�i���X�N���v�g�̃_���v */
	int DumpBinaryScript(const char *FileName);
	int FindLabel(int addr, char *lpstrResult);
	int FindFunction(int addr, char *lpstrFuncName);
	int FindEndFunc(int Addr, char *lpstrFuncName);

	//�@�ϐ�����
	int GetLocalVar(int lpReturn[], int nCount) { memcpy(lpReturn, m_lpVarWork, sizeof(int) * nCount); return 1; }
	int SetLocalVar(int lpValue[], int nCount)  { memcpy(m_lpVarWork, lpValue,  sizeof(int) * nCount); return 1; }
	int GetSystemVar(int lpReturn[], int nCount) { memcpy(lpReturn, m_lpVarSystem, sizeof(int) * nCount); return 1; }
	int SetSystemVar(int lpValue[], int nCount)  { memcpy(m_lpVarSystem, lpValue,  sizeof(int) * nCount); return 1; }

private:
	//�@�A�N�Z�X����ϐ��̃|�C���^���擾����
	int GetAccessVar(LP_SCRIPT_RECORD lpRecord, LP_SCRIPT_ACCESS lpAccess);

private:
	/* �X�N���v�g�f�[�^ */
	int m_ScriptSize;		/* �X�N���v�g�̃T�C�Y */
	int *m_lpScriptBase;	/* �X�N���v�g�̊i�[���ꂽ������ */
	int m_ScriptPos;		/* ���ݎ��s���̃X�N���v�g�̈ʒu */

	int m_DataSize;			/* �f�[�^�̃T�C�Y */
	char *m_lpDataBase;		/* �f�[�^�̊i�[���ꂽ������ */

	int m_LabelCount;		/* ���x���̐� */
	int *m_lpLabel;			/* ���x���̃W�����v�� */

	int m_FuncCount;		/* �֐��̐� */
	FUNC_TABLE *m_lpFuncAddr;	/* �֐��̃W�����v�� */

	// �f�o�b�O���
	int m_FlagDebugInfo;		/* �f�o�b�O���̗L�� */
	LP_LABEL_INFO m_lpLabelInfo;
	FUNC_INFO *m_lpFuncCall;	/* �֐��̌Ăяo���Ɋւ����� */

	// �Ō�Ɏ��s�����R�}���h�̃p�����[�^
	SCRIPT_RECORD m_LastCommand;	//�R�}���h
	int m_CmdParamCount;			//�p�����[�^��
	SCRIPT_RECORD m_CmdParam[64];	//�p�����[�^

	//�@�ϐ��̈�
	int *m_lpVarWork;		//�@���[�N�ϐ�
	int *m_lpVarSystem;		//�@�V�X�e���ϐ�
	int *m_lpVarPrivate;	//�@�v���C�x�[�g�ϐ�
	int *m_lpVarStack;		//�@�X�^�b�N�̈�
	char *m_lpStrWork;		//�@������ϐ�

	/* �R�[���X�^�b�N */
	int m_CallStackPointer;		/* SP */
	int m_CallStack[256];		/* �X�^�b�N�̓��e */

	//�@�X�g���[��
	CStream *m_Stream;
};

#endif
