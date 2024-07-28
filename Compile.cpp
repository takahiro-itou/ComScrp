//////////////////////////////////////////////////////////////////////
//																	//
//		Compile.cpp		CCompile�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/12/25		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Compile.h"
#include <stdio.h>

//�@�R���X�g���N�^�^�f�X�g���N�^
CCompile::CCompile() {
	InitScriptFormat(SCRIPT_SEMICORON);
	m_lpSource = NULL;
	m_lpScript = NULL;
	m_lpData = NULL;
	m_SourceSize = 0;
	m_ScriptSize = 0;

	m_lpNextIn = NULL;
	m_lpNextOut = NULL;
	m_lpDataOut = NULL;
	m_TotalIn = 0;
	m_TotalOut = 0;
	m_TotalDataSize = 0;

	m_LabelCount = 0;
	m_lpLabel = NULL;
	m_lpLabelInfo = NULL;

	m_FuncCount = 0;
	m_lpFuncAddr = NULL;
	m_lpFuncCall = NULL;

	m_IfElseSP = 0;

	m_lpDebugScript = NULL;
	m_DebugScriptSize = 0;
}

CCompile::~CCompile() {
	MEMORY_FREE(m_lpSource);
	MEMORY_FREE(m_lpScript);
	MEMORY_FREE(m_lpData);
	MEMORY_FREE(m_lpLabel);
	MEMORY_FREE(m_lpLabelInfo);
	MEMORY_FREE(m_lpFuncCall);
	MEMORY_FREE(m_lpFuncAddr);

	m_SourceSize = 0;
	m_ScriptSize = 0;

	m_lpNextIn = NULL;
	m_lpNextOut = NULL;
	m_TotalIn = 0;
	m_TotalOut = 0;
	m_TotalDataSize = 0;

	m_LabelCount = 0;

	MEMORY_FREE(m_lpDebugScript);
	m_DebugScriptSize = 0;
}

