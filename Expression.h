//////////////////////////////////////////////////////////////////////
//																	//
//		Expression.h	CExpression�N���X�̃C���^�[�t�F�C�X			//
//																	//
//		�N���X�̊T�v�F���������i�t�|�[�����h�L�@�j					//
//				Copyright Takahiro Ito, 2003/01/13 - 2003/01/13		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_EXPRESSION_H__
#define __COMSCRP_INCLUDE_EXPRESSION_H__

#include "Lexer.h"
#include "Command.h"

/* ���Z�q�̗D�揇�ʂƁA�R�}���h�Ƃ̑Ή� */
typedef struct {
	char OpeName[12];	/* ���Z�q */
	int Priority;		/* �D�揇�� */
	char CmdName[16];	/* �R�}���h�� */
} OPERATOR_TABLE;

/* ��͌��� */
typedef struct {
	char let_command[32];	/* let??? */
	char parameter[64];		/* parameter */
	int indll_var_id;		/* source variable id */
	int flag_parameter;		/* parameter�����l�萔�Ȃ�1,������萔�Ȃ�2 */
	int numeric_parameter;	/* �p�����[�^�����l�̎��̂� */
	int reserved;
} SYNTAX_ANALYZE_RESULT, *LP_SYNTAX_ANALYZE_RESULT;

class CExpression {
public:
	CExpression();
	~CExpression();

	void Initialize(FUNCPTR_WRITE_SCRIPT_RECORD func_write_script, CCommand *lpCmdTable);
	int ToPolish(CLexer &lexer, int Start, int End);
	int PolishToBinary(int *Buf, int Start, int MaxSize);

	int GetSingleParameter(LP_SCRIPT_RECORD lpResult);

private:
	int GetPri(const char *sOpe);
	void Calclate(COMMAND_TABLE v_stack[], int &val_sp, char *sOpe);

private:
	/* �p�����[�^�̖|�󌋉� */
	int m_Count;
	SYNTAX_ANALYZE_RESULT m_SyntaxResult[MAX_TOKEN];

	/* �V���O���p�����[�^ */
	int m_FlagSingleParam;
	SCRIPT_RECORD m_SingleParameter;

	/* �X�N���v�g�̃��R�[�h�������݊֐� */
	FUNCPTR_WRITE_SCRIPT_RECORD m_lpFuncPtrWriteScript;

	CCommand *m_lpCommand;							//�@�R�}���h�e�[�u��

	int m_BaseInternalVarIndex;		/* ���[�N�Ŏg���ϐ��̍ŏ��̔ԍ� */
};

/* ���Z�q�̗D�揇�ʕ\ */
static OPERATOR_TABLE OperatorTable[] = {
	{")",   0, ""},
	{"||",  2, "LogOr"},  {"&&",  3, "LogAnd"},
	{"|",   5, "BitOr"},  {"or",  5, "BitOr"},
	{"^",   6, "BitXor"}, {"xor", 6, "BitXor"},
	{"&",   7, "BitAnd"}, {"and", 7, "BitAnd"},
	{"==",  9, "IsEqu"},  {"!=",  9, "IsNeq"},
	{"<",  10, "IsSml"},  {">",  10, "IsGra"},
	{"<=", 10, "IsESm"},  {">=", 10, "IsEGr"},
	{"<<", 13, "shl"},  {">>", 13, "shr"},
	{"+",  14, "Add"},  {"-",  14, "Sub"},
	{"*",  15, "Mul"},  {"/",  15, "Div"}, {"%", 15, "Mod"},
	{"(", 65535}
};
#define OPERATOR_TABLE_COUNT	( sizeof(OperatorTable)/sizeof(OPERATOR_TABLE) )

//�@�C�����C�������o�֐�
inline int CExpression::GetPri(const char *sOpe) {

	if ( sOpe == NULL || (*sOpe) == 0 ) {
		return -1;
	}

	for (int i = 0; i < OPERATOR_TABLE_COUNT; i++) {
		if ( !STRCMP(OperatorTable[i].OpeName, sOpe) ) {
			return (OperatorTable[i].Priority);
		}
	}
	return (-1);
}

#endif
