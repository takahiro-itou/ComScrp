//////////////////////////////////////////////////////////////////////
//																	//
//		Stream.h		CStreamクラスのインターフェイス				//
//																	//
//		クラスの概要：コマンド、変数などコードタイプの管理			//
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
	// 入出力プロセス
	typedef enum {
		STREAM_READ = 0,
		STREAM_WRITE = 1,
		STREAM_TEXT = 0,
		STREAM_BINARY = 2,
	} STREAM_MODE;

	STREAM_MODE m_StreamMode;
	int m_StreamPos;		//　アクセスを行う位置

	void *m_StreamBuf;		//　ストリームのバッファ
	size_t m_StreamSize;	//　ストリームのサイズ
	int m_TotalOut;			//　出力サイズ
	char m_StreamFileName[1024];
};

#endif
