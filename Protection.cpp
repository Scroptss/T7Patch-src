#include "Protection.h"

bool has_set_window_text = false;
bool Protection::IsFriendsOnly = false;
bool Protection::IsInjectorlessInstall = true;
__int64 Protection::PrivatePassword[3] = { 0, 0 };
char Protection::CustomName[16] = { 0 };
std::unordered_map<BYTE, std::function<void(__int32* lobbyMsgTypePtr, __int64 lobbyMsg)>> Protection::handle_packet_callbacks;
__int64 Protection::Old_lobbymsgprints = NULL;
__int64 Protection::CachedRetnAddy = NULL;
__int64 Protection::CachedXUID = NULL;
tZwContinue Protection::ZwContinue = NULL;
tI_stricmp Protection::I_stricmp = NULL;
char* Protection::UILocalizeDefaultText = NULL;
tLobbyMsgRW_PackageInt Protection::LobbyMsgRW_PackageInt = NULL;
tLobbyMsgRW_PackageUChar Protection::LobbyMsgRW_PackageUChar = NULL;
tLobbyMsgRW_PackageString Protection::LobbyMsgRW_PackageString = NULL;
tLobbyMsgRW_PackageXuid Protection::LobbyMsgRW_PackageXuid = NULL;
tLobbyMsgRW_PackageBool Protection::LobbyMsgRW_PackageBool = NULL;
tLobbyMsgRW_PackageUInt Protection::LobbyMsgRW_PackageUInt = NULL;
tLobbyMsgRW_PackageShort Protection::LobbyMsgRW_PackageShort = NULL;
tLobbyMsgRW_PackageUInt64 Protection::LobbyMsgRW_PackageUInt64 = NULL;
tLobbyMsgRW_PackageArrayStart Protection::LobbyMsgRW_PackageArrayStart = NULL;
tLobbyMsgRW_PackageElement Protection::LobbyMsgRW_PackageElement = NULL;
tLobbyMsgRW_PackageGlob Protection::LobbyMsgRW_PackageGlob = NULL;
tMsgMutableClientInfo_Package Protection::MsgMutableClientInfo_Package = NULL;
tProbeLobbyInfo Protection::ProbeLobbyInfo = NULL;
tdwInstantHandleLobbyMessage Protection::dwInstantHandleLobbyMessage = NULL;
tNET_OutOfBandPrint Protection::NET_OutOfBandPrint = NULL;
tdwCommonAddrToNetadr Protection::dwCommonAddrToNetadr = NULL;
tdwRegisterSecIDAndKey Protection::dwRegisterSecIDAndKey = NULL;
tLobbyMsgRW_PrepWriteMsg Protection::LobbyMsgRW_PrepWriteMsg = NULL;
tLobbyMsgRW_PackageUShort Protection::LobbyMsgRW_PackageUShort = NULL;
tLobbyMsgRW_PackageFloat Protection::LobbyMsgRW_PackageFloat = NULL;
tMSG_Init Protection::MSG_Init = NULL;
tMSG_WriteString Protection::MSG_WriteString = NULL;
tMSG_WriteShort Protection::MSG_WriteShort = NULL;
tMSG_WriteByte Protection::MSG_WriteByte = NULL;
tMSG_WriteData Protection::MSG_WriteData = NULL;
tCom_ControllerIndex_GetLocalClientNum Protection::Com_ControllerIndex_GetLocalClientNum = NULL;
tCom_LocalClient_GetNetworkID Protection::Com_LocalClient_GetNetworkID = NULL;
tNET_OutOfBandData Protection::NET_OutOfBandData = NULL;
tLobbyMsgTransport_SendToAdr Protection::LobbyMsgTransport_SendToAdr = NULL;
tMSG_ReadData Protection::MSG_ReadData = NULL;
tLobbyMsgRW_PrepReadData Protection::LobbyMsgRW_PrepReadData = NULL;
tMSG_InfoResponse Protection::MSG_InfoResponse = NULL;
tLobbyMsgRW_PackageChar Protection::LobbyMsgRW_PackageChar = NULL;
tdwInstantSendMessage Protection::dwInstantSendMessage = NULL;
tLobbySession_GetControllingLobbySession Protection::LobbySession_GetControllingLobbySession = NULL;
tLobbySession_GetSession Protection::LobbySession_GetSession = NULL;
tLobbySession_GetClientByClientNum Protection::LobbySession_GetClientByClientNum = NULL;
tLobbySession_GetClientNetAdrByIndex Protection::LobbySession_GetClientNetAdrByIndex = NULL;
tLobbyJoin_Reserve Protection::LobbyJoin_Reserve = NULL;
tCL_GetConfigString Protection::CL_GetConfigString = NULL;
tCbuf_AddText Protection::Cbuf_AddText = NULL;

const char* sMstart;
const char* sMdata;
const char* sMhead;
const char* sMstate;
const char* sConnectResponse;
const char* sRcon;
const char* sRequestStats;
const char* sRequestStats2;
const char* sLoading;
const char* sRA;
const char* sV;
const char* sVT;
const char* sRelay;
const char* sLMGI;

SD(targetlobby)
SD(sourcelobby)
SD(jointype)
SD(probedxuid)
SD(playlistid)
SD(playlistver)
SD(ffotdver)
SD(networkmode)
SD(netchecksum)
SD(protocol)
SD(changelist)
SD(pingband)
SD(dlcbits)
SD(joinnonce)
SD(chunk)
SD(isStarterPack)
SD(password)
SD(membercount)
SD(members)
SD(xuid)
SD(lobbyid)
SD(skillrating)
SD(skillvariance)
SD(pprobation)
SD(aprobation)
SD(statenum)
SD(mainmode)
SD(partyprivacy)
SD(lobbytype)
SD(lobbymode)
SD(sessionstatus)
SD(uiscreen)
SD(leaderactivity)
SD(key)
SD(leader)
SD(platformsession)
SD(maxclients)
SD(isadvertised)
SD(clientcount)
SD(sessionid)
SD(sessioninfo)
SD(ugcName)
SD(ugcVersion)
SD(clientlist)
SD(clientNum)
SD(gamertag)
SD(isGuest)
SD(connectbit)
SD(score)
SD(address)
SD(qport)
SD(band)
SD(netsrc)
SD(joinorder)
SD(dlcBits)
SD(migratebits)
SD(lasthosttimems)
SD(nomineelist)
SD(serverstatus)
SD(launchnonce)
SD(matchhashlow)
SD(matchhashhigh)
SD(status)
SD(statusvalue)
SD(gamemode)
SD(gametype)
SD(map)
SD(cpqueuedlevel)
SD(movieskipped)
SD(team)
SD(mapvote)
SD(readyup)
SD(plistid)
SD(plistcurr)
SD(plistentries)
SD(plistnext)
SD(plistprev)
SD(plistprevcount)
SD(votecount)
SD(votes)
SD(itemtype)
SD(item)
SD(itemgroup)
SD(attachment)
SD(votetype)
SD(votexuid)
SD(pregamepos)
SD(pregamestate)
SD(clvotecount)
SD(character)
SD(loadout)
SD(settingssize)
SD(compstate)
SD(heartbeatnum)
SD(nonce)
SD(nattype)
SD(lobbies)
SD(valid)
SD(hostxuid)
SD(hostname)
SD(secid)
SD(seckey)
SD(addrbuff)

EXPORT void SetFriendsOnly(bool isFriendsOnly)
{
    Protection::IsFriendsOnly = isFriendsOnly;
}

EXPORT void SetPlayerName(const char* name)
{
    if (strlen(name) > 15)
    {
        return;
    }
    memset(Protection::CustomName, 0, sizeof(Protection::CustomName));
    strcpy_s(Protection::CustomName, name);
    Protection::CustomName[sizeof(Protection::CustomName) - 1] = 0;

    strncpy_s((char*)(pUserData + 0x8), 16, Protection::CustomName, sizeof(Protection::CustomName));
    strncpy_s((char*)(pNameBuffer), 16, Protection::CustomName, sizeof(Protection::CustomName));
}

EXPORT void SetNetworkPassword(const char* pass)
{
    if (strlen(pass) == 0 || !(*pass))
    {
        Protection::SetNetworkPassword(0);
    }
    else
    {
        Protection::SetNetworkPassword(GSCUHashing::canon_hash64(pass));
    }
}

IHOOK_HEADER(IsProcessorFeaturePresent, BOOL, (DWORD processorFeature))
{
    if (processorFeature == 0x17)
    {
        // force a crash
        *(__int64*)0x17 = processorFeature;
        *(__int64*)processorFeature = 0x17;
    }
    return IHOOK_ORIGINAL(IsProcessorFeaturePresent, (processorFeature));
}

