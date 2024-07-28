//////////////////////////////////////////////////////////////////////
//																	//
//		Script.cpp		CScriptクラスのインプリメンテーション		//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/12/13		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Script.h"

// グローバル関数

// スクリプトの１レコードを書き込む
int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec) {
	int r;

	if (ScrRec->CodeType == CODETYPE_LOCAL_VAR) {
		// ローカル変数
		ScrRec->CodeType = CODETYPE_CONST_NUMBER;
		ScrRec->CodeParam++;
		ScrRec->AccessVarEx = 1;
	} else if (ScrRec->CodeType == CODETYPE_LOCAL_PARAMETER) {
		// 関数の仮引数
		ScrRec->CodeType = CODETYPE_CONST_NUMBER;
		ScrRec->CodeParam++; 
		ScrRec->AccessVarEx = 2;
	}

	r = 0;
	if (ScrRec->SepCode)  { r |= 0x40000000; }
	if (ScrRec->SepParam) { r |= 0x20000000; }

	r |= ( ((ScrRec->CodeType) & 0x0f) << 24);

	if (ScrRec->VarPoint) {
		r |= 0x00200000;
		r |= ( ((ScrRec->VarPointDest) & 3) << 19);
	}
	r |= ( ((ScrRec->AccessVarEx) & 0x03) << 16);

	if (ScrRec->CodeParam > 0xffff) {
		/* パラメータが２バイトに収まらない場合 */
		r |= ((ScrRec->CodeParam) & 0xffff);
		r |= 0x00800000;
		Buf[wp] = r;
		wp++;

		r = ( ((ScrRec->CodeParam) >> 16) & 0xffff);
		Buf[wp] = r;
	} else {
		/* パラメータを格納する */
		r |= (ScrRec->CodeParam & 0xffff);
		Buf[wp] = r;
	}

	wp++;
	return 1;
}

int ReadSingleCode(int *Buf, int &rp, LP_SCRIPT_RECORD ScrRec) {
	int r;

	r = Buf[rp++];
	memset(ScrRec, 0, sizeof(SCRIPT_RECORD));

	if (r & 0x40000000) { ScrRec->SepCode = 1; }
	if (r & 0x20000000) { ScrRec->SepParam = 1; }

	ScrRec->CodeType = ((r >> 24) & 0x0f);

	if (r & 0x00200000) {
		ScrRec->VarPoint = 1;
		ScrRec->VarPointDest = ((r >> 19) & 3);
	}

	ScrRec->AccessVarEx = ((r >> 16) & 0x03);

	if (r & 0x00800000) {
		/* パラメータが２バイトに収まっていない場合 */
		ScrRec->CodeParam = (r & 0xffff);

		r = Buf[rp++];
		ScrRec->CodeParam |= ((r & 0xffff) << 16);
	} else {
		/* パラメータを格納する */
		ScrRec->CodeParam = (r & 0xffff);
	}

	return 1;
}

//　コンストラクタ／デストラクタ
CScript::CScript() {
}

CScript::~CScript() {
}

//　ＤＬＬの初期化
int CScript::ScriptDLLInit(int ScriptFormat) {

	//　コンパイラを初期化する
	m_Compiler.InitScriptFormat(ScriptFormat);

	//　初期化成功
	return 1;
}

//　スクリプトで使用されるコマンドの定義テーブルを作成する
int CScript::ScriptCommandInit(const char *FileName, int uFlagArchive) {
	//　コマンド
	gAppCommand.InitCommandTable(FileName, uFlagArchive);

	//　OK
	return 1;
}

//　コンパイル時のデバッグオプションを設定する
int CScript::SetCompileOption(int uFlagOption) {
	m_Compiler.SetDebugOption(uFlagOption);
	return 1;
}

//　ソースファイルをコンパイルする
int CScript::CompileSource(const char *SourceFile) {
	int r;

	if (m_Compiler.ReadSourceFile(SourceFile) == 0) {
		return 0;
	}

	r = 0;
	m_Compiler.CompileText(NULL, r);
	return 1;
}

//　作成したバイナリをファイルに書き込む
int CScript::WriteScript(const char *BinaryFile, char *lpstrResult) {
	int r;

	memset(sResult, 0, sizeof(sResult));
	r = m_Compiler.WriteScriptFile(BinaryFile, sResult);
	strcpy(lpstrResult, sResult);
	return r;
}

//　バイナリスクリプトをロードして、実行の準備をする
int CScript::ReadBinary(const char *FileName) {

	if ( !m_ScriptPlayer.ReadBinary(FileName) ) {
		sprintf(sResult, "This file is not Binary Script!\n");
		return 0;
	}
	if ( m_ScriptPlayer.InitializeVariable() ) {
		sprintf(sResult, "Shortage of Memory.\n");
		return 0;
	}
	return 1;
}

//　スクリプトの１ステップを実行する
int CScript::StepCommand(char *lpstrResult) {
	int r;

	r = m_ScriptPlayer.StepCommand(lpstrResult);
	return r;
}
