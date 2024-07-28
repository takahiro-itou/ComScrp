//////////////////////////////////////////////////////////////////////
//																	//
//		Lexer.cpp		CLexer�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2002/12/25 - 2002/12/25		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Lexer.h"

// �R���X�g���N�^�^�f�X�g���N�^
CLexer::CLexer() {
	InitLexer(LexerTokenTableDefault, LEXER_TOKEN_TABLE_DEFAULT_COUNT);
}

CLexer::~CLexer() {
}

//�@�g�[�N���e�[�u���̍쐬
int CLexer::InitLexer(LP_LEXER_TOKEN_TABLE lpLexerTokenTable, int TableCount) {
	int i, len, max;
	unsigned char c;

	m_LastGetToken = -1;

	m_LexerTokenTableCount = TableCount;
	m_lpLexerTokenTable = lpLexerTokenTable;

	memset(m_TokenTableFirstLetter, 0, sizeof(m_TokenTableFirstLetter));
	for (c = 1; c < 128; c++) {
		//�@���̕������g���Ă��邩
		max = 0;
		for (i = 0; i < TableCount; i++) {
			if (lpLexerTokenTable[i].sCharacter[0] == c) {
				//�@�ő咷���L�^����
				len = strlen(lpLexerTokenTable[i].sCharacter);
				if (len > max) { max = len;	}
			}
			m_TokenTableFirstLetter[c] = max;
		}

	}

	return (1);
}

//�@�w�肵���e�L�X�g���A�g�[�N���ɕ������Ĕz��Ɋi�[����
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
		/* �P�����ǂݍ��� */
		p = *s;
		r = IsCharacterInTable(p);

		/* �ǂݍ��񂾕������A\"�̏ꍇ�A�����萔 */
		if (p == '\"') {
			AddToken(sBuffer, tkType, sBufWrite);
			/* ����\"�܂œǂݍ���ŁA�P�g�[�N���Ƃ��Ċi�[���� */
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
			//�@���x���̎w��Ȃ̂ŁA�Ƃ肠���������܂ł���؂�Ƃ���B
			AddToken(sBuffer, tkType, sBufWrite);

			tkType = TOKEN_LABEL;
			strcpy(sBuffer, "#");
			sBufWrite = 1;

		} else if (p == ' ' || p == '\t' || p == ',') {
			/*�@��؂�L���Ȃ̂ŁA���O�܂łƁA����ȍ~�͕ʂ̃g�[�N�� */
			AddToken(sBuffer, tkType, sBufWrite);
			if (p == ',') {
				strcpy(sBuffer, ",");
				tkType = TOKEN_SEPARATOR;
				AddToken(sBuffer, TOKEN_SEPARATOR, sBufWrite);
			}

			/* ���̌�ɑ����󔒕������X�L�b�v���� */
			temp = (s + 1);
			while (*temp) {
				p = *(temp);
				if (p != ' ' && p != '\t') { break; }
				temp++;
			}
			s = temp - 1;

		} else if (r) {
			/*�@���̕������A��`�e�[�u���̒��ɂ���ꍇ�B
			�@�@�ЂƂ܂��A���̍ő咷���̕�������o�b�t�@�ɓ]������ */
			memset(sTable, 0, sizeof(sTable));
			memcpy(sTable, s, r);
			do {
				/* �e�[�u���̒��Ɉ�v���镶���񂪂��邩���ׂ� */
				index = IsStringInTable(sTable);
				if (index >= 0) {
					// ���������̂ŁA�������̃g�[�N���Ƃ��Ċi�[����
					AddToken(sBuffer, tkType, sBufWrite);

					strcpy(sBuffer, sTable);
					tkType = m_lpLexerTokenTable[index].token_type;
					AddToken(sBuffer, tkType, sBufWrite);

					if (tkType == TOKEN_SEPARATOR) {
						/* ���̌�ɑ����󔒕������X�L�b�v���� */
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

				// ������Ȃ��̂ŁA�P���������炵�Ă݂�
				r--;
				sTable[r] = 0;
			} while (r > 0);

		} else {
			//�@���̑��̕���
			if (tkType == TOKEN_LABEL) {
				//�@���x��
				sBuffer[sBufWrite++] = p;
			} else if (p >= '0' && p <= '9') {
				//�@����
				if (sBufWrite == 0) { tkType = TOKEN_NUMERIC; }
				sBuffer[sBufWrite++] = p;
			} else {
				//�@����ȊO
				if (tkType == TOKEN_NUMERIC) {
					AddToken(sBuffer, tkType, sBufWrite);
				} 
				sBuffer[sBufWrite++] = p;
				tkType = TOKEN_OTHERS;
			}
		}
		s++;
	}

	/* �o�b�t�@����Ȃ���΁A�g�[�N���ɒǉ����� */
	AddToken(sBuffer, tkType, sBufWrite);

	//�@�Q�Ԗڈȍ~�̃g�[�N�����ɁA���x�������������ꍇ��
	//�@��̏������y�ɂ��邽�߁A#���΂�
	for (r = 1; r < m_TokenCount; r++) {
		if ( *(m_Token[r].token) == '#' ) {
			_ASSERT (m_Token[r].token_type == TOKEN_LABEL);
			strcpy(sBuffer, m_Token[r].token + 1);
			strcpy(m_Token[r].token, sBuffer);
		}
	}

	// �g�[�N���̐���Ԃ�
	return (m_TokenCount);
}

//�@�g�[�N���̈ꕔ�����o���B
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
