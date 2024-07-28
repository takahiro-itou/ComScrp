//////////////////////////////////////////////////////////////////////
//																	//
//		Types.h			�^���A�y�э\���̂̐錾						//
//																	//
//				Copyright Takahiro Ito, 2003/01/25 - 2003/01/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_TYPES_H__
#define __COMSCRP_INCLUDE_TYPES_H__

//�@�����f�[�^
typedef struct {
	char Name[64];		/* �P�� */
	int CodeType;		/* �R�[�h��� */
	int CodeParam;		/* �p�����[�^ */
	int DataSize;		/* �ϐ��Ȃǂ���L����f�[�^�T�C�Y */
	int Reserved;		/* �\�� */
} COMMAND_TABLE, *LP_COMMAND_TABLE;

//�@�X�N���v�g�̃��R�[�h
typedef union {
	int Value;
	int *Address;
	char *String;
} SCRIPT_RECORD_APPREQUESTED, *LP_SCRIPT_RECORD_APPREQUESTED;
typedef SCRIPT_RECORD_APPREQUESTED SRAPP;
typedef LP_SCRIPT_RECORD_APPREQUESTED LP_SRAPP;

typedef struct {
	int SepCode;		/* �R�[�h�̋�؂� */
	int SepParam;		/* �p�����[�^�̋�؂� */
	int CodeType;		/* �R�[�h�̎�� */
	int CodeParam;		/* �R�[�h�̃p�����[�^ */
	int VarPoint;		/* �ϐ��|�C���^ */
	int VarPointDest;	/* �ϐ��|�C���^�̑Ώ� */
	int DataPoint;		/* ������f�[�^�̎Q�� */
	int DataAddress;	/* ������f�[�^�̈ʒu */

	char *StrData;		/* ������f�[�^�̃|�C���^ */
	int AccessVarEx;	/* �g���ϐ��A�N�Z�X */

	SRAPP AppRequested;	/* �A�v���P�[�V���������v�������t�H�[�}�b�g�̃f�[�^ */
	int Reserved0;		/* �\�� */

	int Reserved3[4];
} SCRIPT_RECORD, *LP_SCRIPT_RECORD;

/* ���x�� */
#define MAX_LABEL	65536
typedef struct {
	char Name[60];			/* ���x���� */
	int Address;			/* �A�h���X */
} LABEL_INFO, *LP_LABEL_INFO;

/* �֐��̌Ăяo�� */
#define MAX_FUNC	65536
typedef struct {
	char FuncName[64];		/* �֐��� */
	int ParamCount;			/* �p�����[�^�� */
	int EntryPoint;			/* �J�n�A�h���X */
	int ExitPoint;			/* �I���A�h���X */
	int Reserved0;			/* �\�� */
} FUNC_INFO, *LP_FUNC_INFO;

typedef struct {
	int ParamCount;
	int EntryPoint;
	int ExitPoint;
	int Reserved0;
} FUNC_TABLE, *LP_FUNC_TABLE;

// �ϐ��ւ̃A�N�Z�X
typedef struct {
	int AccessType;				// ���:(0:�萔,1:���l�萔,2:������萔,3:������ϐ�)
	union ACCESSVALUE{
		int IntConst;			// ���l�萔
		int *lpVarPointer;		// ���l�ϐ��ւ̃|�C���^
		char *lpStrPointer;		// ������萔�A�ϐ�
		int EntryIndex;			// �֐��̃G���g���ԍ�
		int JumpAddress;		// ���x���̃W�����v��
	} Data;
} SCRIPT_ACCESS, *LP_SCRIPT_ACCESS;

#define ACCESS_NO_ENTRY		0
#define ACCESS_INT_CONST	1
#define ACCESS_INT_VAR		2
#define ACCESS_STR_CONST	3
#define ACCESS_STR_VAR		4
#define ACCESS_FUNCTION		9
#define ACCESS_LABEL		10

#endif
