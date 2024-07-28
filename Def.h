/////////////////////////////////////////////////////////////////////
//																	//
//		Def.h			Common Script System.	ver 0.10			//
//																	//
//		このスクリプトのライブラリを使用するときは、				//
//		このヘッダーファイルをComScrp.hと同じディレクトリ			//
//		に置いて、ComScrp.hをインクルードして下さい。				//
//		　このヘッダーファイルには定数の定義などが含まれています	//
//																	//
//				Copyright Takahiro Ito, 2002/12/13 - 2003/02/26		//
//////////////////////////////////////////////////////////////////////

#if !defined __COMSCRIP_LIBARY_INCLUDE_DEF_H__
#define __COMSCRIP_LIBARY_INCLUDE_DEF_H__

//　スクリプトの形式
#define SCRIPT_CRLF			0	/*　改行を区切り記号とする、１行式		*/
#define SCRIPT_SEMICORON	1	/*　セミコロンを区切りとする、複数行式	*/

//　オプション
#define DEBUGOPT_NOTHING		0		//　指定なし
#define DEBUGOPT_LEXER			1		//	スクリプトの読み出し状況を表示
#define DEBUGOPT_INFORMATION	2		//　バイナリにデバッグ情報を出力

#endif
