//////////////////////////////////////////////////////////////////////
//																	//
//		IfElse.h		CIfElseクラスのインターフェイス				//
//																	//
//		クラスの概要：IF,ELSEコマンドで必要なリストの管理			//
//				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_IFELSE_H__
#define __COMSCRP_INCLUDE_IFELSE_H__

//IF-ELSEIF-ENDIFのELSEIFの許容回数
#define MAX_ELSEIF		64

class CIfElse {
public:
	CIfElse();
	~CIfElse();

	void SetFalsePoint(int *p);
	int WriteFalsePoint(int value);

	void InitExitPointList();
	int AddExitPoint(int *p);
	int WriteExitPoint(int value);

private:
	int *m_IfFalsePoint;			//条件不成立時のジャンプ先を書き込む位置
	int m_ExitPointCount;
	int *m_ExitPoint[MAX_ELSEIF];	//条件成立時のELSEブロックのスキップ先を書き込む位置
};

//　エラー通知用
extern int gIfElseErrorInfo;

#define IFELSE_ERR_NOTFINALIZE	1

#endif
