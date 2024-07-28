//////////////////////////////////////////////////////////////////////
//																	//
//		Errors.h		�G���[�̒�`								//
//																	//
//				Copyright Takahiro Ito, 2003/02/19 - 2003/02/19		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_ERRORS_H__
#define __COMSCRIP_INCLUDE_ERRORS_H__

//�@�X�N���v�g�̏I��
#define SCRIPT_END			-1
#define SCRIPT_STOP			-2

//�@�G���[�R�[�h
enum ErrorCode {
	ERROR_NOERRORS = 0,
	ERROR_ILLEGAL_INSTRUCTION,
	ERROR_ILLEGAL_OPERATION,
	ERROR_ILLEGAL_FUNCTION_CALL,
	ERROR_DIVISION_BY_ZERO,
	ERROR_OVERFLOW,
	ERROR_RETURN_WITHOUT_GOSUB,
	ERROR_IO_ERROR
};

//�@�R���p�C���G���[
enum CompileErrorCode {
	CLERR_NOERRORS = 0,
	CLERR_SYNTAX_ERROR,
	CLERR_UNDEC_IDENTIFIER
};

#endif
