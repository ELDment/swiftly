#ifndef _player_player_h
#define _player_player_h

#include "../../sdk/entity/CBasePlayerController.h"
#include "../../sdk/entity/CCSPlayerController.h"
#include "../../sdk/entity/CBasePlayerPawn.h"
#include "../../sdk/entity/CCSPlayerPawn.h"
#include "../../sdk/entity/CCSPlayerPawnBase.h"
#include "../../sdk/entity/CBaseViewModel.h"
#include "../../server/menus/Menu.h"
#include "../../server/menus/MenuRenderer.h"
#include "../../plugins/core/scripting.h"

#include <string>
#include <public/mathlib/vector.h>
#include <public/playerslot.h>
#include <ctime>

#include <any>

#define HUD_PRINTNOTIFY 1
#define HUD_PRINTCONSOLE 2
#define HUD_PRINTTALK 3
#define HUD_PRINTCENTER 4

enum ListenOverride
{
    Listen_Default = 0,
    Listen_Mute,
    Listen_Hear
};

enum VoiceFlagValue
{
    Speak_Normal = 0,
    Speak_Muted = 1 << 0,
    Speak_All = 1 << 1,
    Speak_ListenAll = 1 << 2,
    Speak_Team = 1 << 3,
    Speak_ListenTeam = 1 << 4,
};

typedef uint8_t VoiceFlag_t;

class Player
{
public:
    Player(bool m_isFakeClient, int m_slot, const char* m_name, uint64 m_xuid, std::string ip_address);
    ~Player();

    bool IsFakeClient();
    CPlayerSlot GetSlot();
    uint32_t GetConnectedTime();
    std::string GetIPAddress();

    void SendMsg(int dest, const char* msg, ...);

    void RenderCenterText(uint64_t time);

    const char* GetName();
    uint64 GetSteamID();

    CBasePlayerController* GetController();
    CBasePlayerPawn* GetPawn();
    CCSPlayerController* GetPlayerController();
    CCSPlayerPawn* GetPlayerPawn();
    CCSPlayerPawnBase* GetPlayerBasePawn();

    Vector GetCoords();
    void SetCoords(float x, float y, float z);

    void SwitchTeam(int team);
    void ChangeTeam(int team);

    void SetButtons(uint64_t new_buttons);

    std::string tag = "";
    std::string tagcolor = "{default}";
    std::string namecolor = "{teamcolor}";
    std::string chatcolor = "{default}";

    bool IsFirstSpawn();
    void SetFirstSpawn(bool value);

    bool HasCenterText();

    void PerformCommand(std::string command);

    void SetClientConvar(std::string cmd, std::string val);

    std::any GetInternalVar(std::string name);
    void SetInternalVar(std::string name, std::any value);

    void SetListen(CPlayerSlot slot, ListenOverride listen);
    void SetVoiceFlags(VoiceFlag_t flags);
    VoiceFlag_t GetVoiceFlags();
    ListenOverride GetListen(CPlayerSlot slot) const;

    CBaseViewModel* EnsureCustomView(int index);
    PluginPlayer* GetPlayerObject();

    CPlayerBitVec m_selfMutes[64] = {};

    std::string language = "";
    MenuRenderer* menu_renderer = nullptr;

private:
    int slot;
    bool isFakeClient = false;

    IGameEvent* centerMessageEvent = nullptr;
    IGameEventListener2* playerListener = nullptr;

    std::time_t connectTime;
    const char* name;
    uint64 xuid;
    std::string ip_address = "0.0.0.0";

    uint64_t centerMessageEndTime = 0;
    std::string centerMessageText;

    bool firstSpawn = true;

    uint64_t buttons = 0;

    std::map<std::string, std::any> internalVars;

    ListenOverride m_listenMap[66] = {};
    VoiceFlag_t m_voiceFlag = 0;
    PluginPlayer* playerObject = nullptr;
};
extern std::map<std::string, std::string> colors;
#endif