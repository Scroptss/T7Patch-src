#pragma once
#include "framework.h"

enum MsgType_z
{
	MESSAGE_TYPE_ZBR_LOBBYINFO_REQUEST = 0x21, // UNUSED
	MESSAGE_TYPE_ZBR_LOBBYINFO_RESPONSE = 0x22,
	MESSAGE_TYPE_ZBR_LOBBYSTATE = 0x23,
	MESSAGE_TYPE_ZBR_CLIENTRELIABLE = 0x24,
	MESSAGE_TYPE_ZBR_CHARACTERRPC = 0x25,
	MESSAGE_TYPE_ZBR_COUNT
};

enum ControllerIndex_t
{
	INVALID_CONTROLLER_PORT = 0xFFFFFFFF,
	CONTROLLER_INDEX_FIRST = 0x0,
	CONTROLLER_INDEX_0 = 0x0,
	CONTROLLER_INDEX_1 = 0x1,
	CONTROLLER_INDEX_2 = 0x2,
	CONTROLLER_INDEX_3 = 0x3,
	CONTROLLER_INDEX_COUNT = 0x4,
};

enum netsrc_t
{
	NS_NULL = -1,
	NS_CLIENT1 = 0,
	NS_CLIENT2 = 1,
	NS_CLIENT3 = 2,
	NS_CLIENT4 = 3,
	NS_SERVER = 4,
	NS_MAXCLIENTS = 4,
	NS_PACKET = 5
};

struct netadr_t
{
	unsigned char ipv4[4];
	unsigned __int16 port;
	unsigned __int16 pad;
	__int32 type;
	netsrc_t localNetID;
};

enum LobbyModule
{
	LOBBY_MODULE_INVALID = 0xFFFFFFFF,
	LOBBY_MODULE_HOST = 0x0,
	LOBBY_MODULE_CLIENT = 0x1,
	LOBBY_MODULE_PEER_TO_PEER = 0x3,
	LOBBY_MODULE_COUNT = 0x4,
};

enum LobbyType
{
	LOBBY_TYPE_INVALID = 0xFFFFFFFF,
	LOBBY_TYPE_PRIVATE = 0x0,
	LOBBY_TYPE_GAME = 0x1,
	LOBBY_TYPE_TRANSITION = 0x2,
	LOBBY_TYPE_COUNT = 0x3,
	LOBBY_TYPE_FIRST = 0x0,
	LOBBY_TYPE_LAST = 0x2,
	LOBBY_TYPE_AUTO = 0x3,
};

enum LobbyMode
{
	LOBBY_MODE_INVALID = 0xFFFFFFFF,
	LOBBY_MODE_PUBLIC = 0x0,
	LOBBY_MODE_CUSTOM = 0x1,
	LOBBY_MODE_THEATER = 0x2,
	LOBBY_MODE_ARENA = 0x3,
	LOBBY_MODE_FREERUN = 0x4,
	LOBBY_MODE_COUNT = 0x5,
};


enum SessionActive
{
	SESSION_INACTIVE = 0x0,
	SESSION_KEEP_ALIVE = 0x1,
	SESSION_ACTIVE = 0x2,
};

enum NetChanMsgType_e
{
	NETCHAN_INVALID_CHANNEL = 0xFFFFFFFF,
	NETCHAN_SNAPSHOT = 0x0,
	NETCHAN_CLIENTMSG = 0x1,
	NETCHAN_VOICE = 0x2,
	NETCHAN_LOBBY_VOICE = 0x3,
	NETCHAN_LOBBYPRIVATE_STATE = 0x4,
	NETCHAN_LOBBYPRIVATE_HEARTBEAT = 0x5,
	NETCHAN_LOBBYPRIVATE_RELIABLE = 0x6,
	NETCHAN_LOBBYPRIVATE_UNRELIABLE = 0x7,
	NETCHAN_LOBBYPRIVATE_MIGRATE = 0x8,
	NETCHAN_LOBBYGAME_STATE = 0x9,
	NETCHAN_LOBBYGAME_HEARTBEAT = 0xA,
	NETCHAN_LOBBYGAME_RELIABLE = 0xB,
	NETCHAN_LOBBYGAME_UNRELIABLE = 0xC,
	NETCHAN_LOBBYGAME_MIGRATE = 0xD,
	NETCHAN_LOBBY_JOIN = 0xE,
	NETCHAN_PTP = 0xF,
	NETCHAN_CLIENT_CONTENT = 0x10,
	NETCHAN_MAX_CHANNELS = 0x11,
};

struct LobbySession
{
	LobbyModule module;
	LobbyType type;
	LobbyMode mode;
	char pad[0x34];
	SessionActive active;
	char pad2[0x121D4];
};

struct JoinPartyMember
{
	__int64 XUID;
	__int64 lobbyID;
	float skillRating;
	float skilVariance;
	__int32 probationTimeRemaining[2];
};

struct FixedClientInfo
{
	__int64 xuid;
	char gamertag[32];
};

struct SessionInfo
{
	bool inSession;
	char pad[7];
	netadr_t netAdr;
	time_t lastMessageSentToPeer;
};

