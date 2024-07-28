//////////////////////////////////////////////////////////////////////
//																	//
//		Player.h		CScriptPlayerクラスのインターフェイス		//
//																	//
//		クラスの概要：スクリプトの実行、逆コンパイル(デバッグ用)	//
//				Copyright Takahiro Ito, 2003/01/14 - 2003/02/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_PLAYER_H__
#define __COMSCRP_INCLUDE_PLAYER_H__

#include "Command.h"
#include "Stream.h"

int ReadSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);

class CScriptPlayer {
public:
	CScriptPlayer();
	~CScriptPlayer();

	/* 初期化、読み込み */
	int InitializeVariable();
	int ReadBinary(const char *FileName);

	/* 実行 */
	int SetScriptPosition(int nIP) { m_ScriptPos = nIP; return (m_ScriptPos); }
	int StepCommand(char *lpstrResult);

	/* アプリケーションへの指示 */
	int GetLastCommand(LP_SCRIPT_RECORD lpRecord) { memcpy(lpRecord, &m_LastCommand, sizeof(SCRIPT_RECORD)); return 1; }
	int GetParameterCount() { return (m_CmdParamCount); }
	int GetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags);

	/* バイナリスクリプトのダンプ */
	int DumpBinaryScript(const char *FileName);
	int FindLabel(int addr, char *lpstrResult);
	int FindFunction(int addr, char *lpstrFuncName);
	int FindEndFunc(int Addr, char *lpstrFuncName);

	//　変数操作
	int GetLocalVar(int lpReturn[], int nCount) { memcpy(lpReturn, m_lpVarWork, sizeof(int) * nCount); return 1; }
	int SetLocalVar(int lpValue[], int nCount)  { memcpy(m_lpVarWork, lpValue,  sizeof(int) * nCount); return 1; }
	int GetSystemVar(int lpReturn[], int nCount) { memcpy(lpReturn, m_lpVarSystem, sizeof(int) * nCount); return 1; }
	int SetSystemVar(int lpValue[], int nCount)  { memcpy(m_lpVarSystem, lpValue,  sizeof(int) * nCount); return 1; }

private:
	//　アクセスする変数のポインタを取得する
	int GetAccessVar(LP_SCRIPT_RECORD lpRecord, LP_SCRIPT_ACCESS lpAccess);

private:
	/* スクリプトデータ */
	int m_ScriptSize;		/* スクリプトのサイズ */
	int *m_lpScriptBase;	/* スクリプトの格納されたメモリ */
	int m_ScriptPos;		/* 現在実行中のスクリプトの位置 */

	int m_DataSize;			/* データのサイズ */
	char *m_lpDataBase;		/* データの格納されたメモリ */

	int m_LabelCount;		/* ラベルの数 */
	int *m_lpLabel;			/* ラベルのジャンプ先 */

	int m_FuncCount;		/* 関数の数 */
	FUNC_TABLE *m_lpFuncAddr;	/* 関数のジャンプ先 */

	// デバッグ情報
	int m_FlagDebugInfo;		/* デバッグ情報の有無 */
	LP_LABEL_INFO m_lpLabelInfo;
	FUNC_INFO *m_lpFuncCall;	/* 関数の呼び出しに関する情報 */

	// 最後に実行したコマンドのパラメータ
	SCRIPT_RECORD m_LastCommand;	//コマンド
	int m_CmdParamCount;			//パラメータ数
	SCRIPT_RECORD m_CmdParam[64];	//パラメータ

	//　変数領域
	int *m_lpVarWork;		//　ワーク変数
	int *m_lpVarSystem;		//　システム変数
	int *m_lpVarPrivate;	//　プライベート変数
	int *m_lpVarStack;		//　スタック領域
	char *m_lpStrWork;		//　文字列変数

	/* コールスタック */
	int m_CallStackPointer;		/* SP */
	int m_CallStack[256];		/* スタックの内容 */

	//　ストリーム
	CStream *m_Stream;
};

#endif
