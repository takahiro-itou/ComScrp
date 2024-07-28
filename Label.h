//////////////////////////////////////////////////////////////////////
//																	//
//		Label.h			CLabelクラスのインターフェイス				//
//																	//
//		クラスの概要：ラベル、関数呼び出しの処理					//
//				Copyright Takahiro Ito, 2003/01/24 - 2003/01/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_LABEL_H__
#define __COMSCRP_INCLUDE_LABEL_H__

#include "Command.h"

#define MAX_LABEL	65536
#define MAX_FUNC	65536

/* ラベル定義 */
typedef struct {
	char Name[48];		/* ラベル名 */
	int LabelID;		/* ラベルのID */
	int JumpAddr;		/* ジャンプ先アドレス */
	int Reserved0;
	int Reserved1;
} LABEL_INFO, LP_LABEL_INFO;

/* 関数呼び出し時のパラメータ */
typedef struct {
	char Name[28];			/* 仮引数の名 */
	int CodeType;			/* 仮引数の型 */
} FUNC_PARAM_INFO, LP_FUNC_PARAM_INFO;

/* 関数呼び出し */
typedef struct {
	char Name[48];			/* 関数名 */
	int ParameterCount;		/* 引数の数 */
	LP_FUNC_PARAM_INFO *Arg;	/* 引数の情報 */
} FUNC_CALL_INFO, LP_FUNC_CALL_INFO;

class CLabel {
	CLabel();
	~CLabel();

	int Initialize(CCommand *lpCmdTable);

	int ReferLabel(char *LabelName, LP_LABEL_INFO lpLabel);
	int RegisterLabel(char *LabelName, LP_LABEL_INFO lpLabel);

	int RegisterFunction(char *FuncName, LP_FUNC_CALL_INFO lpFunc);
	int ReferFunction(char *FuncName, LP_FUNC_CALL_INFO lpFunc);

	int GetLabelCount();
	int GetFunctionCount();
private:
	CCommand theLocalCommand;			/* ローカル用コマンドテーブル */
	LABEL_INFO m_Label[MAX_LABEL];		/* ラベル */
	FUNC_CALL_INFO m_Func[MAX_FUNC];	/* 関数 */
};

//　インラインメンバ関数
inline int GetLabelCount() {
	int r;
	r = theLocalCommand.Get
#endif
