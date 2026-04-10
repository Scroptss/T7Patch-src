#include "framework.h"

typedef LONG(NTAPI* NtSuspendProcess)(IN HANDLE ProcessHandle);
void SuspendProcess()
{
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    NtSuspendProcess pfnNtSuspendProcess = (NtSuspendProcess)GetProcAddress(
        GetModuleHandleA("ntdll"), "NtSuspendProcess");

    pfnNtSuspendProcess(processHandle);
    CloseHandle(processHandle);
}

typedef unsigned long long(__fastcall* ZwContinue_t)(PCONTEXT ThreadContext, BOOLEAN RaiseAlert);
ZwContinue_t ZwContinue = reinterpret_cast<ZwContinue_t>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwContinue"));

char* UILocalizeDefaultText = NULL;

struct debug_msg_info
{
    __int64 timeEmitted;
    __int32 numEmitted;
};

std::unordered_map<INT64, CONTEXT> SavedExceptions;
std::unordered_map<DWORD, bool> EncounteredIPs;
std::unordered_map<DWORD, debug_msg_info> PrevErrors;
#define DMSG_COOLDOWN_TICKS 5000
DWORD lck_dumping = NULL;
char error_msg_buff[1024];
const char clientfield_doesnt_exist[] = "Clientfield does not exist";
bool is_unhooking = false;

void __nullsub()
{
    return;
}

