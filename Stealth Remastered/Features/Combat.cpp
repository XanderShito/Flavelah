#include "main.h"

#include "Features/LocalMacroTraining.h"

namespace
{
    FlavelahTraining::Scheduler localMacro;
    const char* localMacroStatus = "Idle";
    unsigned localMacroCompleted = 0;
    char localMacroStatusBuffer[160]{};

    bool SendTrainingKey(FlavelahTraining::KeyEvent event)
    {
        if (!event.key) return true;
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wScan = static_cast<WORD>(MapVirtualKeyA(
            static_cast<UINT>(event.key), MAPVK_VK_TO_VSC));
        if (!input.ki.wScan) return false;
        input.ki.dwFlags = KEYEVENTF_SCANCODE |
            (event.down ? 0 : KEYEVENTF_KEYUP);
        return SendInput(1, &input, sizeof(input)) == 1;
    }

    const char* LocalTrainingBlockReason()
    {
        if (!pSAMP || !pSAMP->isInited || !pSAMP->getInfo())
            return "Waiting for SA-MP";
        const auto* info = pSAMP->getInfo();
        if (info->iGameState != GAMESTATE_CONNECTED ||
            !FlavelahTraining::IsLoopback(info->szIP))
            return "Requires local test server";
        if (!pSAMP->getInput() || pSAMP->getInput()->iInputEnabled)
            return "Paused: chat";
        if (FrontEndMenuManager.m_bMenuActive || !pMenu || pMenu->bOpen)
            return "Paused: menu";
        DWORD foregroundPid = 0;
        GetWindowThreadProcessId(GetForegroundWindow(), &foregroundPid);
        if (foregroundPid != GetCurrentProcessId())
            return "Paused: game not focused";
        if (!FindPlayerPed() || FindPlayerVehicle(-1, false))
            return "Paused: on foot only";
        if (g_Config.g_Combat.bAutoCBug || g_Config.g_Combat.bAutoScroll)
            return "Paused: conflicting options";
        return nullptr;
    }

    void UpdateLocalMacro()
    {
        if (const char* reason = LocalTrainingBlockReason())
        {
            SendTrainingKey(localMacro.cancel());
            localMacroStatus = reason;
            return;
        }
        auto* ped = FindPlayerPed();
        const int weapon = ped->m_aWeapons[ped->m_nActiveWeaponSlot].m_nType;
        const bool aim = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        const bool fire = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

        const auto event = localMacro.tick(GetTickCount64(), true, aim, fire,
            weapon, g_Config.g_Combat.bLocalMacro);
        if (!SendTrainingKey(event))
        {
            // Best-effort release on error, then reset the scheduler.
            if (event.key && !event.down) SendTrainingKey(event);
            SendTrainingKey(localMacro.cancel());
            localMacroStatus = "Input rejected by Windows";
            return;
        }

        if (localMacro.completed())
        {
            ++localMacroCompleted;
            localMacroStatus = "Sequence sent";
        }
        else if (localMacro.running())
            localMacroStatus = "Sequence running";
        else if (localMacro.armed())
            localMacroStatus = "Armed: release both mouse buttons";
        else if (localMacroStatus != nullptr &&
                 strcmp(localMacroStatus, "Input rejected by Windows") != 0)
            localMacroStatus = "Ready";
    }
}

const char* Combat::LocalMacroStatus()
{
    snprintf(localMacroStatusBuffer, sizeof(localMacroStatusBuffer),
        "%s | sent: %u", localMacroStatus, localMacroCompleted);
    return localMacroStatusBuffer;
}

void Combat::CancelLocalMacro()
{
    SendTrainingKey(localMacro.cancel());
}


void Combat::Update()
{
	NoReload();
	AllowCBUG();
	RapidFire();
	FastCrosshair();
	NoRecoil();
	AutoScroll();
	AutoCBUG();
	UpdateLocalMacro();
}

void Combat::NoReload()
{
	if (!g_Config.g_Combat.bNoReload)
		return;

	int iWeapon = FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].m_nType;
	if (iWeapon >= 22 && iWeapon <= 42 && FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].m_nAmmoInClip == 1)
		FindPlayerPed()->GiveWeapon((eWeaponType)iWeapon, 0, false);
}

void Combat::AllowCBUG()
{
	static bool bAllowCBug = false;
	if (g_Config.g_Combat.bAllowCBug)
	{
		if (g_Config.g_Combat.bCBugFreeze)
			pSecure->memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x0168E0), "\xC3\x90", 2);
		if (g_Config.g_Combat.bCBugAnim)
			pSecure->memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x016FA0), "\xC3\x90", 2);
		if (g_Config.g_Combat.bCBugWeapon)
			pSecure->memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x015530), "\xC3\x90", 2);
		if (g_Config.g_Combat.bCBugText)
			pSecure->memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x015F40), "\xC3\x90", 2);
		bAllowCBug = true;
	}
	else if (bAllowCBug)
	{
		Memory::memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x0168E0), "\xE9\x49", 2);
		Memory::memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x016FA0), "\x55\x8B", 2);
		Memory::memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x015530), "\xE9\x18", 2);
		Memory::memcpy_safe((void*)(pSAMP->g_dwSAMP_Addr + 0x015F40), "\xE9\x2C", 2);
		bAllowCBug = false;
	}
}