struct ActiveClient
{
	char pad[0x410];
	FixedClientInfo fixedClientInfo;
	SessionInfo sessionInfo[2];
};

struct SessionClient
{
	char pad[0x8];
	ActiveClient* activeClient;
};

enum MsgType
{
	MESSAGE_TYPE_NONE = -1,
	MESSAGE_TYPE_INFO_REQUEST = 0,
	MESSAGE_TYPE_INFO_RESPONSE = 1,
	MESSAGE_TYPE_LOBBY_STATE_PRIVATE = 2,
	MESSAGE_TYPE_LOBBY_STATE_GAME = 3,
	MESSAGE_TYPE_LOBBY_STATE_GAMEPUBLIC = 4,
	MESSAGE_TYPE_LOBBY_STATE_GAMECUSTOM = 5,
	MESSAGE_TYPE_LOBBY_STATE_GAMETHEATER = 6,
	MESSAGE_TYPE_LOBBY_HOST_HEARTBEAT = 7,
	MESSAGE_TYPE_LOBBY_HOST_DISCONNECT = 8,
	MESSAGE_TYPE_LOBBY_HOST_DISCONNECT_CLIENT = 9,
	MESSAGE_TYPE_LOBBY_HOST_LEAVE_WITH_PARTY = 0xA,
	MESSAGE_TYPE_LOBBY_CLIENT_HEARTBEAT = 0xB,
	MESSAGE_TYPE_LOBBY_CLIENT_DISCONNECT = 0xC,
	MESSAGE_TYPE_LOBBY_CLIENT_RELIABLE_DATA = 0xD,
	MESSAGE_TYPE_LOBBY_CLIENT_CONTENT = 0xE,
	MESSAGE_TYPE_LOBBY_MODIFIED_STATS = 0xF,
	MESSAGE_TYPE_JOIN_LOBBY = 0x10,
	MESSAGE_TYPE_JOIN_RESPONSE = 0x11,
	MESSAGE_TYPE_JOIN_AGREEMENT_REQUEST = 0x12,
	MESSAGE_TYPE_JOIN_AGREEMENT_RESPONSE = 0x13,
	MESSAGE_TYPE_JOIN_COMPLETE = 0x14,
	MESSAGE_TYPE_JOIN_MEMBER_INFO = 0x15,
	MESSAGE_TYPE_SERVERLIST_INFO = 0x16,
	MESSAGE_TYPE_PEER_TO_PEER_CONNECTIVITY_TEST = 0x17,
	MESSAGE_TYPE_PEER_TO_PEER_INFO = 0x18,
	MESSAGE_TYPE_LOBBY_MIGRATE_TEST = 0x19,
	MESSAGE_TYPE_MIGRATE_ANNOUNCE_HOST = 0x1A,
	MESSAGE_TYPE_MIGRATE_START = 0x1B,
	MESSAGE_TYPE_INGAME_MIGRATE_TO = 0x1C,
	MESSAGE_TYPE_MIGRATE_NEW_HOST = 0x1D,
	MESSAGE_TYPE_VOICE_PACKET = 0x1E,
	MESSAGE_TYPE_VOICE_RELAY_PACKET = 0x1F,
	MESSAGE_TYPE_DEMO_STATE = 0x20,
	MESSAGE_TYPE_COUNT = MESSAGE_TYPE_DEMO_STATE + 1
};

struct msg_t
{
	bool overflowed; // 0x0
	bool readOnly; // 0x1
	__int16 pad1; // 0x2
	__int32 pad2; // 0x4
	char* data; // 0x8
	char* splitData; // 0x10
	unsigned __int32 maxsize; // 0x18
	unsigned __int32 cursize; // 0x1C
	unsigned __int32 splitSize; // 0x20
	unsigned __int32 readcount; // 0x24
	__int32 bit; // 0x28
	__int32 lastEntityRef; // 0x2C
	__int32 flush; // 0x30
	netsrc_t targetLocalNetID; // 0x34
};

struct LobbyMsg
{
	msg_t msg;
	__int32 msgType; // 0x38
	char encodeFlags;
	__int32 packageType;
};

struct bdSecurityID
{
	__int64 id;
};

struct bdSecurityKey
{
	char ab[16];
};

struct SerializedAdr
{
	char valid;
	char addrBuf[37];
};

struct LobbyParams
{
	__int32 networkMode;
	__int32 mainMode;
};

struct InfoResponseLobby
{
	char isValid; // 0x0
	char pad[3];
	__int32 pad2;
	__int64 hostXuid; // 0x8
	char hostName[0x20]; // 0x10
	bdSecurityID secId; // 0x30
	bdSecurityKey secKey; // 0x38
	SerializedAdr serializedAdr; // 0x48
	char pad3[2]; // 0x6E
	LobbyParams lobbyParams; // 0x70
	char ugcName[0x20]; // 0x78
	__int32 ugcVersion; // 0x98
	char pad4[0x4]; // 0x9C
};

struct Msg_InfoResponse
{
	__int32 nonce;
	__int32 uiScreen;
	char natType;
	char pad[3];
	__int32 pad2;
	InfoResponseLobby lobby[2];
};

