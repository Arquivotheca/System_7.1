#
#	File:		DataAccessMgr.make
#
#	Contains:	Makefile for the Data Access Manager.
#
#	Written by:	Kurt Clark, Chas Spillar, and Tim Nichols
#
#	Copyright:	� 1992 by Apple Computer, Inc., all rights reserved.
#
#	Change History (most recent first):
#
#	   <SM2>	 12/2/92	kc		Added " || Exit 1" to commands with a double dependency.


DataAccessMgrPackObjs			=	"{ObjDir}DataAccessMgr.a.o"						�
									"{ObjDir}DataAccessMgr.c.o"						�
									"{IfObjDir}interface.o"							�


DataAccessRTTObjs				=	"{ObjDir}DataAccessResultsHandlers.a.o"			�
									"{ObjDir}DataAccessResultsHandlers.c.o"			�
									"{IfObjDir}interface.o"							�
									"{Libraries}Runtime.o"


DataAccessCompObjs				=	"{ObjDir}DataAccessCompletion.a.o"

"{LibDir}DataAccessMgr.lib"		�	"{ObjDir}DataAccessPackEntry.a.o"
	Lib	{StdLibOpts} -o "{Targ}" "{ObjDir}DataAccessPackEntry.a.o"


"{RsrcDir}DataAccessMgr.rsrc"	��	{DataAccessMgrPackObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt PACK=13 -ra =resSysHeap -m DataAccessPackEntry {DataAccessMgrPackObjs} || Exit 1


"{RsrcDir}DataAccessMgr.rsrc"	��	{DataAccessRTTObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt proc=-5728 -ra =resSysHeap -m DataAccessHandler {DataAccessRTTObjs} || Exit 1


"{RsrcDir}DataAccessMgr.rsrc"	��	{DataAccessCompObjs}
	Link {StdLOpts} {StdAlign} -o "{Targ}" -rt proc=-5727 -ra =resSysHeap -m DataAccessCR {DataAccessCompObjs} || Exit 1


"{ObjDir}DataAccessMgr.a.o" 	�	"{DataAccessDir}DataAccessMgr.a"				�
									"{ObjDir}StandardEqu.d"							�
									"{AIncludes}DatabaseAccess.a"
	Asm {StdAOpts} -o "{Targ}" "{DataAccessDir}DataAccessMgr.a"


"{ObjDir}DataAccessMgr.c.o" 	�	"{DataAccessDir}DataAccessMgr.c"				�
									"{DataAccessDir}DatabaseAccessPriv.h"			�
									"{CIncludes}DatabaseAccess.h"					�
									"{IntCIncludes}DDEV.h"
	C {StdCOpts} -o "{Targ}" "{DataAccessDir}DataAccessMgr.c"


"{ObjDir}DataAccessResultsHandlers.a.o" �	"{DataAccessDir}DataAccessResultsHandlers.a"	�
											"{ObjDir}StandardEqu.d"							�
											"{AIncludes}DatabaseAccess.a"
	Asm {StdAOpts} -o "{Targ}" "{DataAccessDir}DataAccessResultsHandlers.a"


"{ObjDir}DataAccessResultsHandlers.c.o" �	"{DataAccessDir}DataAccessResultsHandlers.c"	�
											"{DataAccessDir}DatabaseAccessPriv.h"			�
											"{CIncludes}DatabaseAccess.h"
	C {StdCOpts} -o "{Targ}" "{DataAccessDir}DataAccessResultsHandlers.c"


"{ObjDir}DataAccessCompletion.a.o"	�	"{DataAccessDir}DataAccessCompletion.a"		�
										"{ObjDir}StandardEqu.d"						�
										"{AIncludes}DatabaseAccess.a"
	Asm {StdAOpts} -o "{Targ}" "{DataAccessDir}DataAccessCompletion.a"


"{ObjDir}DataAccessPackEntry.a.o" 	�	"{DataAccessDir}DataAccessPackEntry.a"		�
									"{ObjDir}StandardEqu.d"							�
									"{AIncludes}DatabaseAccess.a"					�
									"{IntAIncludes}DispatchHelperPriv.a"
	Asm {StdAOpts} -o "{targ}" "{DataAccessDir}DataAccessPackEntry.a"