void ExceptHook(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord)
{

    if (is_unhooking)
    {
        ContextRecord->Dr7 = 0;
        ContextRecord->Rip = ROP_RETN;
        is_unhooking = false;
        ZwContinue(ContextRecord, false);
    }

    Protection::ExceptHook(ExceptionRecord, ContextRecord);

    if ((INT64)ContextRecord->Rcx == 0xFFEEDDCC44332211)
    {
        return;
    }

    if (REBASE(0x22D2F0D) == (INT64)ExceptionRecord->ExceptionAddress) // killcam anim crash
    {
        ContextRecord->Rax = 0;
        ContextRecord->Rip = REBASE(0x22D389E);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (REBASE(0x464FEF) == (INT64)ExceptionRecord->ExceptionAddress) // CG_ZBarrierAttachWeapon
    {
        ContextRecord->Rax = 0;
        ContextRecord->Rip = REBASE(0x4651A2);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (REBASE(0x15E4B7A) == (INT64)ExceptionRecord->ExceptionAddress) // asmsetanimationrate
    {
        ContextRecord->Rip = REBASE(0x15E4BA3);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x12EE4EC)) // weird orphaned thread crash
    {
        ContextRecord->Rip = REBASE(0x12EE5E8);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x22C965C)) // character index crash
    {
        ContextRecord->Rip = REBASE(0x22C9686);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)LuaCrash1)
    {
        ContextRecord->Rip = LuaCrash1Rip; // LOL FUCK THIS GAME :) :)
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)LuaCrash2)
    {
        if (!UILocalizeDefaultText)
        {
            UILocalizeDefaultText = (char*)malloc(4);
            strcpy_s(UILocalizeDefaultText, 4, "");
        }
        ContextRecord->Rdx = (INT64)UILocalizeDefaultText; // TREYARCH WHY AM I FIXING YOUR GAME
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)LuaCrash3)
    {
        if (!UILocalizeDefaultText)
        {
            UILocalizeDefaultText = (char*)malloc(4);
            strcpy_s(UILocalizeDefaultText, 4, "");
        }
        ContextRecord->Rsi = (INT64)UILocalizeDefaultText; // TREYARCH WHY AM I FIXING YOUR GAME
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x22328F6))
    {
        if (!UILocalizeDefaultText)
        {
            UILocalizeDefaultText = (char*)malloc(4);
            strcpy_s(UILocalizeDefaultText, 4, "");
        }
        ContextRecord->Rcx = (INT64)UILocalizeDefaultText;
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1E9E657))
    {
        ContextRecord->Rip = REBASE(0x1E9E7E3);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A604C4)) // non-existent clientfield
    {
        ContextRecord->Rip = REBASE(0x1A6054B);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x133EC1) || (PVOID)REBASE(0x133EEB) == ExceptionRecord->ExceptionAddress) // non-existent clientfield
    {
        ContextRecord->Rip = REBASE(0x133F12);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x133F31)) // non-existent clientfield
    {
        ContextRecord->Rip = REBASE(0x133F42);
        ZwContinue(ContextRecord, false);
        return;
    }

    // CSC
    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0xC15B80) || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0xC15C50) // non-existent clientfield
        || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0xC18CF5))
    {
        ContextRecord->Rcx = 1;
        ContextRecord->Rdx = (__int64)clientfield_doesnt_exist;
        ContextRecord->R8 = 0;
        ContextRecord->Rip = REBASE(0x12EA450);
        ZwContinue(ContextRecord, false);
        return;
    }

    // GSC
    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5F94B) || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5FA5E) // non-existent clientfield
        || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5FB5E) || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5FBFD)
        || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5FE76) || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A5FF86)
        || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A6003D) || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A602C7)
        || ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x1A604C4))
    {
        ContextRecord->Rcx = 0;
        ContextRecord->Rdx = (__int64)clientfield_doesnt_exist;
        ContextRecord->R8 = 0;
        ContextRecord->Rip = REBASE(0x12EA450);
        ZwContinue(ContextRecord, false);
        return;
    }

    // some random crash we got on ZNS
    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x13591F3))
    {
        ContextRecord->Rip = REBASE(0x13591FA);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)SCRVM_Error)
    {
        ContextRecord->Rip += 4;
        ContextRecord->Rsp -= 0x30;

        bool isFatal = *(bool*)(REBASE(0x5124869) + 35392 * (BYTE)ContextRecord->Rcx);
        INT64 error_msg_ptr = *(INT64*)(REBASE(0x51A3710) + 0x78 * (BYTE)ContextRecord->Rcx);
        const char* error_msg = "unknown";
        if (error_msg_ptr && *(char*)error_msg_ptr)
        {
            error_msg = (char*)error_msg_ptr;
        }

        if (strstr(error_msg, "Invalid opcode"))
        {
            isFatal = true;
        }

        INT64* ip = 0;
        const char* name = 0;
        INT64* fs = (INT64*)SCRVM_FS;
        //report_script_error(ContextRecord->Rcx, error_msg, ip, name);

        if (isFatal) // fatal crash, dump vm
        {

#ifdef USE_NLOG
            ALOG("Script Fatal Exception at : %p\n", fs[(int)ContextRecord->Rcx * 4]);
            ALOG("Error Message: %s", error_msg);
#endif
            FILE* f;
            f = fopen(CRASH_LOG_NAME, "a+"); // a+ (create + append)
            if (!f)
            {
                // we cant log??? fuck.
                ExitProcess(-444);
            }

            fprintf(f, "Script Exception Type: %s", (ContextRecord->Rcx ? "Client" : "Server"));
            fprintf(f, "Script Fatal Exception at : %p\n", fs[(int)ContextRecord->Rcx * 4]);
            fprintf(f, "\t at: %s+%x", name, fs[(int)ContextRecord->Rcx * 4] - (INT64)ip);
            fprintf(f, "Error Message: %s\n", error_msg);

            std::fflush(f);
            std::fclose(f);

            MessageBoxA(NULL, "Unfortunately, a fatal script error has occured in Black Ops III. The error has been recorded in steamapps/common/Black Ops III/crashes.log.", "Fatal Script Error", MB_OK);
            exit(0);
        }

    exitLbl:
        ZwContinue(ContextRecord, false);
        return;
    }

    INT64 faultingModule = 0;
    char module_name[MAX_PATH];

    bool b_fatal = false;
    if ((ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) || (ExceptionRecord->ExceptionFlags & EXCEPTION_NONCONTINUABLE)/* || (strstr(module_name, "blackops3.exe") && STATUS_ILLEGAL_INSTRUCTION == ExceptionRecord->ExceptionCode)*/)
    {
        if (lck_dumping)
        {
            INT64 addy = (INT64)ExceptionRecord->ExceptionAddress;
            SavedExceptions[addy] = *ContextRecord;
        }
        else
        {
            lck_dumping = GetCurrentThreadId();

            FILE* f;
            f = fopen(CRASH_LOG_NAME, "a+"); // a+ (create + append)
            if (!f)
            {
#ifdef USE_NLOG
                ALOG("File not opened");
#endif
                // we cant log??? fuck.
                ExitProcess(-444);
            }

            //ALOG("Crash log %p\n\n", time(NULL));
            fprintf(f, "Crash log %p\n\n", time(NULL));
            INT64 addy = (INT64)ExceptionRecord->ExceptionAddress;
            SavedExceptions[addy] = *ContextRecord;
            while (SavedExceptions.size())
            {
                auto kvp = *SavedExceptions.begin();
                RtlPcToFileHeader((PVOID)kvp.first, (PVOID*)&faultingModule);
                if (faultingModule)
                {
                    GetModuleFileNameA((HMODULE)faultingModule, module_name, MAX_PATH);
                }
                else
                {
                    strcpy_s(module_name, "<Unknown Module>");
                }

                //ALOG("Game: %p\n", (void*)REBASE(0x0));
                fprintf(f, "Game: %p\n", (void*)REBASE(0x0));

                //ALOG("Module: %s\n", module_name);
                fprintf(f, "Module: %s\n", module_name);

                //ALOG("[%p]Exception at (%p) (RIP:%p) (Rsp:%p) (RBP: %p)\n", faultingModule, kvp.first - faultingModule, kvp.second.Rip, kvp.second.Rsp, kvp.second.Rbp);
                fprintf(f, "[%p]Exception at (%p) (RIP:%p) (Rsp: %p)\n", faultingModule, kvp.first - faultingModule, kvp.second.Rip, kvp.second.Rsp, kvp.second.Rbp);

                //ALOG("[%p]Rcx: (%p) Rdx: (%p) R8: (%p) R9: (%p)\n", kvp.first, kvp.second.Rcx, kvp.second.Rdx, kvp.second.R8, kvp.second.R9);
                fprintf(f, "[%p]Rcx: (%p) Rdx: (%p) R8: (%p) R9: (%p)\n", kvp.first, kvp.second.Rcx, kvp.second.Rdx, kvp.second.R8, kvp.second.R9);

                //ALOG("[%p]Rax: (%p) Rbx: (%p) Rsi: (%p) Rdi: (%p)\n", kvp.first, kvp.second.Rax, kvp.second.Rbx, kvp.second.Rsi, kvp.second.Rdi);
                fprintf(f, "[%p]Rax: (%p) Rbx: (%p) Rsi: (%p) Rdi: (%p)\n", kvp.first, kvp.second.Rax, kvp.second.Rbx, kvp.second.Rsi, kvp.second.Rdi);

                //ALOG("[%p]R10: (%p) R11: (%p) R12: (%p) R13: (%p)\n", kvp.first, kvp.second.R10, kvp.second.R11, kvp.second.R12, kvp.second.R13);
                fprintf(f, "[%p]R10: (%p) R11: (%p) R12: (%p) R13: (%p)\n", kvp.first, kvp.second.R10, kvp.second.R11, kvp.second.R12, kvp.second.R13);

                //ALOG("[%p]Rbp: (%p) R14: (%p) R15: (%p) Dr7: (%p)\n", kvp.first, kvp.second.Rbp, kvp.second.R14, kvp.second.R15, kvp.second.Dr7);
                fprintf(f, "[%p]Rbp: (%p) R14: (%p) R15: (%p) Dr7: (%p)\n", kvp.first, kvp.second.Rbp, kvp.second.R14, kvp.second.R15, kvp.second.Dr7);

                //ALOG("[%p]Dr0: (%p) Dr1: (%p) Dr2: (%p) Dr3: (%p)\n", kvp.first, kvp.second.Dr0, kvp.second.Dr1, kvp.second.Dr2, kvp.second.Dr3);
                fprintf(f, "[%p]Dr0: (%p) Dr1: (%p) Dr2: (%p) Dr3: (%p)\n", kvp.first, kvp.second.Dr0, kvp.second.Dr1, kvp.second.Dr2, kvp.second.Dr3);

                for (int i = 0; i < ((STATUS_ILLEGAL_INSTRUCTION == ExceptionRecord->ExceptionCode) ? 0x1200 : 0x400); i += 0x10)
                {
                    //ALOG("[%p] %p %p\n", kvp.second.Rsp + i, *(int64_t*)(kvp.second.Rsp + i), *(int64_t*)(kvp.second.Rsp + i + 8));
                    fprintf(f, "[%p] %p %p\n\n\n", kvp.second.Rsp + i, *(int64_t*)(kvp.second.Rsp + i), *(int64_t*)(kvp.second.Rsp + i + 8));
                }
                SavedExceptions.erase(kvp.first);
            }

            // dump script context
            //dump_script_context(f);

            std::fflush(f);
            std::fclose(f);
            lck_dumping = NULL;
        }

        if (STATUS_ILLEGAL_INSTRUCTION != ExceptionRecord->ExceptionCode)
        {
            b_fatal = true;
        }
    }

    if (b_fatal)
    {
        if (lck_dumping && lck_dumping != GetCurrentThreadId())
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId());
            ContextRecord->Rip = (INT64)SuspendThread;
            ContextRecord->Rcx = (INT64)hThread;
            ZwContinue(ContextRecord, false);
        }
        else
        {
            SuspendProcess();
        }
    }
}

