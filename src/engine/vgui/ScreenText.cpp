#include "ScreenText.h"

ScreenText::ScreenText() {}

ScreenText::~ScreenText()
{
    if (pScreenEntity.IsValid()) {
        pScreenEntity->Despawn();
    }
}

void ScreenText::Create(Color color, std::string font, int size, bool drawBackground, bool isMenu)
{
    m_col = color;
    m_font = font;
    m_size = size;
    m_drawBackground = drawBackground;
    m_isMenu = isMenu;

    pScreenEntity.Set((CPointWorldText*)(CreateEntityByName("point_worldtext").GetPtr()));
    if (!pScreenEntity) return;

    CEntityKeyValues* pMenuKV = new CEntityKeyValues();

    pMenuKV->SetBool("enabled", false);
    pMenuKV->SetFloat("world_units_per_pixel", (0.25 / 1050) * size);
    pMenuKV->SetInt("justify_horizontal", 0);
    pMenuKV->SetInt("justify_vertical", 2);
    pMenuKV->SetInt("reorient_mode", 0);
    pMenuKV->SetInt("fullbright", 1);
    pMenuKV->SetFloat("font_size", size);
    pMenuKV->SetString("font_name", font.c_str());
    pMenuKV->SetColor("color", color);

    if(drawBackground) {
        pMenuKV->SetBool("draw_background", true);

        if(isMenu) {
            pMenuKV->SetFloat("background_border_width", 0.2);
            pMenuKV->SetFloat("background_border_height", 0.15);
        } else {
            pMenuKV->SetFloat("background_border_width", g_Config->FetchValue<float>("core.vgui.textBackground.paddingX"));
            pMenuKV->SetFloat("background_border_height", g_Config->FetchValue<float>("core.vgui.textBackground.paddingY"));
        }

        pMenuKV->SetFloat("background_away_units", 0.04);
        pMenuKV->SetFloat("background_world_to_uv", 0.05);
    }

    pScreenEntity->DispatchSpawn(pMenuKV);
}

void ScreenText::SetupViewForPlayer(Player* player)
{
    m_player = player;

    if (!pScreenEntity) return;
    if (!player) return;
    if (player->IsFakeClient()) return;

    CBaseViewModel* pViewModel = player->EnsureCustomView(1);
    if (!pViewModel) return;

    pScreenEntity->SetParent(pViewModel);
    pScreenEntity->m_hOwnerEntity(pViewModel->GetRefEHandle());
}

void ScreenText::SetText(std::string text)
{
    m_text = text;

    if (!pScreenEntity) return;

    pScreenEntity->SetText(m_text.c_str());
    pScreenEntity->Enable();
}

void ScreenText::SetPosition(float posX, float posY)
{
    m_posX = posX;
    m_posY = posY;

    if (!m_player) return;
    if (m_player->IsFakeClient()) return;
    if(!pScreenEntity) return;

    CCSPlayerPawn* pawn = m_player->GetPlayerPawn();
    if (!pawn) return;
    if (pawn->m_lifeState() == 2) {
        if (m_player->GetPlayerController()->m_bControllingBot()) {
            return;
        }
        else {
            auto pPawn = m_player->GetPawn();
            if (!pPawn) return;

            auto observerPawn = pPawn->m_pObserverServices->m_hObserverTarget();
            if (!observerPawn) return;

            auto observerController = ((CCSPlayerPawn*)(observerPawn.Get()))->m_hOriginalController();
            if (!observerController) return;

            auto observer = g_playerManager->GetPlayer(observerController->entindex() - 1);
            if (!observer) return;
            pawn = observer->GetPlayerPawn();
        }
    }
    if (!pawn) return;

    QAngle eyeAngles = pawn->m_angEyeAngles();
    Vector fwd, right, up;
    AngleVectors(eyeAngles, &fwd, &right, &up);

    Vector eyePos(0.0, 0.0, 0.0);
    eyePos += fwd * 7;
    eyePos += right * (-9.2 + (posX * 18.35));
    eyePos += up * (-4.9 + (posY * 10.13));

    QAngle ang(0, eyeAngles.y + 270, 90 - eyeAngles.x);

    eyePos += pawn->m_CBodyComponent->m_pSceneNode->m_vecAbsOrigin() + Vector(0, 0, pawn->m_pCameraServices->m_flOldPlayerViewOffsetZ());

    pScreenEntity->Teleport(&eyePos, &ang, nullptr);
}

bool ScreenText::IsValidEntity()
{
    return pScreenEntity.IsValid();
}

void ScreenText::RegenerateText(bool recreate)
{
    if (recreate) {
        if (pScreenEntity.IsValid()) pScreenEntity->Despawn();

        Create(m_col, m_font, m_size, m_drawBackground, m_isMenu);
        SetupViewForPlayer(m_player);
        SetText(m_text);
        SetPosition(m_posX, m_posY);
    }
    else {
        SetupViewForPlayer(m_player);
        SetPosition(m_posX, m_posY);
    }
}

Player* ScreenText::GetPlayer()
{
    return m_player;
}

int ScreenText::GetEntityIndex()
{
    if (!pScreenEntity) return 0;

    return pScreenEntity->GetEntityIndex().Get();
}

bool ScreenText::IsRenderingTo(CHandle<CBaseEntity> renderingTo)
{
    return renderingTo == pRenderingTo;
}

void ScreenText::SetRenderingTo(CBaseEntity* ent)
{
    pRenderingTo.Set(ent);
}