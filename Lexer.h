//////////////////////////////////////////////////////////////////////
//																	//
//		Lexer.h			CLexer�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F������										//
//				Copyright Takahiro Ito, 2002/12/25 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LEXER_H__
#define __COMSCRP_INCLUDE_LEXER_H__

//�@�g�[�N���̍ő吔�ƍő咷
#define MAX_TOKEN			64
#define MAX_TOKEN_LEN		64

//�@�g�[�N���̎��
#define TOKEN_SEPARATOR		0		/* ��؂�L�� */
#define TOKEN_NUMERIC		1		/* ���l */
#define TOKEN_OPERATOR		2		/* ���Z�q */
#define TOKEN_STRING		3		/* ������ */
#define TOKEN_LABEL			4		/* ���x�� */
#define TOKEN_OTHERS		5		/* ���̑� */

//�@�g�[�N��
typedef struct {
	int token_type;					/* �g�[�N���̎�� */
	char token[MAX_TOKEN_LEN];		/* �g�[�N���̓��e */
} LEXER_TOKEN, *LP_LEXER_TOKEN;

//�@��`�e�[�u��
typedef struct {
	char sCharacter[MAX_TOKEN_LEN];	/* �L�� */
	int token_type;					/* ���̋L���̎�� */
} LEXER_TOKEN_TABLE, *LP_LEXER_TOKEN_TABLE;

class CLexer {
public:
	CLexer();
	~CLexer();

	// �L���A���Z�q�Ȃǂ̒�`
	int InitLexer(LP_LEXER_TOKEN_TABLE lpLexerTokenTable, int nTableCount);

	// �w�肵���e�L�X�g���A�g�[�N����͂���
	int LexicalAnalyze(const char *sText);

	//�@�g�[�N����͂̌��ʂ����o��
	int GetTokenCount() { return (m_TokenCount); }
	LP_LEXER_TOKEN GetToken(int index = -1);

	//�@�g�[�N���̈ꕔ���������o���B
	int Slice(CLexer *Dest, int start, int end);

private:
	//�@�g�[�N����z��Ɋi�[����
	int AddToken(char *sToken, int TokenType, int &WritePos);
	int AddToken(LP_LEXER_TOKEN lpToken);

	//�@�w�肵���������A��`�e�[�u���̍ŏ��̕������ǂ����𔻒f����
	int IsCharacterInTable(char p);

	//�@�w�肵�������񂪁A��`�e�[�u���̒��ɂ��邩�ǂ����𒲂ׂ�
	int IsStringInTable(char *s);

private:
	int m_LexerTokenTableCount;					/* ����e�[�u���Ɋi�[���ꂽ�f�[�^�� */
	LP_LEXER_TOKEN_TABLE m_lpLexerTokenTable;	/* ����e�[�u���f�[�^ */
	int m_TokenTableFirstLetter[256];			/* ����e�[�u���̍ŏ��̕��� */

	int m_TokenCount;					/* �g�[�N���̑��� */
	LEXER_TOKEN m_Token[MAX_TOKEN];		/* �g�[�N�� */
	int m_LastGetToken;					/* �Ō�Ɏ��o�����g�[�N���̃C���f�b�N�X */
};

//�@�C�����C�������o�֐�
inline LP_LEXER_TOKEN CLexer::GetToken(int index) {
	if (index == -1) {
		//�@�Ō�Ɏ��o�����g�[�N���̎��̃g�[�N�������o��
		m_LastGetToken++;
	} else {
		/* �w�肵���g�[�N�������o�� */
		m_LastGetToken = index;
	}

	if (m_LastGetToken >= m_TokenCount) {
		return NULL;
	}
	return &m_Token[m_LastGetToken];
}

//�@�g�[�N����z��Ɋi�[����
inline int CLexer::AddToken(char *sToken, int TokenType, int &WritePos) {
	if (m_TokenCount >= MAX_TOKEN) { return 0; }
	if (sToken == NULL || sToken[0] == 0) { return -1; }

	strcpy(m_Token[m_TokenCount].token, sToken);
	m_Token[m_TokenCount].token_type = TokenType;
	m_TokenCount++;

	/* �n���ꂽ��������N���A���� */
	memset(sToken, 0, MAX_TOKEN_LEN);
	WritePos = 0;

	/* ���݂̃g�[�N������Ԃ� */
	return (m_TokenCount);
}

inline int CLexer::AddToken(LP_LEXER_TOKEN lpToken) {
	if (m_TokenCount >= MAX_TOKEN) { return 0; }
	memcpy(&m_Token[m_TokenCount], lpToken, sizeof(LEXER_TOKEN));
	m_TokenCount++;

	return (m_TokenCount);
}

//�@�w�肵���������A��`�e�[�u���̒��ɂ��邩�ǂ����𒲂ׂ�
inline int CLexer::IsCharacterInTable(char p) {
	if (p <= 0 || p >= 128) { return 0; }
	return (m_TokenTableFirstLetter[p]);
}

//�@�w�肵�������񂪁A��`�e�[�u���̒��ɂ��邩�ǂ����𒲂ׂ�
inline int CLexer::IsStringInTable(char *s) {
	int i;
	for (i = 0; i <= m_LexerTokenTableCount; i++) {
		if ( !STRCMP(s, m_lpLexerTokenTable[i].sCharacter) ) {
			/* �e�[�u���̃C���f�b�N�X��Ԃ� */
			return i;
		}
	}
	/* Not Found. */
	return -1;
}

//////////////////////////////////////////////////////////////////////
//�@�f�t�H���g�̃g�[�N���e�[�u��
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
