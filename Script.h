//////////////////////////////////////////////////////////////////////
//																	//
//		Script.h		CScript�N���X�̃C���^�[�t�F�C�X				//
//																	//
//		�N���X�̊T�v�F�X�N���v�g�̏����S�ʂ��Ǘ�����N���X			//
//				Copyright Takahiro Ito, 2002/12/13 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __INCLUDE_SCRIPT_H__
#define __INCLUDE_SCRIPT_H__

#include "Compile.h"
#include "Player.h"

//�@�X�N���v�g�̂P���R�[�h���������ޏ���
int WriteSingleCode(int *Buf, int &wp, LP_SCRIPT_RECORD ScrRec);
int ReadSingleCode(int *Buf, int &rp, LP_SCRIPT_RECORD ScrRec);

//�@CScript�N���X
class CScript {
public:
	CScript();
	~CScript();

	//�@������
	int ScriptDLLInit(int ScriptFormat);
	int ScriptCommandInit(const char *FileName, int uFlagArchive);

	//�@���ʂ̎擾
	int GetResult(char *lpstrResult) { strcpy(lpstrResult, sResult); return (strlen(sResult)); }

	//�@�R���p�C��
	int SetCompileOption(int uFlagOption);
	int CompileSource(const char *SourceFile);
	int WriteScript(const char *BinaryFile, char *lpstrResult);
	int WriteDebug(const char *FileName) { m_Compiler.DebugWriteSource(FileName); return 1; }
	int WriteMap(const char *FileName) { return (m_Compiler.WriteMapFile(FileName)); }
	int DumpBinaryScript(const char *FileName) { return (m_ScriptPlayer.DumpBinaryScript(FileName)); }

	//�@���s
	int SetScriptPosition(int nIP) { return ( m_ScriptPlayer.SetScriptPosition(nIP) ); }
	int ReadBinary(const char *FileName);
	int StepCommand(char *lpstrResult);

	//�@���s���ʂ̎擾
	int GetLastCommand(LP_SCRIPT_RECORD lpRecord) { return ( m_ScriptPlayer.GetLastCommand(lpRecord) ); }
	int GetParameterCount() { return ( m_ScriptPlayer.GetParameterCount() ); }
	int GetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags) {
		return ( m_ScriptPlayer.GetParameter(index, lpRecord, Flags) );
	}

	//�@�ϐ��̓]��
	int GetLocalVar(int lpReturn[], int nCount)  { return ( m_ScriptPlayer.GetLocalVar(lpReturn, nCount) ); }
	int SetLocalVar(int lpValue[], int nCount)   { return ( m_ScriptPlayer.SetLocalVar(lpValue,  nCount) ); }
	int GetSystemVar(int lpReturn[], int nCount) { return ( m_ScriptPlayer.GetSystemVar(lpReturn, nCount)); }
	int SetSystemVar(int lpValue[], int nCount)  { return ( m_ScriptPlayer.SetSystemVar(lpValue, nCount) ); }

private:
	//�@�R���p�C���A�y�сA�v���C���[
	CCompile m_Compiler;
	CScriptPlayer m_ScriptPlayer;

	//�@���s�̌���
	char sResult[1024];
};

#endif
