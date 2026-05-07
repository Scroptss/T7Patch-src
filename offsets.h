#pragma once
#include "framework.h"

#define OFFSET(x) ((INT64)GetModuleHandleA(NULL) + (INT64)x)
#define REBASE(x) OFFSET(x)
#define _DOFFSET(x) x
#define EXPORT extern "C" __declspec(dllexport)

#define SD(x) const char* s ## x;
#define SS(x) s##x = #x;
#define STR(x) s##x

#define OD(x) __int64 llp ## x;
#define OS(x) llp ## x = x;
#define OG(x) llp ## x

typedef bool(__fastcall* tReadP2PPacket)(uintptr_t thisptr, void* pub_dest, unsigned int cub_dest, unsigned int* cub_msg_size, unsigned __int64* steam_id_remote, int n_channel);
typedef char* (__fastcall* tGetPersonaName) (DWORD_PTR _this);
typedef __int64(__fastcall* tLiveSteam_FilterPersonaName)(char* buffer, int size, char asciionly);
extern tReadP2PPacket oReadP2PPacket;
extern tGetPersonaName oGetPersonaName;
extern tLiveSteam_FilterPersonaName oLiveSteam_FilterPersonaName;

#define SCRVM_Error REBASE(0x12EA4E0)
#define OFF_SCRVM_RuntimeError REBASE(0x12EA450)
#define SCRVM_FS REBASE(0x513DD30)
#define LuaCrash1 REBASE(0x1C9F121)
#define LuaCrash1Rip REBASE(0x1C9F2CE)
#define LuaCrash2 REBASE(0x221CEC3)
#define LuaCrash3 REBASE(0x221C726)
#define SCR_VmErrorString REBASE(0x52660F0)
#define SCR_VMContext REBASE(0x5124850)
#define INT3_2_BO3 REBASE(0x1059)
#define BO3_HWND REBASE(0x17DF74A0)
#define ROP_RETN REBASE(0x1048)
#define s_playerData_ptr REBASE(0x3390190)
#define PTR_LobbyVM REBASE(0x157588D0)
#define PTR_CL_DispatchConnectionless OFFSET(0x134CDCD)
#define PTR_Name1 OFFSET(0x113A4970)
#define PTR_Name2 OFFSET(0x113A48F0)
#define PTR_UpdatePreloadIdleFN REBASE(0x32A75A8)
#define PTR_LobbyMsgRW_PackageInt REBASE(0x1EEA3E0)
#define PTR_LobbyMsgRW_PackageUChar REBASE(0x1EEA450)
#define PTR_LobbyMsgRW_PackageString REBASE(0x1EEA420)
#define PTR_LobbyMsgRW_PackageXuid REBASE(0x1EEA4D0)
#define PTR_LobbyMsgRW_PackageBool REBASE(0x1EEA2E0)
#define PTR_LobbyMsgRW_PackageUInt REBASE(0x1EEA490)
#define PTR_LobbyMsgRW_PackageShort REBASE(0x1EEA400)
#define PTR_LobbyMsgRW_PackageUInt64 REBASE(0x1EEA470)
#define PTR_LobbyMsgRW_PackageArrayStart REBASE(0x1EEA260)
#define PTR_LobbyMsgRW_PackageElement REBASE(0x1EEA320)
#define PTR_LobbyMsgRW_PackageGlob REBASE(0x1EEA3B0)
#define PTR_dwInstantHandleLobbyMessage REBASE(0x143A7A0)
#define PTR_ProbeLobbyInfo REBASE(0x1EDA510)
#define PTR_NET_OutOfBandPrint REBASE(0x211B310)
#define PTR_dwCommonAddrToNetadr REBASE(0x143C380)
#define PTR_dwRegisterSecIDAndKey REBASE(0x143E140)
#define PTR_LobbyMsgTansport_SendOutOfBand REBASE(0x1EEC360)
#define PTR_LobbyMsgRW_PrepWriteMsg REBASE(0x1EEA560)
#define PTR_LobbyMsgRW_PackageUShort REBASE(0x1EEA4B0)
#define PTR_LobbySession_GetSession REBASE(0x1EC1650)
#define PTR_LobbySession_GetClientByClientNum REBASE(0x1EF3FB0)
#define PTR_LobbySession_GetControllingLobbySession REBASE(0x1EBFDA0)
#define PTR_MsgMutableClientInfo_Package REBASE(0x1EC8400)
#define PTR_MSG_Init REBASE(0x20FCB80)
#define PTR_MSG_WriteString REBASE(0x20FFE20)
#define PTR_MSG_WriteShort REBASE(0x211A2D0)
#define PTR_MSG_WriteByte REBASE(0x20FF3C0)
#define PTR_MSG_WriteData REBASE(0x20FF3E0)
#define PTR_Com_ControllerIndex_GetLocalClientNum REBASE(0x20E3700)
#define PTR_Com_LocalClient_GetNetworkID REBASE(0x20E3890)
#define PTR_NET_OutOfBandData REBASE(0x211B200)
#define PTR_LobbyMsgTransport_SendToAdr REBASE(0x1EEC740)
#define PTR_MSG_ReadData REBASE(0x20FD0B0)
#define PTR_LobbyMsgRW_PrepReadData REBASE(0x1EEA150)
#define PTR_MSG_InfoResponse REBASE(0x1ED5B40)
#define PTR_LobbyMsgRW_PackageChar REBASE(0x1EEA300)
#define PTR_LobbyMsgRW_PackageFloat REBASE(0x1EEA390)
#define PTR_dwInstantSendMessage REBASE(0x143A830)
#define PTR_dwInstantDispatchMessage REBASE(0x143A620)
#define PTR_I_stricmp REBASE(0x227CB20)
#define PTR_Cbuf_AddText REBASE(0x20DFF50)
#define PTR_LobbySession_GetClientNetAdrByIndex REBASE(0x1EBFCC0)
#define PTR_LobbyJoin_Reserve REBASE(0x1EDBA00)
#define PTR_CL_GetConfigString REBASE(0x1321130)
#define PTR_LiveFriends_IsFriendByXUID REBASE(0x1DECFD0)
#define STEAMAPI_STEAMUSER REBASE(0x10B3DC20)
#define STEAMAPI_INTERFACE REBASE(0x10B3DC40)
#define STEAMAPI_FRIENDS REBASE(0x10B3DC20)
#define STEAMAPI_MATCHMAKING REBASE(0x10B3DC30)
#define STEAMAPI_NETWORKING REBASE(0x10B3DC50)
#define DW_LOBBY REBASE(0x9B35878)
#define pUserData OFFSET(0x14F344B0)
#define pNameBuffer OFFSET(0x15E056C8)
#define OFF_s_runningUILevel REBASE(0x1686E99E)
#define PTR_MSG_ReadByte REBASE(0x20FD050)
#define PTR_lobbymsgprints REBASE(0x156CE8D0)
#define PTR_saveLobbyMsgExceptAddy REBASE(0x1EEBF74)
#define PTR_Cmp_TokenizeStringInternal REBASE(0x20E2A3E + 0x3)
#define Sys_Checksum(msg, size) ((unsigned __int16(__fastcall*)(const unsigned char*, __int32))REBASE(0x211F430))(msg, size)
#define BigShort(val) ((__int16(__fastcall*)(__int16))REBASE(0x227B4A0))(val)

