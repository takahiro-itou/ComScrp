/////////////////////////////////////////////////////////////////////
//																	//
//		ComScrp.h		Common Script System.	ver 0.10			//
//																	//
//		���̃X�N���v�g�̃��C�u�������g�p����Ƃ��́A				//
//		���̃w�b�_�[�t�@�C�����C���N���[�h���ĉ������B				//
//		���̃w�b�_�[�t�@�C���ɂ͊֐��̃v���g�^�C�v�錾�Ȃǂ�		//
//		�܂܂�Ă��܂��B											//
//		*����*														//
//		���̃w�b�_�t�@�C���͈ȉ��̃t�@�C�����C���N���[�h���܂��B	//
//		�����̃t�@�C�����A���̃w�b�_�t�@�C���Ɠ���				//
//		�f�B���N�g���ɂ����Ă��������B								//
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

//�@�c�k�k������������
int InitDLL(const char *TempDirectory, int ScriptFormat);

//�@�e�L�X�g�R���p�C���p�̃R�}���h�^�V�X�e���ϐ����̒�`���s��
int MakeCommandTable(const char *CommandDefFile, int uFlagArchive);

//�@�R���p�C���A���s�Ȃǂ̌��ʂ��擾����
int GetLastActionResult(char *lpstrResult);

//�@�e�L�X�g�X�N���v�g���R���p�C������
int CompileTextScript(const char *SourceFile, int uFlags);

//�@���ԃR�[�h����������
int WriteDebugText(const char *FileName);

//�@�o�C�i���X�N���v�g����������
int WriteBinaryScript(const char *FileName, char *lpstrResult);

//�@�}�b�v�t�@�C������������
int WriteBinaryMap(const char *FileName);

//�@�o�C�i���X�N���v�g���_���v����
int DumpBinary(const char *FileName);

//�@�o�C�i���X�N���v�g��ǂݍ���
int ReadBinaryScript(const char *FileName);

//�@�X�N���v�g�̂P�X�e�b�v�����s����
int ExecuteScript(int nScriptPointer, char *lpstrReturn, int nReturnSize);

//�@���s�������ʂ��擾����
int ExecuteGetCommand(LP_SCRIPT_RECORD lpRecord);
int ExecuteGetParameterCount();
int ExecuteGetParameter(int index, LP_SCRIPT_RECORD lpRecord, int Flags);

//�@���[�J���ϐ����A�v���P�[�V�������֓]������
int GetLocalVariable(int lpReturn[], int nCount);
//�@�A�v���P�[�V����������A���[�J���ϐ����󂯎��
int SetLocalVariable(int lpValue[], int nCount);

//�@�V�X�e���ϐ����A�v���P�[�V�������֓]������
int GetSystemVariable(int lpReturn[], int nCount);
//�@�A�v���P�[�V����������A�V�X�e���ϐ����󂯎��
int SetSystemVariable(int lpValue[], int nCount);

#endif
