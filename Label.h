//////////////////////////////////////////////////////////////////////
//																	//
//		Label.h			CLabel�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F���x���A�֐��Ăяo���̏���					//
//				Copyright Takahiro Ito, 2003/01/24 - 2003/01/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LABEL_H__
#define __COMSCRP_INCLUDE_LABEL_H__

#include "Command.h"

#define MAX_LABEL	65536
#define MAX_FUNC	65536

/* ���x����` */
typedef struct {
	char Name[48];		/* ���x���� */
	int LabelID;		/* ���x����ID */
	int JumpAddr;		/* �W�����v��A�h���X */
	int Reserved0;
	int Reserved1;
} LABEL_INFO, LP_LABEL_INFO;

/* �֐��Ăяo�����̃p�����[�^ */
typedef struct {
	char Name[28];			/* �������̖� */
	int CodeType;			/* �������̌^ */
} FUNC_PARAM_INFO, LP_FUNC_PARAM_INFO;

/* �֐��Ăяo�� */
typedef struct {
	char Name[48];			/* �֐��� */
	int ParameterCount;		/* �����̐� */
	LP_FUNC_PARAM_INFO *Arg;	/* �����̏�� */
} FUNC_CALL_INFO, LP_FUNC_CALL_INFO;

class CLabel {
	CLabel();
	~CLabel();

	int Initialize(CCommand *lpCmdTable);

	int ReferLabel(char *LabelName, LP_LABEL_INFO lpLabel);
	int RegisterLabel(char *LabelName, LP_LABEL_INFO lpLabel);

	int RegisterFunction(char *FuncName, LP_FUNC_CALL_INFO lpFunc);
	int ReferFunction(char *FuncName, LP_FUNC_CALL_INFO lpFunc);

	int GetLabelCount();
	int GetFunctionCount();
private:
	CCommand theLocalCommand;			/* ���[�J���p�R�}���h�e�[�u�� */
	LABEL_INFO m_Label[MAX_LABEL];		/* ���x�� */
	FUNC_CALL_INFO m_Func[MAX_FUNC];	/* �֐� */
};

//�@�C�����C�������o�֐�
inline int GetLabelCount() {
	int r;
	r = theLocalCommand.Get
#endif
