#pragma once

#include <map>
#include <string>
#include <variant.h>
#include "../../../vendor/dynlib/module.h"

class CBasePlayerController;
class CCSPlayerController;
class CBaseEntity;
class CBaseModelEntity;
class CEntityInstance;
class CCSPlayer_ItemServices;
class CCSPlayerPawn;
class CCSPlayerPawnBase;
class CTakeDamageInfo;
class IRecipientFilter;
class CEntityIndex;
class CCSPlayer_MovementServices;

struct EmitSound_t;
struct SndOpEventGuid_t;

typedef void (*CCSPlayerController_SwitchTeam)(CCSPlayerController* pController, unsigned int team);
typedef void* (*UTIL_CreateEntityByName)(const char*, int);
typedef void (*CBaseModelEntity_SetModel_t)(CBaseModelEntity*, const char*);
typedef void (*CBaseEntity_DispatchSpawn)(CBaseEntity*, void*);
typedef void (*UTIL_Remove)(CEntityInstance*);
typedef void (*CEntityInstance_AcceptInput)(CEntityInstance*, const char*, CEntityInstance*, CEntityInstance*, variant_t*, int);
typedef void (*CAttributeList_SetOrAddAttributeValueByName_t)(void*, const char*, float);
typedef void (*CBaseModelEntity_SetBodygroup_t)(void*, const char*, ...);
typedef void (*GiveNamedItem_t)(CCSPlayer_ItemServices*, const char*, int, int, int, int);
typedef SndOpEventGuid_t(*CBaseEntity_EmitSoundFilter)(IRecipientFilter& filter, CEntityIndex ent, const EmitSound_t& params);
typedef void (*CBaseEntity_EmitSoundParams)(CBaseEntity*, const char*, int, float, float);
typedef void (*CBaseEntity_TakeDamage_t)(CBaseEntity*, CTakeDamageInfo*);
typedef void (*CTakeDamageInfo_Constructor)(CTakeDamageInfo*, CBaseEntity*, CBaseEntity*, CBaseEntity*, const Vector*, const Vector*, float, int, int, void*);

DynLibUtils::CModule DetermineModuleByLibrary(std::string library);

class Signatures
{
private:
    std::map<std::string, void*> signatures;

public:
    void LoadSignatures();

    template <typename T>
    T FetchSignature(std::string name)
    {
        if (!this->Exists(name))
            return nullptr;

        return reinterpret_cast<T>(this->signatures[name]);
    }

    void* FetchRawSignature(std::string name)
    {
        if (!this->Exists(name))
            return nullptr;

        return this->signatures[name];
    }

    bool Exists(std::string name);
};

extern Signatures* g_Signatures;