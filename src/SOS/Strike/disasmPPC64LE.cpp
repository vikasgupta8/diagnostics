// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#ifndef _TARGET_POWERPC64_
#define _TARGET_POWERPC64_
#endif

#ifndef TARGET_POWERPC64
#define TARGET_POWERPC64
#endif

#include "strike.h"
#include "util.h"
#include <dbghelp.h>

#include "disasm.h"

#include "corhdr.h"
#include "cor.h"
#include "dacprivate.h"

namespace PPC64LEGCDump
{
#undef TARGET_X86
#undef LIMITED_METHOD_CONTRACT
#define LIMITED_METHOD_DAC_CONTRACT ((void)0)
#define SUPPORTS_DAC ((void)0)
#define LF_GCROOTS
#define LL_INFO1000
#define LOG(x)
#define LOG_PIPTR(pObjRef, gcFlags, hCallBack)
#define DAC_ARG(x)
#include "gcdumpnonx86.cpp"
}

#if defined(_TARGET_WIN64_)
#error This file only supports SOS targeting PPC64LE from a 64-bit debugger
#endif

#if !defined(SOS_TARGET_POWERPC64)
#error This file should be used to support SOS targeting PPC64LE debuggees
#endif


/**********************************************************************\
* Routine Description:                                                 *
*                                                                      *
*    This function is called to determine if a DWORD on the stack is   *
*    a return address.                                                 *
*    It does this by checking several bytes before the DWORD to see if *
*    there is a call instruction.                                      *
*                                                                      *
\**********************************************************************/

void PPC64LEMachine::IsReturnAddress(TADDR retAddr, TADDR* whereCalled) const
{
	*whereCalled = 0;
	//TODO vikas
	return;
}

// Determine if a value is MT/MD/Obj
void HandleValue(TADDR value)
{
    // A MethodTable?
    if (IsMethodTable(value))
    {
        NameForMT_s (value, g_mdName,mdNameLen);
        ExtOut (" (MT: %S)", g_mdName);
        return;
    }
    
    // A Managed Object?
    TADDR dwMTAddr;
    move_xp (dwMTAddr, value);
    if (IsStringObject(value))
    {
        ExtOut (" (\"");
        StringObjectContent (value, TRUE);
        ExtOut ("\")");
        return;
    }
    else if (IsMethodTable(dwMTAddr))
    {
        NameForMT_s (dwMTAddr, g_mdName,mdNameLen);
        ExtOut (" (Object: %S)", g_mdName);
        return;
    }
    
    // A MethodDesc?
    if (IsMethodDesc(value))
    {        
        NameForMD_s (value, g_mdName,mdNameLen);
        ExtOut (" (MD: %S)", g_mdName);
        return;
    }

    // A JitHelper?
    const char* name = HelperFuncName(value);
    if (name) {
        ExtOut (" (JitHelp: %s)", name);
        return;
    }

    // A call to managed code?
    // TODO vikas

    // Random symbol.
    // TODO vikas
}

/**********************************************************************\
* Routine Description:                                                 *
*                                                                      *
*    Unassembly a managed code.  Translating managed object,           *
*    call.                                                             *
*                                                                      *
\**********************************************************************/
void PPC64LEMachine::Unassembly (
    TADDR PCBegin,
    TADDR PCEnd,
    TADDR PCAskedFor,
    TADDR GCStressCodeCopy,
    GCEncodingInfo *pGCEncodingInfo,
    SOSEHInfo *pEHInfo,
    BOOL bSuppressLines,
    BOOL bDisplayOffsets,
    std::function<void(ULONG*, UINT*, BYTE*)> displayIL) const
{
	// TODO vikas
}

BOOL PPC64LEMachine::GetExceptionContext (TADDR stack, TADDR PC, TADDR *cxrAddr, CROSS_PLATFORM_CONTEXT * cxr,
                          TADDR * exrAddr, PEXCEPTION_RECORD exr) const
{
    // TODO vikas
    return FALSE;
}

///
/// Dump PPC64LE GCInfo table
///
void PPC64LEMachine::DumpGCInfo(GCInfoToken gcInfoToken, unsigned methodSize, printfFtn gcPrintf, bool encBytes, bool bPrintHeader) const
{
    if (bPrintHeader)
    {
        ExtOut("Pointer table:\n");
    }

    PPC64LEGCDump::GCDump gcDump(gcInfoToken.Version, encBytes, 5, true);
    gcDump.gcPrintf = gcPrintf;

    gcDump.DumpGCTable(dac_cast<PTR_BYTE>(gcInfoToken.Info), methodSize, 0);
}
