//////////////////////////////////////////////////////////////////////
//																	//
//		Expression.cpp	CExpressionクラスのインプリメンテーション	//
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

	/* メモリを初期化する */
	memset(v, 0, sizeof(v));
	memset(ope, 0, sizeof(ope));
	memset(&ct, 0, sizeof(COMMAND_TABLE));
	n = m_Count;

	/* テンポラリの内部変数の開始インデックス */
	sp_v = m_BaseInternalVarIndex++;
	sp_ope = 0;

	/* 番兵を立てる */
	sprintf(ope[sp_ope].Name, "");
	ope[sp_ope].CodeType = 0;
	ope[sp_ope].CodeParam = 0;
	sp_ope++;

	for (i = Start; i <= End; i++) {
		/* トークンを取り出す */
		lpToken = lexer.GetToken(i);

		/* 取り出したトークンが数値か変数なら計算用スタックに積む */
		if (lpToken->token_type == TOKEN_NUMERIC) {			/* 数値 */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_CONST_NUMBER;
			v[sp_v].CodeParam = atoi(lpToken->token);
			sp_v++;
		} else if (lpToken->token_type == TOKEN_OTHERS) {	/* 変数 */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_VAR_EXTERNAL;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_STRING) {	/* 文字列 */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_CONST_STRING;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_LABEL) {	/* ラベル */
			sprintf(v[sp_v].Name, "%s", lpToken->token);
			v[sp_v].CodeType = CODETYPE_LABEL;

			r = m_lpCommand->FindCommand(lpToken->token, &ct);
			_ASSERT (r >= 0);
			v[sp_v].CodeType = ct.CodeType;
			_ASSERT (ct.CodeType == CODETYPE_LABEL);
			v[sp_v].CodeParam = ct.CodeParam;
			sp_v++;

		} else if (lpToken->token_type == TOKEN_OPERATOR) {	/* 演算子 */

			/* 演算子用スタックのトップと優先順位を比較する */
			while ( GetPri(lpToken->token) <= GetPri(ope[sp_ope - 1].Name)
				&& STRCMP(ope[sp_ope-1].Name , "(") ) {
				/* スタックトップの演算を実行する */
				Calclate(v, sp_v, ope[sp_ope - 1].Name);
				sp_ope--;
			}

			/* 取り出した演算子が、")"でなければ、その演算子をスタックに積む */
			if ( STRCMP(lpToken->token, ")") ) {
				sprintf(ope[sp_ope].Name, lpToken->token);
				sp_ope++;
			} else {
				sp_ope--;
			}
		}
	}

	/* スタックの残りを処理する */
	while (sp_ope > 1) {
		Calclate(v, sp_v, ope[sp_ope - 1].Name);
		sp_ope--;
	}

	if ( (m_Count - n) <= 0) {
		/* 数値定数、または変数の単独式 */
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

	/* 書き込むデータが1個しかない場合は、最適化用のデータを準備する */
	if ( (m_Count - n) == 1) {
		/* シングルパラメータ */
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

/* 計算式をスクリプトに変換する */
int CExpression::PolishToBinary(int *Buf, int Start, int MaxSize) {
	int i, p;
	COMMAND_TABLE ct;
	SCRIPT_RECORD Cmd;

	if (m_lpFuncPtrWriteScript == NULL) {
		/* 書き込み用の関数が用意されていない */
		return 0;
	}

	p = Start;
	for (i = 0; i < m_Count; i++) {
		if ((p - Start) >= MaxSize) {
			/* バッファが足りない */
			return 0;
		}

		/* 実行するコマンドを、テーブルから探す */
		memset(&ct, 0, sizeof(COMMAND_TABLE));
		m_lpCommand->FindCommand(m_SyntaxResult[i].let_command, &ct);
		/* ファーストバイト、コマンド */
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 1;
		Cmd.SepParam = 0;
		_ASSERT(ct.CodeType == CODETYPE_COMMAND_G);
		Cmd.CodeType = ct.CodeType;		/* 実行するコマンドの種類 */
		Cmd.CodeParam = ct.CodeParam;	/* 実行するコマンドの番号 */
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);

		/* セカンドバイト：ベース変数 */
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 0;
		Cmd.SepParam = 0;
		Cmd.CodeType = CODETYPE_VAR_INTERNAL;
		Cmd.CodeParam = m_SyntaxResult[i].indll_var_id;
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);

		/* サードバイト：ディスティネーション */
		memset(&ct, 0, sizeof(COMMAND_TABLE));
		m_lpCommand->FindCommand(m_SyntaxResult[i].parameter, &ct);
		memset(&Cmd, 0, sizeof(Cmd));
		Cmd.SepCode = 0;
		Cmd.SepParam = 0;
		Cmd.CodeType = ct.CodeType;		/* 変数 */
		Cmd.CodeParam = ct.CodeParam;
		Cmd.VarPoint = 0;
		Cmd.VarPointDest = 0;
		(*m_lpFuncPtrWriteScript)(Buf, p, &Cmd);
	}

	/* 正常終了 */
	return (p);
}