bool Protection::ReadP2PPacket(uintptr_t thisptr, void* pub_dest, unsigned int cub_dest, unsigned int* cub_msg_size, unsigned __int64* steam_id_remote, int n_channel) {

	bool result = ((bool(__fastcall*)(uintptr_t, void*, unsigned int, unsigned int*, uint64_t*, int))GetOriginalSteamPtr(STEAMAPI_NETWORKING, STEAMAPI_NETWORKING_READP2PPACKET))(thisptr, pub_dest, cub_dest, cub_msg_size, steam_id_remote, n_channel);

    if (result) {

        char* data = reinterpret_cast<char*>(pub_dest);

        auto game_im = data[5];

        if (game_im == 104) // info request
        {
            if (Protection::IsFriendsOnly && !Protection::IsFriendByXUIDUncached(*steam_id_remote)) {
                return false;
            }
        }

        if (game_im == 102) // join request
        {
            return false;
        }

        if (game_im == 101 || game_im == 109) // cbuf
        {
            return false;
        }
    }

    return result;
}

const char* Protection::GetUsernamePtr(INT64 a)
{
    return CustomName;
}

unsigned __int64 next_update_friendslist_time = 0;
std::unordered_set<__int64> friends_set;
bool Protection::IsFriendByXUIDUncached(__int64 xuid) // ok I say its "uncached" but thats because I don't want this running a billion times per second and I think it might hitch with huge friends lists.
{
    if (GetTickCount64() < next_update_friendslist_time || !(*(char*)OFF_s_runningUILevel)) // we will just not update the friends list in game because I really think this will hitch. STEAMAPI SUCKS
    {
        return friends_set.find(xuid) != friends_set.end();
    }

    auto isteamfriends = *(__int64*)STEAMAPI_FRIENDS;
    auto fn_GetFriendCount = *(__int64*)(*(__int64*)isteamfriends + 0x18);
    int num_friends = ((int(__fastcall*)(__int64, int))fn_GetFriendCount)(isteamfriends, 4);

    friends_set.clear();

    // GetFriendByIndex must have been different in the api they used back then
    auto fn_GetFriendByIndex = *(__int64*)(*(__int64*)isteamfriends + 0x20);
    for (int i = 0; i < num_friends; i++)
    {
        __int64 out_friend = 0;
        ((void(__fastcall*)(__int64, __int64&, int, int))fn_GetFriendByIndex)(isteamfriends, out_friend, i, 4);
        if (out_friend)
        {
            friends_set.insert(out_friend);
        }
    }

    next_update_friendslist_time = GetTickCount64() + 30 * 1000; // once every 30 seconds
    return friends_set.find(xuid) != friends_set.end();
}

unsigned __int64 check_dlc_next = 0;
std::unordered_map<INT32, bool> dlcContent;
// BIsDlcInstalled
// 0x30 has similar signature and seems to be used the same way
// 0x18 isvacbanned
// 0xB0 GetDlcDownloadProgress

bool Protection::GetOwnsContent(INT64 _interface, INT32 itemid)
{
    if (SPOOF_UNLOCK_ALL == true)
    {
        return true;
    }

    if (dlcContent.find(itemid) == dlcContent.end())
    {
        check_dlc_next = GetTickCount64() + (60 * 10 * 1000);
        dlcContent[itemid] = ((bool(__fastcall*)(INT64, INT64))GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT))(_interface, itemid);
    }
    return dlcContent[itemid];
}

bool Protection::GetOwnsContent2(INT64 _interface, INT32 itemid)
{
    if (SPOOF_UNLOCK_ALL == true)
    {
        return true;
    }

    if (dlcContent.find(itemid) == dlcContent.end())
    {
        check_dlc_next = GetTickCount64() + (60 * 10 * 1000);
        dlcContent[itemid] = ((bool(__fastcall*)(INT64, INT64))GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT2))(_interface, itemid);
    }
    return dlcContent[itemid];
}

bool Protection::IsVacBanned(INT64 a)
{
    return false;
}

struct download_progress
{
    __int64 a;
    __int64 b;
    unsigned __int64 next;
};

std::unordered_map<INT32, download_progress> downloadProgress;
void Protection::GetDlcDownloadProgress(INT64 a, INT32 b, INT64* c, INT64* d)
{
    auto now = GetTickCount64();

    if (downloadProgress.find(b) != downloadProgress.end())
    {
        if (now >= downloadProgress[b].next)
        {
            downloadProgress[b].next = GetTickCount64() + (60 * 5 * 1000);
            ((void(__fastcall*)(INT64, INT32, INT64*, INT64*))GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_GET_DLC_DOWNLOAD_PROGRESS))(a, b, c, d);
            downloadProgress[b].a = *c;
            downloadProgress[b].b = *d;
            return;
        }
        *c = downloadProgress[b].a;
        *d = downloadProgress[b].b;
        return;
    }

    downloadProgress[b] = download_progress();
    downloadProgress[b].next = GetTickCount64() + (60 * 5 * 1000);
    ((void(__fastcall*)(INT64, INT32, INT64*, INT64*))GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_GET_DLC_DOWNLOAD_PROGRESS))(a, b, c, d);
    downloadProgress[b].a = *c;
    downloadProgress[b].b = *d;
}

__int32 Protection::GetLobbyChatEntry(INT64 api, INT64 csteamidlobby, INT64 chatid, INT64 psteamuserid, INT64 pvdata, INT64 cubdata, INT64 chatentrytype)
{
    // pump the queue anyways
    auto result = ((int(__fastcall*)(INT64, INT64, INT64, INT64, INT64, INT64, INT64))GetOriginalSteamPtr(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_GETLOBBYCHATENTRY))(api, csteamidlobby, chatid, psteamuserid, pvdata, cubdata, chatentrytype);
    

    auto steamid = *(__int64*)psteamuserid;

    LobbyType sessionType = LobbyType::LOBBY_TYPE_AUTO;

    if (Protection::LobbySession_GetControllingLobbySession(LOBBY_MODULE_CLIENT))
    {
        sessionType = LOBBY_TYPE_GAME;
    }
    else
    {
        sessionType = LOBBY_TYPE_PRIVATE;
    }

    const auto session = Protection::LobbySession_GetSession(sessionType);
    bool found_xuid = false;

    for (int i = 0; i < 18; i++)
    {
        const auto client = Protection::LobbySession_GetClientByClientNum(session, i);
        if (client->activeClient && client->activeClient->fixedClientInfo.xuid == steamid)
        {
            found_xuid = true;
        }
    }

    if (!found_xuid)
    {
        return 0; // user isnt in our bo3 lobby and thus cannot chat with us
    }

    if (result > 0 && *(__int32*)(chatentrytype))
    {
        char* msg = (char*)pvdata;

        for (int i = 0; i < strlen(msg); i++)
        {
            if (msg[i] == '^')
            {
                msg[i] = '.';
            }
            else if (msg[i] == '%')
            {
                msg[i] = '.';
            }
            else if (msg[i] == '$' && msg[i + 1] == '(')
            {
                msg[i] = '.';
            }
            else if (msg[i] == '[' && msg[i + 1] == '{')
            {
                msg[i] = '.';
            }
            else if (msg[i] < 0x20)
            {
                msg[i] = '.';
            }
            else if (msg[i] > 126)
            {
                msg[i] = '.';
            }
        }
    }

    return result;
}

__int64 Protection::CreateLobby(INT64 api, __int32 lobbyCreateType, __int32 maxplayers)
{
    return ((__int64(__fastcall*)(INT64, INT64, INT64))GetOriginalSteamPtr(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_CREATELOBBY))(api, 1, maxplayers);
}

const char* Protection::GetUsernameXUIDPtr(INT64 a, INT64 b)
{
    if (b == **(__int64**)s_playerData_ptr)
    {
        return CustomName;
    }
    return ((char* (__fastcall*)(INT64, INT64))GetOriginalSteamPtr(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_VT_NAMEBYXUID))(a, b);
}

__int32 Protection::CL_SwitchState_Idle_Update(INT64 sw)
{
    if (strcmp(((const char* (__fastcall*)())REBASE(0x20EAD00))(), "CP"))
    {
        return 0;
    }
    return ((__int32(__fastcall*)(__int64))REBASE(0x131E350))(sw);
}