__int64 __fn_ptr_hook = 0;
#define HOOK_SIZE_WINE 0x1B
unsigned __int8 old_data[HOOK_SIZE_WINE];
unsigned __int8 new_data[HOOK_SIZE_WINE] = { 0x48, 0xB8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x48, 0x89, 0xE2, 0x48, 0x8D, 0x8C, 0x24, 0xF0, 0x04, 0x00, 0x00, 0xFF, 0xD0, 0x90, 0x90, 0x90, 0x90 };
void wine_installhook(void* func, __int64 kiuserexceptiondispatcher)
{
    // We have 0x1A (26 bytes) of space to work with. we need to mov rdx, rsp and lea rcx, [rsp+arg_4E8], then mov rax, call and call rax
    auto begin_write = kiuserexceptiondispatcher + 0xB;

    // grab old asm
    memcpy(old_data, (void*)begin_write, HOOK_SIZE_WINE);

    // prep new_data by inserting pointer to func
    __int64 addy = (__int64)func;
    *(__int64*)(new_data + 2) = addy;

    auto OldProtection = 0ul;
    VirtualProtect(reinterpret_cast<void*>(begin_write), HOOK_SIZE_WINE, PAGE_EXECUTE_READWRITE, &OldProtection);
    memcpy((void*)begin_write, new_data, HOOK_SIZE_WINE);
    VirtualProtect(reinterpret_cast<void*>(begin_write), HOOK_SIZE_WINE, OldProtection, &OldProtection);
}

