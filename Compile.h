//////////////////////////////////////////////////////////////////////
//																	//
//		Compile.h		CCompileクラスのインターフェイス			//
//																	//
//		クラスの概要：コンパイルのメインクラス						//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/02/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __INCLUDE_COMPILE_H__
#define __INCLUDE_COMPILE_H__

#include "Lexer.h"
#include "Syntax.h"
#include "IfElse.h"

// IFのネスト回数の制限
#define MAX_IF_NEST		32

//	FORのネスト回数の制限
#define MAX_FOR_NEST	64

//	WHILEのネスト回数の制限
#define MAX_WHILE_NEST	64

int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);

//　スクリプトの形式
#define SCRIPT_CRLF			0	/*　改行を区切り記号とする、１行式		*/
#define SCRIPT_SEMICORON	1	/*　セミコロンを区切りとする、複数行式	*/

//　オプション
#define DEBUGOPT_NOTHING		0		//　指定なし
#define DEBUGOPT_LEXER			1		//	スクリプトの読み出し状況を表示
#define DEBUGOPT_INFORMATION	2		//　バイナリにデバッグ情報を出力

//　コンパイルのメインクラス
class CCompile {
public:
	CCompile();
	~CCompile();

	//　スクリプトの記述に関する初期化
	void InitScriptFormat(int SeparateFormat) { m_ScriptFormat = SeparateFormat; }
	void SetDebugOption(int uFlagOutputScript) {
		m_FlagDebugOption = uFlagOutputScript;
	}

	//　コンパイル、ファイルの読み書き
	int CompileText(char *sExitStatement, int WritePos);
	int ReadSourceFile(const char *FileName);
	int WriteScriptFile(const char *FileName, char *sOut);
	int WriteMapFile(const char *FileName);

	//　関数呼び出し規約
	int RegisterFunction(char *Name, int nPCount, int nStartIP, int nEndIP);
	int RegisterLabel(char *LabelName, int IP);

	//　入出力
	int PreProcess(char *buf);
	int ReadLine(char *buf);
	int WriteScript(void *buf, int size);

	//　その他の入出力
	void DebugWriteSource(const char *FileName);

private:
	//　コンパイルの処理
	int CompileStatement(char *lpstrStatement, int WritePos);
	//int CompileStatement(CLexer *lpLexer, int WritePos);
	
	//　コンパイルの内部処理
	int CompileIfStatement(CLexer *lpLexer, int WritePos);
	int CompileElseStatement(CLexer *lpLexer, int WritePos);
	int CompileEndifStatement(int WritePos);

	int CompileForStatement(CLexer *lpLexer, int WritePos);
	int CompileNextStatement(CLexer *lpLexer, int WritePos);

	int CompileWhileStatement(CLexer *lpLexer, int WritePos);
	int CompileWendStatement(int WritePos);

private:
	int m_ScriptFormat;		/* スクリプトのフォーマット */

	//　オプション
	int m_FlagDebugOption;

	//　入出力ストリーム
	char *m_lpNextIn;		/* 次の入力位置 */
	void *m_lpNextOut;		/* 次の出力位置 */
	char *m_lpDataOut;		/* データ領域の出力位置 */
	int m_TotalIn;			/* 入力の合計サイズ */
	int m_TotalOut;			/* 出力の合計サイズ */
	int m_TotalDataSize;	/* データ領域のサイズ */

	char *m_lpSource;		/* ソースファイル */
	int *m_lpScript;		/* スクリプト */
	char *m_lpData;			/* データ領域 */
	int m_SourceSize;		/* ソースファイルのサイズ */
	int m_ScriptSize;		/* スクリプトのサイズ */

	int m_LabelCount;		/* ラベルの数 */
	int *m_lpLabel;			/* ラベルデータ */
	LP_LABEL_INFO m_lpLabelInfo;

	int m_FuncCount;		/* 関数の数 */
	FUNC_TABLE *m_lpFuncAddr;	/* 関数のジャンプ先 */
	FUNC_INFO *m_lpFuncCall;	/* 関数の呼び出しに関する情報 */

	//　関数のコンパイル用
	int m_FlagNowFunction;		/* 関数をコンパイル中か？ */
	char m_FunctionName[64];	/* 関数名 */
	char m_FunctionText[1024];	/* 関数の宣言 */
	CIfElse m_ExtraProc[32];				// 関数内のResultコマンドの処理

	//　IFコマンドのコンパイル用
	int m_IfElseSP;
	CIfElse m_IfElseProc[MAX_IF_NEST];		//　IFスタック
	
	//	FOR-NEXTコマンドのコンパイル用
	int m_ForNextSP;
	int m_ForNextStack[MAX_FOR_NEST];		//	FORスタック

	//	WHILE-WENDコマンドのコンパイル用
	int m_WhileSP;
	int m_WhileStack[MAX_WHILE_NEST];		//	WHILEスタック

	//　スクリプトのデバッグ用
	char *m_lpDebugScript;	/* ソーススクリプトの出力 */
	int m_DebugScriptSize;	/* そのサイズ */
};

#endif