void Protection::SetNetworkPassword(__int64 pass)
{
    //XLOG("PASSWORD: %p", pass);
    Protection::PrivatePassword[0] = Protection::PrivatePassword[1];
    Protection::PrivatePassword[1] = pass;
    Protection::PrivatePassword[2] = GetTickCount64();
}

bool Protection::IsBadReadPtr(void* p)
{
    MEMORY_BASIC_INFORMATION mbi = { 0 };
    if (::VirtualQuery(p, &mbi, sizeof(mbi)))
    {
        DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
        bool b = !(mbi.Protect & mask);
        // check the page is not a guard page
        if (mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS)) b = true;

        return b;
    }
    return true;
}

std::unordered_map<__int64, std::unordered_map<int, __int64>> Protection::SteamHAPIHooks;
void Protection::SwapSteamAPIPointer(__int64 hLibrary, int vPointerIndex, void* CallFuncReplace)
{
    auto steamLibrary = *(INT64*)hLibrary;
    auto OldProtection = 0ul;
    INT64* vtable = *(INT64**)steamLibrary;

    if (SteamHAPIHooks.find(hLibrary) == SteamHAPIHooks.end())
    {
        SteamHAPIHooks[hLibrary] = std::unordered_map<int, __int64>();
    }

    SteamHAPIHooks[hLibrary][vPointerIndex] = *(vtable + vPointerIndex);

    VirtualProtect(reinterpret_cast<void*>(vtable + vPointerIndex), 8, PAGE_EXECUTE_READWRITE, &OldProtection);
    *reinterpret_cast<void**>(vtable + vPointerIndex) = CallFuncReplace;
    VirtualProtect(reinterpret_cast<void*>(vtable + vPointerIndex), 8, OldProtection, &OldProtection);
}

INT64 Protection::GetOriginalSteamPtr(__int64 hLibrary, int vtIndex)
{
    if (SteamHAPIHooks.find(hLibrary) == SteamHAPIHooks.end())
    {
        return 0;
    }
    return SteamHAPIHooks[hLibrary][vtIndex];
}

bool fs_exists(const char* filename)
{
    if (!std::filesystem::exists(filename))
    {
        return false;
    }
    if (INVALID_FILE_ATTRIBUTES == GetFileAttributesA(filename) && GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        return false;
    }
    return true;
}

struct patch_config
{
    char playername[16];
    int isfriendsonly;
    char* networkpassword;
    bool exists;
    std::filesystem::file_time_type modified;

    patch_config()
    {
        networkpassword = (char*)malloc(4);
        memset(networkpassword, 0, 4);
        isfriendsonly = true;
        exists = false;
        modified = std::filesystem::file_time_type();
        __playername();
        strcat_s(playername, "Unknown Soldier");
    }

    void __playername()
    {
        memset(playername, 0, 16);
    }

    bool update_watcher_time(const char* path)
    {
        bool did_exist_before = exists;
        if (!fs_exists(path))
        {
            exists = false;
            return did_exist_before != exists;
        }

        exists = true;
        auto time = std::filesystem::last_write_time(path);

        bool was_same_time = modified == time;
        modified = time;

        return (did_exist_before != exists) || !was_same_time;
    }

    void saveto(const char* path)
    {
        std::ofstream outfile;
        outfile.open(path, std::ofstream::out | std::ofstream::binary);

        if (!outfile.is_open())
        {
            update_watcher_time(path);
            return;
        }

        outfile << "playername=" << playername << std::endl;
        outfile << "isfriendsonly=" << isfriendsonly << std::endl;
        outfile << "networkpassword=" << networkpassword << std::endl;

        outfile.close();
        update_watcher_time(path);
    }

    void loadfrom(const char* path)
    {
        std::ifstream infile;
        infile.open(path, std::ifstream::in | std::ifstream::binary);

        if (!infile.is_open())
        {
            update_watcher_time(path);
            return;
        }

        std::string line;
        while (!std::getline(infile, line).eof())
        {
            auto sep = line.find("=");
            if (sep == std::string::npos || sep >= (line.length() - 1)) // must have a value
            {
                continue;
            }

            // is this config resilliant to whitespace issues? nope!
            auto token = line.substr(0, sep);
            auto val = line.substr(sep + 1);
            switch (fnv1a(token.data()))
            {
            case FNV32("playername"):
            {
                if (val.length() > 15)
                {
                    val = val.substr(0, 15);
                }
                __playername();
                strcpy_s(playername, sizeof(playername), val.data());
            }
            break;
            case FNV32("isfriendsonly"):
            {
                std::istringstream ivalread(val);
                ivalread >> isfriendsonly;
                if (ivalread.fail())
                {
                    isfriendsonly = false; // its better to have it fail then to have people who cant disable this setting because of whatever reason
                }
            }
            break;
            case FNV32("networkpassword"):
            {
                if (networkpassword)
                {
                    free(networkpassword);
                    networkpassword = NULL;
                }
                if (val.length() > 1023)
                {
                    val = val.substr(0, 1023); // seriously?!
                }
                auto bufsize = val.length() + 1;
                networkpassword = (char*)malloc(bufsize);
                strcpy_s(networkpassword, bufsize, val.data());
            }
            break;
            }
        }

        infile.close();
        update_watcher_time(path);
    }
};

patch_config user_config;

void apply_settings()
{
    SetPlayerName(user_config.playername);
    SetFriendsOnly(user_config.isfriendsonly);
    SetNetworkPassword(user_config.networkpassword);
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
    std::srand(time(NULL));
    *(__int32*)OFFSET(0x11250898) = rand();

    for (;;)
    {
        if (user_config.update_watcher_time(PATCH_CONFIG_LOCATION))
        {
            user_config.loadfrom(PATCH_CONFIG_LOCATION);
            apply_settings();
        }
        Sleep(1000);
    }

    return 0;
}

void load_settings_initial()
{
    if (!fs_exists(PATCH_CONFIG_LOCATION))
    {
        user_config.saveto(PATCH_CONFIG_LOCATION);
    }
    else
    {
        user_config.loadfrom(PATCH_CONFIG_LOCATION);
    }
    apply_settings();
}

__int64 old_IsProcessorFeaturePresent = 0;