void wine_uninstallhook(__int64 kiuserexceptiondispatcher)
{
    auto begin_write = kiuserexceptiondispatcher + 0xB;

    auto OldProtection = 0ul;
    VirtualProtect(reinterpret_cast<void*>(begin_write), HOOK_SIZE_WINE, PAGE_EXECUTE_READWRITE, &OldProtection);
    memcpy((void*)begin_write, old_data, HOOK_SIZE_WINE);
    VirtualProtect(reinterpret_cast<void*>(begin_write), HOOK_SIZE_WINE, OldProtection, &OldProtection);
}

void old_windows_installhook(void* func, __int64 kiuserexceptiondispatcher)
{
    // TODO
}

void old_windows_uninstallhook(__int64 kiuserexceptiondispatcher)
{
    // TODO
}

void InstallHook(void* func)
{
    auto kiuserexceptiondispatcher = (__int64)GetProcAddress(GetModuleHandleA("ntdll.dll"), "KiUserExceptionDispatcher");

    if (kiuserexceptiondispatcher)
    {
        if (*(__int32*)kiuserexceptiondispatcher == 0x58B48FC)
        {
            auto distance = *(DWORD*)(kiuserexceptiondispatcher + 4);
            auto ptr = (kiuserexceptiondispatcher + 8) + distance;
            __fn_ptr_hook = ptr;

            auto OldProtection = 0ul;
            VirtualProtect(reinterpret_cast<void*>(ptr), 8, PAGE_EXECUTE_READWRITE, &OldProtection);
            *reinterpret_cast<void**>(ptr) = func;
            VirtualProtect(reinterpret_cast<void*>(ptr), 8, OldProtection, &OldProtection);
        }
        else if (*(__int32*)kiuserexceptiondispatcher == 0x248C8B48)
        {
            wine_installhook(func, kiuserexceptiondispatcher);
        }
        else
        {
            old_windows_installhook(func, kiuserexceptiondispatcher);
        }
    }
}

