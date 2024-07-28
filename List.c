/*********************************************************************
**																	**
**		Lexer.cpp		CLexer�N���X�̃C���v�������e�[�V����		**
**																	**
**				Copyright Takahiro Ito, 2003/02/24 - 2002/02/24		**
*********************************************************************/
#include "List.h"
#include "malloc.h"

/*
	���X�g��V�K�쐬����
	�����FFirstData�A�V�K�쐬����郊�X�g�̐擪�̃f�[�^
*/
LIST *CreateList(int FirstData) {
	LIST *lpList;

	/* ���������m�ۂ��� */
	lpList = malloc(sizeof(LIST));

	/* ���������� */
	lpList->data = FirstData;
	lpList->next = NULL;
	lpList->prev = NULL;
	lpList->reserved = 0;

	/* �|�C���^��Ԃ� */
	return (lpList);
}

/*
	���X�g�Ƀf�[�^��ǉ�����
*/
LIST *InsertList(