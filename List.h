/*********************************************************************
**																	**
**		List.h			���X�g�\���̏���							**
**																	**
**				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		**
*********************************************************************/
#if !defined __COMSCRP_INCLUDE_LIST_H__
#define __COMSCRIP_INCLUDE_LIST_H__

#ifndef NULL
#	define NULL		0
#endif

/* �����^�E���X�g�\�� */
typedef struct {
	int data;		/*�@�f�[�^�@*/
	void *next;		/*�@���̃f�[�^�ւ̃|�C���^�@*/
	void *prev;		/*�@�O�̃f�[�^�ւ̃|�C���^�@*/
	int reserved;	/*�@�\��@*/
} LIST, *LP_LIST;

/*�@���X�g�̏����@*/
LIST *CreateList(int FirstData);

#endif