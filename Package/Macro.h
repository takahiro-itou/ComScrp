//////////////////////////////////////////////////////////////////////
//																	//
//		Macro.h			様々なマクロの定義							//
//																	//
//				Copyright Takahiro Ito, 2002/12/27 - 2003/02/19		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_MACRO_H__
#define __COMSCRP_INCLUDE_MACRO_H__

//　文字列比較に関する、大文字小文字のチェック（通常はしない）
#if !defined STRCMP
#	if defined STRING_COMPARE_WITH_CASE
#		define		STRCMP		strcmp
#	else
#		define		STRCMP		stricmp
#	endif
#endif

//　文字列比較(文字数指定)の、大文字小文字のチェック
#if !defined STRNCMP
#	if defined STRING_COMPARE_WITH_CASE
#		define		STRNCMP		strncmp
#	else
#		define		STRNCMP		strnicmp
#	endif
#endif

//　メモリの確保、解放（通常はmalloc,freeを使用）
#if defined MEMORY_ALLOC_USE_GLOBAL
#	define	MEMORY_ALLOC(size)	GlobalAlloc(GPTR, (size))
#	define	MEMORY_FREE(p)		if (p) { GlobalFree(p); p = NULL; }
#else
#	define	MEMORY_ALLOC(size)	malloc(size)
#	define	MEMORY_FREE(p)		if (p) { free(p); p = NULL; }
#endif

//　スクリプトのヘッダ情報
#define SCRIPT_IDENTIFY_CODE		0x001A5343
#define SCRIPT_CURRENT_VERSION		0x00010000
#define SCRIPT_INTERNAL_VERSION		0

//　コードタイプ
#define CODETYPE_COMMAND_G		0		/* 汎用コマンド */
#define CODETYPE_COMMAND_E		1		/* 拡張コマンド */

#define CODETYPE_VAR_INTERNAL	4		/* 内部変数 */
#define CODETYPE_VAR_EXTERNAL	5		/* 外部公開変数 */
#define CODETYPE_VAR_SYSTEM		6		/* システム変数 */
#define CODETYPE_VAR_STRING		7		/* 文字列変数 */

#define CODETYPE_CONST_NUMBER	8		/* 数値定数 */
#define CODETYPE_CONST_STRING	9		/* 文字列定数 */

#define CODETYPE_FUNCTION		12		/* 関数 */
#define CODETYPE_LABEL			13		/* ラベル */

#define CODETYPE_LOCAL_VAR		-1		/* ローカル変数 */
#define CODETYPE_LOCAL_PARAMETER	-2	/* 関数の引数 */

//　パラメータ取得法
#define GETPARAM_DIRECT			0		//　そのまま
#define GETPARAM_VARIABLE_SET	1		//　変数は、値を返す。
#define GETPARAM_VARIABLE_ADDR	2		//　変数は、アドレスを返す。

//　コードタイプマクロ
#define CODETYPE_MASK_COMMAND	0x03		/* コマンド */
#define CODETYPE_MASK_VAR		0x0c		/* 変数 */

#define CODETYPE_MASK_INTERNAL	0		/* 内部処理 */
#define CODETYPE_MASK_EXTERNAL	1		/* 外部処理 */
#define CODETYPE_MASK_SYSTEM	2		/* 外部処理（システム）*/

#endif
