//////////////////////////////////////////////////////////////////////
//																	//
//		Module.h		CModule�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F���W���[�������̊Ǘ�							//
//				Copyright Takahiro Ito, 2003/01/23 - 2003/01/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_MODULE_H__
#define __COMSCRP_INCLUDE_MODULE_H__

/* ���x���\�� */
typedef struct {
	int ModuleID;			/* ���W���[����ID */
	char LabelName[52];		/* ���x���� */
	int LabelID;			/* ���x����ID */
	int Address;			/* �W�����v�� */
} MODULE_LABEL, LP_MODULE_LABEL;

/* �֐��Ăяo�� */
typedef struct {
	int ModuleID;				/* ���W���[��ID */
	char FunctionName[252];		/* �֐��� */
	int FunctionID;				/* �֐�ID */
	int FunctionAddress;		/* �֐��̃A�h���X */
} MODULE_FUNC, LP_MODULE_FUNC;

// CModule�N���X
class CModule {
public:
	CModule();
	~CModule();

	int CreateModule(char ModuleName);
	int EnterModule(char ModuleName);
	int ExitModule(char ModuleName);
private:

	int m_ModuleCount;				/* ���W���[���̐� */
	int m_CurrentModule;			/* ���݂̃��W���[�� */
};

#endif
