//////////////////////////////////////////////////////////////////////
//																	//
//		Command.h		CCommandクラスのインターフェイス			//
//																	//
//		クラスの概要：コマンド、変数などコードタイプの管理			//
//				Copyright Takahiro Ito, 2002/12/27 - 2003/02/23		//
//////////////////////////////////////////////////////////////////////
#if !defined __COMSCRP_INCLUDE_COMMAND_H__
#define __COMSCRP_INCLUDE_COMMAND_H__

#define MAX_WORD_NAME		131072
#define MAX_WORD_NAME_LEN	64

//　スクリプトのレコードを読み書きする関数のポインタ型
typedef int (*FUNCPTR_WRITE_SCRIPT_RECORD)(int *, int &, LP_SCRIPT_RECORD);

//　コマンド管理
class CCommand {
public:
	CCommand();
	~CCommand();

	/* コマンド定義テーブルを初期化する */
	int AddCommandTable(const char *FileName, int archive);
	int AddCommandTable(const char *lpstrArchive);
	int AddCommandTable(LP_COMMAND_TABLE lpTable, int nCount);

	/* コンパイル用 */
	int AddCommand(const char *sName, int Type, int Param, int Size = 1);
	int AddCommand(LP_COMMAND_TABLE lpTable);

	int FindCommand(const char *sName, LP_COMMAND_TABLE lpResult);
	int FindCommand(const char *sName, int *nResultType, int *nResultParam);

	int FindCommandSpecifiedType(const char *sName, int Type);

	int DeleteCommand(LP_COMMAND_TABLE lpCmd);
	int DeleteCommand(int index);
	int DeleteCommandType(int nCodeType);

	int GetCountSpecifiedType(int nType);
	int GetSizeSpecifiedType(int nType, int nMinSize = 1);

	/* ダンプ用 */
	/* コマンドのタイプとパラメータから、コマンド名を取得する */
	int DisFindCommand(LP_COMMAND_TABLE lpCmd, char *sDest);

	/* その他の関数 */
	int GetCountCommand() { return (m_NowTableCount); }
	LP_COMMAND_TABLE GetCommandRef(int index) { return &m_CommandTable[index]; }

	/* 演算子のオーバーロード */
	void operator = (CCommand *lpCmd);

	//　コマンドテーブルを初期化する
	void InitCommandTable(const char *FileName, int archive);
	void InitCommandTable(const char *lpstrArchive);
	void InitCommandTable(LP_COMMAND_TABLE lpTable, int nCount);

private:
	int m_NowTableCount;
	COMMAND_TABLE m_CommandTable[MAX_WORD_NAME];
};

//　インラインメンバ関数
inline void CCommand::InitCommandTable(LP_COMMAND_TABLE lpTable, int nCount) {
	memcpy(m_CommandTable, lpTable, nCount * sizeof(COMMAND_TABLE));
	m_NowTableCount = nCount;
}

inline int CCommand::AddCommandTable(LP_COMMAND_TABLE lpTable, int nCount) {
	memcpy(&m_CommandTable[m_NowTableCount], lpTable, nCount * sizeof(COMMAND_TABLE));
	m_NowTableCount += nCount;
	return (m_NowTableCount);
}

//　データを追加する
inline int CCommand::AddCommand(const char *sName, int Type, int Param, int Size) {
	int r;
	COMMAND_TABLE Cmd;

	if (m_NowTableCount >= MAX_WORD_NAME) { return 0; }

	strcpy(Cmd.Name, sName);
	Cmd.CodeType = Type;
	Cmd.CodeParam = Param;
	Cmd.DataSize = Size;

	r = AddCommand(&Cmd);
	return r;
}

inline int CCommand::AddCommand(LP_COMMAND_TABLE lpTable) {
	int i;

	for (i = 0; i < m_NowTableCount; i++) {
		if ( !STRCMP(m_CommandTable[i].Name, lpTable->Name) ) {
			//　すでに登録されている
			if (m_CommandTable[i].CodeType != lpTable->CodeType) {
				return -2;
			} else if (m_CommandTable[i].CodeParam != lpTable->CodeParam) {
				return -3;
			} else {
				return i;
			}
		}
	}
	if (m_NowTableCount >= MAX_WORD_NAME) { return -1; }

	memcpy( &m_CommandTable[m_NowTableCount], lpTable, sizeof(COMMAND_TABLE));

	m_NowTableCount++;
	return (m_NowTableCount);
}

//　データを検索する
inline int CCommand::FindCommand(const char *sName, int *nResultType, int *nResultParam) {
	LP_COMMAND_TABLE lpTemp;
	int r;

	r = FindCommand(sName, lpTemp);
	if (r == -1) { return -1; }

	(*nResultType)  = lpTemp->CodeType;
	(*nResultParam) = lpTemp->CodeParam;

	return r;
}

//　指定した種類のデータが何件登録されているか調べる */
inline int CCommand::GetCountSpecifiedType(int nType) {
	int r = 0;
	for (int i = 0; i < m_NowTableCount; i++) {
		if (m_CommandTable[i].CodeType == nType) { r++; }
	}

	return r;
}

//　指定した種類のデータが全部でどれくらいの容量を占めるかを調べる */
inline int CCommand::GetSizeSpecifiedType(int nType, int nMinSize) {
	int r = 0;
	int c;

	for (int i = 0; i < m_NowTableCount; i++) {
		if (m_CommandTable[i].CodeType == nType) {
			c = m_CommandTable[i].DataSize;
			if (c < nMinSize) { c = nMinSize; }
			r += c;
		}
	}

	return r;
}

//　コマンドを削除する
inline int CCommand::DeleteCommand(int index) {
	int i;
	COMMAND_TABLE ct;

	if (index >= m_NowTableCount) { return -1; }

	for (i = index + 1; i < m_NowTableCount; i++) {
		memcpy(&ct, &m_CommandTable[i], sizeof(COMMAND_TABLE));
		memcpy(&m_CommandTable[i-1], &ct, sizeof(COMMAND_TABLE));
	}

	m_NowTableCount--;
	return (m_NowTableCount);
}

//　デフォルトの辞書データ
#define COMMAND_TABLE_DEFAULT	"GenCode.cmd", 1

// 実体
extern CCommand gAppCommand;

#endif