void Protection::install()
{
    LobbyMsgRW_PackageInt = (tLobbyMsgRW_PackageInt)PTR_LobbyMsgRW_PackageInt;
    LobbyMsgRW_PackageUChar = (tLobbyMsgRW_PackageUChar)PTR_LobbyMsgRW_PackageUChar;
    LobbyMsgRW_PackageString = (tLobbyMsgRW_PackageString)PTR_LobbyMsgRW_PackageString;
    LobbyMsgRW_PackageXuid = (tLobbyMsgRW_PackageXuid)PTR_LobbyMsgRW_PackageXuid;
    LobbyMsgRW_PackageBool = (tLobbyMsgRW_PackageBool)PTR_LobbyMsgRW_PackageBool;
    LobbyMsgRW_PackageUInt = (tLobbyMsgRW_PackageUInt)PTR_LobbyMsgRW_PackageUInt;
    LobbyMsgRW_PackageShort = (tLobbyMsgRW_PackageShort)PTR_LobbyMsgRW_PackageShort;
    LobbyMsgRW_PackageUInt64 = (tLobbyMsgRW_PackageUInt64)PTR_LobbyMsgRW_PackageUInt64;
    LobbyMsgRW_PackageArrayStart = (tLobbyMsgRW_PackageArrayStart)PTR_LobbyMsgRW_PackageArrayStart;
    LobbyMsgRW_PackageElement = (tLobbyMsgRW_PackageElement)PTR_LobbyMsgRW_PackageElement;
    LobbyMsgRW_PackageGlob = (tLobbyMsgRW_PackageGlob)PTR_LobbyMsgRW_PackageGlob;
    MsgMutableClientInfo_Package = (tMsgMutableClientInfo_Package)PTR_MsgMutableClientInfo_Package;
    ProbeLobbyInfo = (tProbeLobbyInfo)PTR_ProbeLobbyInfo;
    dwInstantHandleLobbyMessage = (tdwInstantHandleLobbyMessage)PTR_dwInstantHandleLobbyMessage;
    NET_OutOfBandPrint = (tNET_OutOfBandPrint)PTR_NET_OutOfBandPrint;
    dwCommonAddrToNetadr = (tdwCommonAddrToNetadr)PTR_dwCommonAddrToNetadr;
    dwRegisterSecIDAndKey = (tdwRegisterSecIDAndKey)PTR_dwRegisterSecIDAndKey;
    LobbyMsgRW_PrepWriteMsg = (tLobbyMsgRW_PrepWriteMsg)PTR_LobbyMsgRW_PrepWriteMsg;
    LobbyMsgRW_PackageUShort = (tLobbyMsgRW_PackageUShort)PTR_LobbyMsgRW_PackageUShort;
    LobbyMsgRW_PackageFloat = (tLobbyMsgRW_PackageFloat)PTR_LobbyMsgRW_PackageFloat;
    MSG_Init = (tMSG_Init)PTR_MSG_Init;
    MSG_WriteString = (tMSG_WriteString)PTR_MSG_WriteString;
    MSG_WriteShort = (tMSG_WriteShort)PTR_MSG_WriteShort;
    MSG_WriteByte = (tMSG_WriteByte)PTR_MSG_WriteByte;
    MSG_WriteData = (tMSG_WriteData)PTR_MSG_WriteData;
    Com_ControllerIndex_GetLocalClientNum = (tCom_ControllerIndex_GetLocalClientNum)PTR_Com_ControllerIndex_GetLocalClientNum;
    Com_LocalClient_GetNetworkID = (tCom_LocalClient_GetNetworkID)PTR_Com_LocalClient_GetNetworkID;
    NET_OutOfBandData = (tNET_OutOfBandData)PTR_NET_OutOfBandData;
    LobbyMsgTransport_SendToAdr = (tLobbyMsgTransport_SendToAdr)PTR_LobbyMsgTransport_SendToAdr;
    MSG_ReadData = (tMSG_ReadData)PTR_MSG_ReadData;
    LobbyMsgRW_PrepReadData = (tLobbyMsgRW_PrepReadData)PTR_LobbyMsgRW_PrepReadData;
    MSG_InfoResponse = (tMSG_InfoResponse)PTR_MSG_InfoResponse;
    LobbyMsgRW_PackageChar = (tLobbyMsgRW_PackageChar)PTR_LobbyMsgRW_PackageChar;
    I_stricmp = (tI_stricmp)PTR_I_stricmp;
    dwInstantSendMessage = (tdwInstantSendMessage)PTR_dwInstantSendMessage;
    LobbySession_GetControllingLobbySession = (tLobbySession_GetControllingLobbySession)PTR_LobbySession_GetControllingLobbySession;
    LobbySession_GetSession = (tLobbySession_GetSession)PTR_LobbySession_GetSession;
    LobbySession_GetClientByClientNum = (tLobbySession_GetClientByClientNum)PTR_LobbySession_GetClientByClientNum;
    LobbySession_GetClientNetAdrByIndex = (tLobbySession_GetClientNetAdrByIndex)PTR_LobbySession_GetClientNetAdrByIndex;
    LobbyJoin_Reserve = (tLobbyJoin_Reserve)PTR_LobbyJoin_Reserve;
    CL_GetConfigString = (tCL_GetConfigString)PTR_CL_GetConfigString;
    Cbuf_AddText = (tCbuf_AddText)PTR_Cbuf_AddText;
    I_stricmp = (tI_stricmp)PTR_I_stricmp;
    CachedXUID = **(__int64**)s_playerData_ptr;

    SetNetworkPassword(0);

    sMstart = "mstart";
    sMdata = "mdata";
    sMhead = "mhead";
    sMstate = "mstate";
    sConnectResponse = "connectResponseMigration";
    sRcon = "rcon";
    sRequestStats = "requeststats";
    sRequestStats2 = "requeststats\n";
    sLoading = "loadingnewmap";
    sRA = "RA";
    sV = "v";
    sVT = "vt";
    sRelay = "relay";
    sLMGI = "LMgetinfo";

    SS(targetlobby)
    SS(sourcelobby)
    SS(jointype)
    SS(probedxuid)
    SS(playlistid)
    SS(playlistver)
    SS(ffotdver)
    SS(networkmode)
    SS(netchecksum)
    SS(protocol)
    SS(changelist)
    SS(pingband)
    SS(dlcbits)
    SS(joinnonce)
    SS(chunk)
    SS(isStarterPack)
    SS(password)
    SS(membercount)
    SS(members)
    SS(xuid)
    SS(lobbyid)
    SS(skillrating)
    SS(skillvariance)
    SS(pprobation)
    SS(aprobation)
    SS(statenum)
    SS(mainmode)
    SS(partyprivacy)
    SS(lobbytype)
    SS(lobbymode)
    SS(sessionstatus)
    SS(uiscreen)
    SS(leaderactivity)
    SS(key)
    SS(leader)
    SS(platformsession)
    SS(maxclients)
    SS(isadvertised)
    SS(clientcount)
    SS(sessionid)
    SS(sessioninfo)
    SS(ugcName)
    SS(ugcVersion)
    SS(clientlist)
    SS(clientNum)
    SS(gamertag)
    SS(isGuest)
    SS(connectbit)
    SS(score)
    SS(address)
    SS(qport)
    SS(band)
    SS(netsrc)
    SS(joinorder)
    SS(dlcBits)
    SS(migratebits)
    SS(lasthosttimems)
    SS(nomineelist)
    SS(dlcBits)
    SS(dlcBits)
    SS(serverstatus)
    SS(launchnonce)
    SS(matchhashlow)
    SS(matchhashhigh)
    SS(status)
    SS(statusvalue)
    SS(gamemode)
    SS(gametype)
    SS(map)
    SS(cpqueuedlevel)
    SS(movieskipped)
    SS(team)
    SS(mapvote)
    SS(readyup)
    SS(plistid)
    SS(plistcurr)
    SS(plistentries)
    SS(plistnext)
    SS(plistprev)
    SS(plistprevcount)
    SS(votecount)
    SS(votes)
    SS(itemtype)
    SS(item)
    SS(itemgroup)
    SS(attachment)
    SS(votetype)
    SS(votexuid)
    SS(pregamepos)
    SS(pregamestate)
    SS(clvotecount)
    SS(character)
    SS(loadout)
    SS(settingssize)
    SS(compstate)
    SS(heartbeatnum)
    SS(nonce)
    SS(nattype)
    SS(lobbies)
    SS(valid)
    SS(hostxuid)
    SS(hostname)
    SS(secid)
    SS(seckey)
    SS(addrbuff)

    if (*CustomName)
    {
        memcpy((void*)REBASE(0x15E84638), CustomName, strlen(CustomName) + 1);
        memcpy((void*)PTR_Name1, CustomName, strlen(CustomName) + 1);
        memcpy((void*)PTR_Name2, CustomName, strlen(CustomName) + 1);
        if (!IsBadReadPtr((VOID*)s_playerData_ptr) && *(INT64*)s_playerData_ptr)
        {
            memset((void*)(*(INT64*)s_playerData_ptr + 0x8), 0, 16);
            memcpy((void*)(*(INT64*)s_playerData_ptr + 0x8), CustomName, strlen(CustomName) + 1);
        }
    }

    old_IsProcessorFeaturePresent = (__int64)&IsProcessorFeaturePresent;

    oReadP2PPacket = (decltype(&ReadP2PPacket))((*(__int64*)(STEAMAPI_NETWORKING + 0x10)));
    IHOOK_INSTALL(IsProcessorFeaturePresent, GetModuleHandleA(0));

    SwapSteamAPIPointer(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_GETUSERNAME, GetUsernamePtr);
    SwapSteamAPIPointer(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_VT_NAMEBYXUID, GetUsernameXUIDPtr);

    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT, GetOwnsContent);
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT2, GetOwnsContent2);
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_IS_VAC_BANNED, IsVacBanned);
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_GET_DLC_DOWNLOAD_PROGRESS, GetDlcDownloadProgress);

    SwapSteamAPIPointer(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_GETLOBBYCHATENTRY, GetLobbyChatEntry);
    SwapSteamAPIPointer(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_CREATELOBBY, CreateLobby);
    SwapSteamAPIPointer(STEAMAPI_NETWORKING, STEAMAPI_NETWORKING_READP2PPACKET, ReadP2PPacket);

    *(__int64*)PTR_UpdatePreloadIdleFN = (INT64)CL_SwitchState_Idle_Update;


    // join party
    Protection::handle_packet_callbacks[0x10] = [](__int32* lobbyMsgTypePtr, __int64 lobbyMsg)
        {
            memset(requestOut, 0, sizeof(requestOut));
            memset(lobbyMsgCpy, 0, sizeof(lobbyMsgCpy));
            memcpy_s(lobbyMsgCpy, sizeof(lobbyMsgCpy), (void*)lobbyMsg, 0x44);

            auto result = MSG_JoinParty_Package_Inspect(requestOut, lobbyMsgCpy);
            if (result)
            {
                // crash attempt
                *lobbyMsgTypePtr = 0xFF; // drop

            }
        };

    // modified stats
    Protection::handle_packet_callbacks[0xF] = [](__int32* lobbyMsgTypePtr, __int64 lobbyMsg)
        {
            *lobbyMsgTypePtr = 0xFF; // drop
        };

    // auto-drops
    Protection::handle_packet_callbacks[0x1E] = Protection::handle_packet_callbacks[0xF];
    Protection::handle_packet_callbacks[0x16] = Protection::handle_packet_callbacks[0xF];
    Protection::handle_packet_callbacks[0x17] = Protection::handle_packet_callbacks[0xF];
    Protection::handle_packet_callbacks[0x18] = Protection::handle_packet_callbacks[0xF];

    Protection::handle_packet_callbacks[MESSAGE_TYPE_LOBBY_HOST_HEARTBEAT] = [](__int32* lobbyMsgTypePtr, __int64 lobbyMsg)
        {
            memset(requestOut, 0, sizeof(requestOut));
            memset(lobbyMsgCpy, 0, sizeof(lobbyMsgCpy));
            memcpy_s(lobbyMsgCpy, sizeof(lobbyMsgCpy), (void*)lobbyMsg, 0x44);
            __int32* _this = (__int32*)requestOut;

            BOOL packageOK =
                LobbyMsgRW_PackageInt((void*)lobbyMsgCpy, "heartbeatnum", (__int32*)_this) &&
                LobbyMsgRW_PackageInt((void*)lobbyMsgCpy, "lobbytype", (__int32*)_this) &&
                LobbyMsgRW_PackageInt((void*)lobbyMsgCpy, "lasthosttimems", (__int32*)_this);

            if (!packageOK)
            {
                return;
            }

            LobbyMsgRW_PackageArrayStart((void*)lobbyMsgCpy, "nomineelist");

            unsigned __int64 xuidValue = 1;
            for (int i = 0; i < 0x12; i++)
            {
                bool res = LobbyMsgRW_PackageElement((void*)lobbyMsgCpy, true);
                if (!res)
                {
                    return;
                }
                res = LobbyMsgRW_PackageXuid((void*)lobbyMsgCpy, "xuid", &xuidValue);

                if (!res)
                {
                    return;
                }
            }

            if (LobbyMsgRW_PackageElement((void*)lobbyMsgCpy, true))
            {
                *lobbyMsgTypePtr = 0xFF;
            }
        };

    Protection::handle_packet_callbacks[0x2] = [](__int32* lobbyMsgTypePtr, __int64 lobbyMsg)
        {
            memset(requestOut, 0, sizeof(requestOut));
            memset(lobbyMsgCpy, 0, sizeof(lobbyMsgCpy));
            memcpy_s(lobbyMsgCpy, sizeof(lobbyMsgCpy), (void*)lobbyMsg, 0x44);

            auto result = MSG_LobbyState_Package_Inspect(requestOut, lobbyMsgCpy);
            if (result)
            {
                // crash attempt
                *lobbyMsgTypePtr = 0xFF;
            }
        };

    Protection::handle_packet_callbacks[0x3] = [](__int32* lobbyMsgTypePtr, __int64 lobbyMsg)
        {
            memset(requestOut, 0, sizeof(requestOut));
            memset(lobbyMsgCpy, 0, sizeof(lobbyMsgCpy));
            memcpy_s(lobbyMsgCpy, sizeof(lobbyMsgCpy), (void*)lobbyMsg, 0x44);

            auto result = MSG_LobbyStateGame_Package_Inspect(requestOut, lobbyMsgCpy);
            if (result)
            {
                // crash attempt
                *lobbyMsgTypePtr = 0xFF;
            }
        };

    Protection::Old_lobbymsgprints = *(__int64*)PTR_lobbymsgprints;
    *(__int64*)PTR_lobbymsgprints = 0xFFEEDDCC44332212;
    Protection::CachedRetnAddy = PTR_saveLobbyMsgExceptAddy;

    // Call create lobby again
    ((void(__fastcall*)(__int64))REBASE(0x1EA6010))(REBASE(0x113A4A60));

    INT64 ptrDvar = *(INT64*)(REBASE(0x1686ED20));
    *(DWORD*)(ptrDvar + 0x18) = 0; // clear flags

    Dvar_SetFromStringByName("ui_error_callstack_ship", "1", true);

    ptrDvar = *(INT64*)(REBASE(0xA0378B8));
    *(DWORD*)(ptrDvar + 0x18) = 0; // clear flags

    Dvar_SetFromStringByName("g_allowvote", "0", true);
    Dvar_SetFromStringByName("sv_mapswitch", "0", true);
    Dvar_SetFromStringByName("maxvoicepacketsperframe", "0", true);

    if (IsInjectorlessInstall)
    {
        load_settings_initial();
        CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
    }
}

