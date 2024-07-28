//////////////////////////////////////////////////////////////////////
//																	//
//		Compile.cpp		CCompileクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/12/25		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Compile.h"
#include <stdio.h>

//　コンストラクタ／デストラクタ
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
//　コンパイルのメイン処理
int CCompile::CompileText(char *sExitStatement, int WritePos) {
	int i, c, r, wp;
	int nLocalVarSize;
	int nLocalParamSize;
	char sLine[1024];
	CLexer lexer;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;

	if (sExitStatement == NULL || !STRCMP(sExitStatement, "init")) {
		/* 前の処理のクリア */
		MEMORY_FREE(m_lpScript);
		MEMORY_FREE(m_lpData);
		MEMORY_FREE(m_lpLabel);
		MEMORY_FREE(m_lpFuncAddr);
		MEMORY_FREE(m_lpFuncCall);
		m_ScriptSize = 0;
		m_TotalDataSize = 0;
		m_LabelCount = 0;
		m_FuncCount = 0;

		/*　スタンバイ		デバッグ用出力が設定されていれば、メモリを確保する */
		if (m_FlagDebugOption & DEBUGOPT_LEXER) {
			m_lpDebugScript = (char *)MEMORY_ALLOC(m_SourceSize);
			memset(m_lpDebugScript, 0, m_SourceSize);
		}

		/* ポインタのセット　*/
		m_lpNextIn = m_lpSource;

		/* メモリの確保 */
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

		//　システムラベル mainを登録し、そこへジャンプする
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
		/* ローカル関数の場合の処理 */
		nLocalVarSize = 0;
		wp = WritePos;
		m_FlagNowFunction = 1;

		// 登録処理
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

		/* プロローグコード */
		r = CompileStatement("push BP", wp); wp = r;
		r = CompileStatement("BP = SP", wp); wp = r;
		WritePos = wp;
		r = CompileStatement("SP -= 0", wp); wp = r;
	}
	nLocalVarSize = 0;

	/*　リード			ソーススクリプトから１行を読み込む */
	for (;;) {
		memset(sLine, 0, sizeof(sLine));
		if (ReadLine(sLine) <= 0) { break; }
		
		/*　トークンに分解する	*/
		c = lexer.LexicalAnalyze(sLine);

		/*　デバッグ用出力	*/
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

		/*　宣言かどうかを判断する */
		if (c == 0) { break; }

		/* 終了ステートメントかどうかを調べる */
		token = lexer.GetToken(0);
		if (sExitStatement != NULL && STRCMP(sExitStatement, "init") ) {
			if ( !STRCMP(token->token, sExitStatement) ) {
				/* プロローグコードを完了する */
				c = wp;
				wp = WritePos;
				if (nLocalVarSize >= 65535) {
					return 0;
				}
				sprintf(sLine, "SP -= %d", nLocalVarSize);
				r = CompileStatement(sLine, wp);
				wp = c;

				/* 終了ポイントを書き込む */
				RegisterFunction(m_FunctionName, 0, 0, wp);

				// resultコマンドを解決する
				m_ExtraProc[0].WriteExitPoint(wp);

				/* エピローグコードを書き込む */
				r = CompileStatement("SP = BP", wp); wp = r;
				r = CompileStatement("pop BP", wp);  wp = r;
				r = CompileStatement("return", wp);  wp = r;

				// ローカル変数の定義を削除する
				gAppCommand.DeleteCommandType(CODETYPE_LOCAL_VAR);
				gAppCommand.DeleteCommandType(CODETYPE_LOCAL_PARAMETER);
				m_FlagNowFunction = 0;

				return (r);
			}
		}

		if ( !STRCMP(token->token, "int")) {
			/* 数値変数の宣言 */
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
					/* 構文エラー */
					return 0;
				}
			}
			continue;

		} else if ( !STRCMP(token->token, "function")) {
			if (m_FlagNowFunction) {
				/* エラー */
				return 0;
			}

			/* 関数名を取得する */
			token = lexer.GetToken(1);
			strcpy(m_FunctionName, token->token);
			strcpy(ct.Name, m_FunctionName);
			strcpy(m_FunctionText, sLine);

			// 引数のリストを作成する
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
						return 0;				/* 構文エラー */
					}
				}
			}

			// この関数を登録する
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
			/* 関数のプロトタイプ宣言 */
			token = lexer.GetToken(1);
			if ( STRCMP(token->token, "function") ) {
				return 0;
			}

			token = lexer.GetToken(2);
			strcpy(m_FunctionName, token->token);
			strcpy(ct.Name, m_FunctionName);

			// 引数のリストを作成する
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
						return 0;				/* 構文エラー */
					}
				}
			}

			// この関数を登録する
			r = RegisterFunction(m_FunctionName, 0, wp, 0);

			ct.CodeType = CODETYPE_FUNCTION;
			ct.CodeParam = r;
			gAppCommand.AddCommand(&ct);
			continue;

		} else if ( *(token->token) == '#' ) {
			/* ラベルの登録 */
			strcpy(ct.Name, token->token + 1);
			r = RegisterLabel(ct.Name, wp);
			ct.CodeType = CODETYPE_LABEL;
			ct.CodeParam = r;
			gAppCommand.AddCommand(&ct);
			continue;
		}

		//　行の中に、ラベルがあればそれを登録しておく。
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

		/* この行をコンパイルする */
		r = CompileStatement(sLine, wp);
		wp = r;
	}

	//　最後にENDコマンドを書き込む
	r = CompileStatement("end", wp);
	wp = r;

	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;

	return 0;
}


