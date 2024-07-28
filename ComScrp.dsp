# Microsoft Developer Studio Project File - Name="ComScrp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ComScrp - Win32 Test
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "ComScrp.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "ComScrp.mak" CFG="ComScrp - Win32 Test"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "ComScrp - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "ComScrp - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "ComScrp - Win32 Test" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ComScrp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMSCRP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMSCRP_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMSCRP_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMSCRP_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ComScrp___Win32_Test"
# PROP BASE Intermediate_Dir "ComScrp___Win32_Test"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Test"
# PROP Intermediate_Dir "Test"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "COMSCRP_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PLAY_TEST" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ComScrp - Win32 Release"
# Name "ComScrp - Win32 Debug"
# Name "ComScrp - Win32 Test"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Compile.cpp
# End Source File
# Begin Source File

SOURCE=.\ComScrp.cpp

!IF  "$(CFG)" == "ComScrp - Win32 Release"

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Expression.cpp
# End Source File
# Begin Source File

SOURCE=.\IfElse.cpp
# End Source File
# Begin Source File

SOURCE=.\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\Loop.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\ReadText.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Stream.cpp
# End Source File
# Begin Source File

SOURCE=.\Syntax.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\Compile.h
# End Source File
# Begin Source File

SOURCE=.\ComScrp.h
# End Source File
# Begin Source File

SOURCE=.\Def.h
# End Source File
# Begin Source File

SOURCE=.\Errors.h
# End Source File
# Begin Source File

SOURCE=.\Expression.h
# End Source File
# Begin Source File

SOURCE=.\IfElse.h
# End Source File
# Begin Source File

SOURCE=.\Lexer.h
# End Source File
# Begin Source File

SOURCE=.\Loop.h
# End Source File
# Begin Source File

SOURCE=.\Macro.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Stream.h
# End Source File
# Begin Source File

SOURCE=.\Syntax.h
# End Source File
# Begin Source File

SOURCE=.\Types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "TestCode"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\TestCpl.cpp

!IF  "$(CFG)" == "ComScrp - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestMain.cpp

!IF  "$(CFG)" == "ComScrp - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TestPlay.cpp

!IF  "$(CFG)" == "ComScrp - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

!ENDIF 

# End Source File
# End Group
# Begin Group "Doc"

# PROP Default_Filter "txt;doc"
# Begin Source File

SOURCE=.\Algorithm.txt
# End Source File
# Begin Source File

SOURCE=.\Command.txt
# End Source File
# Begin Source File

SOURCE=.\Doc\Script.txt
# End Source File
# End Group
# Begin Group "Script Source"

# PROP Default_Filter "tsf"
# Begin Source File

SOURCE=.\Sample\Const.tsf
# End Source File
# Begin Source File

SOURCE=.\Sample\Func.tsf
# End Source File
# Begin Source File

SOURCE=.\Sample\If.tsf
# End Source File
# Begin Source File

SOURCE=.\Sample\Label.tsf
# End Source File
# Begin Source File

SOURCE=.\Sample\Var.tsf
# End Source File
# End Group
# Begin Source File

SOURCE=.\ComScrp.def

!IF  "$(CFG)" == "ComScrp - Win32 Release"

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Debug"

!ELSEIF  "$(CFG)" == "ComScrp - Win32 Test"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GenCode.cmd
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
