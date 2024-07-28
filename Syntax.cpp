//////////////////////////////////////////////////////////////////////
//																	//
//		Syntax.cpp		CSyntax�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2002/12/27 - 2002/12/28		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Syntax.h"

// �R���X�g���N�^�^�f�X�g���N�^
CSyntax::CSyntax() {
	InitSyntax();
}

CSyntax::~CSyntax() {
	m_ParameterCount = 0;
	m_lpCommand = NULL;
	m_lpLexer = NULL;
}

//�@������
int CSyntax::InitSyntax() {
	m_ParameterCount = 0;
	return 1;
}

//�@�\�����
int CSyntax::SyntaxAnalyze(CLexer &lexer, CCommand &cmd, int *Buf, int &wp,
						   FUNCPTR_WRITE_SCRIPT_RECORD func_write, char *lpDataArea, int &DataSize)
{
	int i, count, r;
	int start, end, param_count;
	int flagString;
	LP_LEXER_TOKEN lpToken, lpTokenPr;
	COMMAND_TABLE CmdTable;
	SCRIPT_RECORD ScrRec;
	CExpression ExpressionAnalyze;

	//�N���X�I�u�W�F�N�g�̃|�C���^���i�[����B����Ƀg�[�N���������o��
	m_lpLexer = &lexer;
	m_lpCommand = &cmd;
	count = m_lpLexer->GetTokenCount();

	/*
		�t�F�[�Y�P�@�R�}���h�ȗ�����
		������߂̏ȗ��𔻒肷��
	*/

	// ��P�g�[�N�������o���A�R�}���h�����ǂ����𔻒肷��
	lpToken = m_lpLexer->GetToken(0);
	r = m_lpCommand->FindCommand(lpToken->token, &CmdTable);

	if ( r == -1) {
		// ����`�̎��ʎq
		return -(CLERR_UNDEC_IDENTIFIER);
	}

	if ( (CmdTable.CodeType & 0xfffffffc) == 0 ) { 
		/* �R�}���h�� */
		memcpy(&m_tokenCommand, lpToken, sizeof(LEXER_TOKEN));
		i = 1;

	} else if ( (CmdTable.CodeType & 0xffffffffc) == 4
		|| CmdTable.CodeType == CODETYPE_LOCAL_VAR
		|| CmdTable.CodeType == CODETYPE_LOCAL_PARAMETER) {
		/* ���o�^�܂��́A�ϐ��� */
		lpTokenPr = m_lpLexer->GetToken(1);
		if ( !STRCMP(lpTokenPr->token, "=") ) {
			/* ����\�� */
			strcpy(m_tokenCommand.token, "Let");

			/* ���Ӓl���P�p�����[�^�ɃZ�b�g���� */
			memset(&ScrRec, 0, sizeof(SCRIPT_RECORD));
			ScrRec.SepCode = 0;
			ScrRec.SepParam = 0;
			ScrRec.CodeType = CmdTable.CodeType;
			ScrRec.CodeParam = CmdTable.CodeParam;
			ScrRec.VarPoint = 0;
			ScrRec.VarPointDest = 0;
			AddParameter(&ScrRec);
			
			i = 2;

		} else if ( (lpTokenPr->token[0] != '=') && (lpTokenPr->token[1] == '=') ) {
			/* �v�Z����\�� */
			start = -1;
			for (i = 0; i < OPERATOR_TABLE_COUNT; i++) {
				if (OperatorTable[i].OpeName[0] == lpTokenPr->token[0]) {
					start = i;
					break;
				}
			}
			if (start >= 0) {
				strcpy(m_tokenCommand.token, OperatorTable[i].CmdName);

				/* ���Ӓl���P�p�����[�^�ɃZ�b�g���� */
				memset(&ScrRec, 0, sizeof(SCRIPT_RECORD));
				ScrRec.SepCode = 0;
				ScrRec.SepParam = 0;
				ScrRec.CodeType = CmdTable.CodeType;
				ScrRec.CodeParam = CmdTable.CodeParam;
				ScrRec.VarPoint = 0;
				ScrRec.VarPointDest = 0;
				AddParameter(&ScrRec);

				i = 2;
			} else {
				/* �\���G���[ */
				return 0;
			}

		} else {
			/* ����ȊO */
			i = 0;
		}

	} else {
		/* �\���G���[ */
		i = 0;
		return 0;
	}

	/*
		�t�F�[�Y�Q�@�p�����[�^���
		�p�����[�^�̐������t�|�[�����h�L�@�ɖ|�󂷂�
	*/

	//�@�p�����[�^����͂���
	flagString = 0;
	ExpressionAnalyze.Initialize(func_write, m_lpCommand);
	param_count = 0;
	for (start = i; i < count; i++) {
		lpToken = lexer.GetToken(i);
		m_lpCommand->FindCommand(lpToken->token, &CmdTable);

		if (i == start) { 
			//�@�����񑀍삩�ǂ��������肷��
			if (lpToken->token_type == TOKEN_STRING || CmdTable.CodeType == CODETYPE_VAR_STRING) {
				// �����񑀍�
				flagString = 1;
			} else {
				flagString = 0;
			}
		}

		if (lpToken->token_type == TOKEN_SEPARATOR) {
			end = i - 1;
			RegisterParameter(&ExpressionAnalyze, lexer, Buf, wp, start, end, flagString, param_count, lpDataArea, DataSize);
			param_count++;
			start = i + 1;
		}
	}

	//�@�o�b�t�@�Ɏc���Ă���(�Ō��)�p�����[�^����������
	end = count - 1;
	if (start <= end) {
		RegisterParameter(&ExpressionAnalyze, lexer, Buf, wp, start, end, flagString, param_count, lpDataArea, DataSize);
		param_count++;
	}

	/* �p�����[�^�̌���Ԃ� */
	return (m_ParameterCount);
}