void UninstallHook()
{
    // unload current thread's dr7
    __int64 off = INT3_2_BO3;
    ((void(__fastcall*)())off)(); // force an exception to install the exception handler and setup debug registers

    auto kiuserexceptiondispatcher = (__int64)GetProcAddress(GetModuleHandleA("ntdll.dll"), "KiUserExceptionDispatcher");

    if (kiuserexceptiondispatcher)
    {
        if (*(__int32*)kiuserexceptiondispatcher == 0x58B48FC)
        {
            if (__fn_ptr_hook)
            {
                auto ptr = __fn_ptr_hook;
                auto OldProtection = 0ul;
                VirtualProtect(reinterpret_cast<void*>(ptr), 8, PAGE_EXECUTE_READWRITE, &OldProtection);
                *reinterpret_cast<void**>(ptr) = __nullsub;
                VirtualProtect(reinterpret_cast<void*>(ptr), 8, OldProtection, &OldProtection);
            }
        }
        else if (*(__int32*)kiuserexceptiondispatcher == 0x248C8B48)
        {
            wine_uninstallhook(kiuserexceptiondispatcher);
        }
        else
        {
            old_windows_uninstallhook(kiuserexceptiondispatcher);
        }
    }
}

void RunPatching()
{
	// Set the process priority to above normal to help with performance
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// Initialize MinHook
    MH_Initialize();

	// Set a default player name if none is set
    if (!*Protection::CustomName) { snprintf(Protection::CustomName, 16, "Unknown Soldier"); }

    // Take care of arxan
    PatchChecksumComparisons_Precomputed();

    // Apply VMP Hooks
    hooks::ApplyVMTHooks();

	// Apply MinHook hooks
	hooks::ApplyHooks();

	// Apply Memory patches
	hooks::ApplyMemoryPatches();

	// Apply Exception Handler
    InstallHook(ExceptHook);

	// Install the protection hooks
    Protection::install();
}

EXPORT void zbr_run_gamemode_lui(const char* input)
{

    if (CONST32("serious_anticrash_2023") == GSCUHashing::canon_hash(input))
    {
        RunPatching();
    }

}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        break;
    }
    case DLL_PROCESS_DETACH:
    {
		is_unhooking = true;
        UninstallHook();
        Protection::uninstall();
        hooks::DestroyHooks();
        break;
    }
    }
    return TRUE;
}


