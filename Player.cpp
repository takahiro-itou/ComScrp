//////////////////////////////////////////////////////////////////////
//																	//
//		Player.cpp		CScriptPlayerクラスのインプリメンテーション	//
//																	//
//				Copyright Takahiro Ito, 2003/01/14 - 2003/02/23		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Player.h"

//　コンストラクタ・デストラクタ
CScriptPlayer::CScriptPlayer() {
	m_ScriptSize = 0;
	m_lpScriptBase = NULL;
	m_ScriptPos = NULL;

	m_lpVarWork = NULL;
	m_lpVarSystem = NULL;
	m_lpVarPrivate = NULL;
	m_lpVarStack = NULL;
	m_lpStrWork = NULL;

	m_DataSize = 0;
	m_lpDataBase = NULL;

	m_CallStackPointer = 0;
	memset(m_CallStack, 0, sizeof(m_CallStack));

	m_FlagDebugInfo = 0;
}

CScriptPlayer::~CScriptPlayer() {
	MEMORY_FREE(m_lpScriptBase);
	MEMORY_FREE(m_lpDataBase);
	MEMORY_FREE(m_lpLabel);
	MEMORY_FREE(m_lpFuncAddr);

	MEMORY_FREE(m_lpLabelInfo);
	MEMORY_FREE(m_lpFuncCall);

	MEMORY_FREE(m_lpVarWork);
	MEMORY_FREE(m_lpVarSystem);
	MEMORY_FREE(m_lpVarPrivate);
	MEMORY_FREE(m_lpVarStack);
	MEMORY_FREE(m_lpStrWork);

	m_FlagDebugInfo = 0;
}

//　変数領域のメモリを確保して、0クリアする
int CScriptPlayer::InitializeVariable() {
	MEMORY_FREE(m_lpVarWork);
	MEMORY_FREE(m_lpVarSystem);
	MEMORY_FREE(m_lpVarPrivate);
	MEMORY_FREE(m_lpVarStack);
	MEMORY_FREE(m_lpStrWork);

	m_lpVarWork = (int *)(MEMORY_ALLOC(65536));
	m_lpVarSystem = (int *)(MEMORY_ALLOC(65536));
	m_lpVarPrivate = (int *)(MEMORY_ALLOC(131072));
	m_lpVarStack = (int *)(MEMORY_ALLOC(262144));
	m_lpStrWork = (char *)(MEMORY_ALLOC(524288));

	if ((m_lpVarWork == NULL) || (m_lpVarSystem == NULL) || (m_lpVarStack == NULL)) {
		return -1;
	}
	if (m_lpVarPrivate == NULL) { return -2; }
	if (m_lpStrWork == NULL) { return -4; }

	memset(m_lpVarWork,    0,  65536);
	memset(m_lpVarSystem,  0,  65536);
	memset(m_lpVarPrivate, 0, 131072);
	memset(m_lpVarStack,   0, 242144);
	memset(m_lpStrWork,    0, 524288);

	// SP = 131072
	m_lpVarSystem[0] = 131072;
	m_lpVarSystem[1] = 131072;

	return 0;
}

