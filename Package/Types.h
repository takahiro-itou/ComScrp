//////////////////////////////////////////////////////////////////////
//																	//
//		Types.h			型名、及び構造体の宣言						//
//																	//
//				Copyright Takahiro Ito, 2003/01/25 - 2003/01/26		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_TYPES_H__
#define __COMSCRP_INCLUDE_TYPES_H__

//　辞書データ
typedef struct {
	char Name[64];		/* 単語 */
	int CodeType;		/* コード種類 */
	int CodeParam;		/* パラメータ */
	int DataSize;		/* 変数などが占有するデータサイズ */
	int Reserved;		/* 予約 */
} COMMAND_TABLE, *LP_COMMAND_TABLE;

//　スクリプトのレコード
typedef union {
	int Value;
	int *Address;
	char *String;
} SCRIPT_RECORD_APPREQUESTED, *LP_SCRIPT_RECORD_APPREQUESTED;
typedef SCRIPT_RECORD_APPREQUESTED SRAPP;
typedef LP_SCRIPT_RECORD_APPREQUESTED LP_SRAPP;

typedef struct {
	int SepCode;		/* コードの区切り */
	int SepParam;		/* パラメータの区切り */
	int CodeType;		/* コードの種類 */
	int CodeParam;		/* コードのパラメータ */
	int VarPoint;		/* 変数ポインタ */
	int VarPointDest;	/* 変数ポインタの対象 */
	int DataPoint;		/* 文字列データの参照 */
	int DataAddress;	/* 文字列データの位置 */

	char *StrData;		/* 文字列データのポインタ */
	int AccessVarEx;	/* 拡張変数アクセス */

	SRAPP AppRequested;	/* アプリケーション側が要求したフォーマットのデータ */
	int Reserved0;		/* 予約 */

	int Reserved3[4];
} SCRIPT_RECORD, *LP_SCRIPT_RECORD;

/* ラベル */
#define MAX_LABEL	65536
typedef struct {
	char Name[60];			/* ラベル名 */
	int Address;			/* アドレス */
} LABEL_INFO, *LP_LABEL_INFO;

/* 関数の呼び出し */
#define MAX_FUNC	65536
typedef struct {
	char FuncName[64];		/* 関数名 */
	int ParamCount;			/* パラメータ数 */
	int EntryPoint;			/* 開始アドレス */
	int ExitPoint;			/* 終了アドレス */
	int Reserved0;			/* 予約 */
} FUNC_INFO, *LP_FUNC_INFO;

typedef struct {
	int ParamCount;
	int EntryPoint;
	int ExitPoint;
	int Reserved0;
} FUNC_TABLE, *LP_FUNC_TABLE;

// 変数へのアクセス
typedef struct {
	int AccessType;				// 種類:(0:定数,1:数値定数,2:文字列定数,3:文字列変数)
	union ACCESSVALUE{
		int IntConst;			// 数値定数
		int *lpVarPointer;		// 数値変数へのポインタ
		char *lpStrPointer;		// 文字列定数、変数
		int EntryIndex;			// 関数のエントリ番号
		int JumpAddress;		// ラベルのジャンプ先
	} Data;
} SCRIPT_ACCESS, *LP_SCRIPT_ACCESS;

#define ACCESS_NO_ENTRY		0
#define ACCESS_INT_CONST	1
#define ACCESS_INT_VAR		2
#define ACCESS_STR_CONST	3
#define ACCESS_STR_VAR		4
#define ACCESS_FUNCTION		9
#define ACCESS_LABEL		10

#endif
