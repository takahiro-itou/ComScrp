//////////////////////////////////////////////////////////////////////
//																	//
//		Stream.h		CStream�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F�R�}���h�A�ϐ��ȂǃR�[�h�^�C�v�̊Ǘ�			//
//				Copyright Takahiro Ito, 2003/02/23 - 2003/02/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_STREAM_H__
#define __COMSCRP_INCLUDE_STREAM_H__

#define STREAM_END		-1

class CStream {
public:
	CStream();
	~CStream();

	int OpenStream(const char *FileName, const char *Mode);
	int WriteStream(void *Buf, int Size);
	int ReadStream(void *Buf, int Size);
	int CloseStream();

private:
	// ���o�̓v���Z�X
	typedef enum {
		STREAM_READ = 0,
		STREAM_WRITE = 1,
		STREAM_TEXT = 0,
		STREAM_BINARY = 2,
	} STREAM_MODE;

	STREAM_MODE m_StreamMode;
	int m_StreamPos;		//�@�A�N�Z�X���s���ʒu

	void *m_StreamBuf;		//�@�X�g���[���̃o�b�t�@
	size_t m_StreamSize;	//�@�X�g���[���̃T�C�Y
	int m_TotalOut;			//�@�o�̓T�C�Y
	char m_StreamFileName[1024];
};

#endif