const static auto flsomeWeirdCharacterIndex = reinterpret_cast<float(__fastcall*)(__int64 a1, int a2, int a3)>REBASE(0x22C9650);
const static auto CL_ConnectionlessCMD = reinterpret_cast<bool(__fastcall*)(int clientNum, netadr_t * from, msg_t * msg)>OFFSET(0x134CD70);
const static auto Sys_GetTLS = reinterpret_cast<__int64(__fastcall*)()>OFFSET(0x212B3D0);
const static auto CL_StoreConfigString = reinterpret_cast<const char* (*)(int configStringIndex, const char* string)>OFFSET(0x13667E0);
const static auto CL_GetConfigString = reinterpret_cast<const char* (__fastcall*)(int configStringIndex)>OFFSET(0x1321130);
const static auto LobbyMsgRW_PrintMessage = reinterpret_cast<void(__fastcall*)()>OFFSET(0x1EEA940);
const static auto LobbyMsgRW_PrintDebugMessage = reinterpret_cast<__int32(__fastcall*)()>OFFSET(0x1EEA680);
const static auto ExecLuaCMD = reinterpret_cast<void(__fastcall*)()>OFFSET(0x1EF8A60);
const static auto SEH_ReplaceDirectiveInStringWithBinding = reinterpret_cast<const char* (__fastcall*)(int localClientNum, const char* translatedString, char* finalString)>OFFSET(0x221CE90);
const static auto qmemcpy = reinterpret_cast<__int64(__fastcall*)(char* dest, char* source, __int32 size)>OFFSET(0x2BC4EB0);
const static auto UI_DoModelStringReplacement = reinterpret_cast<bool(__fastcall*)(__int32 controllerIndex, char* element, const char* source, char* dest, unsigned int destSize)>OFFSET(0x1F27400);
const static auto LobbyTypes_GetMsgTypeName = reinterpret_cast<const char* (__fastcall*)(__int32 index)>OFFSET(0x1EDFA60);
const static auto LobbyMsgRW_PrepWriteMsg = reinterpret_cast<bool(__fastcall*)(__int64 lobbyMsg, __int64 data, int length, int msgType)>OFFSET(0x1EEA560);
const static auto LobbyMsgRW_PrepReadMsg = reinterpret_cast<bool(__fastcall*)(__int64 lm)>OFFSET(0x1EEB8D0);
const static auto Sys_VerifyPacketChecksum = reinterpret_cast<__int64(__fastcall*)(const char* payload, __int32 payloadLen)>OFFSET(0x211F5A0);
const static auto Sys_ChecksumCopy = reinterpret_cast<unsigned __int16(__fastcall*)(const char* desta, const char* srca, __int32 length)>OFFSET(0x211F500);
const static auto COD_GetWindowTitle = reinterpret_cast<const char* (__fastcall*)()>OFFSET(0x20E31C0);
const static auto LiveSteam_InitServer = reinterpret_cast<void(__fastcall*)()>OFFSET(0x1EA9C80);
const static auto COD_GetBuildTitle = reinterpret_cast<void* (__fastcall*)()>OFFSET(0x20E31C0);
const static auto Live_SystemInfo = reinterpret_cast<bool(__fastcall*)(int controllerIndex, int infoType, char* outputString, const int outputLen)>OFFSET(0x1E016C0);
const static auto Dvar_SetFromStringByName = reinterpret_cast<std::uintptr_t(__fastcall*)(const char* dvarname, const char* value, bool createifmissing)>OFFSET(0x226B0A0);
const static auto dwInstantDispatchMessage = reinterpret_cast<__int64(__fastcall*)(__int64 SenderId, unsigned int controllerIndex, __int64 msg, unsigned int messageSize)>OFFSET(0x143A620);
const static auto LivePresence_Serialize = reinterpret_cast<__int64(__fastcall*)(__int64 a1, __int64 a2)>OFFSET(0x1E85450);
const static auto MSG_BeginReading = reinterpret_cast<void(*)(msg_t*)>OFFSET(0x20FC900);
const static auto MSG_InfoResponse = reinterpret_cast<bool(*)(void*, LobbyMsg*)>OFFSET(0x1ED5B40);
const static auto MSG_InitReadOnly = reinterpret_cast<void(*)(msg_t*, const char*, int)>OFFSET(0x20FCC10);
const static auto MSG_ReadByte = reinterpret_cast<std::uint8_t(*)(msg_t*)>OFFSET(0x20FD050);
const static auto MSG_ReadData = reinterpret_cast<void(*)(msg_t*, void*, int)>OFFSET(0x20FD0B0);
const static auto Live_IsUserSignedInToDemonware = reinterpret_cast<bool(*)(const ControllerIndex_t)>REBASE(0x1E013D0);
const static auto LobbyMsg_HandleIM = reinterpret_cast<std::uintptr_t(__fastcall*)(unsigned int targetController, __int64 senderXuid, void* buff, int len)>OFFSET(0x1EEA130);
const static auto LobbyMsgRW_PackageInt = reinterpret_cast<bool(__fastcall*)(LobbyMsg * lobbyMsg, const char* key, __int32* val)>PTR_LobbyMsgRW_PackageInt;
const static auto LobbyMsgRW_PackageUInt = reinterpret_cast<bool(__fastcall*)(LobbyMsg * lobbyMsg, const char* key, unsigned __int32* val)>PTR_LobbyMsgRW_PackageUInt;
const static auto LobbyMsgRW_PackageUChar = reinterpret_cast<bool(__fastcall*)(LobbyMsg * lobbyMsg, const char* key, unsigned char* val)>PTR_LobbyMsgRW_PackageUChar;
const static auto BG_Cache_GetScriptMenuNameForIndex = reinterpret_cast<const char* (__fastcall*)(__int32, __int32)>(REBASE(0xA7DE0));
const static auto BG_Cache_GetEventStringNameForIndex = reinterpret_cast<const char* (__fastcall*)(__int32, __int32)>(REBASE(0xA78A0));
const static auto LobbyHost_IsHost = reinterpret_cast<bool(__fastcall*)(__int32)>(REBASE(0x1ECCDC0));
const static auto Scr_AddString = reinterpret_cast<void(__fastcall*)(__int32, const char*)>(REBASE(0x12E9A50));
const static auto Scr_ExecEntThread = reinterpret_cast<__int32(__fastcall*)(void*, void*, __int32)>(REBASE(0x1B20280));
const static auto Scr_FreeThread = reinterpret_cast<void(__fastcall*)(__int32, __int32)>(REBASE(0x12EAB70));
const static auto SV_Cmd_ConcatArgs = reinterpret_cast<const char* (__fastcall*)(__int32)>(REBASE(0x1972BA0));
const static auto SV_Cmd_ArgvBuffer = reinterpret_cast<void(__fastcall*)(__int32, char*, __int32)>(REBASE(0x20E2FD0));
const static auto UI_Model_AllocateNode = reinterpret_cast<__int32(__fastcall*)(__int32 ancestorIndex, const char* path, bool persistent)>REBASE(0x200CD00);
const static auto UI_Model_CreateModelFromPath = reinterpret_cast<__int32(__fastcall*)(__int64 parentNodeIndex, const char* path)>REBASE(0x200CFC0);
const static auto UI_Model_GetModelFromPath = reinterpret_cast<__int32(__fastcall*)(__int64 parentNodeIndex, const char* path)>REBASE(0x200D5B0);
const static auto UI_Model_GetModelFromPath_0 = reinterpret_cast<__int32(__fastcall*)(__int64 parentNodeIndex, const char* path)>REBASE(0x200CF00);
const static auto BG_Cache_GetLocStringNameForIndex = reinterpret_cast<const char* (__fastcall*)(unsigned __int32 inst, unsigned __int32 index)>REBASE(0xA7AB0);
const static auto BG_Cache_GetLUIMenuForIndex = reinterpret_cast<const char* (__fastcall*)(unsigned __int32 inst, unsigned __int32 index)>REBASE(0xA7A00);
const static auto BG_Cache_GetLUIMenuDataForIndex = reinterpret_cast<const char* (__fastcall*)(unsigned __int32 inst, unsigned __int32 index)>REBASE(0xA7990);
const static auto CMD_MenuResponse_f = reinterpret_cast<void(__fastcall*)(char* ent)>REBASE(0x195F100);
const static auto CMD_MenuResponseCached_f = reinterpret_cast<void(__fastcall*)(char* ent)>REBASE(0x195EFA0);
const static auto Memset = reinterpret_cast<int(__fastcall*)(char* a1, __int64 a2, unsigned __int64 a3)>REBASE(0x2BC53B0);
const static auto Live_Base_UserGetName = reinterpret_cast<UINT8(__fastcall*)(UINT8 * a1, int a2, char a3)>REBASE(0x1EA4960);
const static auto Live_UserGetName = reinterpret_cast<bool(__fastcall*)(ControllerIndex_t controllerIndex, char* buf, int bufSize)>REBASE(0x1EBB200);
const static auto Mods_SubscribeUGC = reinterpret_cast<bool(__fastcall*)(__int64 a1)>REBASE(0x20CB0C0);
const static auto UI_BrowserOpen = reinterpret_cast<char(__fastcall*)(__int64 a1)>REBASE(0x1EA4E30);
const static auto LiveInvites_JoinMessageAction = reinterpret_cast<void(__fastcall*)(unsigned int localControllerIndex, DWORD* from, __int64 recepientXUID)>REBASE(0x1E72040);
const static auto LiveInvites_AcceptInvite = reinterpret_cast<void(__fastcall*)(unsigned int controllerIndex, DWORD* message, __int64 recepientXUID)>REBASE(0x1E19B30);
const static auto LiveInvites_SendJoinInfo = reinterpret_cast<__int64(__fastcall*)(unsigned int controllerIndex, __int64 recepientXUID, __int64 a3)>REBASE(0x1E724A0);