int CScriptPlayer::ReadBinary(const char *FileName) {
	FILE *fp;
	int Header[64];

	/* ファイルを開く */
	fp = fopen(FileName, "rb");
	if (fp == NULL) { return 0; }

	/* ヘッダーを検査する */
	fread(Header, 1, sizeof(Header), fp);
	if ((Header[0] != SCRIPT_IDENTIFY_CODE) || (Header[2] != SCRIPT_INTERNAL_VERSION)) {
		fclose(fp);
		return 0;
	}

	//　すでに読み込まれていたスクリプトがある場合は、
	//　そのメモリを解放する。
	MEMORY_FREE(m_lpScriptBase);
	MEMORY_FREE(m_lpDataBase);
	MEMORY_FREE(m_lpLabel);
	MEMORY_FREE(m_lpFuncAddr);
	MEMORY_FREE(m_lpLabelInfo);
	MEMORY_FREE(m_lpFuncCall);

	/* スクリプトデータを読み出す */
	m_ScriptSize = Header[5];			/* サイズ */
	m_lpScriptBase = (int *)MEMORY_ALLOC(m_ScriptSize);
	fseek(fp, Header[4], SEEK_SET);		/* 位置 */
	fread(m_lpScriptBase, 1, m_ScriptSize, fp);

	/* データを読み出す */
	m_DataSize = Header[7];				/* サイズ */
	m_lpDataBase = (char *)MEMORY_ALLOC(m_DataSize);
	fseek(fp, Header[6], SEEK_SET);		/* 位置 */
	fread(m_lpDataBase, 1, m_DataSize, fp);

	/* ラベルデータを読み出す */
	m_LabelCount = Header[9] / 4;		/* 数 */
	m_lpLabel = (int *)MEMORY_ALLOC(m_LabelCount * 4);
	fseek(fp, Header[8], SEEK_SET);		/* 位置 */
	fread(m_lpLabel, 4, m_LabelCount, fp);

	/* 関数リストを読み出す */
	m_FuncCount = Header[11] / sizeof(FUNC_TABLE);		/* 数 */
	m_lpFuncAddr = (LP_FUNC_TABLE)MEMORY_ALLOC(m_FuncCount * sizeof(FUNC_TABLE));
	fseek(fp, Header[10], SEEK_SET);
	fread(m_lpFuncAddr, sizeof(FUNC_TABLE), m_FuncCount, fp);

	/* デバッグ情報があればそれを読み出す */
	if (Header[12] != NULL) {
		m_FlagDebugInfo = 1;
		fseek(fp, Header[12], SEEK_SET);

		m_lpLabelInfo = (LP_LABEL_INFO)MEMORY_ALLOC( sizeof(LABEL_INFO) * m_LabelCount);
		m_lpFuncCall = (LP_FUNC_INFO)MEMORY_ALLOC( sizeof(FUNC_INFO) * m_FuncCount);

		fread(m_lpLabelInfo, 1, sizeof(LABEL_INFO) * m_LabelCount, fp);
		fread(m_lpFuncCall, 1, sizeof(FUNC_INFO) * m_FuncCount, fp);
	} else {
		m_FlagDebugInfo = 0;
	}

	// 変数を初期化する
	InitializeVariable();

	/* 完了 */
	fclose(fp);

	return 1;
}