//////////////////////////////////////////////////////////////////////
//�@�R���p�C���̃��C������
int CCompile::CompileText(char *sExitStatement, int WritePos) {
	int i, c, r, wp;
	int nLocalVarSize;
	int nLocalParamSize;
	char sLine[1024];
	CLexer lexer;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;

	if (sExitStatement == NULL || !STRCMP(sExitStatement, "init")) {
		/* �O�̏����̃N���A */
		MEMORY_FREE(m_lpScript);
		MEMORY_FREE(m_lpData);
		MEMORY_FREE(m_lpLabel);
		MEMORY_FREE(m_lpFuncAddr);
		MEMORY_FREE(m_lpFuncCall);
		m_ScriptSize = 0;
		m_TotalDataSize = 0;
		m_LabelCount = 0;
		m_FuncCount = 0;

		/*�@�X�^���o�C		�f�o�b�O�p�o�͂��ݒ肳��Ă���΁A���������m�ۂ��� */
		if (m_FlagDebugOption & DEBUGOPT_LEXER) {
			m_lpDebugScript = (char *)MEMORY_ALLOC(m_SourceSize);
			memset(m_lpDebugScript, 0, m_SourceSize);
		}

		/* �|�C���^�̃Z�b�g�@*/
		m_lpNextIn = m_lpSource;

		/* �������̊m�� */
		m_lpScript = (int *)(MEMORY_ALLOC(65536*4));
		m_lpNextOut = m_lpScript;
		m_lpData = (char *)(MEMORY_ALLOC(65536*4));
		m_lpDataOut = m_lpData;
		m_TotalDataSize = 0;

		memset(m_lpScript, 0xff, 65536*4);
		memset(m_lpData, 0, 65536*4);

		m_lpLabel = (int *)MEMORY_ALLOC(MAX_LABEL * sizeof(int));
		m_lpLabelInfo = (LP_LABEL_INFO)MEMORY_ALLOC(MAX_LABEL * sizeof(LABEL_INFO));
		m_lpFuncAddr = (LP_FUNC_TABLE)MEMORY_ALLOC(MAX_FUNC * sizeof(FUNC_TABLE));
		m_lpFuncCall = (LP_FUNC_INFO) MEMORY_ALLOC(MAX_FUNC * sizeof(FUNC_INFO));

		m_FlagNowFunction = 0;

		wp = 0;

		//�@�V�X�e�����x�� main��o�^���A�����փW�����v����
		r = RegisterLabel("main", 0);
		
		strcpy(ct.Name, "main");
		ct.CodeType = CODETYPE_LABEL;
		ct.CodeParam = r;
		ct.DataSize = 1;
		ct.Reserved = 0;

		gAppCommand.AddCommand(&ct);
		r = CompileStatement("goto #main", wp);

		RegisterLabel("main", r);
		wp = r;
	} else {
		/* ���[�J���֐��̏ꍇ�̏��� */
		nLocalVarSize = 0;
		wp = WritePos;
		m_FlagNowFunction = 1;

		// �o�^����
		c = lexer.LexicalAnalyze(m_FunctionText);
		r = 2;
		for (i = 0; i < c; i++) {
			token = lexer.GetToken(i);
			if ( !STRCMP(token->token, m_FunctionName) ) {
				r = i;
				break;
			}
		}
		for (i = r + 1; i < c; i++) {
		}

		/* �v�����[�O�R�[�h */
		r = CompileStatement("push BP", wp); wp = r;
		r = CompileStatement("BP = SP", wp); wp = r;
		WritePos = wp;
		r = CompileStatement("SP -= 0", wp); wp = r;
	}
	nLocalVarSize = 0;

	/*�@���[�h			�\�[�X�X�N���v�g����P�s��ǂݍ��� */
	for (;;) {
		memset(sLine, 0, sizeof(sLine));
		if (ReadLine(sLine) <= 0) { break; }
		
		/*�@�g�[�N���ɕ�������	*/
		c = lexer.LexicalAnalyze(sLine);

		/*�@�f�o�b�O�p�o��	*/
		if (m_FlagDebugOption & DEBUGOPT_LEXER) {
			for (i = 0; i < c; i++) {
				if (i) {
					strcat(m_lpDebugScript, " ");
					m_DebugScriptSize++;
				}

				token = lexer.GetToken(i);
				strcat(m_lpDebugScript, token->token);
				m_DebugScriptSize += strlen(token->token);
			}
			m_lpDebugScript[m_DebugScriptSize++] = 13;
			m_lpDebugScript[m_DebugScriptSize++] = 10;
		}

		/*�@�錾���ǂ����𔻒f���� */
		if (c == 0) { break; }

		/* �I���X�e�[�g�����g���ǂ����𒲂ׂ� */
		token = lexer.GetToken(0);
		if (sExitStatement != NULL && STRCMP(sExitStatement, "init") ) {
			if ( !STRCMP(token->token, sExitStatement) ) {
				/* �v�����[�O�R�[�h���������� */
				c = wp;
				wp = WritePos;
				if (nLocalVarSize >= 65535) {
					return 0;
				}
				sprintf(sLine, "SP -= %d", nLocalVarSize);
				r = CompileStatement(sLine, wp);
				wp = c;

				/* �I���|�C���g���������� */
				RegisterFunction(m_FunctionName, 0, 0, wp);

				// result�R�}���h����������
				m_ExtraProc[0].WriteExitPoint(wp);

				/* �G�s���[�O�R�[�h���������� */
				r = CompileStatement("SP = BP", wp); wp = r;
				r = CompileStatement("pop BP", wp);  wp = r;
				r = CompileStatement("return", wp);  wp = r;

				// ���[�J���ϐ��̒�`���폜����
				gAppCommand.DeleteCommandType(CODETYPE_LOCAL_VAR);
				gAppCommand.DeleteCommandType(CODETYPE_LOCAL_PARAMETER);
				m_FlagNowFunction = 0;

				return (r);
			}
		}

		if ( !STRCMP(token->token, "int")) {
			/* ���l�ϐ��̐錾 */
			for (i = 1; i < c; i += 2) {
				token = lexer.GetToken(i);

				if (m_FlagNowFunction == 0) {
					r = gAppCommand.GetCountSpecifiedType(CODETYPE_VAR_EXTERNAL);
					gAppCommand.AddCommand(token->token, CODETYPE_VAR_EXTERNAL, r);
				} else {
					r = gAppCommand.GetCountSpecifiedType(CODETYPE_LOCAL_VAR);
					gAppCommand.AddCommand(token->token, CODETYPE_LOCAL_VAR, r);
				}
				nLocalVarSize++;

				if ( (i + 1) == c) { break; }
				token = lexer.GetToken(i+1);
				if (token->token_type != TOKEN_SEPARATOR) {
					/* �\���G���[ */
					return 0;
				}
			}
			continue;

		} else if ( !STRCMP(token->token, "function")) {
			if (m_FlagNowFunction) {
				/* �G���[ */
				return 0;
			}

			/* �֐������擾���� */
			token = lexer.GetToken(1);
			strcpy(m_FunctionName, token->token);
			strcpy(ct.Name, m_FunctionName);
			strcpy(m_FunctionText, sLine);

			// �����̃��X�g���쐬����
			nLocalParamSize = 0;
			if (c >= 3) {
				for (i = 2; i < c; i+=2) {
					token = lexer.GetToken(i);

					r = gAppCommand.GetCountSpecifiedType(CODETYPE_LOCAL_PARAMETER);
					gAppCommand.AddCommand(token->token, CODETYPE_LOCAL_PARAMETER, r);
					nLocalParamSize++;

					if ( (i + 1) == c) { break; }
					token = lexer.GetToken(i+1);
					if (token->token_type != TOKEN_SEPARATOR) {
						return 0;				/* �\���G���[ */
					}
				}
			}

			// ���̊֐���o�^����
			m_FlagNowFunction = 1;
			r = RegisterFunction(m_FunctionName, nLocalParamSize, wp, 0);

			ct.CodeType = CODETYPE_FUNCTION;
			ct.CodeParam = r;
			gAppCommand.AddCommand(&ct);
			m_ExtraProc[0].InitExitPointList();

			r = CompileText("endfunc", wp);
			if (r == 0) {
				return 0;
			}

			m_FlagNowFunction = 0;
			wp = r;
			continue;

		} else if ( !STRCMP(token->token, "prototype")) {
			/* �֐��̃v���g�^�C�v�錾 */
			token = lexer.GetToken(1);
			if ( STRCMP(token->token, "function") ) {
				return 0;
			}

			token = lexer.GetToken(2);
			strcpy(m_FunctionName, token->token);
			strcpy(ct.Name, m_FunctionName);

			// �����̃��X�g���쐬����
			nLocalParamSize = 0;
			if (c >= 4) {
				for (i = 3; i < c; i+=2) {
					token = lexer.GetToken(i);

					r = gAppCommand.GetCountSpecifiedType(CODETYPE_LOCAL_PARAMETER);
					gAppCommand.AddCommand(token->token, CODETYPE_LOCAL_PARAMETER, r);
					nLocalParamSize++;

					if ( (i + 1) == c) { break; }
					token = lexer.GetToken(i+1);
					if (token->token_type != TOKEN_SEPARATOR) {
						return 0;				/* �\���G���[ */
					}
				}
			}

			// ���̊֐���o�^����
			r = RegisterFunction(m_FunctionName, 0, wp, 0);

			ct.CodeType = CODETYPE_FUNCTION;
			ct.CodeParam = r;
			gAppCommand.AddCommand(&ct);
			continue;

		} else if ( *(token->token) == '#' ) {
			/* ���x���̓o�^ */
			strcpy(ct.Name, token->token + 1);
			r = RegisterLabel(ct.Name, wp);
			ct.CodeType = CODETYPE_LABEL;
			ct.CodeParam = r;
			gAppCommand.AddCommand(&ct);
			continue;
		}

		//�@�s�̒��ɁA���x��������΂����o�^���Ă����B
		for (i = 1; i < c; i++) {
			token = lexer.GetToken(i);
			if (token->token_type == TOKEN_LABEL) {
				r = RegisterLabel(token->token, 0);
				strcpy(ct.Name, token->token);
				ct.CodeType = CODETYPE_LABEL;
				ct.CodeParam = r;
				gAppCommand.AddCommand(&ct);
			}
		}

		/* ���̍s���R���p�C������ */
		r = CompileStatement(sLine, wp);
		wp = r;
	}

	//�@�Ō��END�R�}���h����������
	r = CompileStatement("end", wp);
	wp = r;

	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;

	return 0;
}


