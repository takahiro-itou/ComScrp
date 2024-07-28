//////////////////////////////////////////////////////////////////////
//																	//
//		ReadText.cpp	CCompileクラスの下請け処理					//
//																	//
//				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Compile.h"

//////////////////////////////////////////////////////////////////////
//　プリプロセス
//　マクロ展開(予定)、及びIF,ELSE,ELSEIF,ENDIFの展開作業
int CCompile::PreProcess(char *buf) {
	char *s;
	char *szNextIn;
	char sLine[1024];
	int i;
	int c, n;
	CLexer lexer;
	LP_LEXER_TOKEN lpToken;

	szNextIn = m_lpNextIn;
	n = 0;

	s = buf;
	if ( s == NULL || (*s) == 0 ) { return 0; }
	m_lpNextIn = buf;

	//　一行または、１命令を読み出す。
	for (;;) {
		memset(sLine, 0, sizeof(sLine));
		if (ReadLine(sLine) <= 0) { break; }

		// トークンに分解する
		c = lexer.LexicalAnalyze(sLine);
		lpToken = lexer.GetToken(0);

		// プリプロセス命令の処理
		if ( !STRCMP(lpToken->token, "$define") ) {
			// マクロ定義
		}
		if ( !STRCMP(lpToken->token, "$if") ) {
		}
		if ( !STRCMP(lpToken->token, "$else") ) {
		}
		if ( !STRCMP(lpToken->token, "$endif") ) {
		}

		// IF,ELSE,ELSEIF,ENDIFの処理
		i = 1;
		if ( !STRCMP(lpToken->token, "if") ) {
			//　キーワード then, gotoを検索する
			for (; i < c; i++) {
				lpToken = lexer.GetToken(i);
				if ( !STRCMP(lpToken->token, "then") ) {
					// ここまでを取り出す。
				}
				if ( !STRCMP(lpToken->token, "goto") ) {
				}
			}
		}

		if ( !STRCMP(lpToken->token, "elseif") ) {
			// このelseifの直前にターミネータを挿入する
			for (; i < c; i++) {
				lpToken = lexer.GetToken(i);
				if ( !STRCMP(lpToken->token, "then") ) {
				}
			}
		}

		if ( !STRCMP(lpToken->token, "else") ) {
			// このelseの直前と、直後にターミネータを挿入する
		}
	}

	return n;
}

//////////////////////////////////////////////////////////////////////
//　入出力関係
//　一行、または一ステートメントを読み込む
int CCompile::ReadLine(char *buf) {
	char p, *s, *write;
	int n;

	s = m_lpNextIn;
	write = buf;
	n = 0;

	if ( s == NULL || (*s) == 0 ) { return (0); }

	//　一行、または一命令を読み出す
	if (m_ScriptFormat == SCRIPT_CRLF) {
		/* 改行形式 */
		while (*s) {
			p = (*s);
			if (p == '\n' || p == 13) {
				/* ここまでを読み出す */
				break;
			} else if (p == ';') {
				//　一行型コメント
				while (*s) {
					p = (*s);
					if (p == '\n') { break; }
					s++;
				}
				break;

			} else if (p == '/') {
				//　コメントの可能性がある
				p = *(s+1);
				if (p == '/') {
					//　１行型のコメント
					while (*s) {
						p = (*s);
						if (p == '\n') { break; }
						s++;
					}
					break;
				} else if (p == '*') {
					//　標準的なＣのコメント形式
					while (*s) {
						if ( (*(s) == '*') && (*(s+1) == '/') ) {
							s += 2;	break;
						}
						s++;
					}
					s++;
					continue;

				} else {
					*(write++) = *(s); s++; p = (*s);
				}

			}
			*(write++) = p; s++;
		}

	} else {
		/* セミコロン形式 */
		while (*s) {
			p = (*s);
			if (p == ';') {
				/* ここまでを読み出す */
				break;
			} else if (p == '\n' || p == 13) {
				//　改行文字はスペースに直す
				p = ' ';

			} else if (p == '/') {
				//　コメントの可能性がある
				p = *(s+1);
				if (p == '/') {
					//　１行型のコメント
					while (*s) {
						p = (*s);
						if (p == '\n') { break; }
						s++;
					}
					s++;
					continue;
				} else if (p == '*') {
					//　標準的なＣのコメント形式
					while (*s) {
						if ( (*(s) == '*') && (*(s+1) == '/') ) {
							s += 2; break;
						}
						s++;
					}
					s++;
					continue;

				} else {
					*(write++) = *(s); s++; p = (*s);
				}
			}
			*(write++) = p;	s++;
		}
	}

	n = (int)(s - (char *)m_lpNextIn);
	m_lpNextIn = (s + 1);
	return (n);
}

