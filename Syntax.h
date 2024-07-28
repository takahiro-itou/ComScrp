//////////////////////////////////////////////////////////////////////
//																	//
//		Syntax.h		CSyntax�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F�\����͂��s���N���X							//
//				Copyright Takahiro Ito, 2002/12/27 - 2002/12/28		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_SYNTAX_H__
#define __COMSCRP_INCLUDE_SYNTAX_H__

#include "Lexer.h"
#include "Command.h"
#include "Expression.h"

//�@�\����̓N���X
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
	//�@�\����͂̌���
	LEXER_TOKEN m_tokenCommand;						//�@�R�}���h
	int m_ParameterCount;							//�@�p�����[�^��
	SCRIPT_RECORD m_scrParameter[MAX_TOKEN];		//�@�p�����[�^

	//�@�\����͂ɕK�v�ȏ���^����
	CLexer *m_lpLexer;								//�@�����͂̌���
	CCommand *m_lpCommand;							//�@�R�}���h�e�[�u��
};

#endif