void Protection::uninstall()
{
    *(__int64*)PTR_lobbymsgprints = *(__int64*)PTR_lobbymsgprints;
    SetNetworkPassword(0);
    Iat_hook_::detour_iat_ptr("IsProcessorFeaturePresent", (void*)old_IsProcessorFeaturePresent);

    SwapSteamAPIPointer(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_GETUSERNAME, (void*)GetOriginalSteamPtr(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_GETUSERNAME));
    SwapSteamAPIPointer(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_VT_NAMEBYXUID, (void*)GetOriginalSteamPtr(STEAMAPI_STEAMUSER, STEAMAPI_STEAMUSER_VT_NAMEBYXUID));

    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT, (void*)GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT));
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT2, (void*)GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_CHECK_OWNS_CONTENT2));
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_IS_VAC_BANNED, (void*)GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_IS_VAC_BANNED));
    SwapSteamAPIPointer(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_GET_DLC_DOWNLOAD_PROGRESS, (void*)GetOriginalSteamPtr(STEAMAPI_INTERFACE, STEAMAPI_INTERFACE_GET_DLC_DOWNLOAD_PROGRESS));

    SwapSteamAPIPointer(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_GETLOBBYCHATENTRY, (void*)GetOriginalSteamPtr(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_GETLOBBYCHATENTRY));
    SwapSteamAPIPointer(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_CREATELOBBY, (void*)GetOriginalSteamPtr(STEAMAPI_MATCHMAKING, STEAMAPI_MATCHMAKING_CREATELOBBY));
    SwapSteamAPIPointer(STEAMAPI_NETWORKING, STEAMAPI_NETWORKING_READP2PPACKET, (void*)GetOriginalSteamPtr(STEAMAPI_NETWORKING, STEAMAPI_NETWORKING_READP2PPACKET));

    *(__int64*)PTR_UpdatePreloadIdleFN = REBASE(0x131E350);
}

char Protection::requestOut[0x20000]{};
char Protection::lobbyMsgCpy[0x50]{};
void Protection::InspectLM(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord)
{
    __int64 retnAddy = *(__int64*)(ContextRecord->Rsp + 0x28);

    // we expect a very specific return addy to operate on
    if (retnAddy != CachedRetnAddy)
    {
        goto ExitCleanly;
    }

    {
        __int64 ctxSuperRsp = ContextRecord->Rsp + 0x30; // calc old Rsp
        __int64 ctxSuperRbx = *(__int64*)(ContextRecord->Rsp + 0x20); // grab old rbx off the stack
        __int32 msgType = *(__int32*)(ctxSuperRsp + 0xB0); // grab the message type [0=host,1=client,3=peer]
        __int64 lobbyMsg = (ctxSuperRsp + 0x88 - 0x58); // lobby message is on the stack (its a LEA)
        __int32* lobbyMsgTypePtr = (__int32*)(lobbyMsg + 0x38);
        __int32 lobbyMsgType = *lobbyMsgTypePtr;

        if (Protection::handle_packet_callbacks.find(lobbyMsgType) != Protection::handle_packet_callbacks.end())
        {
            Protection::handle_packet_callbacks[lobbyMsgType](lobbyMsgTypePtr, lobbyMsg);
            if (*lobbyMsgTypePtr == 0xFF)
            {
                //XLOG("^6DROPPED LOBBYMESSAGE %d", lobbyMsgType);
            }
        }
    }

ExitCleanly:
    ContextRecord->Rcx = Protection::Old_lobbymsgprints;
    ContextRecord->Rbx = Protection::Old_lobbymsgprints;
}

