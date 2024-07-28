/////////////////////////////////////////////////////////////////////
//																	//
//		ComScrp.h		Common Script System.	ver 0.10			//
//																	//
//		このスクリプトのライブラリを使用するときは、				//
//		このヘッダーファイルをインクルードして下さい。				//
//		このヘッダーファイルには関数のプロトタイプ宣言などが		//
//		含まれています。											//
//		*注意*														//
//		このヘッダファイルは以下のファイルをインクルードします。	//
//		それらのファイルを、このヘッダファイルと同じ				//
//		ディレクトリにおいてください。								//
//		Types.h, Macro.h, Errors.h, Def.h							//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __LIBARY_INCLUDE_COMSCRP_H__
#define __LIBARY_INCLUDE_COMSCRP_H__

#if !defined COMSCRP_INCLUDED_FROM_THIS
#	include "Types.h"
#	include "Macro.h"
#	include "Errors.h"
#	include "Def.h"
#endif

//　ＤＬＬを初期化する
int InitDLL(const char *TempDirectory, int ScriptFormat);

//　テキストコンパイル用のコマンド／システム変数名の定義を行う
int MakeCommandTable(const char *CommandDefFile, int uFlagArchive);

//　コンパイル、実行などの結果を取得する
int GetLastActionResult(char *lpstrResult);

//　テキストスクリプトをコンパイルする
int CompileTextScript(const char *SourceFile, int uFlags);

//　中間コードを書き込む
int WriteDebugText(const char *FileName);

//　バイナリスクリプトを書き込む
int WriteBinaryScript(const char *FileName, char *lpstrResult);

//　マップファイルを書き込む
int WriteBinaryMap(const char *FileName);

//　バイナリスクリプトをダンプする
int DumpBinary(const char *FileName);

//　バイナリスクリプトを読み込む
int ReadBinaryScript(const char *FileName);

//　スクリプトの１ステップを実行する
int ExecuteScript(int nScriptPointer, char *lpstrReturn, int nReturnSize);

//　実行した結果を取得する
int ExecuteGetCommand(LP_SCRIPT_RECORD lpRecord);
int ExecuteGetParameterCount();
int ExecuteGetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags);

//　ローカル変数をアプリケーション側へ転送する
int GetLocalVariable(int lpReturn[], int nCount);
//　アプリケーション側から、ローカル変数を受け取る
int SetLocalVariable(int lpValue[], int nCount);

//　システム変数をアプリケーション側へ転送する
int GetSystemVariable(int lpReturn[], int nCount);
//　アプリケーション側から、システム変数を受け取る
int SetSystemVariable(int lpValue[], int nCount);

#endif
