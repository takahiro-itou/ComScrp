//////////////////////////////////////////////////////////////////////
//																	//
//		Expression.cpp	CExpression�N���X�̃C���v�������e�[�V����	//
//																	//
//				Copyright Takahiro Ito, 2003/01/13 - 2003/01/13		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Expression.h"

CExpression::CExpression() {
	memset(&m_SingleParameter, 0, sizeof(SCRIPT_RECORD));
	m_lpFuncPtrWriteScript = NULL;
	m_lpCommand = NULL;
	m_BaseInternalVarIndex = 0;
	m_Count = 0;
	m_FlagSingleParam = 0;
}

CExpression::~CExpression() {
	memset(&m_SingleParameter, 0, sizeof(SCRIPT_RECORD));
	m_lpFuncPtrWriteScript = NULL;
	m_lpCommand = NULL;
	m_Count = 0;
	m_FlagSingleParam = 0;
}

void CExpression::Initialize(FUNCPTR_WRITE_SCRIPT_RECORD func_write_script, CCommand *lpCmdTable) {
	memset(&m_SingleParameter, 0, sizeof(SCRIPT_RECORD));
	m_lpFuncPtrWriteScript = func_write_script;
	m_lpCommand = lpCmdTable;
	m_BaseInternalVarIndex;
	m_Count = 0;
	m_FlagSingleParam = 0;
}

int CExpression::ToPolish(CLexer &lexer, int Start, int End) {
	int i, n;
	int r;
	LP_LEXER_TOKEN lpToken;
	COMMAND_TABLE v[MAX_TOKEN], ope[MAX_TOKEN];
	COMMAND_TABLE ct;
	int sp_v, sp_ope;

	/* ������������������ */
	memset(v, 0, sizeof(v));
	memset(ope, 0, sizeof(ope));
	memset(&ct, 0, sizeof(COMMAND_TABLE));
	n = m_Count;

	/* �e���|�����̓����ϐ��̊J�n�C���f�b�N�X */
	sp_v = m_BaseInternalVarIndex++;
	sp_ope = 0;

	/* �ԕ��𗧂Ă� */
	sprintf(ope[sp_ope].Name, "");
	ope[sp_ope].CodeType = 0;
	ope[sp_ope].CodeParam = 0;
	sp_ope++;

	for (i = Start; i <= End; i++) {
		/* �g�[�N�������o�� */
		lpToken = lexer.GetToken(i);

		/* ���o�����g�[�N�������l���ϐ��Ȃ�v�Z�p�X�^�b�N�ɐς� */
		if (lpToken->token_type == TOKEN_NUMERIC) {			/* ���l */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_CONST_NUMBER;
			v[sp_v].CodeParam = atoi(lpToken->token);
			sp_v++;
		} else if (lpToken->token_type == TOKEN_OTHERS) {	/* �ϐ� */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_VAR_EXTERNAL;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_STRING) {	/* ������ */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_CONST_STRING;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_LABEL) {	/* ���x�� */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_LABEL;

			r = m_lpCommand->FindCommand(lpToken->token, &ct);
			_ASSERT (r >= 0);
			v[sp_v].CodeType = ct.CodeType;
			_ASSERT (ct.CodeType == CODETYPE_LABEL);
			v[sp_v].CodeParam = ct.CodeParam;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_OPERATOR) {	/* ���Z�q */

			/* ���Z�q�p�X�^�b�N�̃g�b�v�ƗD�揇�ʂ��r���� */
			while ( GetPri(lpToken->token) <= GetPri(ope[sp_ope - 1].Name)
				&& STRCMP(ope[sp_ope-1].Name , "(") ) {
				/* �X�^�b�N�g�b�v�̉��Z�����s���� */
				Calclate(v, sp_v, ope[sp_ope - 1].Name);
				sp_ope--;
			}

			/* ���o�������Z�q���A")"�łȂ���΁A���̉��Z�q���X�^�b�N�ɐς� */
			if ( STRCMP(lpToken->token, ")") ) {
				sprintf(ope[sp_ope].Name, lpToken->token);
				sp_ope++;
			} else {
				sp_ope--;
			}
		}
	}

	/* �X�^�b�N�̎c����������� */
	while (sp_ope > 1) {
		Calclate(v, sp_v, ope[sp_ope - 1].Name);
		sp_ope--;
	}

	if ( (m_Count - n) <= 0) {
		/* ���l�萔�A�܂��͕ϐ��̒P�Ǝ� */
		strcpy(m_SyntaxResult[0].let_command, "let");
		strcpy(m_SyntaxResult[0].parameter, v[sp_v - 1].Name);
		if (v[sp_v - 1].CodeType == CODETYPE_CONST_NUMBER) {
			m_SyntaxResult[0].flag_parameter = CODETYPE_CONST_NUMBER;
			m_SyntaxResult[0].numeric_parameter = atoi(v[sp_v - 1].Name);
		}
		m_SyntaxResult[0].indll_var_id = sp_v - 1;
		m_SyntaxResult[0].reserved = 0;
		m_Count = n + 1;
		sp_v--;
	}

	/* �������ރf�[�^��1�����Ȃ��ꍇ�́A�œK���p�̃f�[�^���������� */
	if ( (m_Count - n) == 1) {
		/* �V���O���p�����[�^ */
		m_FlagSingleParam = 1;
		m_lpCommand->FindCommand(m_SyntaxResult[0].parameter, &ct);
		m_SingleParameter.SepCode = 0;
		m_SingleParameter.SepParam = 0;
		m_SingleParameter.CodeType = ct.CodeType;
		m_SingleParameter.CodeParam = ct.CodeParam;
		m_SingleParameter.VarPoint = 0;
		m_SingleParameter.VarPointDest = 0;
		return 1;
	}

	return 0;
}

