//////////////////////////////////////////////////////////////////////
//																	//
//		TestPlay.cpp	DLLのテスト。コンパイルを行う				//
//																	//
//				Copyright Takahiro Ito, 2003/02/19 - 2003/02/20		//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "Compile.h"
#include "Player.h"

int ReadLineTest(const char *filename) {
	char sLine[4096];
	CCompile cl;

	cl.InitScriptFormat(SCRIPT_SEMICORON);
	cl.ReadSourceFile(filename);

	memset(sLine, 0, sizeof(sLine));
	while (cl.ReadLine(sLine)) {
		printf("%s\n", sLine);
		memset(sLine, 0, sizeof(sLine));
	}

	printf("\n\nTest2.txt\n");
	cl.InitScriptFormat(SCRIPT_CRLF);
	cl.ReadSourceFile("Test2.txt");

	memset(sLine, 0, sizeof(sLine));
	while (cl.ReadLine(sLine)) {
		printf("%s\n", sLine);
		memset(sLine, 0, sizeof(sLine));
	}

	for (;;) {
		sLine[0] = getchar();
		if (sLine[0] = 0x0a) { break; }
	}

	return 1;
}

void CompileScriptTest() {
	int r;
	CCompile cl;
	CScriptPlayer pl;
	char SourceFile[1024], OutputFile[1024];
	char DebugFile[1024],  DumpFile[1024];
	char sResult[1024];

	printf("Source File:");
	scanf("%s", SourceFile);

	strcpy(OutputFile, SourceFile);
	strcpy(&OutputFile[strlen(OutputFile) - 3], "bs\0");

	strcpy(DebugFile, SourceFile);
	strcpy(&DebugFile[strlen(DebugFile) - 3], "iss\0");
	strcpy(DumpFile, SourceFile);
	strcpy(&DumpFile[strlen(DumpFile) - 3], "dmp\0");

	printf("Output File:%s\n", OutputFile);
	printf("Debug File :%s\n", DebugFile);
	printf("Dump File  :%s\n", DumpFile);

	cl.InitScriptFormat(SCRIPT_SEMICORON);
	cl.SetDebugOption(1);
	if (cl.ReadSourceFile(SourceFile) == 0) { return; }
	r = 0;
	cl.CompileText(NULL, r);
	cl.DebugWriteSource(DebugFile);

	memset(sResult, 0, sizeof(sResult));
	r = cl.WriteScriptFile(OutputFile, sResult);
	printf("%s\n%d Bytes written.\n\n", sResult, r);

	/* Now Dumping... */
	printf("Dumping...\n");
	if (pl.ReadBinary(OutputFile) == 0) {
		printf("Fatal Error. Cannot Open File: %s\n\n", OutputFile);
		return;
	}
	r = pl.DumpBinaryScript(DumpFile);
	printf("%d Bytes written.\n\n", r);
}
