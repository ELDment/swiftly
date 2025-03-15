#pragma once

#include <platform.h>
#include "globaltypes.h"
#include <tier1/utlvector.h>
#include "CCSWeaponBase.h"

class CBaseEntity;
class CCSPlayerPawn;
class CBasePlayerPawn;

class CPlayerPawnComponent
{
public:
    virtual ~CPlayerPawnComponent() = 0;

private:
    [[maybe_unused]] uint8_t __pad0008[0x28]; // 0x8
public:
    CBasePlayerPawn* m_pPawn; // 0x30
};

struct CSPerRoundStats_t
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CSPerRoundStats_t)

    SCHEMA_FIELD_OFFSET(int, m_iKills, 0);
    SCHEMA_FIELD_OFFSET(int, m_iDeaths, 0);
    SCHEMA_FIELD_OFFSET(int, m_iAssists, 0);
    SCHEMA_FIELD_OFFSET(int, m_iDamage, 0);
};

struct CSMatchStats_t : public CSPerRoundStats_t
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CSMatchStats_t)
};

class CCSPlayerController_ActionTrackingServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayerController_ActionTrackingServices)

    SCHEMA_FIELD_OFFSET(CSMatchStats_t, m_matchStats, 0)
};

class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CPlayer_MovementServices)

    SCHEMA_FIELD_OFFSET(CInButtonState, m_nButtons, 0);
    SCHEMA_FIELD_OFFSET(uint64_t, m_nQueuedButtonDownMask, 0);
    SCHEMA_FIELD_OFFSET(uint64_t, m_nQueuedButtonChangeMask, 0);
    SCHEMA_FIELD_OFFSET(uint64_t, m_nButtonDoublePressed, 0);
    SCHEMA_FIELD_POINTER_OFFSET(uint32_t, m_pButtonPressedCmdNumber, 0);
    SCHEMA_FIELD_OFFSET(uint32_t, m_nLastCommandNumberProcessed, 0);
    SCHEMA_FIELD_OFFSET(uint64_t, m_nToggleButtonDownMask, 0);
    SCHEMA_FIELD_OFFSET(float, m_flMaxspeed, 0);
};

class CPlayer_MovementServices_Humanoid : public CPlayer_MovementServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CPlayer_MovementServices_Humanoid)
};

class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayer_MovementServices)
};

class CCSPlayerController_InGameMoneyServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayerController_InGameMoneyServices)

    SCHEMA_FIELD_OFFSET(int, m_iAccount, 0)
};

class CCSPlayer_ItemServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayer_ItemServices)

    virtual ~CCSPlayer_ItemServices() = 0;

    SCHEMA_FIELD_OFFSET(bool, m_bHasDefuser, 0);
    SCHEMA_FIELD_OFFSET(bool, m_bHasHelmet, 0);
    SCHEMA_FIELD_OFFSET(bool, m_bHasHeavyArmor, 0);

private:
    virtual void unk_01() = 0;
    virtual void unk_02() = 0;
    virtual void unk_03() = 0;
    virtual void unk_04() = 0;
    virtual void unk_05() = 0;
    virtual void unk_06() = 0;
    virtual void unk_07() = 0;
    virtual void unk_08() = 0;
    virtual void unk_09() = 0;
    virtual void unk_10() = 0;
    virtual void unk_11() = 0;
    virtual void unk_12() = 0;
    virtual void unk_13() = 0;
    virtual void unk_14() = 0;
    virtual void unk_15() = 0;
    virtual void unk_16() = 0;
    virtual CBaseEntity* _GiveNamedItem(const char* pchName) = 0;

public:
    virtual bool GiveNamedItemBool(const char* pchName) = 0;
    virtual CBaseEntity* GiveNamedItem(const char* pchName) = 0;
    virtual void DropPlayerWeapon(CBasePlayerWeapon* weapon) = 0;
    virtual void StripPlayerWeapons(bool removeSuit) = 0;
};

// We need an exactly sized class to be able to iterate the vector, our schema system implementation can't do this
class WeaponPurchaseCount_t
{
private:
    virtual void unk01() {};
    uint64_t unk1 = 0;  // 0x8
    uint64_t unk2 = 0;  // 0x10
    uint64_t unk3 = 0;  // 0x18
    uint64_t unk4 = 0;  // 0x20
    uint64_t unk5 = -1; // 0x28
public:
    uint16_t m_nItemDefIndex; // 0x30
    uint16_t m_nCount;        // 0x32
private:
    uint32_t unk6 = 0;
};

struct WeaponPurchaseTracker_t
{
public:
    DECLARE_SCHEMA_CLASS_BASE(WeaponPurchaseTracker_t)

    SCHEMA_FIELD_POINTER_OFFSET(CUtlVector<WeaponPurchaseCount_t>, m_weaponPurchases, 0)
};

class CCSPlayer_ActionTrackingServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayer_ActionTrackingServices)

    SCHEMA_FIELD_OFFSET(WeaponPurchaseTracker_t, m_weaponPurchasesThisRound, 0)
};

class CPlayer_WeaponServices : public CPlayerPawnComponent
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CPlayer_WeaponServices)

    SCHEMA_FIELD_POINTER_OFFSET(CUtlVector<CHandle<CBasePlayerWeapon>>, m_hMyWeapons, 0);
    SCHEMA_FIELD_OFFSET(CHandle<CBasePlayerWeapon>, m_hActiveWeapon, 0);

    void DropWeapon(CBasePlayerWeapon* pWeapon, Vector* pVecTarget = nullptr, Vector* pVelocity = nullptr)
    {
        static int offset = g_Offsets->GetOffset("CCSPlayer_WeaponServices_DropWeapon");
        CALL_VIRTUAL(void, offset, this, pWeapon, pVecTarget, pVelocity);
    }

    void RemoveWeapon(CBasePlayerWeapon* weapon)
    {
        this->DropWeapon(weapon);
        weapon->Despawn();
    }
};

class CPlayer_CameraServices : public CPlayerPawnComponent
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CPlayer_CameraServices);

    SCHEMA_FIELD_OFFSET(float, m_flOldPlayerViewOffsetZ, 0);
    SCHEMA_FIELD_OFFSET(CHandle<CBasePlayerPawn>, m_hViewEntity, 0);
};

class CCSPlayerController_InventoryServices
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CCSPlayerController_InventoryServices)

    SCHEMA_FIELD_OFFSET(uint16_t, m_unMusicID, 0);
    SCHEMA_FIELD_POINTER_OFFSET(uint8_t, m_rank, 0);
};

class CPlayer_ObserverServices : public CPlayerPawnComponent
{
public:
    DECLARE_SCHEMA_CLASS_BASE(CPlayer_ObserverServices);

    SCHEMA_FIELD_OFFSET(uint8, m_iObserverMode, 0);
    SCHEMA_FIELD_OFFSET(CHandle<CBaseEntity>, m_hObserverTarget, 0);
    SCHEMA_FIELD_OFFSET(bool, m_bForcedObserverMode, 0);
};