//////////////////////////////////////////////////////////////////////
//																	//
//		TestPlay.cpp	DLLのテスト。スクリプトの実行を行う			//
//																	//
//				Copyright Takahiro Ito, 2003/02/19 - 2003/02/20		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "Player.h"

void wait_key_push();

void PlayScriptTest() {
	int r;
	CScriptPlayer pl;
	char SourceFile[1024];
	char sResult[1024];

	printf("Source File:");
	scanf("%s", SourceFile);

	if (!pl.ReadBinary(SourceFile)) {
		printf("This file is not Binary Script!\n");
		return;
	}
	if (pl.InitializeVariable()) {
		printf("Shortage of Memory.\n");
		return;
	}

	for (;;) {
		memset(sResult, 0, sizeof(sResult));
		r = pl.StepCommand(sResult);
		if (r == 0) {
			printf("%s\n", sResult);
			wait_key_push();
		} else if (r == SCRIPT_END) {
			printf("\n\n--- end of script ---\n");
			break;
		} else if (r == SCRIPT_STOP) {
			printf("\n--- script is stopping, hit enter key ---\n");
			wait_key_push();
		} else {
			printf("%s\n", sResult);
			break;
		}
	}
}