int Protection::MSG_JoinParty_Package_Inspect(char* _this, char* lobbyMsg)
{
    BOOL packageOK =
        LobbyMsgRW_PackageInt(lobbyMsg, "targetlobby", (__int32*)_this)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(sourcelobby), (__int32*)(_this + 4))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(jointype), (__int32*)(_this + 8))
        && LobbyMsgRW_PackageXuid(lobbyMsg, STR(probedxuid), (unsigned __int64*)(_this + 16))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(playlistid), (__int32*)(_this + 612))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(playlistver), (__int32*)(_this + 616))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(ffotdver), (__int32*)(_this + 620))
        && LobbyMsgRW_PackageShort(lobbyMsg, STR(networkmode), (__int16*)(_this + 624))
        && LobbyMsgRW_PackageUInt(lobbyMsg, STR(netchecksum), (unsigned __int32*)(_this + 628))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(protocol), (__int32*)(_this + 632))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(changelist), (__int32*)(_this + 636))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(pingband), (__int32*)(_this + 640))
        && LobbyMsgRW_PackageUInt(lobbyMsg, STR(dlcbits), (unsigned __int32*)(_this + 644))
        && LobbyMsgRW_PackageUInt64(lobbyMsg, STR(joinnonce), (unsigned __int64*)(_this + 648));

    for (int i = 0; packageOK && (i < 3); i++)
    {
        packageOK = packageOK && LobbyMsgRW_PackageUChar(lobbyMsg, STR(chunk), (char*)(_this + i + 689));
    }

    packageOK = packageOK &&
        LobbyMsgRW_PackageBool(lobbyMsg, STR(isStarterPack), (char*)(_this + 656))
        && LobbyMsgRW_PackageString(lobbyMsg, STR(password), (char*)(_this + 657), 0x20)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(membercount), (__int32*)(_this + 24))
        && LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(members));

    if (!packageOK)
    {
        // bad packet
        return 1;
    }

    if (*(__int32*)(_this + 24) > 18)
    {
        // crash attempt via BoF
        return 2;
    }

    if (LobbyMsgRW_PackageElement(lobbyMsg, *(__int32*)(_this + 24) > 0))
    {
        if (*(__int32*)(_this + 24) <= 0) // this is an element even though the packet claims to have 0 members... BoF attempt!
        {
            // crash attempt via BoF
            return 3;
        }

        for (int i = 0; i < *(__int32*)(_this + 24); i++)
        {
            packageOK = packageOK
                && LobbyMsgRW_PackageXuid(lobbyMsg, STR(xuid), (unsigned __int64*)(_this + 616))
                && LobbyMsgRW_PackageUInt64(lobbyMsg, STR(lobbyid), (unsigned __int64*)(_this + 616))
                && LobbyMsgRW_PackageFloat(lobbyMsg, STR(skillrating), (float*)(_this + 616))
                && LobbyMsgRW_PackageFloat(lobbyMsg, STR(skillvariance), (float*)(_this + 616))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(pprobation), (unsigned __int32*)(_this + 616))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(aprobation), (unsigned __int32*)(_this + 616));

            if (!packageOK)
            {
                return 4; // bad packet
            }

            bool expected = (i + 1) < *(__int32*)(_this + 24);
            bool result = LobbyMsgRW_PackageElement(lobbyMsg, expected);
            if (result && !expected)
            {
                return 5; // BoF attempt via package element overflow
            }
        }
    }

    return 0; // packet is fine
}

int Protection::MSG_LobbyState_Package_Inspect(char* __this, char* lobbyMsg)
{
    __int32* _this = (__int32*)__this;

    BOOL packageOK =
        LobbyMsgRW_PackageInt(lobbyMsg, STR(statenum), (__int32*)_this)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(networkmode), (__int32*)(_this + 1))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(mainmode), (__int32*)(_this + 2))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(partyprivacy), (__int32*)(_this + 3))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(lobbytype), (__int32*)(_this + 4))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(lobbymode), (__int32*)(_this + 5))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(sessionstatus), (__int32*)(_this + 6))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(uiscreen), (__int32*)(_this + 7))
        && LobbyMsgRW_PackageUChar(lobbyMsg, STR(leaderactivity), (char*)_this + 32)
        && LobbyMsgRW_PackageString(lobbyMsg, STR(key), (char*)(_this + 9), 0x20)
        && LobbyMsgRW_PackageXuid(lobbyMsg, STR(leader), (unsigned __int64*)(_this + 9))
        && LobbyMsgRW_PackageXuid(lobbyMsg, STR(platformsession), (unsigned __int64*)(_this + 10))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(maxclients), (__int32*)(_this + 22))
        && LobbyMsgRW_PackageBool(lobbyMsg, STR(isadvertised), (char*)_this + 92)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(clientcount), (__int32*)(_this + 24))
        && LobbyMsgRW_PackageString(lobbyMsg, STR(sessionid), (char*)(_this + 5320), 0x40)
        && LobbyMsgRW_PackageString(lobbyMsg, STR(sessioninfo), (char*)(_this + 5336), 0x40)
        && LobbyMsgRW_PackageString(lobbyMsg, STR(ugcName), (char*)(_this + 5352), 0x20)
        && LobbyMsgRW_PackageUInt(lobbyMsg, STR(ugcVersion), (unsigned __int32*)(_this + 5360));

    if (!packageOK)
    {
        // bad packet
        return 1;
    }

    LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(clientlist));

    if (_this[24] > 18)
    {
        // crash attempt via BoF
        return 2;
    }

    int index = 0;
    bool hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, index < _this[24]);


    while (hasNextElement && index < _this[24])
    {
        __int32 offset = 292 * index;
        __int64 offset2 = (__int64)&_this[offset + 26];

        packageOK = packageOK
            && LobbyMsgRW_PackageXuid(lobbyMsg, STR(xuid), (unsigned __int64*)(offset2))
            && LobbyMsgRW_PackageUChar(lobbyMsg, STR(clientNum), (char*)(offset2 + 8))
            && LobbyMsgRW_PackageString(lobbyMsg, STR(gamertag), (char*)(offset2 + 9), 0x20)
            && LobbyMsgRW_PackageBool(lobbyMsg, STR(isGuest), (char*)(offset2 + 41))
            && LobbyMsgRW_PackageUInt64(lobbyMsg, STR(lobbyid), (unsigned __int64*)(offset2 + 48))
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(connectbit), (__int32*)(offset2 + 1108))
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(score), (__int32*)(offset2 + 1104))
            && LobbyMsgRW_PackageGlob(lobbyMsg, STR(address), (char*)(offset2 + 1113), 37)
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(qport), (__int32*)(offset2 + 56))
            && LobbyMsgRW_PackageUChar(lobbyMsg, STR(band), (char*)(offset2 + 60))
            && LobbyMsgRW_PackageUInt(lobbyMsg, STR(netsrc), (unsigned __int32*)(offset2 + 1152))
            && LobbyMsgRW_PackageUInt(lobbyMsg, STR(joinorder), (unsigned __int32*)(offset2 + 1156))
            && LobbyMsgRW_PackageUInt(lobbyMsg, STR(dlcBits), (unsigned __int32*)(offset2 + 1160))
            && MsgMutableClientInfo_Package((char*)(offset2 + 64), lobbyMsg);

        if (!packageOK)
        {
            return 100 + 1; // bad packet
        }

        index++;
        hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, index < _this[24]);
    }

    if (hasNextElement)
    {
        return 100 + 2; // Crash attempt via BoF and PackElem RO exploit
    }

    packageOK = packageOK
        && LobbyMsgRW_PackageUChar(lobbyMsg, STR(migratebits), (char*)_this + 21128)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(lasthosttimems), (__int32*)(_this + 5283));

    if (!packageOK)
    {
        // bad packet
        return 3;
    }

    LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(nomineelist));

    __int32 count = 0;
    hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (count < 18));

    while (hasNextElement && (count < 18))
    {
        packageOK = packageOK && LobbyMsgRW_PackageXuid(lobbyMsg, STR(xuid), (unsigned __int64*)(&_this[0x14A4 + (2 * count)]));
        count++;
        hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (count < 18));
    }

    if (hasNextElement)
    {
        return 100 + 3; // crash attempt via PERO nominee
    }

    if (!packageOK)
    {
        // bad packet
        return 4;
    }

    return 0;
}

