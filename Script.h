//////////////////////////////////////////////////////////////////////
//																	//
//		Script.h		CScriptクラスのインターフェイス				//
//																	//
//		クラスの概要：スクリプトの処理全般を管理するクラス			//
//				Copyright Takahiro Ito, 2002/12/13 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __INCLUDE_SCRIPT_H__
#define __INCLUDE_SCRIPT_H__

#include "Compile.h"
#include "Player.h"

//　スクリプトの１レコードを書き込む処理
int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);
int ReadSingleCode(int *Buf, int &rp, LP_SCRIPT_RECORD ScrRec);

//　CScriptクラス
class CScript {
public:
	CScript();
	~CScript();

	//　初期化
	int ScriptDLLInit(int ScriptFormat);
	int ScriptCommandInit(const char *FileName, int uFlagArchive);

	//　結果の取得
	int GetResult(char *lpstrResult) { strcpy(lpstrResult, sResult); return (strlen(sResult)); }

	//　コンパイル
	int SetCompileOption(int uFlagOption);
	int CompileSource(const char *SourceFile);
	int WriteScript(const char *BinaryFile, char *lpstrResult);
	int WriteDebug(const char *FileName) { m_Compiler.DebugWriteSource(FileName); return 1; }
	int WriteMap(const char *FileName) { return (m_Compiler.WriteMapFile(FileName)); }
	int DumpBinaryScript(const char *FileName) { return (m_ScriptPlayer.DumpBinaryScript(FileName)); }

	//　実行
	int SetScriptPosition(int nIP) { return ( m_ScriptPlayer.SetScriptPosition(nIP) ); }
	int ReadBinary(const char *FileName);
	int StepCommand(char *lpstrResult);

	//　実行結果の取得
	int GetLastCommand(LP_SCRIPT_RECORD lpRecord) { return ( m_ScriptPlayer.GetLastCommand(lpRecord) ); }
	int GetParameterCount() { return ( m_ScriptPlayer.GetParameterCount() ); }
	int GetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags) {
		return ( m_ScriptPlayer.GetParameter(index, lpRecord, Flags) );
	}

	//　変数の転送
	int GetLocalVar(int lpReturn[], int nCount)  { return ( m_ScriptPlayer.GetLocalVar(lpReturn, nCount) ); }
	int SetLocalVar(int lpValue[], int nCount)   { return ( m_ScriptPlayer.SetLocalVar(lpValue,  nCount) ); }
	int GetSystemVar(int lpReturn[], int nCount) { return ( m_ScriptPlayer.GetSystemVar(lpReturn, nCount)); }
	int SetSystemVar(int lpValue[], int nCount)  { return ( m_ScriptPlayer.SetSystemVar(lpValue, nCount) ); }

private:
	//　コンパイラ、及び、プレイヤー
	CCompile m_Compiler;
	CScriptPlayer m_ScriptPlayer;

	//　実行の結果
	char sResult[1024];
};

#endif