//////////////////////////////////////////////////////////////////////
//�@�R���p�C������
int CCompile::CompileStatement(char *lpstrStatement, int WritePos) {
	int i, c, r, wp;
	int FlagProc = 0;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* �������݂̃|�C���^���Z�b�g���� */
	wp = WritePos;

	/*�@�g�[�N���ɕ�������	*/
	c = lexer.LexicalAnalyze(lpstrStatement);

	/*�@���ʂɏ����̕K�v�Ȗ��߂��ǂ����𔻒f����@*/
	token = lexer.GetToken(0);
	if ( !STRCMP(token->token, "if") ) {
		return (CompileIfStatement(&lexer, wp));
	}
	if ( !STRCMP(token->token, "elseif") ) {
		return (CompileElseStatement(&lexer, wp));
	}
	if ( !STRCMP(token->token, "else") ) {
		return (CompileElseStatement(&lexer, wp));
	}
	if ( !STRCMP(token->token, "endif") ) {
		return (CompileEndifStatement(wp));
	}

	if ( !STRCMP(token->token, "for") ) {
		return (CompileForStatement(&lexer, wp));
	}
	if ( !STRCMP(token->token, "next") ) {
		return (CompileNextStatement(&lexer, wp));
	}

	if ( !STRCMP(token->token, "while") ) {
		return (CompileWhileStatement(&lexer, wp));
	}
	if ( !STRCMP(token->token, "Wend") ) {
		return (CompileWendStatement(wp));
	}

	FlagProc = 0;
	if ( !STRCMP(token->token, "result") ) {
		FlagProc = 1;
	}

	/*�@�\������͂���	*/
	syn.InitSyntax();
	r = wp;
	c = syn.SyntaxAnalyze(lexer, gAppCommand, m_lpScript, wp, WriteSingleCode,
						m_lpData, m_TotalDataSize);
	m_TotalOut += ( (wp - r) * sizeof(int));
	if (c < 0) {
		switch (-c) {
		case CLERR_UNDEC_IDENTIFIER:
			printf("Error(%d). Undeclared identifier. - %s\n", -c, lpstrStatement);
			break;
		default:
			printf("Error(%d). Undefined error. - %s\n", -c, lpstrStatement);
		}
		return 0;
	}

	/*�@�|�󂵂āA�X�N���v�g����������	*/
	token = syn.GetCommandToken();
	gAppCommand.FindCommand(token->token, &ct);

	memset(&rec, 0, sizeof(SCRIPT_RECORD));
	rec.SepCode = 1;
	rec.SepParam = 0;
	rec.CodeType = ct.CodeType;
	rec.CodeParam = ct.CodeParam;

	rec.VarPoint = 0;
	rec.VarPointDest = 0;
	r = wp;
	WriteSingleCode(m_lpScript, wp, &rec);
	m_TotalOut += ((wp - r) * sizeof(int));

	/* �|��ς݂̃p�����[�^���X�g���������� */
	for (i = 0; i < c; i++) {
		r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
		m_TotalOut += (r * sizeof(int));
	}

	//�@���ʂȏ������K�v�ȏꍇ
	if (FlagProc == 1) {
		m_ExtraProc[0].AddExitPoint(&m_lpScript[wp]);
		wp++;
	}


	// ���̏������݈ʒu��Ԃ�
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

//�@IF�R�}���h�̏������s��
int CCompile::CompileIfStatement(CLexer *lpLexer, int WritePos) {
	int i, c, r, wp;
	int ProcCount, cnt, TrueBlock;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* �������݂̃|�C���^���Z�b�g���� */
	wp = WritePos;

	/*�@�\������͂���	*/
	syn.InitSyntax();
	r = wp;

	c = lpLexer->GetTokenCount();
	for (i = 0; i < c; i++) {
		token = lpLexer->GetToken(i);
		if ( !STRCMP(token->token, "goto") ) {
			cnt = i;
			TrueBlock = 0;
			break;
		} else if ( !STRCMP(token->token, "then") ) {
			cnt = i;
			TrueBlock = 1;
			break;
		}
	}

	for (ProcCount = 0; ProcCount < 2; ProcCount++) {
		switch (ProcCount) {
		case 0:
			lpLexer->Slice(&lexer, 0, cnt-1);
			break;
		case 1:
			c = lpLexer->GetTokenCount();
			if (cnt == c - 1) {
				//�@�u���b�N�`��
				if (TrueBlock == 1) {
					//�@�������񂾃T�C�Y��Ԃ�
					m_TotalOut = (wp * sizeof(int));
					m_ScriptSize = m_TotalOut;
					return wp;
				} else {
					//�@goto�̃W�����v��w��~�X
					return 0;
				}
			}

			//�@�g�[�N����؂�o��
			lpLexer->Slice(&lexer, cnt, c-1);
			break;
		}

		c = syn.SyntaxAnalyze(lexer, gAppCommand, m_lpScript, wp, WriteSingleCode, m_lpData, m_TotalDataSize);
		m_TotalOut += ( (wp - r) * sizeof(int));

		/*�@�|�󂵂āA�X�N���v�g����������	*/
		token = syn.GetCommandToken();
		switch (ProcCount) {
		case 0:
			_ASSERT ( STRCMP(token->token, "if") == 0 );
			gAppCommand.FindCommand("if", &ct);
			break;
		case 1:
			gAppCommand.FindCommand(token->token, &ct);
			break;
		}

		memset(&rec, 0, sizeof(SCRIPT_RECORD));
		rec.SepCode = 1;
		rec.SepParam = 0;
		rec.CodeType = ct.CodeType;
		rec.CodeParam = ct.CodeParam;

		rec.VarPoint = 0;
		rec.VarPointDest = 0;
		r = wp;
		WriteSingleCode(m_lpScript, wp, &rec);
		m_TotalOut += ((wp - r) * sizeof(int));

		/* �|��ς݂̃p�����[�^���X�g���������� */
		for (i = 0; i < c; i++) {
			r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
			m_TotalOut += (r * sizeof(int));
		}

		//�@���������������Ȃ��������̃W�����v�����������
		switch (ProcCount) {
		case 0:
			m_IfElseSP++;
			m_IfElseProc[m_IfElseSP].SetFalsePoint(&m_lpScript[wp]);
			wp++;
			break;
		case 1:
			_ASSERT (m_IfElseSP >= 1);
			m_IfElseProc[m_IfElseSP].WriteFalsePoint(wp);
			m_IfElseSP--;
			break;
		}
	}

	//�@�������񂾃T�C�Y��Ԃ�
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

//�@ELSE/ELSEIF�̏������s��
int CCompile::CompileElseStatement(CLexer *lpLexer, int WritePos) {
	int i, c, r, wp;
	int ProcCount, cnt;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* �������݂̃|�C���^���Z�b�g���� */
	wp = WritePos;

	//�@�܂��A�R�}���h �uelse�v����������
	gAppCommand.FindCommand("else", &ct);
	_ASSERT(ct.CodeType == 0 && ct.CodeParam == 36);
	memset(&rec, 0, sizeof(SCRIPT_RECORD));
	rec.SepCode = 1;
	rec.SepParam = 0;
	rec.CodeType = ct.CodeType;
	rec.CodeParam = ct.CodeParam;

	rec.VarPoint = 0;
	rec.VarPointDest = 0;
	r = wp;
	WriteSingleCode(m_lpScript, wp, &rec);
	m_TotalOut += ((wp - r) * sizeof(int));

	//�@�X�L�b�v�e�[�u���̗p��
	m_IfElseProc[m_IfElseSP].AddExitPoint(&m_lpScript[wp]);
	wp++;

	// ��O��IF�ɑ΂���A�W�����v��̎w����s��
	m_IfElseProc[m_IfElseSP].WriteFalsePoint(wp);

	/* �\������͂��� */
	syn.InitSyntax();
	r = wp;

	c = lpLexer->GetTokenCount();
	if (c == 1) {
		// �P�Ƃ�ELSE
		token = lpLexer->GetToken(0);
		if ( STRCMP(token->token, "else") ) {
			// �\���G���[
			return 0;
		} else {
			// OK
			return wp;
		}
	}

	for (i = 0; i < c; i++) {
		token = lpLexer->GetToken(i);
		if ( !STRCMP(token->token, "goto") ) {
			//�@����̓G���[
			return 0;
		} else if ( !STRCMP(token->token, "then") ) {
			cnt = i;
			break;
		}
	}

	for (ProcCount = 0; ProcCount < 2; ProcCount++) {
		switch (ProcCount) {
		case 0:
			lpLexer->Slice(&lexer, 0, cnt-1);
			break;
		case 1:
			c = lpLexer->GetTokenCount();
			if (cnt == c - 1) {
				//�@�u���b�N�`��
				//�@�������񂾃T�C�Y��Ԃ�
				m_TotalOut = (wp * sizeof(int));
				m_ScriptSize = m_TotalOut;
				return wp;
			}

			//�@�g�[�N����؂�o��
			lpLexer->Slice(&lexer, cnt, c-1);
			break;
		}

		c = syn.SyntaxAnalyze(lexer, gAppCommand, m_lpScript, wp, WriteSingleCode, m_lpData, m_TotalDataSize);
		m_TotalOut += ( (wp - r) * sizeof(int));

		/*�@�|�󂵂āA�X�N���v�g����������	*/
		token = syn.GetCommandToken();
		switch (ProcCount) {
		case 0:
			_ASSERT ( STRCMP(token->token, "elseif") == 0 );
			gAppCommand.FindCommand("if", &ct);
			break;
		case 1:
			gAppCommand.FindCommand(token->token, &ct);
			break;
		}

		memset(&rec, 0, sizeof(SCRIPT_RECORD));
		rec.SepCode = 1;
		rec.SepParam = 0;
		rec.CodeType = ct.CodeType;
		rec.CodeParam = ct.CodeParam;

		rec.VarPoint = 0;
		rec.VarPointDest = 0;
		r = wp;
		WriteSingleCode(m_lpScript, wp, &rec);
		m_TotalOut += ((wp - r) * sizeof(int));

		/* �|��ς݂̃p�����[�^���X�g���������� */
		for (i = 0; i < c; i++) {
			r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
			m_TotalOut += (r * sizeof(int));
		}

		//�@���������������Ȃ��������̃W�����v�����������
		switch (ProcCount) {
		case 0:
			m_IfElseProc[m_IfElseSP].SetFalsePoint(&m_lpScript[wp]);
			wp++;
			break;
		case 1:
			_ASSERT (m_IfElseSP >= 1);
			m_IfElseProc[m_IfElseSP].WriteFalsePoint(wp);
			m_IfElseSP--;
			break;
		}
	}

	//�@�������񂾃T�C�Y��Ԃ�
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

// ENDIF�̏���
int CCompile::CompileEndifStatement(int WritePos) {

	if (m_IfElseSP < 1) {
		return 0;
	} else {
		m_IfElseProc[m_IfElseSP].WriteFalsePoint(WritePos);
		m_IfElseProc[m_IfElseSP].WriteExitPoint(WritePos);
		m_IfElseSP--;
	}
	return (WritePos);
}

// FOR�R�}���h�̏���
int CCompile::CompileForStatement(CLexer *lpLexer, int WritePos) {

	if (m_ForNextSP >= MAX_FOR_NEST - 1) {
		printf("Nest too depth");
		return 0;
	} else {
		m_IfElseProc[m_IfElseSP].WriteFalsePoint(WritePos);
		m_IfElseProc[m_IfElseSP].WriteExitPoint(WritePos);
		m_IfElseSP--;
	}
	return (WritePos);
}

// NEXT�R�}���h�̏���
int CCompile::CompileNextStatement(CLexer *lpLexer, int WritePos) {
}

// WHILE�R�}���h�̏���
int CCompile::CompileWhileStatement(CLexer *lpLexer, int WritePos) {
}

// WEND�R�}���h�̏���
int CCompile::CompileWendStatement(int WritePos) {
}
