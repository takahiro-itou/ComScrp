/*********************************************************************
**																	**
**		List.h			リスト構造の処理							**
**																	**
**				Copyright Takahiro Ito, 2003/02/24 - 2003/02/24		**
*********************************************************************/
#if !defined __COMSCRP_INCLUDE_LIST_H__
#define __COMSCRIP_INCLUDE_LIST_H__

#ifndef NULL
#	define NULL		0
#endif

/* 整数型・リスト構造 */
typedef struct {
	int data;		/*　データ　*/
	void *next;		/*　次のデータへのポインタ　*/
	void *prev;		/*　前のデータへのポインタ　*/
	int reserved;	/*　予約　*/
} LIST, *LP_LIST;

/*　リストの処理　*/
LIST *CreateList(int FirstData);

#endif