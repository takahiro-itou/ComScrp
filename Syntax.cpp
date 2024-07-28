//////////////////////////////////////////////////////////////////////
//																	//
//		Syntax.cpp		CSyntaxクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2002/12/27 - 2002/12/28		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Syntax.h"

// コンストラクタ／デストラクタ
CSyntax::CSyntax() {
	InitSyntax();
}

CSyntax::~CSyntax() {
	m_ParameterCount = 0;
	m_lpCommand = NULL;
	m_lpLexer = NULL;
}

//　初期化
int CSyntax::InitSyntax() {
	m_ParameterCount = 0;
	return 1;
}

//　構文解析
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

	//クラスオブジェクトのポインタを格納する。さらにトークン数を取り出す
	m_lpLexer = &lexer;
	m_lpCommand = &cmd;
	count = m_lpLexer->GetTokenCount();

	/*
		フェーズ１　コマンド省略検査
		代入命令の省略を判定する
	*/

	// 第１トークンを取り出し、コマンド名かどうかを判定する
	lpToken = m_lpLexer->GetToken(0);
	r = m_lpCommand->FindCommand(lpToken->token, &CmdTable);

	if ( r == -1) {
		// 未定義の識別子
		return -(CLERR_UNDEC_IDENTIFIER);
	}

	if ( (CmdTable.CodeType & 0xfffffffc) == 0 ) { 
		/* コマンド名 */
		memcpy(&m_tokenCommand, lpToken, sizeof(LEXER_TOKEN));
		i = 1;

	} else if ( (CmdTable.CodeType & 0xffffffffc) == 4
		|| CmdTable.CodeType == CODETYPE_LOCAL_VAR
		|| CmdTable.CodeType == CODETYPE_LOCAL_PARAMETER) {
		/* 未登録または、変数名 */
		lpTokenPr = m_lpLexer->GetToken(1);
		if ( !STRCMP(lpTokenPr->token, "=") ) {
			/* 代入構文 */
			strcpy(m_tokenCommand.token, "Let");

			/* 左辺値を第１パラメータにセットする */
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
			/* 計算代入構文 */
			start = -1;
			for (i = 0; i < OPERATOR_TABLE_COUNT; i++) {
				if (OperatorTable[i].OpeName[0] == lpTokenPr->token[0]) {
					start = i;
					break;
				}
			}
			if (start >= 0) {
				strcpy(m_tokenCommand.token, OperatorTable[i].CmdName);

				/* 左辺値を第１パラメータにセットする */
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
				/* 構文エラー */
				return 0;
			}

		} else {
			/* それ以外 */
			i = 0;
		}

	} else {
		/* 構文エラー */
		i = 0;
		return 0;
	}

	/*
		フェーズ２　パラメータ解析
		パラメータの数式を逆ポーランド記法に翻訳する
	*/

	//　パラメータを解析する
	flagString = 0;
	ExpressionAnalyze.Initialize(func_write, m_lpCommand);
	param_count = 0;
	for (start = i; i < count; i++) {
		lpToken = lexer.GetToken(i);
		m_lpCommand->FindCommand(lpToken->token, &CmdTable);

		if (i == start) { 
			//　文字列操作かどうかを決定する
			if (lpToken->token_type == TOKEN_STRING || CmdTable.CodeType == CODETYPE_VAR_STRING) {
				// 文字列操作
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

	//　バッファに残っている(最後の)パラメータを処理する
	end = count - 1;
	if (start <= end) {
		RegisterParameter(&ExpressionAnalyze, lexer, Buf, wp, start, end, flagString, param_count, lpDataArea, DataSize);
		param_count++;
	}

	/* パラメータの個数を返す */
	return (m_ParameterCount);
}

//　パラメータの追加
int CSyntax::AddParameter(LP_SCRIPT_RECORD lpScrRec) {
	memcpy(&m_scrParameter[m_ParameterCount], lpScrRec, sizeof(SCRIPT_RECORD));
	m_ParameterCount++;
	return 1;
}

//　パラメータの登録処理
int CSyntax::RegisterParameter(CExpression *lpExpression, CLexer &lexer, int *Buf, int &wp,
							   int Start, int End, int FlagStr, int ParameterCount,
							   char *lpDataArea, int &DataSize) {
	int i;
	int r;
	COMMAND_TABLE Cmd;
	SCRIPT_RECORD ScrRec;
	LP_LEXER_TOKEN lpToken;

	if (FlagStr == 0) {
		// 数値パラメータ
		if (lpExpression->ToPolish(lexer, Start, End)) {
			// シングルパラメータ
			lpExpression->GetSingleParameter(&ScrRec);
			AddParameter(&ScrRec);
		} else {
			r = lpExpression->PolishToBinary(Buf, wp, 65536 * 4);
			wp = r;
			
			// インターナル変数をパラメータとする
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
		// 文字列パラメータ
		// +、&以外の演算子は全て無視する。
		for (i = Start; i <= End; i++) {
			lpToken = lexer.GetToken(i);
			if (lpToken->token_type == TOKEN_OPERATOR) {
				if ( STRCMP(lpToken->token, "+") && STRCMP(lpToken->token, "&") ) {
					continue;
				}
			} 
			if (lpToken->token_type == TOKEN_STRING) {
				// 文字列定数
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
		
//　パラメータの書き込み
int CSyntax::WriteParameter(int index, int *Buf, int &wp, FUNCPTR_WRITE_SCRIPT_RECORD func_write) {
	int r;
	r = wp;
	(*func_write)(Buf, wp, &m_scrParameter[index]);
	return (wp - r);
}

