//////////////////////////////////////////////////////////////////////
//																	//
//		Lexer.cpp		CLexerクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2002/12/25 - 2002/12/25		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Lexer.h"

// コンストラクタ／デストラクタ
CLexer::CLexer() {
	InitLexer(LexerTokenTableDefault, LEXER_TOKEN_TABLE_DEFAULT_COUNT);
}

CLexer::~CLexer() {
}

//　トークンテーブルの作成
int CLexer::InitLexer(LP_LEXER_TOKEN_TABLE lpLexerTokenTable, int TableCount) {
	int i, len, max;
	unsigned char c;

	m_LastGetToken = -1;

	m_LexerTokenTableCount = TableCount;
	m_lpLexerTokenTable = lpLexerTokenTable;

	memset(m_TokenTableFirstLetter, 0, sizeof(m_TokenTableFirstLetter));
	for (c = 1; c < 128; c++) {
		//　この文字が使われているか
		max = 0;
		for (i = 0; i < TableCount; i++) {
			if (lpLexerTokenTable[i].sCharacter[0] == c) {
				//　最大長を記録する
				len = strlen(lpLexerTokenTable[i].sCharacter);
				if (len > max) { max = len;	}
			}
			m_TokenTableFirstLetter[c] = max;
		}

	}

	return (1);
}

//　指定したテキストを、トークンに分割して配列に格納する
int CLexer::LexicalAnalyze(const char *sText) {
	char p, *s, *temp;
	char sBuffer[MAX_TOKEN_LEN];
	char sTable[MAX_TOKEN_LEN];
	int sBufWrite, tkType, r, index;

	m_TokenCount = 0;
	memset(sBuffer, 0, MAX_TOKEN_LEN);
	sBufWrite = 0;
	tkType = 0;

	s = (char *)(sText);
	while (*s) {
		/* １文字読み込む */
		p = *s;
		r = IsCharacterInTable(p);

		/* 読み込んだ文字が、\"の場合、文字定数 */
		if (p == '\"') {
			AddToken(sBuffer, tkType, sBufWrite);
			/* 次の\"まで読み込んで、１トークンとして格納する */
			temp = s + 1;
			while (*temp) {
				if ( (*temp) == '\"' ) {
					break;
				}
				temp++;
			}
			memcpy(sBuffer, s + 1, (temp - s - 1));
			s = temp + 1;
			AddToken(sBuffer, TOKEN_STRING, sBufWrite);

		} else if (p == '#') {
			//　ラベルの指定なので、とりあえずここまでを区切りとする。
			AddToken(sBuffer, tkType, sBufWrite);

			tkType = TOKEN_LABEL;
			strcpy(sBuffer, "#");
			sBufWrite = 1;

		} else if (p == ' ' || p == '\t' || p == ',') {
			/*　区切り記号なので、直前までと、これ以降は別のトークン */
			AddToken(sBuffer, tkType, sBufWrite);
			if (p == ',') {
				strcpy(sBuffer, ",");
				tkType = TOKEN_SEPARATOR;
				AddToken(sBuffer, TOKEN_SEPARATOR, sBufWrite);
			}

			/* この後に続く空白文字をスキップする */
			temp = (s + 1);
			while (*temp) {
				p = *(temp);
				if (p != ' ' && p != '\t') { break; }
				temp++;
			}
			s = temp - 1;

		} else if (r) {
			/*　この文字が、定義テーブルの中にある場合。
			　　ひとまず、その最大長分の文字列をバッファに転送する */
			memset(sTable, 0, sizeof(sTable));
			memcpy(sTable, s, r);
			do {
				/* テーブルの中に一致する文字列があるか調べる */
				index = IsStringInTable(sTable);
				if (index >= 0) {
					// 見つかったので、これを一つのトークンとして格納する
					AddToken(sBuffer, tkType, sBufWrite);

					strcpy(sBuffer, sTable);
					tkType = m_lpLexerTokenTable[index].token_type;
					AddToken(sBuffer, tkType, sBufWrite);

					if (tkType == TOKEN_SEPARATOR) {
						/* この後に続く空白文字をスキップする */
						temp = (s + 1);
						while (*temp) {
							p = *(temp);
							if (p != ' ' && p != '\t') { break; }
							temp++;
						}
						s = temp - 1;
					} else {
						s += (r - 1);
					}

					break;
				}

				// 見つからないので、１文字分減らしてみる
				r--;
				sTable[r] = 0;
			} while (r > 0);

		} else {
			//　その他の文字
			if (tkType == TOKEN_LABEL) {
				//　ラベル
				sBuffer[sBufWrite++] = p;
			} else if (p >= '0' && p <= '9') {
				//　数字
				if (sBufWrite == 0) { tkType = TOKEN_NUMERIC; }
				sBuffer[sBufWrite++] = p;
			} else {
				//　それ以外
				if (tkType == TOKEN_NUMERIC) {
					AddToken(sBuffer, tkType, sBufWrite);
				} 
				sBuffer[sBufWrite++] = p;
				tkType = TOKEN_OTHERS;
			}
		}
		s++;
	}

	/* バッファが空なければ、トークンに追加する */
	AddToken(sBuffer, tkType, sBufWrite);

	//　２番目以降のトークン中に、ラベルが見つかった場合は
	//　後の処理を楽にするため、#を飛ばす
	for (r = 1; r < m_TokenCount; r++) {
		if ( *(m_Token[r].token) == '#' ) {
			_ASSERT (m_Token[r].token_type == TOKEN_LABEL);
			strcpy(sBuffer, m_Token[r].token + 1);
			strcpy(m_Token[r].token, sBuffer);
		}
	}

	// トークンの数を返す
	return (m_TokenCount);
}

//　トークンの一部を取り出す。
int CLexer::Slice(CLexer *lpDest, int start, int end) {
	int i;
	LEXER_TOKEN token;

	lpDest->m_TokenCount = 0;
	for (i = start; i <= end; i++) {
		strcpy(token.token, m_Token[i].token);
		token.token_type = m_Token[i].token_type;

		lpDest->AddToken(&token);
	}

	return (end - start + 1);
}
