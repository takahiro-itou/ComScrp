//////////////////////////////////////////////////////////////////////
//																	//
//		TestMain.cpp	DLLのテストを行う							//
//																	//
//				Copyright Takahiro Ito, 2002/12/25 - 2002/12/26		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <stdio.h>

void CompileScriptTest();
void PlayScriptTest();

void wait_key_push() {
	char c;

	for(;;) {
		c = getchar();
		if (c == 13 || c == 10) { break; }
	}
}

int main() {
	int r;

	//printf("ReadLine Test.\n");
	//ReadLineTest("test.txt");

	for (;;) {
		printf("Menu\n0:Exit\n1:Compile, 2:Play\n");
		scanf("%d", &r);

		if (r == 0) {
			break;
		} else if (r == 1) {
			CompileScriptTest();
		} else if (r == 2) {
			PlayScriptTest();
		}
	}

	printf("Finish.\n");

	return 0;
}