//�@�p�����[�^�̒ǉ�
int CSyntax::AddParameter(LP_SCRIPT_RECORD lpScrRec) {
	memcpy(&m_scrParameter[m_ParameterCount], lpScrRec, sizeof(SCRIPT_RECORD));
	m_ParameterCount++;
	return 1;
}

//�@�p�����[�^�̓o�^����
int CSyntax::RegisterParameter(CExpression *lpExpression, CLexer &lexer, int *Buf, int &wp,
							   int Start, int End, int FlagStr, int ParameterCount,
							   char *lpDataArea, int &DataSize) {
	int i;
	int r;
	COMMAND_TABLE Cmd;
	SCRIPT_RECORD ScrRec;
	LP_LEXER_TOKEN lpToken;

	if (FlagStr == 0) {
		// ���l�p�����[�^
		if (lpExpression->ToPolish(lexer, Start, End)) {
			// �V���O���p�����[�^
			lpExpression->GetSingleParameter(&ScrRec);
			AddParameter(&ScrRec);
		} else {
			r = lpExpression->PolishToBinary(Buf, wp, 65536 * 4);
			wp = r;
			
			// �C���^�[�i���ϐ����p�����[�^�Ƃ���
			memset(&ScrRec, 0, sizeof(SCRIPT_RECORD));
			ScrRec.SepCode = 0;
			ScrRec.SepParam = 1;
			ScrRec.CodeType = CODETYPE_VAR_INTERNAL;
			ScrRec.CodeParam = ParameterCount;
			ScrRec.VarPoint = 0;
			ScrRec.VarPointDest = 0;
			ScrRec.AccessVarEx = 0;

			AddParameter(&ScrRec);
		}
	} else {
		// ������p�����[�^
		// +�A&�ȊO�̉��Z�q�͑S�Ė�������B
		for (i = Start; i <= End; i++) {
			lpToken = lexer.GetToken(i);
			if (lpToken->token_type == TOKEN_OPERATOR) {
				if ( STRCMP(lpToken->token, "+") && STRCMP(lpToken->token, "&") ) {
					continue;
				}
			} 
			if (lpToken->token_type == TOKEN_STRING) {
				// ������萔
				Cmd.CodeType = CODETYPE_CONST_STRING;
				Cmd.CodeParam = DataSize;

				strcpy(&lpDataArea[DataSize], lpToken->token);
				DataSize += (strlen(lpToken->token) + 1);
				DataSize = (DataSize + 15) & 0xfffffff0;
			} else {
				r = m_lpCommand->FindCommand(lpToken->token, &Cmd);
			}

			memset(&ScrRec, 0, sizeof(SCRIPT_RECORD));
			ScrRec.SepCode = 0;
			ScrRec.SepParam = 1;
			ScrRec.CodeType = Cmd.CodeType;
			ScrRec.CodeParam = Cmd.CodeParam;
			ScrRec.VarPoint = 0;
			ScrRec.VarPointDest = 0;
			ScrRec.AccessVarEx = 0;

			AddParameter(&ScrRec);
		}
	}

	return wp;
}
		
//�@�p�����[�^�̏�������
int CSyntax::WriteParameter(int index, int *Buf, int &wp, FUNCPTR_WRITE_SCRIPT_RECORD func_write) {
	int r;
	r = wp;
	(*func_write)(Buf, wp, &m_scrParameter[index]);
	return (wp - r);
}