void Combat::RapidFire()
{
	if (!g_Config.g_Combat.bRapidFire)
		return;

	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "PYTHON_CROUCHFIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "PYTHON_FIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "PYTHON_FIRE_POOR", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "RIFLE_CROUCHFIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "RIFLE_FIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "RIFLE_FIRE_POOR", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "SHOTGUN_CROUCHFIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "SHOTGUN_FIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "SHOTGUN_FIRE_POOR", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "SILENCED_CROUCH_FIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "SILENCED_FIRE", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "TEC_crouchfire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "TEC_fire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "UZI_crouchfire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "UZI_fire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "UZI_fire_POOR", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "idle_rocket", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "Rocket_Fire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "run_rocket", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "walk_rocket", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "WALK_start_rocket", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "WEAPON_sniper", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "buddy_crouchfire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "buddy_fire", g_Config.g_Combat.fRapidFire);
	plugin::Command<plugin::Commands::SET_CHAR_ANIM_SPEED>(FindPlayerPed(), "buddy_fire_POOR", g_Config.g_Combat.fRapidFire);
}

void Combat::FastCrosshair()
{
	static bool bFastCrosshair = false;
	if (g_Config.g_Combat.bFastCrosshair)
	{
		pSecure->memcpy_safe((void*)0x0058E1D9, "\xEB", 1);
		bFastCrosshair = true;
	}
	else if (bFastCrosshair)
	{
		Memory::memcpy_safe((void*)0x0058E1D9, "\x74", 1);
		bFastCrosshair = false;
	}
}

void Combat::NoRecoil()
{
	if (g_Config.g_Combat.bNoRecoil)
		pSecure->memcpy_safe((void*)0x8D610F, "\xBE\x00\x00\x00\x00", 5);
	else Memory::memcpy_safe((void*)0x8D610F, "\xBE\x00\x00\x40\x3F", 5);
}

void Combat::WeaponSwitcher()
{
	if (g_Config.g_Combat.bWeaponSwitcher && !FindPlayerVehicle(-1, false))
	{
		for (size_t i = 0; i < g_Config.g_Hotkeys.iWeaponSwitch.size(); i++)
		{
			if (isKeyPressed(g_Config.g_Hotkeys.iWeaponSwitch[i]))
			{
				if (g_Config.g_Combat.bFastSwitch)
					FindPlayerPed()->SetCurrentWeapon(i + 2);
				else *(DWORD*)0xB7CDBC = i + 2;
			}
		}
	}
}

void Combat::AutoScroll()
{
	if (g_Config.g_Combat.bAutoScroll)
	{
		static int iSlot = 0;
		static int iState = 0;

		if (pSAMP->getPlayers()->pLocalPlayer->onFootData.stSampKeys.keys_aim && FindPlayerPed()->m_nActiveWeaponSlot == 2) {
			iState = 1;
		}
		else if (iState == 2)
		{
			FindPlayerPed()->SetCurrentWeapon(iSlot);
			iState = 0;
		}
		else if (iState == 1)
		{
			iSlot = FindPlayerPed()->m_nActiveWeaponSlot;
			FindPlayerPed()->m_nActiveWeaponSlot = 0;
			iState = 2;
		}
	}
}

void Combat::AutoCBUG()
{
	if (!g_Config.g_Combat.bAutoCBug)
		return;

	if (pSAMP->getPlayers()->pLocalPlayer->byteCurrentWeapon != 24)
		return;

	static int iStep = 0;
	if (FindPlayerPed()->m_aWeapons[FindPlayerPed()->m_nActiveWeaponSlot].m_nAmmoInClip && GetAsyncKeyState(VK_LBUTTON) && GetAsyncKeyState(VK_RBUTTON))
	{
		pKeyHook->g_GameKeyState[17] = { 0, true };
		static ULONGLONG ulWait = 0;
		static ULONGLONG ulTick = 0;
		if (GetTickCount64() - ulTick > ulWait)
		{
			ulTick = GetTickCount64();
			switch (iStep)
			{
			case 0:
			{
				pKeyHook->g_GameKeyState[17] = { 0xFF, true };
				ulWait = g_Config.g_Combat.iCBugDelay[1];
				break;
			}
			case 1:
			{
				pKeyHook->g_GameKeyState[6] = { 0, true };
				pKeyHook->g_GameKeyState[18] = { 0xFF, true };
				ulWait = g_Config.g_Combat.iCBugDelay[0];
				break;
			}
			}

			if (iStep++ > 1)
				iStep = 0;
		}
	}
	else iStep = 0;
}