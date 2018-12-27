// See: https://docs.oracle.com/javase/9/docs/api/java/lang/ProcessHandle.Info.html#arguments
// https://stackoverflow.com/questions/4570174/how-to-get-the-process-name-in-c
// https://blogs.msdn.microsoft.com/oldnewthing/20120217-00/?p=8283/
// https://docs.microsoft.com/en-us/windows/desktop/RstMgr/restart-manager-portal
// https://stackoverflow.com/questions/6931972/how-to-find-which-process-has-a-handle-on-a-file-from-the-file-name
// https://superuser.com/questions/998558/windows-10-start-menu-all-apps-missing-icons
// https://www.google.com/search?q=uninstall+shortcut+in+windows+start+menu+disappers&rlz=1C1GCEU_enVN821VN821&oq=uninstall+shortcut+in+windows+start+menu+disappers&aqs=chrome..69i57&sourceid=chrome&ie=UTF-8

#include <windows.h>
#include <RestartManager.h>
#include <stdio.h>

int __cdecl wmain(int argc, WCHAR **argv)
{
	DWORD dwSession;
	WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };
	DWORD dwError = RmStartSession(&dwSession, 0, szSessionKey);
	wprintf(L"RmStartSession returned %d\n", dwError);
	if (dwError == ERROR_SUCCESS) {
		PCWSTR pszFile = argv[1];
		dwError = RmRegisterResources(dwSession, 1, &pszFile,
			0, NULL, 0, NULL);
		wprintf(L"RmRegisterResources(%ls) returned %d\n",
			pszFile, dwError);
		if (dwError == ERROR_SUCCESS) {
			DWORD dwReason;
			UINT i;
			UINT nProcInfoNeeded;
			UINT nProcInfo = 10;
			RM_PROCESS_INFO rgpi[10];
			dwError = RmGetList(dwSession, &nProcInfoNeeded,
				&nProcInfo, rgpi, &dwReason);
			wprintf(L"RmGetList returned %d\n", dwError);
			if (dwError == ERROR_SUCCESS) {
				wprintf(L"RmGetList returned %d infos (%d needed)\n",
					nProcInfo, nProcInfoNeeded);
				for (i = 0; i < nProcInfo; i++) {
					wprintf(L"%d.ApplicationType = %d\n", i,
						rgpi[i].ApplicationType);
					wprintf(L"%d.strAppName = %ls\n", i,
						rgpi[i].strAppName);
					wprintf(L"%d.strServiceShortName = %ls\n", i,
						rgpi[i].strServiceShortName);
					wprintf(L"%d.Process.dwProcessId = %d\n", i,
						rgpi[i].Process.dwProcessId);
					HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
						FALSE, rgpi[i].Process.dwProcessId);
					if (hProcess) {
						FILETIME ftCreate, ftExit, ftKernel, ftUser;
						if (GetProcessTimes(hProcess, &ftCreate, &ftExit,
							&ftKernel, &ftUser) &&
							CompareFileTime(&rgpi[i].Process.ProcessStartTime,
								&ftCreate) == 0) {
							WCHAR sz[MAX_PATH];
							DWORD cch = MAX_PATH;
							if (QueryFullProcessImageNameW(hProcess, 0, sz, &cch) &&
								cch <= MAX_PATH) {
								wprintf(L"  = %ls\n", sz);
							}
						}
						CloseHandle(hProcess);
					}
				}
			}
		}
		RmEndSession(dwSession);
	}
	return 0;
}
