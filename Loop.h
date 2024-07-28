//////////////////////////////////////////////////////////////////////
//																	//
//		Loop.h			CLoopクラスのインターフェイス				//
//																	//
//		クラスの概要：ループ系コマンドで必要なリストの管理			//
//				Copyright Takahiro Ito, 2003/09/01 - 2003/09/01		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LOOP_H__
#define __COMSCRP_INCLUDE_LOOP_H__

//
//	CLoopクラス
//
class CLoop {
public:
	CLoop();
	virtual ~CLoop();

	//　リストを初期化する
	int InitWriteList();

	//　リストにデータの書き込み位置を追加する
	int AddBreakPointList(int *p);
	int AddContinuePointList(int *p);

	//　ジャンプ先のアドレスが確定したら、リストにある位置に書き込む
	int WriteBreakPoint(int value);
	int WriteContinuePoint(int value);

protected:
	int 
	int *m_IfFalsePoint;			//条件不成立時のジャンプ先を書き込む位置
	int m_ExitPointCount;
	int *m_ExitPoint[MAX_ELSEIF];	//条件成立時のELSEブロックのスキップ先を書き込む位置

};

#endif
