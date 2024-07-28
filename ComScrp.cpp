// ComScrp.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#include "StdAfx.h"

#define COMSCRP_INCLUDED_FROM_THIS
#include "ComScrp.h"

#include "Script.h"

//�@�X�N���v�g���Ǘ�����ACScript�̃I�u�W�F�N�g
CScript gScriptObject;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//�@�G�N�X�|�[�g�����֐�

//�@�c�k�k������������
int InitDLL(const char *TempDirectory, int ScriptFormat) {
	int r;
	r = gScriptObject.ScriptDLLInit(ScriptFormat);
	return (r);
}

//�@�e�L�X�g�R���p�C���p�̃R�}���h�^�V�X�e���ϐ����̒�`���s��
int MakeCommandTable(const char *CommandDefFile, int uFlagArchive) {
	int r;
	r = gScriptObject.ScriptCommandInit(CommandDefFile, uFlagArchive);
	return (r);
}

//�@�R���p�C���A���s�Ȃǂ̌��ʂ��擾����
int GetLastActionResult(char *lpstrResult) {
	int r;
	r = gScriptObject.GetResult(lpstrResult);
	return r;
}

//�@�e�L�X�g�X�N���v�g���R���p�C������
int CompileTextScript(const char *SourceFile, int uFlags) {
	int r;
	gScriptObject.SetCompileOption(uFlags);
	r = gScriptObject.CompileSource(SourceFile);
	return 0;
}

//�@���ԃR�[�h���o�͂���
int WriteDebugText(const char *FileName) {
	int r;
	r = gScriptObject.WriteDebug(FileName);
	return r;
}

//�@�o�C�i���X�N���v�g����������
int WriteBinaryScript(const char *FileName, char *lpstrResult) {
	int r;
	r = gScriptObject.WriteScript(FileName, lpstrResult);
	return r;
}

//�@�}�b�v�t�@�C������������
int WriteBinaryMap(const char *FileName) {
	int r;
	r = gScriptObject.WriteMap(FileName);
	return r;
}

//�@�o�C�i���X�N���v�g���_���v����
int DumpBinary(const char *FileName) {
	int r;
	r = gScriptObject.DumpBinaryScript(FileName);
	return r;
}

//�@�o�C�i���X�N���v�g��ǂݍ���
int ReadBinaryScript(const char *FileName) {
	int r;
	r = gScriptObject.ReadBinary(FileName);
	return 0;
}

//�@�X�N���v�g�̂P�X�e�b�v�����s����
int ExecuteScript(int nScriptPointer, char *lpstrReturn, int nReturnSize) {
	int r;
	if (nScriptPointer >= 0) {
		r = gScriptObject.SetScriptPosition(nScriptPointer);
	}
	r = gScriptObject.StepCommand(lpstrReturn);
	return r;
}

//�@���s�������ʂ��擾����
int ExecuteGetCommand(LP_SCRIPT_RECORD lpRecord) {
	return ( gScriptObject.GetLastCommand(lpRecord) );
}

int ExecuteGetParameterCount() {
	return ( gScriptObject.GetParameterCount() );
}

int ExecuteGetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags) {
	return ( gScriptObject.GetParameter(index, lpRecord, Flags) );
}

//�@���[�J���ϐ����A�v���P�[�V�������֓]������
int GetLocalVariable(int lpReturn[], int nCount) {
	if ( !gScriptObject.GetLocalVar(lpReturn, nCount) ) { return 0; }
	return (nCount);
}

//�@�A�v���P�[�V����������A���[�J���ϐ����󂯎��
int SetLocalVariable(int lpValue[], int nCount) {
	if ( !gScriptObject.SetLocalVar(lpValue, nCount) ) { return 0; }
	return (nCount);
}

//�@�V�X�e���ϐ����A�v���P�[�V�������֓]������
int GetSystemVariable(int lpReturn[], int nCount) {
	if ( !gScriptObject.GetSystemVar(lpReturn, nCount) ) { return 0; }
	return (nCount);
}

//�@�A�v���P�[�V����������A�V�X�e���ϐ����󂯎��
int SetSystemVariable(int lpValue[], int nCount) {
	if ( !gScriptObject.SetSystemVar(lpValue, nCount) ) { return 0; }
	return (nCount);
}