//////////////////////////////////////////////////////////////////////
//　ファイルの読み書き
int CCompile::ReadSourceFile(const char *FileName) {
	size_t r;
	FILE *fp;

	/*　ファイルを開いて、バッファに格納する */
	fp = fopen(FileName, "rt");
	if (fp == NULL) { return 0; }

	/*	まず、ファイルのサイズを調べる */
	m_SourceSize = filelength(fileno(fp));

	/* メモリを確保する */
	m_lpSource = (char *)(MEMORY_ALLOC(m_SourceSize + 1024));
	memset(m_lpSource, 0, m_SourceSize + 1024);
	m_lpNextIn = m_lpSource;

	/* 実際にファイルから読み込む */
	r = fread(m_lpSource, 1, m_SourceSize, fp);

	/* ファイルをクローズする */
	fclose(fp);

	/* 読み込んだ長さを返す */
	return (r);
}

//　スクリプトをファイルに書き込む
int CCompile::WriteScriptFile(const char *FileName, char *sOut) {
	size_t r, tr;
	FILE *fp;
	int Header[64];

	/* ヘッダを用意する */
	tr = 0;
	memset(Header, 0, sizeof(Header));
	Header[0] = SCRIPT_IDENTIFY_CODE;
	Header[1] = SCRIPT_CURRENT_VERSION;
	Header[2] = SCRIPT_INTERNAL_VERSION;

	Header[4] = 0x0100;
	Header[5] = m_ScriptSize;
	Header[6] = Header[4] + (m_ScriptSize + 15) & 0xfffffff0;
	Header[7] = m_TotalDataSize;

	Header[8] = Header[6] + (m_TotalDataSize + 15) & 0xfffffff0;
	Header[9] = m_LabelCount * 4;

	Header[10] = Header[8] + (m_LabelCount * 4 + 15) & 0xfffffff0;
	Header[11] = m_FuncCount * sizeof(FUNC_TABLE);

	if (m_FlagDebugOption & DEBUGOPT_INFORMATION) {
		Header[12] = Header[10] + (m_FuncCount * sizeof(FUNC_TABLE) + 255) & 0xffffff00;
	} else {
		Header[12] = 0;
	}

	/* ヘッダーを書き込む */
	fp = fopen(FileName, "wb");
	if (fp == NULL) { return 0; }
	r = fwrite(Header, 1, 256, fp);
	tr = r;

	/* スクリプト領域を書き込む */
	r = fwrite(m_lpScript, 1, m_ScriptSize, fp);
	tr += r;

	/* 16バイトの境界にそろえる */
	r = 16 - ((0x0100 + m_ScriptSize) & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* データ領域を書き込む */
	r = fwrite(m_lpData, 1, m_TotalDataSize, fp);
	tr += r;

	/* 16バイトの境界にそろえる */
	r = 16 - (m_TotalDataSize & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* ラベルを書き込む */
	r = fwrite(m_lpLabel, 1, 4 * m_LabelCount, fp);
	tr += r;

	/* 16バイトの境界にそろえる */
	r = 16 - ((m_LabelCount * 4) & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* 関数リストを書き込む */
	r = fwrite(m_lpFuncAddr, 1, sizeof(FUNC_TABLE) * m_FuncCount, fp);
	tr += r;

	if (m_FlagDebugOption & DEBUGOPT_INFORMATION) {
		/* 256バイトの境界にそろえる */
		r = 256 - ((m_FuncCount * sizeof(FUNC_TABLE)) & 0x000000ff);
		r &= 0x00000000ff;
		memset(Header, 0, r);
		fwrite(Header, 1, r, fp);
		tr += r;

		/* デバッグ情報を書き込む */
		r = fwrite(m_lpLabelInfo, 1, sizeof(LABEL_INFO) * m_LabelCount, fp);
		tr += r;
		r = fwrite(m_lpFuncCall, 1, sizeof(FUNC_INFO) * m_FuncCount, fp);
		tr += r;

		/* 256バイトの境界にそろえる */
		r = 256 - ((m_FuncCount * sizeof(FUNC_INFO)) & 0x000000ff);
		r &= 0x00000000ff;
		memset(Header, 0, r);
		fwrite(Header, 1, r, fp);
		tr += r;
	}

	fclose(fp);

	/* ラベル数 */
	sprintf(sOut, "DataSize = %d\n%d Labels\n%d Functions\n", m_TotalDataSize, m_LabelCount, m_FuncCount);

	/* 書き込んだサイズを返す */
	return (tr);
}

//　マップファイルを書き込む
int CCompile::WriteMapFile(const char *FileName) {
	int r, total;
	int Header[8];
	char IdentifyCode[32];
	FILE *fp;

	memset(IdentifyCode, 0, sizeof(IdentifyCode));
	memset(Header, 0, sizeof(Header));
	total = 0;

	//　ヘッダーを書き込む
	sprintf(IdentifyCode, "CS Linker map file\n.");
	Header[0] = 0x0040;
	Header[1] = m_LabelCount;
	Header[2] = Header[0] + (sizeof(LABEL_INFO) * m_LabelCount + 15) & 0xfffffff0;
	Header[3] = m_FuncCount;

	fp = fopen(FileName, "wb");
	r = fwrite(IdentifyCode, 1, sizeof(IdentifyCode), fp);
	total += r;
	r = fwrite(Header, 1, sizeof(Header), fp);
	total += r;

	//　ラベルの情報を書き込む
	fseek(fp, Header[0], SEEK_SET);
	r = fwrite(m_lpLabelInfo, 1, sizeof(LABEL_INFO) * m_LabelCount, fp);
	total += r;

	//　関数の情報を書き込む
	fseek(fp, Header[2], SEEK_SET);
	r = fwrite(m_lpFuncCall, 1, sizeof(FUNC_INFO) * m_FuncCount, fp);
	total += r;

	return (total);
}

//////////////////////////////////////////////////////////////////////
//　関数、ラベルの処理
int CCompile::RegisterFunction(char *Name, int nPCount, int nStartIP, int nEndIP) {
	int i;

	/* 同じ名前の関数があるか確認する */
	for (i = 0; i < m_FuncCount; i++) {
		if ( !STRCMP(m_lpFuncCall[i].FuncName, Name) ) {
			/* ここに書き込む */
			if (nPCount >= 0) {
				m_lpFuncCall[i].ParamCount = nPCount;
			}
			if (nStartIP) {
				m_lpFuncAddr[i].EntryPoint = nStartIP;
				m_lpFuncCall[i].EntryPoint = nStartIP;
			}
			if (nEndIP) {
				m_lpFuncAddr[i].ExitPoint = nEndIP;
				m_lpFuncCall[i].ExitPoint = nEndIP;
			}
			return i;
		}
	}

	// 新規に登録する 
	if (m_FuncCount >= MAX_FUNC) { return 0; }
	memset(&m_lpFuncAddr[m_FuncCount], 0, sizeof(FUNC_TABLE));
	memset(&m_lpFuncCall[m_FuncCount], 0, sizeof(FUNC_INFO));

	m_lpFuncAddr[m_FuncCount].ParamCount = nPCount;
	m_lpFuncAddr[m_FuncCount].EntryPoint = nStartIP;
	m_lpFuncAddr[m_FuncCount].ExitPoint = nEndIP;

	strcpy(m_lpFuncCall[m_FuncCount].FuncName, Name);
	m_lpFuncCall[m_FuncCount].ParamCount = nPCount;
	m_lpFuncCall[m_FuncCount].EntryPoint = nStartIP;
	m_lpFuncCall[m_FuncCount].ExitPoint = nEndIP;

	m_FuncCount++;
	return (m_FuncCount - 1);
}

int CCompile::RegisterLabel(char *LabelName, int IP) {
	int i;

	/* 同じ名前のラベルがあるか確認する */
	for (i = 0; i < m_LabelCount; i++) {
		if ( !STRCMP(m_lpLabelInfo[i].Name, LabelName) ) {
			/* ここに書き込む */
			if (IP) {
				m_lpLabelInfo[i].Address = IP;
				m_lpLabel[i] = IP;
			}
			return i;
		}
	}

	// 新規に登録する 
	if (m_LabelCount >= MAX_LABEL) { return -1; }
	memset(&m_lpLabelInfo[m_LabelCount], 0, sizeof(LABEL_INFO));

	m_lpLabel[m_LabelCount] = IP;

	strcpy(m_lpLabelInfo[m_LabelCount].Name, LabelName);
	m_lpLabelInfo[m_LabelCount].Address = IP;

	m_LabelCount++;
	return (m_LabelCount - 1);
}

//////////////////////////////////////////////////////////////////////
//　入出力関係
//　スクリプトを書き込む
int CCompile::WriteScript(void *Buf, int size) {
	memcpy(m_lpNextOut, Buf, size);

	m_lpNextOut = (char *)(m_lpNextOut) + size;
	m_TotalOut += size;

	return (size);
}

//////////////////////////////////////////////////////////////////////
//　その他の入出力
void CCompile::DebugWriteSource(const char *FileName) {
	FILE *fp;

	if (m_lpDebugScript == NULL || m_DebugScriptSize == 0) { return; }
	fp = fopen(FileName, "wb");
	if (fp == NULL) { return; }

	fwrite(m_lpDebugScript, 1, m_DebugScriptSize, fp);
	fclose(fp);
}
