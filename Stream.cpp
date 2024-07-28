//////////////////////////////////////////////////////////////////////
//																	//
//		Stream.cpp		CStream�N���X�̃C���v�������e�[�V����		//
//																	//
//				Copyright Takahiro Ito, 2003/02/23 - 2002/02/23		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Stream.h"

// �R���X�g���N�^�A�f�X�g���N�^
CStream::CStream() {
	m_StreamBuf = NULL;
	m_StreamPos = 0;
	memset(m_StreamFileName, 0, sizeof(m_StreamFileName));
}

CStream::~CStream() {
	CloseStream();

	if (m_StreamBuf) {
		GlobalFree(m_StreamBuf);
	}
}

// �X�g���[�����J��
int CStream::OpenStream(const char *FileName, const char *Mode) {
	char c, *sTemp;
	int m;
	size_t r;
	FILE *fp;

	//�@���[�h���m�F����
	m = 0;
	for (sTemp = (char *)Mode; (*sTemp); sTemp++) {
		c = *sTemp;
		if ( c == 'r' || c == 'R' ) {
		} else if ( c == 'w' || c == 'W' ) {
			m |= STREAM_WRITE;
		}
		if ( c == 't' || c == 'T' ) {
		} else if ( c == 'b' || c == 'B' ) {
			m |= STREAM_BINARY;
		}
	}

	//�@�X�g���[�����J��
	if ( (m & 1) == 0) {
		//�@�ǂݍ���
		fp = fopen(FileName, "r");
		m_StreamSize = filelength(fileno(fp));
		m_StreamBuf = GlobalAlloc(GPTR, m_StreamSize + 1024);
		r = fread(m_StreamBuf, 1, m_StreamSize, fp);
		fclose(fp);
		if (r != m_StreamSize) { return 0; }
	} else {
		//�@��������
		m_StreamSize = 65536;
		m_StreamBuf = GlobalAlloc(GPTR, m_StreamSize + 1024);
		memset(m_StreamBuf, 0, m_StreamSize);
	}

	strcpy(m_StreamFileName, FileName);

	//�@����
	return 1;
}

//�@�X�g���[�������
int CStream::CloseStream() {
	FILE *fp;

	if (m_StreamMode & STREAM_WRITE) {
		//�@�X�g���[���o�b�t�@���t���b�V������
		fp = fopen(m_StreamFileName, "wb");
		fwrite(m_StreamBuf, 1, m_TotalOut, fp);
		fclose(fp);
	}

	//�@�o�b�t�@���N���A����
	GlobalFree(m_StreamBuf);

	//�@����������1��Ԃ�
	return 1;
}

//�@�X�g���[���ɏ�������
int CStream::WriteStream(void *Buf, int Size) {
	if ( (m_StreamMode & 1) == 0 ) {
		//�@�X�g���[�������[�h�����ŊJ����Ă���
		return 0;
	}

	if (m_StreamPos + Size >= (int)m_StreamSize) {
		//�@���������Ċm�ۂ���
		m_StreamSize = m_StreamPos + Size + 1024;
		m_StreamBuf = GlobalReAlloc(m_StreamBuf, m_StreamSize, GMEM_ZEROINIT);
		if (m_StreamBuf == NULL) {
			//�@�������m�ۃG���[
			return 0;
		}
	}

	memcpy( (BYTE *)m_StreamBuf + m_StreamPos, Buf, Size);

	//�@�������񂾃T�C�Y��Ԃ�
	return Size;
}

//�@�X�g���[������ǂݍ���
int CStream::ReadStream(void *Buf, int Size) {
	if ( m_StreamMode & 2 ) {
		//�@�X�g���[�������C�g�����ŊJ����Ă���
		return 0;
	}

	if (m_StreamPos + Size >= (int)m_StreamSize) {
		//�@�X�g���[���̏I�[���z����
		return STREAM_END;
	}

	memcpy( Buf, (BYTE *)m_StreamBuf + m_StreamPos, Size);

	//�@�ǂݍ��񂾃T�C�Y��Ԃ�
	return Size;
}

