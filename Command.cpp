//////////////////////////////////////////////////////////////////////
//																	//
//		Command.cpp		CCommand�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2002/12/27 - 2002/02/23		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Command.h"

/* �R�}���h�N���X�̎��� */
CCommand gAppCommand;

//	�R���X�g���N�^�^�f�X�g���N�^
CCommand::CCommand() {
	/* �f�t�H���g�̃f�[�^�ŏ��������� */
	InitCommandTable(COMMAND_TABLE_DEFAULT);
}

CCommand::~CCommand() {
}

//�@���Z�q�̃I�[�o�[���[�h
void CCommand::operator = (CCommand *lpCmd) {
	int i, c;
	LP_COMMAND_TABLE lpCT;

	m_NowTableCount = 0;
	c = lpCmd->GetCountCommand();
	for (i = 0; i < c; i++) {
		lpCT = lpCmd->GetCommandRef(i);
		AddCommand(lpCT);
	}
}

//////////////////////////////////////////////////////////////////////
//�@�e�[�u����`�̏�����
void CCommand::InitCommandTable(const char *FileName, int archive) {
	m_NowTableCount = 0;
	AddCommandTable(FileName, archive);
}

void CCommand::InitCommandTable(const char *lpstrArchive) {
	m_NowTableCount = 0;
	AddCommandTable(lpstrArchive);
}

//////////////////////////////////////////////////////////////////////
//�@�e�[�u����`�t�@�C����ǂݏo��
int CCommand::AddCommandTable(const char *FileName, int archive) {
	char *buf;
	int filesize, c;
	FILE *fp;

	fp = fopen(FileName, "rb");
	if (fp == NULL) {
		MessageBox(NULL, FileName, "Command File not Found!", MB_OK);
		return 0;
	}
	filesize = filelength(fileno(fp));
	fseek(fp, 0, SEEK_SET);

	buf = (char *)malloc(filesize);
	fread (buf, 1, filesize, fp);
	c = 0;

	fclose(fp);

	if (archive) {
		//�@���k���Ă���̂ŁA���̊֐��ŉ𓀂���
		InitCommandTable(buf);
	} else {
		//�@���̂܂ܓ]������
		c = *(int *)(buf);
		buf += 4;
		AddCommandTable( (LP_COMMAND_TABLE)(buf), c);
	}

	//�@�������̉��
	free(buf);
	return c;
}

//�@��`�����񂩂�A�R�}���h�e�[�u�����쐬����
int CCommand::AddCommandTable(const char *lpStrArchive) {
	int c, i, nValue, nBase;
	char p, *s, *temp;
	LP_COMMAND_TABLE lpWork;

	lpWork = (LP_COMMAND_TABLE)MEMORY_ALLOC(MAX_WORD_NAME * sizeof(COMMAND_TABLE));
	memset(lpWork, 0, sizeof(COMMAND_TABLE) * MAX_WORD_NAME);

	c = m_NowTableCount;
	memcpy(lpWork, m_CommandTable, sizeof(COMMAND_TABLE) * c);
	s = (char *)lpStrArchive;

	while (*s) {
		if ( (*s) == '*' ) { break; }
		
		/* �X�y�[�X��؂�ŁA�^�C�v�A�p�����[�^�A���O���w�肷��B */
		for (i = 0; i < 2; i++) {
			nBase = 10;
			nValue = 0;
			if ( (*s) == '$' ) {		/* �P�U�i�\�L */
				nBase = 16;
				s++;
			}

			while (*s) {
				p = (*s++);
				if ( p == ' ' || p == ',' || p == '_') { break; }

				nValue *= nBase;
				if ( p >= '0' && p <= '9') {
					nValue += (p - '0');
				} else if (p >= 'A' && p <= 'F') {
					nValue += (p - 'A' + 10);
				} else if (p >= 'a' && p <= 'f') {
					nValue += (p - 'a' + 10);
				}
			}
			if (i == 0) {
				/* �R�[�h�^�C�v */
				lpWork[c].CodeType = (nValue & 0x0f);
			} else {
				/* �R�[�h�p�����[�^ */
				lpWork[c].CodeParam = (nValue & 0xffff);
			}
		}

		/* �R�[�h�� */
		temp = lpWork[c].Name;
		while (*s) {
			if ((*s) == 0x0d) {
				if (*(s+1) == 0x0a) { s++; }
				break;
			}
			*(temp++) = *(s++);
		}
		*(temp++) = 0;
		s++;
		c++;
	}

	/* �o���オ�����e�[�u�����Z�b�g���� */
	InitCommandTable(lpWork, c);

	//�@�m�ۂ������������������
	MEMORY_FREE(lpWork);

	//�@�o�^��������Ԃ�
	return c;
}

//�@�f�[�^���e�[�u�����猟������
int CCommand::FindCommand(const char *sName, LP_COMMAND_TABLE lpResult) {
	int i;
	char p;

	/* �w�肵�����ʎqsName�����l�̏ꍇ�́A���ʂɏ������� */
	p = *(sName);
	if ( p >= '0' && p <= '9' ) {
		sprintf(lpResult->Name, sName);
		lpResult->CodeType = CODETYPE_CONST_NUMBER;
		lpResult->CodeParam = atoi(sName);
		return -1;
	}

	for (i = 0; i < m_NowTableCount; i++) {
		if ( !STRCMP(m_CommandTable[i].Name, sName) ) {
			memcpy(lpResult, &m_CommandTable[i], sizeof(COMMAND_TABLE));
			return i;
		}
	}

	return -1;
}

//�@�w�肵���^�C�v�ɑ�����A�f�[�^���e�[�u�����猟������
int CCommand::FindCommandSpecifiedType(const char *sName, int Type) {
	int i;

	for (i = 0; i < m_NowTableCount; i++) {
		if ( !STRCMP(m_CommandTable[i].Name, sName) ) {
			if (m_CommandTable[i].CodeType == Type) {
				return i;
			}
		}
	}

	return -1;
}

int CCommand::DeleteCommand(LP_COMMAND_TABLE lpCmd) {
	int i;
	for (i = 0; i < m_NowTableCount; i++) {
		if (m_CommandTable[i].CodeParam == lpCmd->CodeParam) {
			if (m_CommandTable[i].CodeType == lpCmd->CodeType) {
				DeleteCommand(i);
				break;
			}
		}
	}
	return (m_NowTableCount);
}

int CCommand::DeleteCommandType(int nCodeType) {
	int i;
	for (i = 0; i < m_NowTableCount;) {
		if (m_CommandTable[i].CodeType == nCodeType) {
			DeleteCommand(i);
		} else {
			i++;
		}
	}
	return (m_NowTableCount);
}

//////////////////////////////////////////////////////////////////////
//�@�_���v�p
int CCommand::DisFindCommand(LP_COMMAND_TABLE lpCmd, char *sDest) {
	int i;
	int nCt, nCp;

	nCt = lpCmd->CodeType;
	nCp = lpCmd->CodeParam;

	strcpy(sDest, "// Syntax. Error.");
	for (i = 0; i < m_NowTableCount; i++) {
		if (m_CommandTable[i].CodeParam == nCp) {
			if (m_CommandTable[i].CodeType == nCt) {
				strcpy(sDest, m_CommandTable[i].Name);
				return i;
			}
		}
	}

	return -1;
}
