//////////////////////////////////////////////////////////////////////
//																	//
//		Module.h		CModuleクラスのインターフェイス				//
//																	//
//		クラスの概要：モジュール処理の管理							//
//				Copyright Takahiro Ito, 2003/01/23 - 2003/01/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_MODULE_H__
#define __COMSCRP_INCLUDE_MODULE_H__

/* ラベル構造 */
typedef struct {
	int ModuleID;			/* モジュールのID */
	char LabelName[52];		/* ラベル名 */
	int LabelID;			/* ラベルのID */
	int Address;			/* ジャンプ先 */
} MODULE_LABEL, LP_MODULE_LABEL;

/* 関数呼び出し */
typedef struct {
	int ModuleID;				/* モジュールID */
	char FunctionName[252];		/* 関数名 */
	int FunctionID;				/* 関数ID */
	int FunctionAddress;		/* 関数のアドレス */
} MODULE_FUNC, LP_MODULE_FUNC;

// CModuleクラス
class CModule {
public:
	CModule();
	~CModule();

	int CreateModule(char ModuleName);
	int EnterModule(char ModuleName);
	int ExitModule(char ModuleName);
private:

	int m_ModuleCount;				/* モジュールの数 */
	int m_CurrentModule;			/* 現在のモジュール */
};

#endif