/* 演算を実行する */
void CExpression::Calclate(COMMAND_TABLE v_stack[], int &val_sp, char *sOpe) {
	int i, cmdID = 0;
	int v1_numeric, v2_numeric, vr;
	COMMAND_TABLE ct;

	/* 演算子に対応するコマンドを探す */
	for (i = 0; i < OPERATOR_TABLE_COUNT; i++) {
		if ( !STRCMP(OperatorTable[i].OpeName, sOpe) ) {
			cmdID = i;
			break;
		}
	}

	/* 演算の対象となる内容について、ともに数値なら、最適化を行う */
	if ( v_stack[val_sp - 2].CodeType == CODETYPE_CONST_NUMBER
		&& v_stack[val_sp - 1].CodeType == CODETYPE_CONST_NUMBER) {
		v1_numeric = v_stack[val_sp - 2].CodeParam;
		v2_numeric = v_stack[val_sp - 1].CodeParam;

		if ( !STRCMP(sOpe, "+") ) {				/* 加算 */
			vr = v1_numeric + v2_numeric;
		} else if ( !STRCMP(sOpe, "-") ) {		/* 減算 */
			vr = v1_numeric - v2_numeric;
		} else if ( !STRCMP(sOpe, "*") ) {		/* 乗算 */
			vr = v1_numeric * v2_numeric;
		} else if ( !STRCMP(sOpe, "/") ) {		/* 除算 */
			vr = v1_numeric / v2_numeric;
		} else if ( !STRCMP(sOpe, "%") ) {		/* 剰余算 */
			vr = v1_numeric % v2_numeric;
		} else if ( !STRCMP(sOpe, "&") ) {		/* 論理積 */
			vr = v1_numeric & v2_numeric;
		} else if ( !STRCMP(sOpe, "|") ) {		/* 論理和 */
			vr = v1_numeric | v2_numeric;
		} else if ( !STRCMP(sOpe, "^") ) {		/* 排他的論理和 */
			vr = v1_numeric ^ v2_numeric;
		}

		sprintf(v_stack[val_sp - 2].Name, "%d", vr);
		v_stack[val_sp - 2].CodeType = CODETYPE_CONST_NUMBER;
		v_stack[val_sp - 2].CodeParam = vr;
		val_sp--;
		return;

	}

	/* スタックの[vap_sp - 2]の内容を確認して、*でないなら、Letコマンドを実行する */
	if ( STRCMP(v_stack[val_sp - 2].Name, "*") ) {
		m_SyntaxResult[m_Count].flag_parameter = 0;

		strcpy(m_SyntaxResult[m_Count].let_command, "let");
		strcpy(m_SyntaxResult[m_Count].parameter, v_stack[val_sp - 2].Name);
		
		/* パラメータが定数かどうかを調べておく */
		m_lpCommand->FindCommand(m_SyntaxResult[m_Count].parameter, &ct);
		if (ct.CodeType == CODETYPE_CONST_NUMBER) {
			m_SyntaxResult[m_Count].flag_parameter = CODETYPE_CONST_NUMBER;
			m_SyntaxResult[m_Count].numeric_parameter = ct.CodeParam;
		}

		m_SyntaxResult[m_Count].indll_var_id = (val_sp - 2);
		m_Count++;
	}

	/* indll変数をパラメータにして、コマンドを実行する */
	m_SyntaxResult[m_Count].flag_parameter = 0;
	strcpy(m_SyntaxResult[m_Count].let_command, OperatorTable[cmdID].CmdName);
	m_SyntaxResult[m_Count].indll_var_id = (val_sp - 2);

	/* スタックの[var_sp - 1]の内容を確認して、今のコマンドのパラメータを決定する */
	if ( STRCMP(v_stack[val_sp - 1].Name, "*") ) {
		/* 右辺値をパラメータにセットする */
		strcpy(m_SyntaxResult[m_Count].parameter, v_stack[val_sp - 1].Name);

		/* パラメータが定数かどうかを調べておく */
		m_lpCommand->FindCommand(m_SyntaxResult[m_Count].parameter, &ct);
		if (ct.CodeType == CODETYPE_CONST_NUMBER) {
			m_SyntaxResult[m_Count].flag_parameter = CODETYPE_CONST_NUMBER;
			m_SyntaxResult[m_Count].numeric_parameter = ct.CodeParam;
		}
	} else {
		/* indll変数をパラメータにする */
		sprintf(m_SyntaxResult[m_Count].parameter, "iv%04d", (val_sp - 1));

		sprintf(ct.Name, m_SyntaxResult[m_Count].parameter);
		ct.CodeType = CODETYPE_VAR_INTERNAL;
		ct.CodeParam = val_sp - 1;

		m_lpCommand->AddCommand(&ct);
		m_SyntaxResult[m_Count].flag_parameter = CODETYPE_VAR_INTERNAL;
		m_SyntaxResult[m_Count].numeric_parameter = (val_sp - 1);
	}
	m_Count++;

	/* スタックの[var_sp - 2]の内容を '*'にして、この値が変数を含むことを知らせる */
	strcpy(v_stack[val_sp - 2].Name, "*");
	v_stack[val_sp - 2].CodeType = CODETYPE_VAR_INTERNAL;
	v_stack[val_sp - 2].CodeParam = val_sp - 2;

	/* スタックポインタを調整する */
	val_sp--;
}

int CExpression::GetSingleParameter(LP_SCRIPT_RECORD lpResult) {
	if (m_FlagSingleParam == 0) { return 0; }
	memcpy(lpResult, &m_SingleParameter, sizeof(SCRIPT_RECORD));
	return 1;
}
