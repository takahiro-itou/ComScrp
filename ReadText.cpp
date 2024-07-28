//////////////////////////////////////////////////////////////////////
//																	//
//		ReadText.cpp	CCompile�N���X�̉���������					//
//																	//
//				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Compile.h"

//////////////////////////////////////////////////////////////////////
//�@�v���v���Z�X
//�@�}�N���W�J(�\��)�A�y��IF,ELSE,ELSEIF,ENDIF�̓W�J���
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

	//�@��s�܂��́A�P���߂�ǂݏo���B
	for (;;) {
		memset(sLine, 0, sizeof(sLine));
		if (ReadLine(sLine) <= 0) { break; }

		// �g�[�N���ɕ�������
		c = lexer.LexicalAnalyze(sLine);
		lpToken = lexer.GetToken(0);

		// �v���v���Z�X���߂̏���
		if ( !STRCMP(lpToken->token, "$define") ) {
			// �}�N����`
		}
		if ( !STRCMP(lpToken->token, "$if") ) {
		}
		if ( !STRCMP(lpToken->token, "$else") ) {
		}
		if ( !STRCMP(lpToken->token, "$endif") ) {
		}

		// IF,ELSE,ELSEIF,ENDIF�̏���
		i = 1;
		if ( !STRCMP(lpToken->token, "if") ) {
			//�@�L�[���[�h then, goto����������
			for (; i < c; i++) {
				lpToken = lexer.GetToken(i);
				if ( !STRCMP(lpToken->token, "then") ) {
					// �����܂ł����o���B
				}
				if ( !STRCMP(lpToken->token, "goto") ) {
				}
			}
		}

		if ( !STRCMP(lpToken->token, "elseif") ) {
			// ����elseif�̒��O�Ƀ^�[�~�l�[�^��}������
			for (; i < c; i++) {
				lpToken = lexer.GetToken(i);
				if ( !STRCMP(lpToken->token, "then") ) {
				}
			}
		}

		if ( !STRCMP(lpToken->token, "else") ) {
			// ����else�̒��O�ƁA����Ƀ^�[�~�l�[�^��}������
		}
	}

	return n;
}

