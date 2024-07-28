//////////////////////////////////////////////////////////////////////
//																	//
//		Macro.h			�l�X�ȃ}�N���̒�`							//
//																	//
//				Copyright Takahiro Ito, 2002/12/27 - 2003/02/19		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_MACRO_H__
#define __COMSCRP_INCLUDE_MACRO_H__

//�@�������r�Ɋւ���A�啶���������̃`�F�b�N�i�ʏ�͂��Ȃ��j
#if !defined STRCMP
#	if defined STRING_COMPARE_WITH_CASE
#		define		STRCMP		strcmp
#	else
#		define		STRCMP		stricmp
#	endif
#endif

//�@�������r(�������w��)�́A�啶���������̃`�F�b�N
#if !defined STRNCMP
#	if defined STRING_COMPARE_WITH_CASE
#		define		STRNCMP		strncmp
#	else
#		define		STRNCMP		strnicmp
#	endif
#endif

//�@�������̊m�ہA����i�ʏ��malloc,free���g�p�j
#if defined MEMORY_ALLOC_USE_GLOBAL
#	define	MEMORY_ALLOC(size)	GlobalAlloc(GPTR, (size))
#	define	MEMORY_FREE(p)		if (p) { GlobalFree(p); p = NULL; }
#else
#	define	MEMORY_ALLOC(size)	malloc(size)
#	define	MEMORY_FREE(p)		if (p) { free(p); p = NULL; }
#endif

//�@�X�N���v�g�̃w�b�_���
#define SCRIPT_IDENTIFY_CODE		0x001A5343
#define SCRIPT_CURRENT_VERSION		0x00010000
#define SCRIPT_INTERNAL_VERSION		0

//�@�R�[�h�^�C�v
#define CODETYPE_COMMAND_G		0		/* �ėp�R�}���h */
#define CODETYPE_COMMAND_E		1		/* �g���R�}���h */

#define CODETYPE_VAR_INTERNAL	4		/* �����ϐ� */
#define CODETYPE_VAR_EXTERNAL	5		/* �O�����J�ϐ� */
#define CODETYPE_VAR_SYSTEM		6		/* �V�X�e���ϐ� */
#define CODETYPE_VAR_STRING		7		/* ������ϐ� */

#define CODETYPE_CONST_NUMBER	8		/* ���l�萔 */
#define CODETYPE_CONST_STRING	9		/* ������萔 */

#define CODETYPE_FUNCTION		12		/* �֐� */
#define CODETYPE_LABEL			13		/* ���x�� */

#define CODETYPE_LOCAL_VAR		-1		/* ���[�J���ϐ� */
#define CODETYPE_LOCAL_PARAMETER	-2	/* �֐��̈��� */

//�@�p�����[�^�擾�@
#define GETPARAM_DIRECT			0		//�@���̂܂�
#define GETPARAM_VARIABLE_SET	1		//�@�ϐ��́A�l��Ԃ��B
#define GETPARAM_VARIABLE_ADDR	2		//�@�ϐ��́A�A�h���X��Ԃ��B

//�@�R�[�h�^�C�v�}�N��
#define CODETYPE_MASK_COMMAND	0x03		/* �R�}���h */
#define CODETYPE_MASK_VAR		0x0c		/* �ϐ� */

#define CODETYPE_MASK_INTERNAL	0		/* �������� */
#define CODETYPE_MASK_EXTERNAL	1		/* �O������ */
#define CODETYPE_MASK_SYSTEM	2		/* �O�������i�V�X�e���j*/

#endif