int Protection::MSG_LobbyStateGame_Package_Inspect(char* __this, char* lobbyMsg)
{
    int lspiResult = MSG_LobbyState_Package_Inspect(__this, lobbyMsg);

    if (lspiResult)
    {
        return lspiResult;
    }

    __int32* _this = (__int32*)__this;
    bool packageOK = LobbyMsgRW_PackageInt(lobbyMsg, STR(serverstatus), (__int32*)(_this + 5362));

    if (!packageOK)
    {
        return 11;
    }

    packageOK = packageOK
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(launchnonce), (__int32*)(_this + 5363))
        && LobbyMsgRW_PackageUInt64(lobbyMsg, STR(matchhashlow), (unsigned __int64*)(_this + 2682))
        && LobbyMsgRW_PackageUInt64(lobbyMsg, STR(matchhashhigh), (unsigned __int64*)(_this + 2683))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(status), (__int32*)(_this + 5394))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(statusvalue), (__int32*)(_this + 5395))
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(gamemode), (__int32*)(_this + 5368))
        && LobbyMsgRW_PackageString(lobbyMsg, STR(gametype), (char*)(_this + 5369), 0x20)
        && LobbyMsgRW_PackageString(lobbyMsg, STR(map), (char*)(_this + 5377), 0x20)
        && LobbyMsgRW_PackageString(lobbyMsg, STR(ugcName), (char*)(_this + 5385), 0x20)
        && LobbyMsgRW_PackageUInt(lobbyMsg, STR(ugcVersion), (unsigned __int32*)(_this + 5393));

    if (!_this[2])
    {
        packageOK = packageOK
            && LobbyMsgRW_PackageString(lobbyMsg, STR(cpqueuedlevel), (char*)(_this + 7293), 0x20)
            && LobbyMsgRW_PackageBool(lobbyMsg, STR(movieskipped), (char*)_this + 29204);
    }

    __int32 lobbyMode = _this[5];

    if (!lobbyMode)
    {
        LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(clientlist));
        int index = 0;
        bool hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));
        while (hasNextElement && (index < 18))
        {
            __int64 currentClient = (__int64)&_this[9 * index];
            packageOK = packageOK
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(team), (__int32*)(currentClient + 28524))
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(mapvote), (__int32*)(currentClient + 28528))
                && LobbyMsgRW_PackageBool(lobbyMsg, STR(readyup), (char*)(currentClient + 28532));

            if (!packageOK)
            {
                // bad pack
                return 20 + 1;
            }

            index++;
            hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));
        }

        if (hasNextElement)
        {
            // PERO
            return 20 + 2;
        }

        packageOK = packageOK
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(plistid), (__int32*)(_this + 7126))
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(plistcurr), (__int32*)(_this + 7127))
            && LobbyMsgRW_PackageGlob(lobbyMsg, STR(plistentries), (char*)_this + 28512, 8)
            && LobbyMsgRW_PackageUChar(lobbyMsg, STR(plistnext), (char*)_this + 28520)
            && LobbyMsgRW_PackageUChar(lobbyMsg, STR(plistprev), (char*)_this + 28521)
            && LobbyMsgRW_PackageUChar(lobbyMsg, STR(plistprevcount), (char*)_this + 28522);

        if (!packageOK)
        {
            // bad packet
            return 5;
        }

        goto PackageOK;
    }

    lobbyMode--;
    if (!lobbyMode)
    {
        packageOK = packageOK && LobbyMsgRW_PackageInt(lobbyMsg, STR(votecount), (__int32*)(_this + 5397));
        LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(votes));

        int numVotes = *(__int32*)(_this + 5397);
        if (packageOK && (numVotes > 216))
        {
            // crash attempt via votecount BoF
            return 20 + 6;
        }

        int index = 0;
        bool hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < numVotes));

        while (hasNextElement && (index < numVotes))
        {
            __int64 currentVote = (__int64)(&_this[8 * index + 5398]);
            packageOK = packageOK
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(itemtype), (__int16*)(currentVote + 8))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(item), (unsigned int*)(currentVote + 12))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(itemgroup), (__int16*)(currentVote + 16))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(attachment), (__int16*)(currentVote + 20))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(votetype), (__int16*)(currentVote + 24))
                && LobbyMsgRW_PackageXuid(lobbyMsg, STR(votexuid), (unsigned __int64*)(currentVote));

            if (!packageOK)
            {
                return 6; // bad packet
            }

            index++;
            hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < numVotes));
        }

        if (hasNextElement)
        {
            // PERO
            return 20 + 3;
        }

        LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(clientlist));
        index = 0;
        hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));
        while (hasNextElement && (index < 18))
        {
            __int64 currentClient = (__int64)&_this[index + 8 * index + 7131];
            packageOK = packageOK
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(team), (__int32*)(currentClient))
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(pregamepos), (__int32*)(currentClient + 4))
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(pregamestate), (__int32*)(currentClient + 3))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(clvotecount), (unsigned __int32*)(currentClient + 6))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(character), (unsigned __int32*)(currentClient + 7))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(loadout), (unsigned __int32*)(currentClient + 8));

            if (!packageOK)
            {
                // bad packet
                return 7;
            }

            index++;
            hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));
        }

        if (hasNextElement)
        {
            // PERO
            return 20 + 4;
        }

        packageOK = packageOK && LobbyMsgRW_PackageInt(lobbyMsg, STR(settingssize), (__int32*)(_this + 7350));

        if ((_this[7350] <= 0) || (_this[7350] > 0xC000))
        {
            // BoF attempt
            return 20 + 5;
        }

        goto PackageOK;
    }

    lobbyMode--;
    if (!lobbyMode)
    {
        goto PackageOK;
    }

    if (lobbyMode == 1)
    {
        packageOK = packageOK
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(compstate), _this + 5396)
            && LobbyMsgRW_PackageInt(lobbyMsg, STR(votecount), _this + 5397);

        if (!packageOK)
        {
            // bad packet
            return 10;
        }


        __int32 numVotes = _this[5397];
        if (_this[5397] > 216)
        {
            return 20 + 7;
        }

        int index = 0;
        bool hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < numVotes));

        while (hasNextElement && (index < numVotes))
        {
            __int64 currentVote = (__int64)(&_this[8 * index + 5398]);
            packageOK = packageOK
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(itemtype), (__int16*)(currentVote + 8))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(item), (unsigned int*)(currentVote + 12))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(itemgroup), (__int16*)(currentVote + 16))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(attachment), (__int16*)(currentVote + 20))
                && LobbyMsgRW_PackageShort(lobbyMsg, STR(votetype), (__int16*)(currentVote + 24))
                && LobbyMsgRW_PackageXuid(lobbyMsg, STR(votexuid), (unsigned __int64*)(currentVote));

            if (!packageOK)
            {
                return 8; // bad packet
            }

            index++;
            hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < numVotes));
        }

        if (hasNextElement)
        {
            // PERO
            return 20 + 8;
        }

        LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(clientlist));
        index = 0;
        hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));

        while (hasNextElement && (index < 18))
        {
            __int64 currentClient = (__int64)&_this[index + 8 * index + 7131];
            packageOK = packageOK
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(team), (__int32*)(currentClient))
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(pregamepos), (__int32*)(currentClient + 4))
                && LobbyMsgRW_PackageInt(lobbyMsg, STR(pregamestate), (__int32*)(currentClient + 3))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(clvotecount), (unsigned __int32*)(currentClient + 6))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(character), (unsigned __int32*)(currentClient + 7))
                && LobbyMsgRW_PackageUInt(lobbyMsg, STR(loadout), (unsigned __int32*)(currentClient + 8));

            if (!packageOK)
            {
                // bad packet
                return 9;
            }

            index++;
            hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (index < 18));
        }

        if (hasNextElement)
        {
            // PERO
            return 20 + 9;
        }

        goto PackageOK;
    }

PackageOK:

    return 0;
}

int Protection::MSG_HostHeartbeat_Inspect(char* __this, char* lobbyMsg)
{
    bool packageOK = LobbyMsgRW_PackageInt(lobbyMsg, STR(heartbeatnum), (__int32*)__this)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(lobbytype), (__int32*)__this)
        && LobbyMsgRW_PackageInt(lobbyMsg, STR(lasthosttimems), (__int32*)__this);

    if (!packageOK) // invalid packet
    {
        return 1;
    }

    LobbyMsgRW_PackageArrayStart(lobbyMsg, STR(nomineelist));

    __int32 count = 0;
    bool hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (count < 18));
    while (hasNextElement && (count < 18))
    {
        packageOK = packageOK && LobbyMsgRW_PackageXuid(lobbyMsg, STR(xuid), (unsigned __int64*)__this);
        count++;
        hasNextElement = LobbyMsgRW_PackageElement(lobbyMsg, (count < 18));
    }

    if (hasNextElement)
    {
        return 100 + 1; // crash attempt via PERO nominee
    }

    if (!packageOK)
    {
        // bad packet
        return 2;
    }

    return 0;
}