//////////////////////////////////////////////////////////////////////
//�@���o�͊֌W
//�@��s�A�܂��͈�X�e�[�g�����g��ǂݍ���
int CCompile::ReadLine(char *buf) {
	char p, *s, *write;
	int n;

	s = m_lpNextIn;
	write = buf;
	n = 0;

	if ( s == NULL || (*s) == 0 ) { return (0); }

	//�@��s�A�܂��͈ꖽ�߂�ǂݏo��
	if (m_ScriptFormat == SCRIPT_CRLF) {
		/* ���s�`�� */
		while (*s) {
			p = (*s);
			if (p == '\n' || p == 13) {
				/* �����܂ł�ǂݏo�� */
				break;
			} else if (p == ';') {
				//�@��s�^�R�����g
				while (*s) {
					p = (*s);
					if (p == '\n') { break; }
					s++;
				}
				break;

			} else if (p == '/') {
				//�@�R�����g�̉\��������
				p = *(s+1);
				if (p == '/') {
					//�@�P�s�^�̃R�����g
					while (*s) {
						p = (*s);
						if (p == '\n') { break; }
						s++;
					}
					break;
				} else if (p == '*') {
					//�@�W���I�Ȃb�̃R�����g�`��
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
		/* �Z�~�R�����`�� */
		while (*s) {
			p = (*s);
			if (p == ';') {
				/* �����܂ł�ǂݏo�� */
				break;
			} else if (p == '\n' || p == 13) {
				//�@���s�����̓X�y�[�X�ɒ���
				p = ' ';

			} else if (p == '/') {
				//�@�R�����g�̉\��������
				p = *(s+1);
				if (p == '/') {
					//�@�P�s�^�̃R�����g
					while (*s) {
						p = (*s);
						if (p == '\n') { break; }
						s++;
					}
					s++;
					continue;
				} else if (p == '*') {
					//�@�W���I�Ȃb�̃R�����g�`��
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
//�@�t�@�C���̓ǂݏ���
int CCompile::ReadSourceFile(const char *FileName) {
	size_t r;
	FILE *fp;

	/*�@�t�@�C�����J���āA�o�b�t�@�Ɋi�[���� */
	fp = fopen(FileName, "rt");
	if (fp == NULL) { return 0; }

	/*	�܂��A�t�@�C���̃T�C�Y�𒲂ׂ� */
	m_SourceSize = filelength(fileno(fp));

	/* ���������m�ۂ��� */
	m_lpSource = (char *)(MEMORY_ALLOC(m_SourceSize + 1024));
	memset(m_lpSource, 0, m_SourceSize + 1024);
	m_lpNextIn = m_lpSource;

	/* ���ۂɃt�@�C������ǂݍ��� */
	r = fread(m_lpSource, 1, m_SourceSize, fp);

	/* �t�@�C�����N���[�Y���� */
	fclose(fp);

	/* �ǂݍ��񂾒�����Ԃ� */
	return (r);
}

//�@�X�N���v�g���t�@�C���ɏ�������
int CCompile::WriteScriptFile(const char *FileName, char *sOut) {
	size_t r, tr;
	FILE *fp;
	int Header[64];

	/* �w�b�_��p�ӂ��� */
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

	/* �w�b�_�[���������� */
	fp = fopen(FileName, "wb");
	if (fp == NULL) { return 0; }
	r = fwrite(Header, 1, 256, fp);
	tr = r;

	/* �X�N���v�g�̈���������� */
	r = fwrite(m_lpScript, 1, m_ScriptSize, fp);
	tr += r;

	/* 16�o�C�g�̋��E�ɂ��낦�� */
	r = 16 - ((0x0100 + m_ScriptSize) & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* �f�[�^�̈���������� */
	r = fwrite(m_lpData, 1, m_TotalDataSize, fp);
	tr += r;

	/* 16�o�C�g�̋��E�ɂ��낦�� */
	r = 16 - (m_TotalDataSize & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* ���x������������ */
	r = fwrite(m_lpLabel, 1, 4 * m_LabelCount, fp);
	tr += r;

	/* 16�o�C�g�̋��E�ɂ��낦�� */
	r = 16 - ((m_LabelCount * 4) & 0x0000000f);
	r &= 0x000000000f;
	memset(Header, 0, r);
	fwrite(Header, 1, r, fp);
	tr += r;

	/* �֐����X�g���������� */
	r = fwrite(m_lpFuncAddr, 1, sizeof(FUNC_TABLE) * m_FuncCount, fp);
	tr += r;

	if (m_FlagDebugOption & DEBUGOPT_INFORMATION) {
		/* 256�o�C�g�̋��E�ɂ��낦�� */
		r = 256 - ((m_FuncCount * sizeof(FUNC_TABLE)) & 0x000000ff);
		r &= 0x00000000ff;
		memset(Header, 0, r);
		fwrite(Header, 1, r, fp);
		tr += r;

		/* �f�o�b�O������������ */
		r = fwrite(m_lpLabelInfo, 1, sizeof(LABEL_INFO) * m_LabelCount, fp);
		tr += r;
		r = fwrite(m_lpFuncCall, 1, sizeof(FUNC_INFO) * m_FuncCount, fp);
		tr += r;

		/* 256�o�C�g�̋��E�ɂ��낦�� */
		r = 256 - ((m_FuncCount * sizeof(FUNC_INFO)) & 0x000000ff);
		r &= 0x00000000ff;
		memset(Header, 0, r);
		fwrite(Header, 1, r, fp);
		tr += r;
	}

	fclose(fp);

	/* ���x���� */
	sprintf(sOut, "DataSize = %d\n%d Labels\n%d Functions\n", m_TotalDataSize, m_LabelCount, m_FuncCount);

	/* �������񂾃T�C�Y��Ԃ� */
	return (tr);
}

//�@�}�b�v�t�@�C������������
int CCompile::WriteMapFile(const char *FileName) {
	int r, total;
	int Header[8];
	char IdentifyCode[32];
	FILE *fp;

	memset(IdentifyCode, 0, sizeof(IdentifyCode));
	memset(Header, 0, sizeof(Header));
	total = 0;

	//�@�w�b�_�[����������
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

	//�@���x���̏�����������
	fseek(fp, Header[0], SEEK_SET);
	r = fwrite(m_lpLabelInfo, 1, sizeof(LABEL_INFO) * m_LabelCount, fp);
	total += r;

	//�@�֐��̏�����������
	fseek(fp, Header[2], SEEK_SET);
	r = fwrite(m_lpFuncCall, 1, sizeof(FUNC_INFO) * m_FuncCount, fp);
	total += r;

	return (total);
}

//////////////////////////////////////////////////////////////////////
//�@�֐��A���x���̏���
int CCompile::RegisterFunction(char *Name, int nPCount, int nStartIP, int nEndIP) {
	int i;

	/* �������O�̊֐������邩�m�F���� */
	for (i = 0; i < m_FuncCount; i++) {
		if ( !STRCMP(m_lpFuncCall[i].FuncName, Name) ) {
			/* �����ɏ������� */
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

	// �V�K�ɓo�^���� 
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

	/* �������O�̃��x�������邩�m�F���� */
	for (i = 0; i < m_LabelCount; i++) {
		if ( !STRCMP(m_lpLabelInfo[i].Name, LabelName) ) {
			/* �����ɏ������� */
			if (IP) {
				m_lpLabelInfo[i].Address = IP;
				m_lpLabel[i] = IP;
			}
			return i;
		}
	}

	// �V�K�ɓo�^���� 
	if (m_LabelCount >= MAX_LABEL) { return -1; }
	memset(&m_lpLabelInfo[m_LabelCount], 0, sizeof(LABEL_INFO));

	m_lpLabel[m_LabelCount] = IP;

	strcpy(m_lpLabelInfo[m_LabelCount].Name, LabelName);
	m_lpLabelInfo[m_LabelCount].Address = IP;

	m_LabelCount++;
	return (m_LabelCount - 1);
}

//////////////////////////////////////////////////////////////////////
//�@���o�͊֌W
//�@�X�N���v�g����������
int CCompile::WriteScript(void *Buf, int size) {
	memcpy(m_lpNextOut, Buf, size);

	m_lpNextOut = (char *)(m_lpNextOut) + size;
	m_TotalOut += size;

	return (size);
}

//////////////////////////////////////////////////////////////////////
//�@���̑��̓��o��
void CCompile::DebugWriteSource(const char *FileName) {
	FILE *fp;

	if (m_lpDebugScript == NULL || m_DebugScriptSize == 0) { return; }
	fp = fopen(FileName, "wb");
	if (fp == NULL) { return; }

	fwrite(m_lpDebugScript, 1, m_DebugScriptSize, fp);
	fclose(fp);
}
