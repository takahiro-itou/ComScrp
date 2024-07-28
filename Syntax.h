//////////////////////////////////////////////////////////////////////
//																	//
//		Syntax.h		CSyntaxクラスのインターフェイス				//
//																	//
//		クラスの概要：構文解析を行うクラス							//
//				Copyright Takahiro Ito, 2002/12/27 - 2002/12/28		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_SYNTAX_H__
#define __COMSCRP_INCLUDE_SYNTAX_H__

#include "Lexer.h"
#include "Command.h"
#include "Expression.h"

//　構文解析クラス
class CSyntax {
public:
	CSyntax();
	~CSyntax();

	int InitSyntax();
	int SyntaxAnalyze(CLexer &lexer, CCommand &cmd, int *Buf, int &wp,
			FUNCPTR_WRITE_SCRIPT_RECORD func_write, char *lpDataArea, int &DataSize);
	int WriteParameter(int index, int *Buf, int &wp, FUNCPTR_WRITE_SCRIPT_RECORD func_write);

	LP_LEXER_TOKEN GetCommandToken() { return &m_tokenCommand; }

private:
	int AddParameter(LP_SCRIPT_RECORD lpScrRec);
	int RegisterParameter(CExpression *lpExpression, CLexer & lexer, int *Buf, int &wp,
				int Start, int End, int FlagStr, int ParameterCount, char *lpDataArea, int &DataSize);

private:
	//　構文解析の結果
	LEXER_TOKEN m_tokenCommand;						//　コマンド
	int m_ParameterCount;							//　パラメータ数
	SCRIPT_RECORD m_scrParameter[MAX_TOKEN];		//　パラメータ

	//　構文解析に必要な情報を与える
	CLexer *m_lpLexer;								//　字句解析の結果
	CCommand *m_lpCommand;							//　コマンドテーブル
};

#endif
