//////////////////////////////////////////////////////////////////////
//																	//
//		Stream.cpp		CStreamクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2003/02/23 - 2002/02/23		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Stream.h"

// コンストラクタ、デストラクタ
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

// ストリームを開く
int CStream::OpenStream(const char *FileName, const char *Mode) {
	char c, *sTemp;
	int m;
	size_t r;
	FILE *fp;

	//　モードを確認する
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

	//　ストリームを開く
	if ( (m & 1) == 0) {
		//　読み込み
		fp = fopen(FileName, "r");
		m_StreamSize = filelength(fileno(fp));
		m_StreamBuf = GlobalAlloc(GPTR, m_StreamSize + 1024);
		r = fread(m_StreamBuf, 1, m_StreamSize, fp);
		fclose(fp);
		if (r != m_StreamSize) { return 0; }
	} else {
		//　書き込み
		m_StreamSize = 65536;
		m_StreamBuf = GlobalAlloc(GPTR, m_StreamSize + 1024);
		memset(m_StreamBuf, 0, m_StreamSize);
	}

	strcpy(m_StreamFileName, FileName);

	//　成功
	return 1;
}

//　ストリームを閉じる
int CStream::CloseStream() {
	FILE *fp;

	if (m_StreamMode & STREAM_WRITE) {
		//　ストリームバッファをフラッシュする
		fp = fopen(m_StreamFileName, "wb");
		fwrite(m_StreamBuf, 1, m_TotalOut, fp);
		fclose(fp);
	}

	//　バッファをクリアする
	GlobalFree(m_StreamBuf);

	//　成功したら1を返す
	return 1;
}

//　ストリームに書き込む
int CStream::WriteStream(void *Buf, int Size) {
	if ( (m_StreamMode & 1) == 0 ) {
		//　ストリームがリード属性で開かれている
		return 0;
	}

	if (m_StreamPos + Size >= (int)m_StreamSize) {
		//　メモリを再確保する
		m_StreamSize = m_StreamPos + Size + 1024;
		m_StreamBuf = GlobalReAlloc(m_StreamBuf, m_StreamSize, GMEM_ZEROINIT);
		if (m_StreamBuf == NULL) {
			//　メモリ確保エラー
			return 0;
		}
	}

	memcpy( (BYTE *)m_StreamBuf + m_StreamPos, Buf, Size);

	//　書き込んだサイズを返す
	return Size;
}

//　ストリームから読み込む
int CStream::ReadStream(void *Buf, int Size) {
	if ( m_StreamMode & 2 ) {
		//　ストリームがライト属性で開かれている
		return 0;
	}

	if (m_StreamPos + Size >= (int)m_StreamSize) {
		//　ストリームの終端を越えた
		return STREAM_END;
	}

	memcpy( Buf, (BYTE *)m_StreamBuf + m_StreamPos, Size);

	//　読み込んだサイズを返す
	return Size;
}