/* �v�Z�����X�N���v�g�ɕϊ����� */
int CExpression::PolishToBinary(int *Buf, int Start, int MaxSize) {
	int i, p;
	COMMAND_TABLE ct;
	SCRIPT_RECORD Cmd;

	if (m_lpFuncPtrWriteScript == NULL) {
		/* �������ݗp�̊֐����p�ӂ���Ă��Ȃ� */
		return 0;
	}

	p = Start;
	for (i = 0; i < m_Count; i++) {
		if ((p - Start) >= MaxSize) {
			/* �o�b�t�@������Ȃ� */
			return 0;
		}

		/* ���s����R�}���h���A�e�[�u������T�� */
		memset(&ct, 0, sizeof(COMMAND_TABLE));
		m_lpCommand->FindCommand(m_SyntaxResult[i].let_command, &ct);
		/* �t�@�[�X�g�o�C�g�A�R�}���h */
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 1;
		Cmd.SepParam = 0;
		_ASSERT(ct.CodeType == CODETYPE_COMMAND_G);
		Cmd.CodeType = ct.CodeType;		/* ���s����R�}���h�̎�� */
		Cmd.CodeParam = ct.CodeParam;	/* ���s����R�}���h�̔ԍ� */
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);

		/* �Z�J���h�o�C�g�F�x�[�X�ϐ� */
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 0;
		Cmd.SepParam = 0;
		Cmd.CodeType = CODETYPE_VAR_INTERNAL;
		Cmd.CodeParam = m_SyntaxResult[i].indll_var_id;
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);

		/* �T�[�h�o�C�g�F�f�B�X�e�B�l�[�V���� */
		memset(&ct, 0, sizeof(COMMAND_TABLE));
		m_lpCommand->FindCommand(m_SyntaxResult[i].parameter, &ct);
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 0;
		Cmd.SepParam = 0;
		Cmd.CodeType = ct.CodeType;		/* �ϐ� */
		Cmd.CodeParam = ct.CodeParam;
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);
	}

	/* ����I�� */
	return (p);
}