bool Protection::CheckPendingInfoRequests(__int64 XUID, msg_t* _msg)
{
    msg_t cpyMsg;
    memcpy(&cpyMsg, _msg, sizeof(msg_t));

    unsigned int size = cpyMsg.cursize - cpyMsg.readcount;
    if (size < 2048u)
    {
        char data[2048]{};
        MSG_ReadData(&cpyMsg, data, size);

        if (!cpyMsg.overflowed)
        {
            LobbyMsg lobby_msg{};
            if (!LobbyMsgRW_PrepReadData(&lobby_msg, data, size))
            {
                //XLOG("DROP LM: FAILED TO READ DATA");
                return false;
            }

            if (lobby_msg.msgType == MESSAGE_TYPE_INFO_RESPONSE)
            {
                Msg_InfoResponse response{};
                __int32 result = MSG_InfoResponseSafe(&response, &lobby_msg);
                if (result)
                {
                    //XLOG("DROP LM: REASON %d", result);
                    return true;
                }

                dwInstantHandleLobbyMessage(XUID, 0, (char*)_msg);
                return true;
            }
            else
            {
                if (!XUID || XUID == **(__int64**)s_playerData_ptr)
                {
                    //XLOG("KEEP LM: XUID LOCAL");
                    return false;
                }

                INT64 expectedMask = 0x110000000000000;
                INT64 andMask = 0xFFFF00000000000;

                if (Protection::IsFriendsOnly)
                {
                    if ((andMask & XUID) != expectedMask)
                    {
                        //XLOG("KEEP LM: SERVER OR SPLITSCREEN");
                        return false; // server or splitscreen
                    }

                    //if (((bool(__fastcall*)(int, __int64))PTR_LiveFriends_IsFriendByXUID)(0, XUID))
                    if (IsFriendByXUIDUncached(XUID))
                    {
                        //XLOG("KEEP LM: FRIEND");
                        return false; // they are a friend! lets respond
                    }

                    //XLOG("DROP LM: UNKNOWN");
                    // not a friend, dont respond
                    return true;
                }
            }
        }
    }

    return false;
}

int Protection::MSG_InfoResponseSafe(Msg_InfoResponse* infoResponse, LobbyMsg* lm)
{
    bool packIsOK = true;
    packIsOK = LobbyMsgRW_PackageUInt(lm, "nonce", (unsigned __int32*)&infoResponse->nonce)
        && LobbyMsgRW_PackageInt(lm, "uiscreen", &infoResponse->uiScreen)
        && LobbyMsgRW_PackageUChar(lm, "nattype", &infoResponse->natType);

    if (!packIsOK)
    {
        return 1;
    }

    LobbyMsgRW_PackageArrayStart(lm, "lobbies");


    bool hasNextElement = LobbyMsgRW_PackageElement(lm, true);
    bool hasNextElement2 = false;
    for (__int32 cResponse = 0; hasNextElement && (cResponse < 2); cResponse++)
    {
        LobbyMsgRW_PackageArrayStart(lm, "lobbies");

        hasNextElement2 = LobbyMsgRW_PackageElement(lm, true);
        for (__int32 i = 0; hasNextElement2 && (i < 2); i++)
        {
            packIsOK = packIsOK && LobbyMsgRW_PackageBool(lm, "valid", &infoResponse->lobby[cResponse].isValid);

            if (!packIsOK)
            {
                return 2;
            }

            if (!infoResponse->lobby[cResponse].isValid)
            {
                hasNextElement2 = LobbyMsgRW_PackageElement(lm, (i < 2));
                continue;
            }

            packIsOK = packIsOK
                && LobbyMsgRW_PackageXuid(lm, "hostxuid", (unsigned __int64*)&infoResponse->lobby[cResponse].hostXuid)
                && LobbyMsgRW_PackageString(lm, "hostname", infoResponse->lobby[cResponse].hostName, 32)
                && LobbyMsgRW_PackageInt(lm, "networkmode", &infoResponse->lobby[cResponse].lobbyParams.networkMode)
                && LobbyMsgRW_PackageInt(lm, "mainmode", &infoResponse->lobby[cResponse].lobbyParams.mainMode)
                && LobbyMsgRW_PackageGlob(lm, "secid", (char*)&infoResponse->lobby[cResponse].secId.id, 8)
                && LobbyMsgRW_PackageGlob(lm, "seckey", infoResponse->lobby[cResponse].secKey.ab, 16)
                && LobbyMsgRW_PackageGlob(lm, "addrbuff", infoResponse->lobby[cResponse].serializedAdr.addrBuf, 37)
                && LobbyMsgRW_PackageString(lm, "ugcName", infoResponse->lobby[cResponse].ugcName, 32)
                && LobbyMsgRW_PackageUInt(lm, "ugcVersion", (unsigned __int32*)&infoResponse->lobby[cResponse].ugcVersion);

            if (!packIsOK)
            {
                return 3;
            }
            hasNextElement2 = LobbyMsgRW_PackageElement(lm, (i < 2));
        }

        if (hasNextElement2)
        {
            return 5;
        }

        hasNextElement = LobbyMsgRW_PackageElement(lm, (cResponse < 2));
    }

    if (hasNextElement || lm->msg.overflowed)
    {
        return 4;
    }

    return 0;
}

void Protection::ExceptHook(PEXCEPTION_RECORD ExceptionRecord, PCONTEXT ContextRecord)
{
    if ((INT64)ContextRecord->Rcx == 0xFFEEDDCC44332212)
    {
        InspectLM(ExceptionRecord, ContextRecord);
        ZwContinue(ContextRecord, false);
        return;
    }

    // From v2.04
    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x22C965C)) // character index crash
    {
        ContextRecord->Rip = REBASE(0x22C9686);
        ZwContinue(ContextRecord, false);
        return;
    }

    // From v2.04
    if (ExceptionRecord->ExceptionAddress == (PVOID)REBASE(0x22C9676)) // character index crash
    {
        ContextRecord->Rip = REBASE(0x22C9686);
        ZwContinue(ContextRecord, false);
        return;
    }

    if (ExceptionRecord->ExceptionAddress == (PVOID)LuaCrash1)
    {
        ContextRecord->Rip = LuaCrash1Rip;
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
        ContextRecord->Rdx = (INT64)UILocalizeDefaultText;
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
        ContextRecord->Rsi = (INT64)UILocalizeDefaultText;
        ZwContinue(ContextRecord, false);
        return;
    }

}


namespace Iat_hook_
{
    void** find(const char* function, HMODULE module)
    {
        if (!module)
            module = GetModuleHandle(0);

        PIMAGE_DOS_HEADER img_dos_headers = (PIMAGE_DOS_HEADER)module;
        PIMAGE_NT_HEADERS img_nt_headers = (PIMAGE_NT_HEADERS)((BYTE*)img_dos_headers + img_dos_headers->e_lfanew);
        PIMAGE_IMPORT_DESCRIPTOR img_import_desc = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)img_dos_headers + img_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
        if (img_dos_headers->e_magic != IMAGE_DOS_SIGNATURE)
            printf("\n");

        for (IMAGE_IMPORT_DESCRIPTOR* iid = img_import_desc; iid->Name != 0; iid++) {
            for (int func_idx = 0; *(func_idx + (void**)(iid->FirstThunk + (size_t)module)) != nullptr; func_idx++) {
                char* mod_func_name = (char*)(*(func_idx + (size_t*)(iid->OriginalFirstThunk + (size_t)module)) + (size_t)module + 2);
                const intptr_t nmod_func_name = (intptr_t)mod_func_name;
                if (nmod_func_name >= 0) {
                    if (!::strcmp(function, mod_func_name))
                        return func_idx + (void**)(iid->FirstThunk + (size_t)module);
                }
            }
        }

        return 0;

    }

    uintptr_t detour_iat_ptr(const char* function, void* newfunction, HMODULE module)
    {
        auto&& func_ptr = find(function, module);
        if (*func_ptr == newfunction || *func_ptr == nullptr)
            return 0;

        DWORD old_rights, new_rights = PAGE_READWRITE;
        VirtualProtect(func_ptr, sizeof(uintptr_t), new_rights, &old_rights);
        uintptr_t ret = (uintptr_t)*func_ptr;
        *func_ptr = newfunction;
        VirtualProtect(func_ptr, sizeof(uintptr_t), old_rights, &new_rights);
        return ret;
    }
};