// 1コマンドを実行する
int CScriptPlayer::StepCommand(char *lpstrResult) {
	int i, c, ca, wp;
	int nPreserve, nCurPos;
	int nCommand;
	int nParam0, nRight;
	int AbsAddr[32];
	int WritePos;
	SCRIPT_RECORD Rec;
	COMMAND_TABLE nCmd;
	SCRIPT_ACCESS Acc1, Acc2;
	char sParam[256];

ProcessExecuteStep:

	//　レコード読み出し前に変数を初期化する
	nParam0 = 0;
	nCurPos = m_ScriptPos * 4;
	memcpy(&m_CmdParam, &Rec, sizeof(m_CmdParam));
	memset(&Acc1, 0, sizeof(SCRIPT_ACCESS));
	memset(&Acc2, 0, sizeof(SCRIPT_ACCESS));

	/* １レコードを読み出す */
	ReadSingleCode(m_lpScriptBase, m_ScriptPos, &Rec);

	// レコードの種類を確認する
	nCmd.CodeType = Rec.CodeType;
	nCmd.CodeParam = Rec.CodeParam;
	nCommand = nCmd.CodeParam;

	memcpy(&m_LastCommand, &Rec, sizeof(SCRIPT_RECORD));

	// パラメータを読み出す
	c = 0;
	ca = 0;
	for (; m_ScriptPos * 4 < m_ScriptSize;) {
		nPreserve = m_ScriptPos;

		if (m_lpScriptBase[m_ScriptPos] & 0x80000000) {
			// IF,ELSEなどの絶対番地指定
			AbsAddr[ca] = (m_lpScriptBase[m_ScriptPos] & 0x7fffffff);
			ca++;
			m_ScriptPos++;
		} else {
			// 通常のレコード
			ReadSingleCode(m_lpScriptBase, m_ScriptPos, &Rec);
			if (Rec.SepCode) {
				// 位置を復元して、終了
				m_ScriptPos = nPreserve;
				break;
			}
			memcpy(&m_CmdParam[c], &Rec, sizeof(SCRIPT_RECORD));
			c++;
		}
	}

	// コマンドのパラメータ数を格納する
	m_CmdParamCount = c;

	if (nCmd.CodeType == CODETYPE_COMMAND_G) {
		// 処理してしまう。

		if (nCommand <= 31) {
			// 変数操作系
			GetAccessVar(&m_CmdParam[0], &Acc1);
			GetAccessVar(&m_CmdParam[1], &Acc2);
			if (Acc1.AccessType == 0) {
				// 定数に値は代入できないのでエラー
				sprintf(lpstrResult, "Illegal Operation in %08x", nCurPos);
				return ERROR_ILLEGAL_OPERATION;
			}
			switch (Acc2.AccessType) {
			case ACCESS_INT_CONST:
				nRight = Acc2.Data.IntConst;
				break;
			case ACCESS_INT_VAR:
				nRight = *(Acc2.Data.lpVarPointer);
				break;
			}

			switch (nCommand) {
			case 0:		/* let */
				*(Acc1.Data.lpVarPointer) = nRight; break;
			case 1:
				*(Acc1.Data.lpVarPointer) += nRight; break;
			case 2:
				*(Acc1.Data.lpVarPointer) -= nRight; break;
			case 3:
				*(Acc1.Data.lpVarPointer) *= nRight; break;
			case 4:
				if (nRight == 0) {
					sprintf(lpstrResult, "Division by Zero in %08x", nCurPos);
					return ERROR_DIVISION_BY_ZERO;
				}
				*(Acc1.Data.lpVarPointer) /= nRight; break;
			case 5:
				if (nRight == 0) {
					sprintf(lpstrResult, "Division by Zero in %08x", nCurPos);
					return ERROR_DIVISION_BY_ZERO;
				}
				*(Acc1.Data.lpVarPointer) %= nRight; break;

			case 6:
				*(Acc1.Data.lpVarPointer) &= nRight; break;
			case 7:
				*(Acc1.Data.lpVarPointer) |= nRight; break;
			case 8:
				*(Acc1.Data.lpVarPointer) ^= nRight; break;
			case 9:		/* neg */
				*(Acc1.Data.lpVarPointer) *= -1; break;

			case 10:	/* logical and */
				if ( *(Acc1.Data.lpVarPointer) && nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 11:	/* logical or */
				if ( *(Acc1.Data.lpVarPointer) || nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;

			case 12:	/* IsEqu */
				if ( *(Acc1.Data.lpVarPointer) == nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 13:	/* IsNeq */
				if ( *(Acc1.Data.lpVarPointer) != nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 14:	/* IsSml */
				if ( *(Acc1.Data.lpVarPointer) < nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 15:	/* IsGra */
				if ( *(Acc1.Data.lpVarPointer) > nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 16:	/* IsESm */
				if ( *(Acc1.Data.lpVarPointer) <= nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;
			case 17:	/* IsEGr */
				if ( *(Acc1.Data.lpVarPointer) >= nRight ) {
					*(Acc1.Data.lpVarPointer) = 1;
				} else {
					*(Acc1.Data.lpVarPointer) = 0;
				}
				break;

			case 18:	/* shl */
				*(Acc1.Data.lpVarPointer) <<= nRight;
				break;
			case 19:	/* shr */
				*(Acc1.Data.lpVarPointer) >>= nRight;
				break;

			default:
				sprintf(lpstrResult, "Illegal Instruction in %08x", nCurPos);
				return ERROR_ILLEGAL_INSTRUCTION;
			}

		} else if (nCommand <= 63) {
			// フロー制御
			GetAccessVar(&m_CmdParam[0], &Acc1);
			switch (Acc1.AccessType) {
			case ACCESS_NO_ENTRY:
				nParam0 = 0;
				break;
			case ACCESS_INT_CONST:
				nParam0 = Acc1.Data.IntConst;
				break;
			case ACCESS_INT_VAR:
				nParam0 = *(Acc1.Data.lpVarPointer);
				break;
			case ACCESS_FUNCTION:
				nParam0 = Acc1.Data.EntryIndex;
				break;
			case ACCESS_LABEL:
				nParam0 = Acc1.Data.JumpAddress;
				break;
			}

			switch (nCommand-32) {
			case 0:		/* goto */
				m_ScriptPos = nParam0;
				break;
			case 1:		/* gosub */
				m_CallStack[m_CallStackPointer++] = m_ScriptPos;
				m_ScriptPos = nParam0;
				break;

			case 2:		/* return */
				if (!m_CallStackPointer) {
					// コールスタックがない。
					sprintf(lpstrResult, "Return without gosub in %08x", nCurPos);
					return ERROR_RETURN_WITHOUT_GOSUB;
				}
				m_ScriptPos = m_CallStack[--m_CallStackPointer];
				if (nParam0) { m_ScriptPos = nParam0; }
				break;

			case 3:		/* if */
				if (!nParam0) {
					// ジャンプする
					m_ScriptPos = AbsAddr[0];
				}
				break;
			case 4:		/* else */
				// 終了ポイントへジャンプする。
				m_ScriptPos = AbsAddr[0];
				break;

			case 5:		/* call */
				// 関数の引数をスタックへ積む
				for (i = c; i >= 1; i--) {
					// 実引数を取り出す
					GetAccessVar(&m_CmdParam[i], &Acc1);
					switch (Acc1.AccessType) {
					case ACCESS_NO_ENTRY:
						nRight = 0; break;
					case ACCESS_INT_CONST:
						nRight = Acc1.Data.IntConst; break;
					case ACCESS_INT_VAR:
						nRight = *(Acc1.Data.lpVarPointer); break;
					case ACCESS_LABEL:
						nRight = Acc1.Data.JumpAddress;
					}
					// スタックへ積む
					wp = m_lpVarSystem[0] - 1;
					m_lpVarStack[wp] = nRight;
					m_lpVarSystem[0] = wp;
				}

				// 関数を呼び出す
				m_CallStack[m_CallStackPointer++] = m_ScriptPos;
				// プロローグコードの実行
				m_ScriptPos = m_lpFuncAddr[nParam0].EntryPoint;
				break;
			case 6:		/* result */
				// 関数の値を返して終了する。
				m_lpVarSystem[3] = nParam0;
				// エピローグコードを実行する
				m_ScriptPos = AbsAddr[0];
				break;

			case 7:		/* push */
				wp = m_lpVarSystem[0] - 1;
				m_lpVarStack[wp] = nParam0;
				m_lpVarSystem[0] = wp;
				break;
			case 8:		/* pop */
				wp = m_lpVarSystem[0];
				*(Acc1.Data.lpVarPointer) = m_lpVarStack[wp];
				m_lpVarSystem[0] = wp + 1;
				break;

			case 9:		/* stop */
				return SCRIPT_STOP;
			case 10:	/* end */
				return SCRIPT_END;

			default:
				sprintf(lpstrResult, "Illegal Instruction in %08x", nCurPos);
				return ERROR_ILLEGAL_INSTRUCTION;
			}

		} else if (nCommand <= 95) {
			// メモリ操作
			switch (nCommand - 64) {
			case 0:			/* memcpy */
			case 1:			/* memset */
			default:
				sprintf(lpstrResult, "Illegal Instruction in %08x", nCurPos);
				return ERROR_ILLEGAL_INSTRUCTION;
			}

		} else if (nCommand <= 128) {
			//　ファイル入出力
			switch (nCommand  - 96) {
			case 0:			/* fopen */
			case 1:			/* fread */
			case 2:			/* fwrite */
			case 3:			/* fprint */
			case 4:			/* finput */
			case 5:			/* fclose */
			default:
				sprintf(lpstrResult, "Illegal Instruction in %08x", nCurPos);
				return ERROR_ILLEGAL_INSTRUCTION;
			}
		}
		goto ProcessExecuteStep;

	} else if (nCmd.CodeType == CODETYPE_COMMAND_E) {
		// パラメータとともに、コマンド文字列をアプリケーションに返す
		WritePos = 0;
		gAppCommand.DisFindCommand(&nCmd, lpstrResult);
		WritePos = strlen(lpstrResult) + 1;

#if defined PLAY_TEST
		if ( nCmd.CodeParam == 0 ) {
			//　コンソールに表示する
			for (i = 0; i < c; i++) {
				GetAccessVar(&m_CmdParam[i], &Acc1);
				switch (Acc1.AccessType) {
				case ACCESS_INT_CONST:
					printf("%d", Acc1.Data.IntConst);
					break;
				case ACCESS_INT_VAR:
					printf("%d", *(Acc1.Data.lpVarPointer));
					break;
				case ACCESS_STR_CONST:
				case ACCESS_STR_VAR:
					printf("%s", Acc1.Data.lpStrPointer);
					break;
				}
			}
			printf("\n");
			goto ProcessExecuteStep;
		}
#endif

		//　パラメータの個数
		sprintf(&lpstrResult[WritePos], "%d", c);
		WritePos += strlen(&lpstrResult[WritePos]);
		WritePos++;
		for (i = 0; i < c; i++) {
			GetAccessVar(&m_CmdParam[i], &Acc1);

			memset(sParam, 0, sizeof(sParam));

			switch (Acc1.AccessType) {
			case ACCESS_INT_CONST:
				sprintf(sParam, "%d", Acc1.Data.IntConst);
				m_CmdParam[i].AppRequested.Value = Acc1.Data.IntConst;
				break;
			case ACCESS_INT_VAR:
				sprintf(sParam, "%d", *(Acc1.Data.lpVarPointer));
				m_CmdParam[i].AppRequested.Address = Acc1.Data.lpVarPointer;
				break;

			case ACCESS_STR_CONST:
			case ACCESS_STR_VAR:
				sprintf(sParam, "%s", Acc1.Data.lpStrPointer);
				m_CmdParam[i].AppRequested.String = Acc1.Data.lpStrPointer;
				break;
			}
			sprintf(&lpstrResult[WritePos], "%02d:%s", Acc1.AccessType, sParam);
			WritePos += (strlen(&lpstrResult[WritePos]));
			WritePos++;
		}
		return 0;
	}

	//　スクリプトの終わりに達した
	return SCRIPT_END;
}

// パラメータをアプリケーションに渡す
int CScriptPlayer::GetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags) {
	if (index >= m_CmdParamCount) { return 0; }

	memcpy(lpRecord, &m_CmdParam[index], sizeof(SCRIPT_RECORD));

	if (Flags == GETPARAM_VARIABLE_SET) {
		if (lpRecord->CodeType == CODETYPE_VAR_INTERNAL) {
			lpRecord->AppRequested.Value = *(lpRecord->AppRequested.Address);
		} else if (lpRecord->CodeType == CODETYPE_VAR_EXTERNAL) {
			lpRecord->AppRequested.Value = *(lpRecord->AppRequested.Address);
		} else if (lpRecord->CodeType == CODETYPE_VAR_SYSTEM) {
			lpRecord->AppRequested.Value= *(lpRecord->AppRequested.Address);
		}
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////
//　バイナリスクリプトをダンプする
#define MAX_DUMP_SIZE		4 * 1024 * 1024		/* 4 M Bytes */
int CScriptPlayer::DumpBinaryScript(const char *FileName) {
	int Pos, flagFirst, r;
	FILE *fp;
	SCRIPT_RECORD Rec;
	char *sDumpScript;
	char sLine[1024];
	COMMAND_TABLE ct;

	sDumpScript = (char *)(MEMORY_ALLOC(MAX_DUMP_SIZE));
	memset(sDumpScript, 0, MAX_DUMP_SIZE);
	memset(sLine, 0, sizeof(sLine));
	flagFirst = 1;

	for (Pos = 0; Pos * 4 < m_ScriptSize; ) {
		// このアドレスに対してラベルか、関数のエントリが記述されていないか調べる
		r = FindFunction(Pos, sLine);
		if (r >= 0) {
			strcat(sDumpScript, "\n_func ");
			strcat(sDumpScript, sLine);
			strcat(sDumpScript, "\n");
		}

		r = FindLabel(Pos, sLine);
		if (r >= 0) {
			strcat(sDumpScript, "\n#");
			strcat(sDumpScript, sLine);
			strcat(sDumpScript, ":\n");
		}

		r = FindEndFunc(Pos, sLine);
		if (r >= 0) {
			strcat(sDumpScript, "_endfunc ");
			strcat(sDumpScript, sLine);
			strcat(sDumpScript, "\n");
		}

		/* アドレスの出力 */
		sprintf(sLine, "%04x :", Pos * 4);
		strcat(sDumpScript, sLine);

		for (; Pos * 4 < m_ScriptSize; ) {
			/* コマンドを読み出す */
			sLine[0] = 0;

			r = Pos;
			if (m_lpScriptBase[Pos] & 0x80000000) {
				sprintf(sLine, " /* ( %08x ) */ ", (m_lpScriptBase[Pos] & 0x7fffffff) * 4);
				strcat(sDumpScript, sLine);
				Pos++;
				continue;
			} else {
				ReadSingleCode(m_lpScriptBase, Pos, &Rec);
				if (Rec.SepCode != 0 && flagFirst == 0) {
					/* 次のコマンド */
					flagFirst = 1;
					Pos = r;
					break;
				}
			}

			flagFirst = 0;
			if (Rec.SepParam) {
				strcat(sDumpScript, ",");
			}

			/* このパラメータを処理する */
			switch (Rec.CodeType) {
			case CODETYPE_COMMAND_G:
			case CODETYPE_COMMAND_E:
				ct.CodeType = Rec.CodeType;
				ct.CodeParam = Rec.CodeParam;
				r = gAppCommand.DisFindCommand(&ct, sLine);
				if (r < 0) {
					sprintf(sLine, "// Syntax Error.");
				}
				break;

			case CODETYPE_VAR_INTERNAL:
				ct.CodeType = Rec.CodeType;
				ct.CodeParam = Rec.CodeParam;
				r = gAppCommand.DisFindCommand(&ct, sLine);
				if (r < 0) {
					sprintf(sLine, "id_%02d", Rec.CodeParam);
				}
				break;
			case CODETYPE_VAR_EXTERNAL:
				ct.CodeType = Rec.CodeType;
				ct.CodeParam = Rec.CodeParam;
				r = gAppCommand.DisFindCommand(&ct, sLine);
				if (r < 0) {
					sprintf(sLine, "v_%02d", Rec.CodeParam);
				}
				break;
			case CODETYPE_VAR_SYSTEM:
				ct.CodeType = Rec.CodeType;
				ct.CodeParam = Rec.CodeParam;
				r = gAppCommand.DisFindCommand(&ct, sLine);
				if (r < 0) {
					sprintf(sLine, "sys_%04d", Rec.CodeParam);
				}
				break;
			case CODETYPE_VAR_STRING:
				sprintf(sLine, "str_%04d", Rec.CodeParam);
				break;

			case CODETYPE_CONST_NUMBER:
				sprintf(sLine, "%d", Rec.CodeParam);
				break;
			case CODETYPE_CONST_STRING:
				sprintf(sLine, "%s", &m_lpDataBase[Rec.CodeParam]);
				break;

			case CODETYPE_FUNCTION:
			case CODETYPE_LABEL:
				ct.CodeType = Rec.CodeType;
				ct.CodeParam = Rec.CodeParam;
				r = gAppCommand.DisFindCommand(&ct, sLine);
				if (r < 0) {
					sprintf(sLine, "(*%08x)", Rec.CodeParam);
				}
			}

			if (Rec.AccessVarEx == 0) {
				strcat(sDumpScript, " ");
				strcat(sDumpScript, sLine);
			} else if (Rec.AccessVarEx == 1) {
				//　ローカル変数へのアクセス
				strcat(sDumpScript, " [BP-");
				strcat(sDumpScript, sLine);
				strcat(sDumpScript, "]");
			} else if (Rec.AccessVarEx == 2) {
				//　引数へのアクセス
				strcat(sDumpScript, " [BP+");
				strcat(sDumpScript, sLine);
				strcat(sDumpScript, "]");
			}
		}
		strcat(sDumpScript, "\x0d\x0a");			
	}

	/* ファイルに書き込む */
	fp = fopen(FileName, "wt");
	if (fp == NULL) { return 0; }
	r = fprintf(fp, "%s", sDumpScript);
	fclose(fp);

	return (r);
}

//　変数へのアクセスを確認する
int CScriptPlayer::GetAccessVar(LP_SCRIPT_RECORD lpRecord, LP_SCRIPT_ACCESS lpAccess) {
	int p, wp;

	if (lpRecord == NULL) {
		lpAccess->AccessType = ACCESS_NO_ENTRY;
		lpAccess->Data.IntConst = 0;
		return ACCESS_NO_ENTRY;
	}

	switch (lpRecord->CodeType) {
	case CODETYPE_VAR_INTERNAL:
		lpAccess->AccessType = ACCESS_INT_VAR;
		lpAccess->Data.lpVarPointer = &m_lpVarPrivate[lpRecord->CodeParam];
		p = *(lpAccess->Data.lpVarPointer);
		break;
	case CODETYPE_VAR_EXTERNAL:
		lpAccess->AccessType = ACCESS_INT_VAR;
		lpAccess->Data.lpVarPointer = &m_lpVarWork[lpRecord->CodeParam];
		p = *(lpAccess->Data.lpVarPointer);
		break;
	case CODETYPE_VAR_SYSTEM:
		lpAccess->AccessType = ACCESS_INT_VAR;
		lpAccess->Data.lpVarPointer = &m_lpVarSystem[lpRecord->CodeParam];
		p = *(lpAccess->Data.lpVarPointer);
		break;

	case CODETYPE_VAR_STRING:
		lpAccess->AccessType = ACCESS_STR_VAR;
		lpAccess->Data.lpStrPointer = &m_lpStrWork[lpRecord->CodeParam];
		break;

	case CODETYPE_CONST_NUMBER:
		lpAccess->AccessType = ACCESS_INT_CONST;
		lpAccess->Data.IntConst = lpRecord->CodeParam;
		p = lpAccess->Data.IntConst;
		break;
	case CODETYPE_CONST_STRING:
		lpAccess->AccessType = ACCESS_STR_CONST;
		lpAccess->Data.lpStrPointer = &m_lpDataBase[lpRecord->CodeParam];
		break;

	case CODETYPE_FUNCTION:
		lpAccess->AccessType = ACCESS_FUNCTION;
		lpAccess->Data.EntryIndex = lpRecord->CodeParam;
		break;
	case CODETYPE_LABEL:
		lpAccess->AccessType = ACCESS_LABEL;
		lpAccess->Data.JumpAddress = m_lpLabel[lpRecord->CodeParam];
		break;

	default:
		lpAccess->AccessType = -1;
		break;
	}

	// 間接アクセスを確認する
	if (lpRecord->AccessVarEx == 1) {
		wp = m_lpVarSystem[1] - p;
		lpAccess->AccessType = ACCESS_INT_VAR;
		lpAccess->Data.lpVarPointer = &m_lpVarStack[wp];
	} else if (lpRecord->AccessVarEx == 2) {
		wp = m_lpVarSystem[1] + p;
		lpAccess->AccessType = ACCESS_INT_VAR;
		lpAccess->Data.lpVarPointer = &m_lpVarStack[wp];
	}


	return (lpAccess->AccessType);
}

// 指定したアドレスをエントリポイントに持つ関数を検索する
int CScriptPlayer::FindFunction(int addr, char *lpstrFuncName) {
	int i;

	for (i = 0; i < m_FuncCount; i++) {
		if (m_FlagDebugInfo) {
			if (m_lpFuncCall[i].EntryPoint == addr) {
				sprintf(lpstrFuncName, "%s", m_lpFuncCall[i].FuncName);
				return i;
			}
		} else {
			if (m_lpFuncAddr[i].EntryPoint == addr) {
				sprintf(lpstrFuncName, "Func%d", i);
				return i;
			}
		}
	}

	return -1;
}

// 指定したアドレスを終了ポイントに持つ関数を検索する
int CScriptPlayer::FindEndFunc(int addr, char *lpstrFuncName) {
	int i;

	for (i = 0; i < m_FuncCount; i++) {
		if (m_FlagDebugInfo) {
			if (m_lpFuncCall[i].ExitPoint == addr) {
				sprintf(lpstrFuncName, "%s", m_lpFuncCall[i].FuncName);
				return i;
			}
		} else {
			if (m_lpFuncAddr[i].ExitPoint == addr) {
				sprintf(lpstrFuncName, "Func%d", i);
				return i;
			}
		}
	}

	return -1;
}

//　指定したアドレスを持つラベルを検索する
int CScriptPlayer::FindLabel(int addr, char *lpstrResult) {
	int i;

	for (i = 0; i < m_LabelCount; i++) {
		if (m_FlagDebugInfo) {
			if (m_lpLabelInfo[i].Address == addr) {
				sprintf(lpstrResult, "%s", m_lpLabelInfo[i].Name);
				return i;
			}
		} else {
			if (m_lpLabel[i] == addr) {
				sprintf(lpstrResult, "Label%d", i);
				return i;
			}
		}
	}

	return -1;
}
