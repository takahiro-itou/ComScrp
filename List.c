/*********************************************************************
**																	**
**		Lexer.cpp		CLexerクラスのインプリメンテーション		**
**																	**
**				Copyright Takahiro Ito, 2003/02/24 - 2002/02/24		**
*********************************************************************/
#include "List.h"
#include "malloc.h"

/*
	リストを新規作成する
	引数：FirstData、新規作成されるリストの先頭のデータ
*/
LIST *CreateList(int FirstData) {
	LIST *lpList;

	/* メモリを確保する */
	lpList = malloc(sizeof(LIST));

	/* 初期化する */
	lpList->data = FirstData;
	lpList->next = NULL;
	lpList->prev = NULL;
	lpList->reserved = 0;

	/* ポインタを返す */
	return (lpList);
}

/*
	リストにデータを追加する
*/
LIST *InsertList(