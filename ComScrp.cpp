// ComScrp.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "StdAfx.h"

#define COMSCRP_INCLUDED_FROM_THIS
#include "ComScrp.h"

#include "Script.h"

//　スクリプトを管理する、CScriptのオブジェクト
CScript gScriptObject;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//　エクスポートされる関数

//　ＤＬＬを初期化する
int InitDLL(const char *TempDirectory, int ScriptFormat) {
	int r;
	r = gScriptObject.ScriptDLLInit(ScriptFormat);
	return (r);
}

//　テキストコンパイル用のコマンド／システム変数名の定義を行う
int MakeCommandTable(const char *CommandDefFile, int uFlagArchive) {
	int r;
	r = gScriptObject.ScriptCommandInit(CommandDefFile, uFlagArchive);
	return (r);
}

//　コンパイル、実行などの結果を取得する
int GetLastActionResult(char *lpstrResult) {
	int r;
	r = gScriptObject.GetResult(lpstrResult);
	return r;
}

//　テキストスクリプトをコンパイルする
int CompileTextScript(const char *SourceFile, int uFlags) {
	int r;
	gScriptObject.SetCompileOption(uFlags);
	r = gScriptObject.CompileSource(SourceFile);
	return 0;
}

//　中間コードを出力する
int WriteDebugText(const char *FileName) {
	int r;
	r = gScriptObject.WriteDebug(FileName);
	return r;
}

//　バイナリスクリプトを書き込む
int WriteBinaryScript(const char *FileName, char *lpstrResult) {
	int r;
	r = gScriptObject.WriteScript(FileName, lpstrResult);
	return r;
}

//　マップファイルを書き込む
int WriteBinaryMap(const char *FileName) {
	int r;
	r = gScriptObject.WriteMap(FileName);
	return r;
}

//　バイナリスクリプトをダンプする
int DumpBinary(const char *FileName) {
	int r;
	r = gScriptObject.DumpBinaryScript(FileName);
	return r;
}

//　バイナリスクリプトを読み込む
int ReadBinaryScript(const char *FileName) {
	int r;
	r = gScriptObject.ReadBinary(FileName);
	return 0;
}

//　スクリプトの１ステップを実行する
int ExecuteScript(int nScriptPointer, char *lpstrReturn, int nReturnSize) {
	int r;
	if (nScriptPointer >= 0) {
		r = gScriptObject.SetScriptPosition(nScriptPointer);
	}
	r = gScriptObject.StepCommand(lpstrReturn);
	return r;
}

//　実行した結果を取得する
int ExecuteGetCommand(LP_SCRIPT_RECORD lpRecord) {
	return ( gScriptObject.GetLastCommand(lpRecord) );
}

int ExecuteGetParameterCount() {
	return ( gScriptObject.GetParameterCount() );
}

int ExecuteGetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags) {
	return ( gScriptObject.GetParameter(index, lpRecord, Flags) );
}

//　ローカル変数をアプリケーション側へ転送する
int GetLocalVariable(int lpReturn[], int nCount) {
	if ( !gScriptObject.GetLocalVar(lpReturn, nCount) ) { return 0; }
	return (nCount);
}

//　アプリケーション側から、ローカル変数を受け取る
int SetLocalVariable(int lpValue[], int nCount) {
	if ( !gScriptObject.SetLocalVar(lpValue, nCount) ) { return 0; }
	return (nCount);
}

//　システム変数をアプリケーション側へ転送する
int GetSystemVariable(int lpReturn[], int nCount) {
	if ( !gScriptObject.GetSystemVar(lpReturn, nCount) ) { return 0; }
	return (nCount);
}

//　アプリケーション側から、システム変数を受け取る
int SetSystemVariable(int lpValue[], int nCount) {
	if ( !gScriptObject.SetSystemVar(lpValue, nCount) ) { return 0; }
	return (nCount);
}
