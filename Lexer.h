//////////////////////////////////////////////////////////////////////
//																	//
//		Lexer.h			CLexerクラスのインターフェイス				//
//																	//
//		クラスの概要：字句解析										//
//				Copyright Takahiro Ito, 2002/12/25 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LEXER_H__
#define __COMSCRP_INCLUDE_LEXER_H__

//　トークンの最大数と最大長
#define MAX_TOKEN			64
#define MAX_TOKEN_LEN		64

//　トークンの種類
#define TOKEN_SEPARATOR		0		/* 区切り記号 */
#define TOKEN_NUMERIC		1		/* 数値 */
#define TOKEN_OPERATOR		2		/* 演算子 */
#define TOKEN_STRING		3		/* 文字列 */
#define TOKEN_LABEL			4		/* ラベル */
#define TOKEN_OTHERS		5		/* その他 */

//　トークン
typedef struct {
	int token_type;					/* トークンの種類 */
	char token[MAX_TOKEN_LEN];		/* トークンの内容 */
} LEXER_TOKEN, *LP_LEXER_TOKEN;

//　定義テーブル
typedef struct {
	char sCharacter[MAX_TOKEN_LEN];	/* 記号 */
	int token_type;					/* その記号の種類 */
} LEXER_TOKEN_TABLE, *LP_LEXER_TOKEN_TABLE;

class CLexer {
public:
	CLexer();
	~CLexer();

	// 記号、演算子などの定義
	int InitLexer(LP_LEXER_TOKEN_TABLE lpLexerTokenTable, int nTableCount);

	// 指定したテキストを、トークン解析する
	int LexicalAnalyze(const char *sText);

	//　トークン解析の結果を取り出す
	int GetTokenCount() { return (m_TokenCount); }
	LP_LEXER_TOKEN GetToken(int index = -1);

	//　トークンの一部だけを取り出す。
	int Slice(CLexer *Dest, int start, int end);

private:
	//　トークンを配列に格納する
	int AddToken(char *sToken, int TokenType, int &WritePos);
	int AddToken(LP_LEXER_TOKEN lpToken);

	//　指定した文字が、定義テーブルの最初の文字かどうかを判断する
	int IsCharacterInTable(char p);

	//　指定した文字列が、定義テーブルの中にあるかどうかを調べる
	int IsStringInTable(char *s);

private:
	int m_LexerTokenTableCount;					/* 字句テーブルに格納されたデータ数 */
	LP_LEXER_TOKEN_TABLE m_lpLexerTokenTable;	/* 字句テーブルデータ */
	int m_TokenTableFirstLetter[256];			/* 字句テーブルの最初の文字 */

	int m_TokenCount;					/* トークンの総数 */
	LEXER_TOKEN m_Token[MAX_TOKEN];		/* トークン */
	int m_LastGetToken;					/* 最後に取り出したトークンのインデックス */
};

//　インラインメンバ関数
inline LP_LEXER_TOKEN CLexer::GetToken(int index) {
	if (index == -1) {
		//　最後に取り出したトークンの次のトークンを取り出す
		m_LastGetToken++;
	} else {
		/* 指定したトークンを取り出す */
		m_LastGetToken = index;
	}

	if (m_LastGetToken >= m_TokenCount) {
		return NULL;
	}
	return &m_Token[m_LastGetToken];
}

//　トークンを配列に格納する
inline int CLexer::AddToken(char *sToken, int TokenType, int &WritePos) {
	if (m_TokenCount >= MAX_TOKEN) { return 0; }
	if (sToken == NULL || sToken[0] == 0) { return -1; }

	strcpy(m_Token[m_TokenCount].token, sToken);
	m_Token[m_TokenCount].token_type = TokenType;
	m_TokenCount++;

	/* 渡された文字列をクリアする */
	memset(sToken, 0, MAX_TOKEN_LEN);
	WritePos = 0;

	/* 現在のトークン数を返す */
	return (m_TokenCount);
}

inline int CLexer::AddToken(LP_LEXER_TOKEN lpToken) {
	if (m_TokenCount >= MAX_TOKEN) { return 0; }
	memcpy(&m_Token[m_TokenCount], lpToken, sizeof(LEXER_TOKEN));
	m_TokenCount++;

	return (m_TokenCount);
}

//　指定した文字が、定義テーブルの中にあるかどうかを調べる
inline int CLexer::IsCharacterInTable(char p) {
	if (p <= 0 || p >= 128) { return 0; }
	return (m_TokenTableFirstLetter[p]);
}

//　指定した文字列が、定義テーブルの中にあるかどうかを調べる
inline int CLexer::IsStringInTable(char *s) {
	int i;
	for (i = 0; i <= m_LexerTokenTableCount; i++) {
		if ( !STRCMP(s, m_lpLexerTokenTable[i].sCharacter) ) {
			/* テーブルのインデックスを返す */
			return i;
		}
	}
	/* Not Found. */
	return -1;
}

//////////////////////////////////////////////////////////////////////
//　デフォルトのトークンテーブル
static LEXER_TOKEN_TABLE LexerTokenTableDefault[] = {
	{"<<=", TOKEN_OPERATOR}, {">>=", TOKEN_OPERATOR},
	{"==", TOKEN_OPERATOR}, {"!=", TOKEN_OPERATOR},
	{"<=", TOKEN_OPERATOR}, {">=", TOKEN_OPERATOR},
	{"<<", TOKEN_OPERATOR}, {">>", TOKEN_OPERATOR},
	{"&&", TOKEN_OPERATOR}, {"||", TOKEN_OPERATOR},
	{"++", TOKEN_OPERATOR}, {"--", TOKEN_OPERATOR},
	{"+=", TOKEN_OPERATOR}, {"-=", TOKEN_OPERATOR},
	{"*=", TOKEN_OPERATOR}, {"/-", TOKEN_OPERATOR},
	{"%=", TOKEN_OPERATOR},	{"&=", TOKEN_OPERATOR},
	{"|=", TOKEN_OPERATOR}, {"^=", TOKEN_OPERATOR},
	{"=", TOKEN_OPERATOR}, {"+", TOKEN_OPERATOR}, {"-", TOKEN_OPERATOR},
	{"*", TOKEN_OPERATOR}, {"/", TOKEN_OPERATOR}, {"%", TOKEN_OPERATOR},
	{"&", TOKEN_OPERATOR}, {"|", TOKEN_OPERATOR}, {"^", TOKEN_OPERATOR},
	{"<", TOKEN_OPERATOR}, {">", TOKEN_OPERATOR},
	{"(", TOKEN_OPERATOR}, {")", TOKEN_OPERATOR}, 
	{"[", TOKEN_OTHERS}, {"]", TOKEN_OTHERS},
	{".", TOKEN_OTHERS}, {"->", TOKEN_OTHERS}
};

#define LEXER_TOKEN_TABLE_DEFAULT_COUNT		(sizeof(LexerTokenTableDefault)/sizeof(LEXER_TOKEN_TABLE))

#endif
