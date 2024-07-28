//////////////////////////////////////////////////////////////////////
//																	//
//		Compile.h		CCompile�N���X�̃C���^�[�t�F�C�X			//
//																	//
//		�N���X�̊T�v�F�R���p�C���̃��C���N���X						//
//				Copyright Takahiro Ito, 2002/12/13 - 2002/02/24		//
//////////////////////////////////////////////////////////////////////
#if !defined __INCLUDE_COMPILE_H__
#define __INCLUDE_COMPILE_H__

#include "Lexer.h"
#include "Syntax.h"
#include "IfElse.h"

// IF�̃l�X�g�񐔂̐���
#define MAX_IF_NEST		32

//	FOR�̃l�X�g�񐔂̐���
#define MAX_FOR_NEST	64

//	WHILE�̃l�X�g�񐔂̐���
#define MAX_WHILE_NEST	64

int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);

//�@�X�N���v�g�̌`��
#define SCRIPT_CRLF			0	/*�@���s����؂�L���Ƃ���A�P�s��		*/
#define SCRIPT_SEMICORON	1	/*�@�Z�~�R��������؂�Ƃ���A�����s��	*/

//�@�I�v�V����
#define DEBUGOPT_NOTHING		0		//�@�w��Ȃ�
#define DEBUGOPT_LEXER			1		//	�X�N���v�g�̓ǂݏo���󋵂�\��
#define DEBUGOPT_INFORMATION	2		//�@�o�C�i���Ƀf�o�b�O�����o��

//�@�R���p�C���̃��C���N���X
class CCompile {
public:
	CCompile();
	~CCompile();

	//�@�X�N���v�g�̋L�q�Ɋւ��鏉����
	void InitScriptFormat(int SeparateFormat) { m_ScriptFormat = SeparateFormat; }
	void SetDebugOption(int uFlagOutputScript) {
		m_FlagDebugOption = uFlagOutputScript;
	}

	//�@�R���p�C���A�t�@�C���̓ǂݏ���
	int CompileText(char *sExitStatement, int WritePos);
	int ReadSourceFile(const char *FileName);
	int WriteScriptFile(const char *FileName, char *sOut);
	int WriteMapFile(const char *FileName);

	//�@�֐��Ăяo���K��
	int RegisterFunction(char *Name, int nPCount, int nStartIP, int nEndIP);
	int RegisterLabel(char *LabelName, int IP);

	//�@���o��
	int PreProcess(char *buf);
	int ReadLine(char *buf);
	int WriteScript(void *buf, int size);

	//�@���̑��̓��o��
	void DebugWriteSource(const char *FileName);

private:
	//�@�R���p�C���̏���
	int CompileStatement(char *lpstrStatement, int WritePos);
	//int CompileStatement(CLexer *lpLexer, int WritePos);
	
	//�@�R���p�C���̓�������
	int CompileIfStatement(CLexer *lpLexer, int WritePos);
	int CompileElseStatement(CLexer *lpLexer, int WritePos);
	int CompileEndifStatement(int WritePos);

	int CompileForStatement(CLexer *lpLexer, int WritePos);
	int CompileNextStatement(CLexer *lpLexer, int WritePos);

	int CompileWhileStatement(CLexer *lpLexer, int WritePos);
	int CompileWendStatement(int WritePos);

private:
	int m_ScriptFormat;		/* �X�N���v�g�̃t�H�[�}�b�g */

	//�@�I�v�V����
	int m_FlagDebugOption;

	//�@���o�̓X�g���[��
	char *m_lpNextIn;		/* ���̓��͈ʒu */
	void *m_lpNextOut;		/* ���̏o�͈ʒu */
	char *m_lpDataOut;		/* �f�[�^�̈�̏o�͈ʒu */
	int m_TotalIn;			/* ���͂̍��v�T�C�Y */
	int m_TotalOut;			/* �o�͂̍��v�T�C�Y */
	int m_TotalDataSize;	/* �f�[�^�̈�̃T�C�Y */

	char *m_lpSource;		/* �\�[�X�t�@�C�� */
	int *m_lpScript;		/* �X�N���v�g */
	char *m_lpData;			/* �f�[�^�̈� */
	int m_SourceSize;		/* �\�[�X�t�@�C���̃T�C�Y */
	int m_ScriptSize;		/* �X�N���v�g�̃T�C�Y */

	int m_LabelCount;		/* ���x���̐� */
	int *m_lpLabel;			/* ���x���f�[�^ */
	LP_LABEL_INFO m_lpLabelInfo;

	int m_FuncCount;		/* �֐��̐� */
	FUNC_TABLE *m_lpFuncAddr;	/* �֐��̃W�����v�� */
	FUNC_INFO *m_lpFuncCall;	/* �֐��̌Ăяo���Ɋւ����� */

	//�@�֐��̃R���p�C���p
	int m_FlagNowFunction;		/* �֐����R���p�C�������H */
	char m_FunctionName[64];	/* �֐��� */
	char m_FunctionText[1024];	/* �֐��̐錾 */
	CIfElse m_ExtraProc[32];				// �֐�����Result�R�}���h�̏���

	//�@IF�R�}���h�̃R���p�C���p
	int m_IfElseSP;
	CIfElse m_IfElseProc[MAX_IF_NEST];		//�@IF�X�^�b�N
	
	//	FOR-NEXT�R�}���h�̃R���p�C���p
	int m_ForNextSP;
	int m_ForNextStack[MAX_FOR_NEST];		//	FOR�X�^�b�N

	//	WHILE-WEND�R�}���h�̃R���p�C���p
	int m_WhileSP;
	int m_WhileStack[MAX_WHILE_NEST];		//	WHILE�X�^�b�N

	//�@�X�N���v�g�̃f�o�b�O�p
	char *m_lpDebugScript;	/* �\�[�X�X�N���v�g�̏o�� */
	int m_DebugScriptSize;	/* ���̃T�C�Y */
};

#endif