/* ���Z�����s���� */
void CExpression::Calclate(COMMAND_TABLE v_stack[], int &val_sp, char *sOpe) {
	int i, cmdID = 0;
	int v1_numeric, v2_numeric, vr;
	COMMAND_TABLE ct;

	/* ���Z�q�ɑΉ�����R�}���h��T�� */
	for (i = 0; i < OPERATOR_TABLE_COUNT; i++) {
		if ( !STRCMP(OperatorTable[i].OpeName, sOpe) ) {
			cmdID = i;
			break;
		}
	}

	/* ���Z�̑ΏۂƂȂ���e�ɂ��āA�Ƃ��ɐ��l�Ȃ�A�œK�����s�� */
	if ( v_stack[val_sp - 2].CodeType == CODETYPE_CONST_NUMBER
		&& v_stack[val_sp - 1].CodeType == CODETYPE_CONST_NUMBER) {
		v1_numeric = v_stack[val_sp - 2].CodeParam;
		v2_numeric = v_stack[val_sp - 1].CodeParam;

		if ( !STRCMP(sOpe, "+") ) {				/* ���Z */
			vr = v1_numeric + v2_numeric;
		} else if ( !STRCMP(sOpe, "-") ) {		/* ���Z */
			vr = v1_numeric - v2_numeric;
		} else if ( !STRCMP(sOpe, "*") ) {		/* ��Z */
			vr = v1_numeric * v2_numeric;
		} else if ( !STRCMP(sOpe, "/") ) {		/* ���Z */
			vr = v1_numeric / v2_numeric;
		} else if ( !STRCMP(sOpe, "%") ) {		/* ��]�Z */
			vr = v1_numeric % v2_numeric;
		} else if ( !STRCMP(sOpe, "&") ) {		/* �_���� */
			vr = v1_numeric & v2_numeric;
		} else if ( !STRCMP(sOpe, "|") ) {		/* �_���a */
			vr = v1_numeric | v2_numeric;
		} else if ( !STRCMP(sOpe, "^") ) {		/* �r���I�_���a */
			vr = v1_numeric ^ v2_numeric;
		}

		sprintf(v_stack[val_sp - 2].Name, "%d", vr);
		v_stack[val_sp - 2].CodeType = CODETYPE_CONST_NUMBER;
		v_stack[val_sp - 2].CodeParam = vr;
		val_sp--;
		return;

	}

	/* �X�^�b�N��[vap_sp - 2]�̓��e���m�F���āA*�łȂ��Ȃ�ALet�R�}���h�����s���� */
	if ( STRCMP(v_stack[val_sp - 2].Name, "*") ) {
		m_SyntaxResult[m_Count].flag_parameter = 0;

		strcpy(m_SyntaxResult[m_Count].let_command, "let");
		strcpy(m_SyntaxResult[m_Count].parameter, v_stack[val_sp - 2].Name);
		
		/* �p�����[�^���萔���ǂ����𒲂ׂĂ��� */
		m_lpCommand->FindCommand(m_SyntaxResult[m_Count].parameter, &ct);
		if (ct.CodeType == CODETYPE_CONST_NUMBER) {
			m_SyntaxResult[m_Count].flag_parameter = CODETYPE_CONST_NUMBER;
			m_SyntaxResult[m_Count].numeric_parameter = ct.CodeParam;
		}

		m_SyntaxResult[m_Count].indll_var_id = (val_sp - 2);
		m_Count++;
	}

	/* indll�ϐ����p�����[�^�ɂ��āA�R�}���h�����s���� */
	m_SyntaxResult[m_Count].flag_parameter = 0;
	strcpy(m_SyntaxResult[m_Count].let_command, OperatorTable[cmdID].CmdName);
	m_SyntaxResult[m_Count].indll_var_id = (val_sp - 2);

	/* �X�^�b�N��[var_sp - 1]�̓��e���m�F���āA���̃R�}���h�̃p�����[�^�����肷�� */
	if ( STRCMP(v_stack[val_sp - 1].Name, "*") ) {
		/* �E�Ӓl���p�����[�^�ɃZ�b�g���� */
		strcpy(m_SyntaxResult[m_Count].parameter, v_stack[val_sp - 1].Name);

		/* �p�����[�^���萔���ǂ����𒲂ׂĂ��� */
		m_lpCommand->FindCommand(m_SyntaxResult[m_Count].parameter, &ct);
		if (ct.CodeType == CODETYPE_CONST_NUMBER) {
			m_SyntaxResult[m_Count].flag_parameter = CODETYPE_CONST_NUMBER;
			m_SyntaxResult[m_Count].numeric_parameter = ct.CodeParam;
		}
	} else {
		/* indll�ϐ����p�����[�^�ɂ��� */
		sprintf(m_SyntaxResult[m_Count].parameter, "iv%04d", (val_sp - 1));

		sprintf(ct.Name, m_SyntaxResult[m_Count].parameter);
		ct.CodeType = CODETYPE_VAR_INTERNAL;
		ct.CodeParam = val_sp - 1;

		m_lpCommand->AddCommand(&ct);
		m_SyntaxResult[m_Count].flag_parameter = CODETYPE_VAR_INTERNAL;
		m_SyntaxResult[m_Count].numeric_parameter = (val_sp - 1);
	}
	m_Count++;

	/* �X�^�b�N��[var_sp - 2]�̓��e�� '*'�ɂ��āA���̒l���ϐ����܂ނ��Ƃ�m�点�� */
	strcpy(v_stack[val_sp - 2].Name, "*");
	v_stack[val_sp - 2].CodeType = CODETYPE_VAR_INTERNAL;
	v_stack[val_sp - 2].CodeParam = val_sp - 2;

	/* �X�^�b�N�|�C���^�𒲐����� */
	val_sp--;
}

int CExpression::GetSingleParameter(LP_SCRIPT_RECORD lpResult) {
	if (m_FlagSingleParam == 0) { return 0; }
	memcpy(lpResult, &m_SingleParameter, sizeof(SCRIPT_RECORD));
	return 1;
}