//////////////////////////////////////////////////////////////////////
//　コンパイル処理
int CCompile::CompileStatement(char *lpstrStatement, int WritePos) {
	int i, c, r, wp;
	int FlagProc = 0;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* 書き込みのポインタをセットする */
	wp = WritePos;

	/*　トークンに分解する	*/
	c = lexer.LexicalAnalyze(lpstrStatement);

	/*　特別に処理の必要な命令かどうかを判断する　*/
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

	/*　構文を解析する	*/
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

	/*　翻訳して、スクリプトを書き込む	*/
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

	/* 翻訳済みのパラメータリストを書き込む */
	for (i = 0; i < c; i++) {
		r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
		m_TotalOut += (r * sizeof(int));
	}

	//　特別な処理が必要な場合
	if (FlagProc == 1) {
		m_ExtraProc[0].AddExitPoint(&m_lpScript[wp]);
		wp++;
	}


	// 次の書き込み位置を返す
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

//　IFコマンドの処理を行う
int CCompile::CompileIfStatement(CLexer *lpLexer, int WritePos) {
	int i, c, r, wp;
	int ProcCount, cnt, TrueBlock;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* 書き込みのポインタをセットする */
	wp = WritePos;

	/*　構文を解析する	*/
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
				//　ブロック形式
				if (TrueBlock == 1) {
					//　書き込んだサイズを返す
					m_TotalOut = (wp * sizeof(int));
					m_ScriptSize = m_TotalOut;
					return wp;
				} else {
					//　gotoのジャンプ先指定ミス
					return 0;
				}
			}

			//　トークンを切り出す
			lpLexer->Slice(&lexer, cnt, c-1);
			break;
		}

		c = syn.SyntaxAnalyze(lexer, gAppCommand, m_lpScript, wp, WriteSingleCode, m_lpData, m_TotalDataSize);
		m_TotalOut += ( (wp - r) * sizeof(int));

		/*　翻訳して、スクリプトを書き込む	*/
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

		/* 翻訳済みのパラメータリストを書き込む */
		for (i = 0; i < c; i++) {
			r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
			m_TotalOut += (r * sizeof(int));
		}

		//　条件式が成立しなかった時のジャンプ先を書き込む
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

	//　書き込んだサイズを返す
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

//　ELSE/ELSEIFの処理を行う
int CCompile::CompileElseStatement(CLexer *lpLexer, int WritePos) {
	int i, c, r, wp;
	int ProcCount, cnt;
	CLexer lexer;
	CSyntax syn;
	LP_LEXER_TOKEN token;
	COMMAND_TABLE ct;
	SCRIPT_RECORD rec;

	/* 書き込みのポインタをセットする */
	wp = WritePos;

	//　まず、コマンド 「else」を書き込む
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

	//　スキップテーブルの用意
	m_IfElseProc[m_IfElseSP].AddExitPoint(&m_lpScript[wp]);
	wp++;

	// 一つ前のIFに対する、ジャンプ先の指定を行う
	m_IfElseProc[m_IfElseSP].WriteFalsePoint(wp);

	/* 構文を解析する */
	syn.InitSyntax();
	r = wp;

	c = lpLexer->GetTokenCount();
	if (c == 1) {
		// 単独のELSE
		token = lpLexer->GetToken(0);
		if ( STRCMP(token->token, "else") ) {
			// 構文エラー
			return 0;
		} else {
			// OK
			return wp;
		}
	}

	for (i = 0; i < c; i++) {
		token = lpLexer->GetToken(i);
		if ( !STRCMP(token->token, "goto") ) {
			//　これはエラー
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
				//　ブロック形式
				//　書き込んだサイズを返す
				m_TotalOut = (wp * sizeof(int));
				m_ScriptSize = m_TotalOut;
				return wp;
			}

			//　トークンを切り出す
			lpLexer->Slice(&lexer, cnt, c-1);
			break;
		}

		c = syn.SyntaxAnalyze(lexer, gAppCommand, m_lpScript, wp, WriteSingleCode, m_lpData, m_TotalDataSize);
		m_TotalOut += ( (wp - r) * sizeof(int));

		/*　翻訳して、スクリプトを書き込む	*/
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

		/* 翻訳済みのパラメータリストを書き込む */
		for (i = 0; i < c; i++) {
			r = syn.WriteParameter(i, m_lpScript, wp, WriteSingleCode);
			m_TotalOut += (r * sizeof(int));
		}

		//　条件式が成立しなかった時のジャンプ先を書き込む
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

	//　書き込んだサイズを返す
	m_TotalOut = (wp * sizeof(int));
	m_ScriptSize = m_TotalOut;
	return wp;
}

// ENDIFの処理
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

// FORコマンドの処理
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

// NEXTコマンドの処理
int CCompile::CompileNextStatement(CLexer *lpLexer, int WritePos) {
}

// WHILEコマンドの処理
int CCompile::CompileWhileStatement(CLexer *lpLexer, int WritePos) {
}

// WENDコマンドの処理
int CCompile::CompileWendStatement(int WritePos) {
}
