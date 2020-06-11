
#ifndef LYNUC_CI_COMM
#define LYNUC_CI_COMM


#ifdef WIN32
#define EXPORT_API      __declspec(dllexport)
#else
#define EXPORT_API
#endif

extern "C"
{
int EXPORT_API GetCICommVersion();
int EXPORT_API CreateCIKernel(int nFlag=0);
int EXPORT_API DestroyCIKernel(int* nAdr, int nFlag=0);
int EXPORT_API GetCISize();
int EXPORT_API GetMacroInfo(int nAdr, int nMacro, unsigned int &nOffset, int &nStart, int &nEnd, char &cType, int* pnUnitExch = 0);
int EXPORT_API GetMacroVal(int nAdr, int nMacro, double &dValue);
int EXPORT_API SetMacroVal(int nAdr, int nMacro, double dValue,bool bRDOnly = false);
int EXPORT_API GetAtMacroVal(int nAdr, int nMacro, double &dValue);
int EXPORT_API SetAtMacroVal(int nAdr, int nMacro, double dValue,bool bRDOnly = false);
int EXPORT_API GetMInchExch(int nAdr, int nMacro, bool &bInch);
int EXPORT_API SetMInchExch(int nAdr, int nMacro, bool bInch);
}

#endif
