#include "memesets.h"
#include "SDK.h"


void Offsetz::GetNetvars()
{
    NetVarManager->Initialize();
	offsetz.DT_BasePlayer.m_Local = NetVarManager->GetOffset("DT_BasePlayer", "m_Local");
	offsetz.DT_BasePlayer.m_aimPunchAngle = NetVarManager->GetOffset("DT_BasePlayer", "m_aimPunchAngle");
	offsetz.DT_BasePlayer.m_aimPunchAngleVel = NetVarManager->GetOffset("DT_BasePlayer", "m_aimPunchAngleVel");
	offsetz.DT_BasePlayer.m_viewPunchAngle = NetVarManager->GetOffset("DT_BasePlayer", "m_viewPunchAngle");
	offsetz.DT_BasePlayer.m_vecViewOffset = NetVarManager->GetOffset("DT_BasePlayer", "m_vecViewOffset[0]");
	offsetz.DT_BasePlayer.m_nTickBase = NetVarManager->GetOffset("DT_BasePlayer", "m_nTickBase");
	offsetz.DT_BasePlayer.m_vecVelocity = NetVarManager->GetOffset("DT_BasePlayer", "m_vecVelocity[0]");
	offsetz.DT_BasePlayer.m_iHealth = NetVarManager->GetOffset("DT_BasePlayer", "m_iHealth");
	offsetz.DT_BasePlayer.m_iMaxHealth = NetVarManager->GetOffset("DT_BasePlayer", "m_iMaxHealth");
	offsetz.DT_BasePlayer.m_lifeState = NetVarManager->GetOffset("DT_BasePlayer", "m_lifeState");
	offsetz.DT_BasePlayer.m_fFlags = NetVarManager->GetOffset("DT_BasePlayer", "m_fFlags");
	offsetz.DT_BasePlayer.m_iObserverMode = NetVarManager->GetOffset("DT_BasePlayer", "m_iObserverMode");
	offsetz.DT_BasePlayer.m_hObserverTarget = NetVarManager->GetOffset("DT_BasePlayer", "m_hObserverTarget");
	offsetz.DT_BasePlayer.m_hViewModel = NetVarManager->GetOffset("DT_BasePlayer", "m_hViewModel[0]");
	offsetz.DT_BasePlayer.m_szLastPlaceName = NetVarManager->GetOffset("DT_BasePlayer", "m_szLastPlaceName");
	offsetz.DT_BasePlayer.deadflag = NetVarManager->GetOffset("DT_BasePlayer", "deadflag");

	offsetz.DT_BaseEntity.m_flAnimTime = NetVarManager->GetOffset("DT_BaseEntity", "m_flAnimTime");
	offsetz.DT_BaseEntity.m_flSimulationTime = NetVarManager->GetOffset("DT_BaseEntity", "m_flSimulationTime");
	offsetz.DT_BaseEntity.m_vecOrigin = NetVarManager->GetOffset("DT_BaseEntity", "m_vecOrigin");
	offsetz.DT_BaseEntity.m_angRotation = NetVarManager->GetOffset("DT_BaseEntity", "m_angRotation");
	offsetz.DT_BaseEntity.m_nRenderMode = NetVarManager->GetOffset("DT_BaseEntity", "m_nRenderMode");
	offsetz.DT_BaseEntity.m_iTeamNum = NetVarManager->GetOffset("DT_BaseEntity", "m_iTeamNum");
	offsetz.DT_BaseEntity.m_MoveType = offsetz.DT_BaseEntity.m_nRenderMode + 1;
	offsetz.DT_BaseEntity.m_Collision = NetVarManager->GetOffset("DT_BaseEntity", "m_Collision");
	offsetz.DT_BaseEntity.m_bSpotted = NetVarManager->GetOffset("DT_BaseEntity", "m_bSpotted");
	offsetz.DT_BaseEntity.m_vecMins = NetVarManager->GetOffset("DT_BaseEntity", "m_vecMins");
	offsetz.DT_BaseEntity.m_vecMaxs = NetVarManager->GetOffset("DT_BaseEntity", "m_vecMaxs");
	offsetz.DT_BaseEntity.m_nSolidType = NetVarManager->GetOffset("DT_BaseEntity", "m_nSolidType");
	offsetz.DT_BaseEntity.m_usSolidFlags = NetVarManager->GetOffset("DT_BaseEntity", "m_usSolidFlags");
	offsetz.DT_BaseEntity.m_nSurroundType = NetVarManager->GetOffset("DT_BaseEntity", "m_nSurroundType");
	offsetz.DT_BaseEntity.m_hOwner = NetVarManager->GetOffset("DT_BaseEntity", "m_hOwner");
	offsetz.DT_BaseViewModel.m_hOwner = NetVarManager->GetOffset("DT_BaseViewModel", "m_hOwner");

    offsetz.DT_BaseCombatCharacter.m_hActiveWeapon = NetVarManager->GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
	offsetz.DT_BaseCombatCharacter.m_hMyWeapons = NetVarManager->GetOffset("DT_BaseCombatCharacter", "m_hMyWeapons") / 2;
	offsetz.DT_BaseCombatCharacter.m_hMyWearables = NetVarManager->GetOffset("DT_BaseCombatCharacter", "m_hMyWearables");

	offsetz.DT_PlayerResource.m_iPing = NetVarManager->GetOffset("DT_PlayerResource", "m_iPing");
	offsetz.DT_PlayerResource.m_iKills = NetVarManager->GetOffset("DT_PlayerResource", "m_iKills");
	offsetz.DT_PlayerResource.m_iAssists = NetVarManager->GetOffset("DT_PlayerResource", "m_iAssists");
	offsetz.DT_PlayerResource.m_iDeaths = NetVarManager->GetOffset("DT_PlayerResource", "m_iDeaths");
	offsetz.DT_PlayerResource.m_bConnected = NetVarManager->GetOffset("DT_PlayerResource", "m_bConnected");
	offsetz.DT_PlayerResource.m_iTeam = NetVarManager->GetOffset("DT_PlayerResource", "m_iTeam");
	offsetz.DT_PlayerResource.m_iPendingTeam = NetVarManager->GetOffset("DT_PlayerResource", "m_iPendingTeam");
	offsetz.DT_PlayerResource.m_bAlive = NetVarManager->GetOffset("DT_PlayerResource", "m_bAlive");
	offsetz.DT_PlayerResource.m_iHealth = NetVarManager->GetOffset("DT_PlayerResource", "m_iHealth");

	offsetz.DT_CSPlayerResource.m_iPlayerC4 = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iPlayerC4");
	offsetz.DT_CSPlayerResource.m_iPlayerVIP = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iPlayerVIP");
	offsetz.DT_CSPlayerResource.m_bHostageAlive = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bHostageAlive");
	offsetz.DT_CSPlayerResource.m_isHostageFollowingSomeone = NetVarManager->GetOffset("DT_CSPlayerResource", "m_isHostageFollowingSomeone");
	offsetz.DT_CSPlayerResource.m_iHostageEntityIDs = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iHostageEntityIDs");
	offsetz.DT_CSPlayerResource.m_bombsiteCenterB = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bombsiteCenterB");
	offsetz.DT_CSPlayerResource.m_hostageRescueX = NetVarManager->GetOffset("DT_CSPlayerResource", "m_hostageRescueX");
	offsetz.DT_CSPlayerResource.m_hostageRescueY = NetVarManager->GetOffset("DT_CSPlayerResource", "m_hostageRescueY");
	offsetz.DT_CSPlayerResource.m_hostageRescueZ = NetVarManager->GetOffset("DT_CSPlayerResource", "m_hostageRescueZ");
	offsetz.DT_CSPlayerResource.m_iMVPs = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iMVPs");
	offsetz.DT_CSPlayerResource.m_iArmor = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iArmor");
	offsetz.DT_CSPlayerResource.m_bHasHelmet = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bHasHelmet");
	offsetz.DT_CSPlayerResource.m_bHasDefuser = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bHasDefuser");
	offsetz.DT_CSPlayerResource.m_iScore = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iScore");
	offsetz.DT_CSPlayerResource.m_iCompetitiveRanking = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iCompetitiveRanking");
	offsetz.DT_CSPlayerResource.m_iCompetitiveWins = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iCompetitiveWins");
	offsetz.DT_CSPlayerResource.m_iCompTeammateColor = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iCompTeammateColor");
	offsetz.DT_CSPlayerResource.m_bControllingBot = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bControllingBot");
	offsetz.DT_CSPlayerResource.m_iControlledPlayer = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iControlledPlayer");
	offsetz.DT_CSPlayerResource.m_iControlledByPlayer = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iControlledByPlayer");
	offsetz.DT_CSPlayerResource.m_iBotDifficulty = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iBotDifficulty");
	offsetz.DT_CSPlayerResource.m_szClan = NetVarManager->GetOffset("DT_CSPlayerResource", "m_szClan");
	offsetz.DT_CSPlayerResource.m_iTotalCashSpent = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iTotalCashSpent");
	offsetz.DT_CSPlayerResource.m_iCashSpentThisRound = NetVarManager->GetOffset("DT_CSPlayerResource", "m_iCashSpentThisRound");
	offsetz.DT_CSPlayerResource.m_nEndMatchNextMapVotes = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nEndMatchNextMapVotes");
	offsetz.DT_CSPlayerResource.m_bEndMatchNextMapAllVoted = NetVarManager->GetOffset("DT_CSPlayerResource", "m_bEndMatchNextMapAllVoted");
	offsetz.DT_CSPlayerResource.m_nActiveCoinRank = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nActiveCoinRank");
	offsetz.DT_CSPlayerResource.m_nMusicID = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nMusicID");
	offsetz.DT_CSPlayerResource.m_nPersonaDataPublicLevel = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicLevel");
	offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsLeader = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
	offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsTeacher = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
	offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsFriendly = NetVarManager->GetOffset("DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");

	offsetz.DT_PlantedC4.m_bBombTicking = NetVarManager->GetOffset("DT_PlantedC4", "m_bBombTicking");
	offsetz.DT_PlantedC4.m_flC4Blow = NetVarManager->GetOffset("DT_PlantedC4", "m_flC4Blow");
	offsetz.DT_PlantedC4.m_bBombDefused = NetVarManager->GetOffset("DT_PlantedC4", "m_bBombDefused");
	offsetz.DT_PlantedC4.m_hBombDefuser = NetVarManager->GetOffset("DT_PlantedC4", "m_hBombDefuser");
	offsetz.DT_PlantedC4.m_flDefuseCountDown = NetVarManager->GetOffset("DT_PlantedC4", "m_flDefuseCountDown");

	offsetz.DT_CSPlayer.m_iShotsFired = NetVarManager->GetOffset("DT_CSPlayer", "m_iShotsFired");
	offsetz.DT_CSPlayer.m_angEyeAngles[0] = NetVarManager->GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
	offsetz.DT_CSPlayer.m_angEyeAngles[1] = NetVarManager->GetOffset("DT_CSPlayer", "m_angEyeAngles[1]");
	offsetz.DT_CSPlayer.m_iAccount = NetVarManager->GetOffset("DT_CSPlayer", "m_iAccount");
	offsetz.DT_CSPlayer.m_totalHitsOnServer = NetVarManager->GetOffset("DT_CSPlayer", "m_totalHitsOnServer");
	offsetz.DT_CSPlayer.m_ArmorValue = NetVarManager->GetOffset("DT_CSPlayer", "m_ArmorValue");
	offsetz.DT_CSPlayer.m_bIsDefusing = NetVarManager->GetOffset("DT_CSPlayer", "m_bIsDefusing");
	offsetz.DT_CSPlayer.m_bIsGrabbingHostage = NetVarManager->GetOffset("DT_CSPlayer", "m_bIsGrabbingHostage");
	offsetz.DT_CSPlayer.m_bIsScoped = NetVarManager->GetOffset("DT_CSPlayer", "m_bIsScoped");
	offsetz.DT_CSPlayer.m_bGunGameImmunity = NetVarManager->GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
	offsetz.DT_CSPlayer.m_bIsRescuing = NetVarManager->GetOffset("DT_CSPlayer", "m_bIsRescuing");
	offsetz.DT_CSPlayer.m_bHasHelmet = NetVarManager->GetOffset("DT_CSPlayer", "m_bHasHelmet");
	offsetz.DT_CSPlayer.m_bHasDefuser = NetVarManager->GetOffset("DT_CSPlayer", "m_bHasDefuser");
	offsetz.DT_CSPlayer.m_flFlashDuration = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashDuration");
	offsetz.DT_CSPlayer.m_flFlashMaxAlpha = NetVarManager->GetOffset("DT_CSPlayer", "m_flFlashMaxAlpha");
	offsetz.DT_CSPlayer.m_flLowerBodyYawTarget = NetVarManager->GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
    offsetz.DT_CSPlayer.m_bHasHeavyArmor = NetVarManager->GetOffset("DT_CSPlayer", "m_bHasHeavyArmor");
	offsetz.DT_CSPlayer.m_iGunGameProgressiveWeaponIndex = NetVarManager->GetOffset("DT_CSPlayer", "m_iGunGameProgressiveWeaponIndex");

	offsetz.DT_BaseAttributableItem.m_iItemDefinitionIndex = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	offsetz.DT_BaseAttributableItem.m_iAccountID = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_iAccountID");
	offsetz.DT_BaseAttributableItem.m_iEntityQuality = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_iEntityQuality");
	offsetz.DT_BaseAttributableItem.m_szCustomName = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_szCustomName");
	offsetz.DT_BaseAttributableItem.m_nFallbackPaintKit = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");
	offsetz.DT_BaseAttributableItem.m_nFallbackSeed = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
	offsetz.DT_BaseAttributableItem.m_flFallbackWear = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
	offsetz.DT_BaseAttributableItem.m_nFallbackStatTrak = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
	offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidLow = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidHigh = NetVarManager->GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");

	offsetz.DT_BaseViewModel.m_nModelIndex = NetVarManager->GetOffset("DT_BaseViewModel", "m_nModelIndex");
	offsetz.DT_BaseViewModel.m_hWeapon = NetVarManager->GetOffset("DT_BaseViewModel", "m_hWeapon");
	offsetz.DT_BaseViewModel.m_hOwner = NetVarManager->GetOffset("DT_BaseViewModel", "m_hOwner");

	offsetz.DT_WeaponCSBase.m_fAccuracyPenalty = NetVarManager->GetOffset("DT_WeaponCSBase", "m_fAccuracyPenalty");
    offsetz.DT_WeaponCSBase.m_flPostponeFireReadyTime = NetVarManager->GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");

	offsetz.DT_WeaponC4.m_bStartedArming = NetVarManager->GetOffset("DT_WeaponC4", "m_bStartedArming");

	offsetz.DT_BaseCombatWeapon.m_flNextPrimaryAttack = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	offsetz.DT_BaseCombatWeapon.m_iViewModelIndex = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
	offsetz.DT_BaseCombatWeapon.m_iWorldModelIndex = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
	offsetz.DT_BaseCombatWeapon.m_hOwner = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_hOwner");
	offsetz.DT_BaseCombatWeapon.m_iClip1 = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_iClip1");
	offsetz.DT_BaseCombatWeapon.m_bInReload = offsetz.DT_BaseCombatWeapon.m_flNextPrimaryAttack + 113;
    offsetz.DT_BaseCombatWeapon.m_hWeaponWorldModel = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
	offsetz.DT_BaseCombatWeapon.m_iReserve = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
	offsetz.DT_BaseCombatWeapon.m_hOwner = NetVarManager->GetOffset("DT_BaseCombatWeapon", "m_hOwner");


	offsetz.DT_BaseCSGrenade.m_bRedraw = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_bRedraw");
	offsetz.DT_BaseCSGrenade.m_bIsHeldByPlayer = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_bIsHeldByPlayer");
	offsetz.DT_BaseCSGrenade.m_bPinPulled = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_bPinPulled");
	offsetz.DT_BaseCSGrenade.m_fThrowTime = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
	offsetz.DT_BaseCSGrenade.m_bLoopingSoundPlaying = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_bLoopingSoundPlaying");
	offsetz.DT_BaseCSGrenade.m_flThrowStrength = NetVarManager->GetOffset("DT_BaseCSGrenade", "m_flThrowStrength");

	offsetz.DT_DynamicProp.m_bShouldGlow = NetVarManager->GetOffset("DT_DynamicProp", "m_bShouldGlow");

	offsetz.DT_CSGameRulesProxy.m_bBombPlanted = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bBombPlanted");
	offsetz.DT_CSGameRulesProxy.m_bIsValveDS = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bIsValveDS");

	offsetz.DT_CSGameRulesProxy.m_bFreezePeriod = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bFreezePeriod");
	offsetz.DT_CSGameRulesProxy.m_bMatchWaitingForResume = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bMatchWaitingForResume");
	offsetz.DT_CSGameRulesProxy.m_bWarmupPeriod = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bWarmupPeriod");
	offsetz.DT_CSGameRulesProxy.m_fWarmupPeriodEnd = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_fWarmupPeriodEnd");
	offsetz.DT_CSGameRulesProxy.m_fWarmupPeriodStart = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_fWarmupPeriodStart");
	offsetz.DT_CSGameRulesProxy.m_bTerroristTimeOutActive = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bTerroristTimeOutActive");
	offsetz.DT_CSGameRulesProxy.m_bCTTimeOutActive = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bCTTimeOutActive");
	offsetz.DT_CSGameRulesProxy.m_flTerroristTimeOutRemaining = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flTerroristTimeOutRemaining");
	offsetz.DT_CSGameRulesProxy.m_flCTTimeOutRemaining = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flCTTimeOutRemaining");
	offsetz.DT_CSGameRulesProxy.m_nTerroristTimeOuts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nTerroristTimeOuts");
	offsetz.DT_CSGameRulesProxy.m_nCTTimeOuts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nCTTimeOuts");
	offsetz.DT_CSGameRulesProxy.m_iRoundTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iRoundTime");
	offsetz.DT_CSGameRulesProxy.m_gamePhase = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_gamePhase");
	offsetz.DT_CSGameRulesProxy.m_totalRoundsPlayed = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_totalRoundsPlayed");
	offsetz.DT_CSGameRulesProxy.m_nOvertimePlaying = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nOvertimePlaying");
	offsetz.DT_CSGameRulesProxy.m_timeUntilNextPhaseStarts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_timeUntilNextPhaseStarts");
	offsetz.DT_CSGameRulesProxy.m_flCMMItemDropRevealStartTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flCMMItemDropRevealStartTime");
	offsetz.DT_CSGameRulesProxy.m_flCMMItemDropRevealEndTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flCMMItemDropRevealEndTime");
	offsetz.DT_CSGameRulesProxy.m_fRoundStartTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_fRoundStartTime");
	offsetz.DT_CSGameRulesProxy.m_bGameRestart = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bGameRestart");
	offsetz.DT_CSGameRulesProxy.m_flRestartRoundTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flRestartRoundTime");
	offsetz.DT_CSGameRulesProxy.m_flGameStartTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flGameStartTime");
	offsetz.DT_CSGameRulesProxy.m_iHostagesRemaining = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iHostagesRemaining");
	offsetz.DT_CSGameRulesProxy.m_bAnyHostageReached = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bAnyHostageReached");
	offsetz.DT_CSGameRulesProxy.m_bMapHasBombTarget = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bMapHasBombTarget");
	offsetz.DT_CSGameRulesProxy.m_bMapHasRescueZone = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bMapHasRescueZone");
	offsetz.DT_CSGameRulesProxy.m_bMapHasBuyZone = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bMapHasBuyZone");
	offsetz.DT_CSGameRulesProxy.m_bIsQueuedMatchmaking = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bIsQueuedMatchmaking");
	offsetz.DT_CSGameRulesProxy.m_bIsValveDS = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bIsValveDS");
	offsetz.DT_CSGameRulesProxy.m_bIsQuestEligible = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bIsQuestEligible");
	offsetz.DT_CSGameRulesProxy.m_bLogoMap = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bLogoMap");
	offsetz.DT_CSGameRulesProxy.m_iNumGunGameProgressiveWeaponsCT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iNumGunGameProgressiveWeaponsCT");
	offsetz.DT_CSGameRulesProxy.m_iNumGunGameProgressiveWeaponsT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iNumGunGameProgressiveWeaponsT");
	offsetz.DT_CSGameRulesProxy.m_iSpectatorSlotCount = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iSpectatorSlotCount");
	offsetz.DT_CSGameRulesProxy.m_bBombDropped = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bBombDropped");
	offsetz.DT_CSGameRulesProxy.m_bBombPlanted = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bBombPlanted");
	offsetz.DT_CSGameRulesProxy.m_iRoundWinStatus = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iRoundWinStatus");
	offsetz.DT_CSGameRulesProxy.m_eRoundWinReason = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_eRoundWinReason");
	offsetz.DT_CSGameRulesProxy.m_flDMBonusStartTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flDMBonusStartTime");
	offsetz.DT_CSGameRulesProxy.m_flDMBonusTimeLength = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flDMBonusTimeLength");
	offsetz.DT_CSGameRulesProxy.m_unDMBonusWeaponLoadoutSlot = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_unDMBonusWeaponLoadoutSlot");
	offsetz.DT_CSGameRulesProxy.m_bDMBonusActive = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bDMBonusActive");
	offsetz.DT_CSGameRulesProxy.m_bTCantBuy = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bTCantBuy");
	offsetz.DT_CSGameRulesProxy.m_bCTCantBuy = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bCTCantBuy");
	offsetz.DT_CSGameRulesProxy.m_flGuardianBuyUntilTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flGuardianBuyUntilTime");
	offsetz.DT_CSGameRulesProxy.m_iMatchStats_RoundResults = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iMatchStats_RoundResults");
	offsetz.DT_CSGameRulesProxy.m_iMatchStats_PlayersAlive_T = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_T");
	offsetz.DT_CSGameRulesProxy.m_iMatchStats_PlayersAlive_CT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iMatchStats_PlayersAlive_CT");
	offsetz.DT_CSGameRulesProxy.m_GGProgressiveWeaponOrderC = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_GGProgressiveWeaponOrderC");
	offsetz.DT_CSGameRulesProxy.m_GGProgressiveWeaponOrderT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_GGProgressiveWeaponOrderT");
	offsetz.DT_CSGameRulesProxy.m_GGProgressiveWeaponKillUpgradeOrderCT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_GGProgressiveWeaponKillUpgradeOrderCT");
	offsetz.DT_CSGameRulesProxy.m_GGProgressiveWeaponKillUpgradeOrderT = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_GGProgressiveWeaponKillUpgradeOrderT");
	offsetz.DT_CSGameRulesProxy.m_MatchDevice = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_MatchDevice");
	offsetz.DT_CSGameRulesProxy.m_bHasMatchStarted = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bHasMatchStarted");
	offsetz.DT_CSGameRulesProxy.m_TeamRespawnWaveTimes = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_TeamRespawnWaveTimes");
	offsetz.DT_CSGameRulesProxy.m_flNextRespawnWave = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_flNextRespawnWave");
	offsetz.DT_CSGameRulesProxy.m_nNextMapInMapgroup = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nNextMapInMapgroup");
	offsetz.DT_CSGameRulesProxy.m_nEndMatchMapGroupVoteOptions = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nEndMatchMapGroupVoteOptions");
	offsetz.DT_CSGameRulesProxy.m_bIsDroppingItems = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_bIsDroppingItems");
	offsetz.DT_CSGameRulesProxy.m_iActiveAssassinationTargetMissionID = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_iActiveAssassinationTargetMissionID");
	offsetz.DT_CSGameRulesProxy.m_fMatchStartTime = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_fMatchStartTime");
	offsetz.DT_CSGameRulesProxy.m_szTournamentEventName = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_szTournamentEventName");
	offsetz.DT_CSGameRulesProxy.m_szTournamentEventStage = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_szTournamentEventStage");
	offsetz.DT_CSGameRulesProxy.m_szTournamentPredictionsTxt = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_szTournamentPredictionsTxt");
	offsetz.DT_CSGameRulesProxy.m_nTournamentPredictionsPct = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nTournamentPredictionsPct");
	offsetz.DT_CSGameRulesProxy.m_szMatchStatTxt = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_szMatchStatTxt");
	offsetz.DT_CSGameRulesProxy.m_nGuardianModeWaveNumber = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nGuardianModeWaveNumber");
	offsetz.DT_CSGameRulesProxy.m_nGuardianModeSpecialKillsRemaining = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nGuardianModeSpecialKillsRemaining");
	offsetz.DT_CSGameRulesProxy.m_nGuardianModeSpecialWeaponNeeded = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nGuardianModeSpecialWeaponNeeded");
	offsetz.DT_CSGameRulesProxy.m_nHalloweenMaskListSeed = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_nHalloweenMaskListSeed");
	offsetz.DT_CSGameRulesProxy.m_numGlobalGiftsGiven = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_numGlobalGiftsGiven");
	offsetz.DT_CSGameRulesProxy.m_numGlobalGifters = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_numGlobalGifters");
	offsetz.DT_CSGameRulesProxy.m_numGlobalGiftsPeriodSeconds = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_numGlobalGiftsPeriodSeconds");
	offsetz.DT_CSGameRulesProxy.m_arrFeaturedGiftersAccounts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_arrFeaturedGiftersAccounts");
	offsetz.DT_CSGameRulesProxy.m_arrFeaturedGiftersGifts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_arrFeaturedGiftersGifts");
	offsetz.DT_CSGameRulesProxy.m_arrTournamentActiveCasterAccounts = NetVarManager->GetOffset("DT_CSGameRulesProxy", "m_arrTournamentActiveCasterAccounts");

    offsetz.DT_BaseAnimating.m_nHitboxSet = NetVarManager->GetOffset("DT_BaseAnimating", "m_nHitboxSet");

}

Offsetz* g_Netvars = new(Offsetz);
COffsetz offsetz;

namespace junk5405243 {
	void junk8609679() {
		return;
		float hwd2r;
		float zb7bmt;
		float wqflef;
		float p6y2qc;
		float ztk5la;
		float f2cbqb;
		float pbpd4;
		float vkv9ji;
		float d1tijd;
		float ytkrhs;
		for (int kwjvrj = 0; kwjvrj > 100; kwjvrj++)
		{
			p6y2qc = 6351301.0520;
		}
		hwd2r = 9048489.8741;
		while (hwd2r == 7190897.5181)
		{
			hwd2r = 7576788.9551;
		}
		ytkrhs = 3830294.7249;
		if (d1tijd == 3625741.1904)
			d1tijd = 1525327.6937;
		ztk5la = 2550081.2645;
		for (int rvs505 = 0; rvs505 > 100; rvs505++)
		{
			zb7bmt = 1246762.8988;
		}
		vkv9ji = 9229535.0825;
		while (p6y2qc == 10230215.8822)
		{
			p6y2qc = 10218043.3778;
		}
		ztk5la = 10538315.7240;
		for (int upc3t = 0; upc3t > 100; upc3t++)
		{
			vkv9ji = 8482664.0040;
		}
		f2cbqb = 1364117.9296;
		for (int tx1hli = 0; tx1hli > 100; tx1hli++)
		{
			p6y2qc = 5076034.6828;
		}
		ytkrhs = 4702194.0529;
		if (f2cbqb == 9650991.9885)
			f2cbqb = 5733356.6682;
		pbpd4 = 4330826.3175;
		if (vkv9ji == 1233924.3125)
			vkv9ji = 4177980.7562;
		p6y2qc = 9923925.7377;
		for (int m6dpk8 = 0; m6dpk8 > 100; m6dpk8++)
		{
			wqflef = 679013.1907;
		}
		ztk5la = 10196097.0713;
	}
	void junk7866525() {
		return;
		float ligja;
		float aj8qka;
		float b9x842;
		float wvk7gr;
		float jw9bof;
		float n0976m;
		float cpczph;
		float hrxcot;
		float bt0dok;
		float t4v028;
		while (b9x842 == 2489113.3175)
		{
			b9x842 = 1801311.1036;
		}
		n0976m = 9214715.3709;
		if (ligja == 8428346.5048)
			ligja = 3203491.0406;
		n0976m = 5387587.3406;
		while (n0976m == 4309684.0554)
		{
			n0976m = 9579276.7162;
		}
		jw9bof = 6234561.0964;
		for (int labutq = 0; labutq > 100; labutq++)
		{
			n0976m = 4477383.2674;
		}
		n0976m = 7666900.6297;
		for (int bt4y8b = 0; bt4y8b > 100; bt4y8b++)
		{
			jw9bof = 5787353.0625;
		}
		jw9bof = 2353498.4050;
		for (int dwuqxd = 0; dwuqxd > 100; dwuqxd++)
		{
			cpczph = 541684.0662;
		}
		jw9bof = 7402387.3570;
		for (int iru55 = 0; iru55 > 100; iru55++)
		{
			aj8qka = 1128076.7608;
		}
		b9x842 = 362476.7958;
		while (aj8qka == 8708197.6640)
		{
			aj8qka = 9020664.5640;
		}
		b9x842 = 1815469.4048;
		while (n0976m == 2170450.0566)
		{
			n0976m = 7573046.8991;
		}
		cpczph = 6798654.1230;
		b9x842 = 2120543.7700;
	}
	void junk4761470() {
		return;
		float hdkewc;
		float aum6o3;
		float xydqfa;
		float jqon7;
		float y5949u;
		float luttoq;
		float yulmq8;
		float h756m9;
		float shqk5;
		float ka6kzr;
		aum6o3 = 7693960.5200;
		while (yulmq8 == 8152525.7606)
		{
			yulmq8 = 8676632.1631;
		}
		ka6kzr = 9974147.7051;
		if (shqk5 == 4720685.0017)
			shqk5 = 2823998.5643;
		ka6kzr = 276685.8700;
		while (h756m9 == 7928321.4034)
		{
			h756m9 = 812107.6467;
		}
		luttoq = 2668221.0768;
		for (int r32q76 = 0; r32q76 > 100; r32q76++)
		{
			shqk5 = 7884344.3298;
		}
		shqk5 = 9342146.7322;
		jqon7 = 7386999.8579;
		if (aum6o3 == 1550965.0885)
			aum6o3 = 5616599.0020;
		xydqfa = 4510496.2904;
		for (int sozltk = 0; sozltk > 100; sozltk++)
		{
			ka6kzr = 7671302.2802;
		}
		shqk5 = 7347881.5952;
		if (yulmq8 == 5894357.6201)
			yulmq8 = 2375819.0176;
		luttoq = 3098945.6638;
		for (int bfy90r = 0; bfy90r > 100; bfy90r++)
		{
			hdkewc = 6590940.7930;
		}
		luttoq = 7014947.1057;
	}
	void junk6084037() {
		return;
		float bsu2xa;
		float p2tk0i;
		float eu3lnq;
		float x2hgiu;
		float r0d1yt;
		float z8kw87;
		float tqq2oh;
		float lw4bs;
		float r7sur;
		float ic01e;
		for (int i7hkns = 0; i7hkns > 100; i7hkns++)
		{
			z8kw87 = 10098285.5228;
		}
		r0d1yt = 8982704.9427;
		if (lw4bs == 5512857.5511)
			lw4bs = 5070331.8768;
		z8kw87 = 4870869.5328;
		while (r0d1yt == 4756936.6394)
		{
			r0d1yt = 10188183.1713;
		}
		lw4bs = 9631506.0712;
		for (int g1q2hy = 0; g1q2hy > 100; g1q2hy++)
		{
			x2hgiu = 6020121.1111;
		}
		lw4bs = 2518568.0877;
		while (z8kw87 == 8773932.8481)
		{
			z8kw87 = 1819090.4203;
		}
		lw4bs = 1241607.1457;
		bsu2xa = 10239131.8204;
		for (int x2fs3 = 0; x2fs3 > 100; x2fs3++)
		{
			z8kw87 = 7244618.7435;
		}
		lw4bs = 8100575.5441;
		z8kw87 = 8149142.7074;
		while (ic01e == 4231786.0107)
		{
			ic01e = 9896682.9185;
		}
		r0d1yt = 5059076.7964;
		for (int c2g6nr = 0; c2g6nr > 100; c2g6nr++)
		{
			lw4bs = 4866525.8773;
		}
		bsu2xa = 6006534.6299;
	}
	void junk1676462() {
		return;
		float a50ica;
		float fgfx2f;
		float oqrgaj;
		float b0m5vb;
		float ho72wg;
		float mtq00h;
		float rrlr8f;
		float buatnp;
		float po2ple;
		float m309ai;
		while (b0m5vb == 13708.0153)
		{
			b0m5vb = 4954558.7723;
		}
		fgfx2f = 5180860.0144;
		b0m5vb = 460142.5841;
		while (rrlr8f == 4124225.3007)
		{
			rrlr8f = 3419614.3136;
		}
		ho72wg = 9727290.7528;
		if (rrlr8f == 1188863.0328)
			rrlr8f = 3468427.3503;
		oqrgaj = 847007.7248;
		if (b0m5vb == 492129.7733)
			b0m5vb = 6504220.1030;
		mtq00h = 8044904.0501;
		while (m309ai == 6071504.9455)
		{
			m309ai = 1205784.7690;
		}
		rrlr8f = 2351369.0302;
		while (ho72wg == 3505650.5285)
		{
			ho72wg = 9019337.3683;
		}
		buatnp = 2838240.3000;
		while (b0m5vb == 7889917.2774)
		{
			b0m5vb = 7934772.6711;
		}
		a50ica = 189139.4513;
		a50ica = 10491146.7423;
		while (po2ple == 9779087.0973)
		{
			po2ple = 2340540.8764;
		}
		b0m5vb = 5309128.3006;
	}
	void junk8500192() {
		return;
		float tbaer;
		float y4r4n;
		float gw5w;
		float a126vr;
		float f9xhaf;
		float jgmo1o;
		float qdma6f;
		float c64yn7x;
		float rehbpu;
		float zxth5t;
		while (f9xhaf == 1403152.1134)
		{
			f9xhaf = 9223594.5797;
		}
		y4r4n = 3155758.1234;
		a126vr = 4766801.6291;
		for (int zxtww = 0; zxtww > 100; zxtww++)
		{
			jgmo1o = 4071909.0685;
		}
		rehbpu = 8349685.4942;
		y4r4n = 3958880.2698;
		if (f9xhaf == 5176911.9340)
			f9xhaf = 216511.4717;
		a126vr = 3280143.4403;
		for (int gukxouj = 0; gukxouj > 100; gukxouj++)
		{
			c64yn7x = 6679272.9200;
		}
		c64yn7x = 8206313.8734;
		if (jgmo1o == 8386340.7337)
			jgmo1o = 4579792.9227;
		qdma6f = 6794026.5164;
		if (y4r4n == 2339901.3855)
			y4r4n = 6885731.5754;
		tbaer = 6477308.3297;
		if (zxth5t == 3642434.4896)
			zxth5t = 7902454.3507;
		rehbpu = 6955433.9330;
		if (rehbpu == 6017918.7117)
			rehbpu = 3154239.7049;
		y4r4n = 207722.7940;
	}
	void junk3509011() {
		return;
		float pe535i;
		float nhizf;
		float wr7fho;
		float punn7;
		float wmaob;
		float btf35r;
		float lezip;
		float lacsdq;
		float o1qgsp;
		float xmulwc;
		if (btf35r == 936620.0972)
			btf35r = 1056208.9918;
		o1qgsp = 7252070.9326;
		pe535i = 6393883.3283;
		pe535i = 3055759.8476;
		for (int t3g52 = 0; t3g52 > 100; t3g52++)
		{
			lezip = 297600.0451;
		}
		nhizf = 428233.3736;
		while (wmaob == 7270471.9377)
		{
			wmaob = 6792660.2738;
		}
		btf35r = 2273277.1731;
		while (lacsdq == 7792603.5617)
		{
			lacsdq = 930669.2610;
		}
		nhizf = 368893.3919;
		for (int sbyk7h = 0; sbyk7h > 100; sbyk7h++)
		{
			btf35r = 2243830.8930;
		}
		punn7 = 6807827.3223;
		if (lezip == 652440.6570)
			lezip = 1813327.9545;
		pe535i = 4375606.6992;
		if (lacsdq == 4425242.2591)
			lacsdq = 529607.2544;
		lezip = 2418040.2005;
		while (wmaob == 2596517.1517)
		{
			wmaob = 4580220.1035;
		}
		pe535i = 7034093.1148;
	}
	void junk1200026() {
		return;
		float h2p76n;
		float gy5yk;
		float mh2988;
		float n7swln;
		float f99yx;
		float x2h9yc;
		float i6i0oe;
		float p02zw;
		float ydnsbb;
		float g8i5pb;
		x2h9yc = 4714822.3605;
		f99yx = 6644069.2361;
		for (int xqr55c = 0; xqr55c > 100; xqr55c++)
		{
			i6i0oe = 396442.0643;
		}
		i6i0oe = 2480254.8076;
		f99yx = 3660083.2265;
		while (ydnsbb == 2986476.0193)
		{
			ydnsbb = 5043333.2414;
		}
		gy5yk = 3585211.5612;
		for (int fqpht = 0; fqpht > 100; fqpht++)
		{
			i6i0oe = 2821662.4287;
		}
		x2h9yc = 9658376.4443;
		while (i6i0oe == 1898376.9763)
		{
			i6i0oe = 2086664.6281;
		}
		i6i0oe = 9593781.4735;
		p02zw = 4308620.9842;
		if (g8i5pb == 3695934.3425)
			g8i5pb = 3529270.3169;
		ydnsbb = 10417185.1741;
		if (x2h9yc == 3750228.2625)
			x2h9yc = 20585.5050;
		i6i0oe = 2980128.6922;
	}
	void junk324913() {
		return;
		float lgwnv;
		float cg6u7d;
		float abgjrf;
		float ec6v4w;
		float tj22w;
		float bndkt7;
		float fn9u95;
		float tgkr6;
		float vvi4x;
		float z4ouw;
		if (bndkt7 == 3900172.8236)
			bndkt7 = 4405482.2838;
		abgjrf = 6591600.5873;
		if (tj22w == 1085301.9662)
			tj22w = 1093074.9434;
		lgwnv = 7239289.8391;
		z4ouw = 8693121.1335;
		for (int fuome = 0; fuome > 100; fuome++)
		{
			z4ouw = 3165879.9490;
		}
		lgwnv = 6122218.2074;
		while (tj22w == 4375079.9078)
		{
			tj22w = 10336852.4198;
		}
		fn9u95 = 781892.3105;
		ec6v4w = 5307842.4830;
		tj22w = 10133453.0114;
		if (lgwnv == 2684215.0210)
			lgwnv = 2722105.3741;
		bndkt7 = 5367074.9024;
		while (z4ouw == 9037906.7136)
		{
			z4ouw = 7166133.0356;
		}
		lgwnv = 2689345.2173;
		cg6u7d = 5707695.3371;
	}
	void junk8502465() {
		return;
		float j1m2s;
		float uyvaf;
		float e7rd09;
		float lyv6me;
		float lnvfyu;
		float hggvsjr;
		float opz11q;
		float i207xlq;
		float pdmap9;
		float t6rnms;
		for (int j3dze = 0; j3dze > 100; j3dze++)
		{
			t6rnms = 9550111.0843;
		}
		lyv6me = 3911499.7541;
		for (int fns3c = 0; fns3c > 100; fns3c++)
		{
			e7rd09 = 6519869.3059;
		}
		e7rd09 = 6396350.8438;
		j1m2s = 5657376.0122;
		for (int mz5kij = 0; mz5kij > 100; mz5kij++)
		{
			lyv6me = 5691213.1920;
		}
		pdmap9 = 1457419.5671;
		i207xlq = 8695610.0304;
		for (int vcpyyn = 0; vcpyyn > 100; vcpyyn++)
		{
			lnvfyu = 5522606.3084;
		}
		hggvsjr = 2446081.0972;
		while (uyvaf == 7899289.6905)
		{
			uyvaf = 8519864.0409;
		}
		lnvfyu = 1006137.3902;
		while (pdmap9 == 3345648.3076)
		{
			pdmap9 = 6328018.0390;
		}
		opz11q = 2532246.5913;
		e7rd09 = 6624756.3920;
		for (int a8ikqh = 0; a8ikqh > 100; a8ikqh++)
		{
			opz11q = 104061.5365;
		}
		pdmap9 = 507900.6299;
	}
	void doJunk() {
		junk5405243::junk8609679();
		junk5405243::junk7866525();
		junk5405243::junk4761470();
		junk5405243::junk6084037();
		junk5405243::junk1676462();
		junk5405243::junk8500192();
		junk5405243::junk3509011();
		junk5405243::junk1200026();
		junk5405243::junk324913();
		junk5405243::junk8502465();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class agfzbsn {
public:
	double raave;
	int pkvob;
	bool zzsmqxedndmjj;
	agfzbsn();
	void harxnhijffwrejze(string lpdzdluzeqiymio, string tzhndmyyhqk, bool ztxwtyxxtnmfo);
	int linvwdfppgjjcianw(string obljwktju, bool mgoallncnrv, bool onzauoudgld, string gmtawvnpogajtar, bool dmmtxgetpwzajh, string mneombdf);
	bool zltzbubnidxhrtfpooxpfutix(double xnkbz, bool zdjptjro, string bbxtfwnxinqkw, int fdcopdxpncfgcx, double jvqpkek, bool zbbkteboes);
	string utnqjikilphehxjajn();
	bool xvvktysngpmddn(string hhobeifilcxuygy);
	bool eexdnlerwyblovxrynmjp(bool rswbcy, int cxjhhfozch, int krxdnkwfdjnnod, bool luonbamvpbmmtjt, bool bmmyfnymbpuciq);
	double bzlutitjjykkhztqshdqrt(double puyyohkjmwdo, bool mnvpejpmtaqezpw, int wcbhvxgfg, double froaaiaqtoyjpqc, int owebgogacghz, double xlvszjzjmbda, double fzkiiwcevnzwx, int fmlftcdpq, int dyvduvwode, string htrvz);
	string rqzjwnyhvfbvqig(bool krjxlzbsevrocnt, bool lngvybwo, string rrxmnprx, int wkcegnbvszp, bool ovpud, int vciljacd, string bsatetyu, string lidgpb);

protected:
	string adukr;
	double orrtmnugxyqmdtp;

	bool jwxhnokqnsfreilfffdpouckg(bool fojblduhem, string mcpstyuxhi, int cpfgjn, bool hcbsleyucrlk, string elkfivnxhu, int frdcxogjmyxc, bool mpsupvvnkwirvv, bool gzgffnvpafjd, string qwztsmxnwcrnty, int flxarzmnzs);
	int qeqxiykxcuzrrisk(bool ddjpfyyobcfsil, int xfvefpcabfsmzrb, int axsvauwi, double zhjuhapibwbfpv, string yyvxvs);

private:
	bool dzuejwzj;
	double fqrllvv;
	bool dgskiijl;

	void znuzkwwqawagjarwvcbuksg(double iibjcdrsxyay, double ejzogy, string yohglmhfnegzon, bool ezeuwrehbjl, bool eslfuiks, int ribygbay);
	double sldfutpfwfrtgbnyvsp(double nkacqnqbzxhbszj);
	bool xfllxphbyzztpvytsxne(bool ukauc, string cvjikz, double fvgtcmirgtqkmug);
	string oycmxswxlrkaxcisyriswxa(string bavudeecemqi, bool xhyuy, int uumhbiawun, double kyqcczdkitanw);
	void znqazuodcbibx(double chvzumpcq, string hkbytqisrcqjla, int dsizpenrqmjrtgw, string wutkuwbriftwbea, bool evewxp, string iegbvwlflr, bool lfyhfhfp, string taskwlum, double avjeaqwgudhxzui);
	bool fmkswqdtbrjkgtjryrjeaycj(double zipkulwgvoo, int haukuxaljdw, int joindfr, int frxnq);
	int tcomnuwrnfkgevhahp(bool oyeyxcucagtg);

};


void agfzbsn::znuzkwwqawagjarwvcbuksg(double iibjcdrsxyay, double ejzogy, string yohglmhfnegzon, bool ezeuwrehbjl, bool eslfuiks, int ribygbay) {
	bool hqjqbgcxvzqg = false;
	bool qfsfexk = false;
	double mezppiozzeavo = 1471;
	if (false == false) {
		int vbatfbfscj;
		for (vbatfbfscj = 7; vbatfbfscj > 0; vbatfbfscj--) {
			continue;
		}
	}
	if (false != false) {
		int jeqfoxlg;
		for (jeqfoxlg = 70; jeqfoxlg > 0; jeqfoxlg--) {
			continue;
		}
	}
	if (false != false) {
		int hxekikqtv;
		for (hxekikqtv = 1; hxekikqtv > 0; hxekikqtv--) {
			continue;
		}
	}
	if (false != false) {
		int hyur;
		for (hyur = 93; hyur > 0; hyur--) {
			continue;
		}
	}
	if (1471 == 1471) {
		int midjkshgmb;
		for (midjkshgmb = 11; midjkshgmb > 0; midjkshgmb--) {
			continue;
		}
	}

}

double agfzbsn::sldfutpfwfrtgbnyvsp(double nkacqnqbzxhbszj) {
	return 377;
}

bool agfzbsn::xfllxphbyzztpvytsxne(bool ukauc, string cvjikz, double fvgtcmirgtqkmug) {
	bool gwjflrysaa = true;
	string zxyvigwwvjzcdt = "noqgmoziiudeceupefttijqlrbtxhlskjdlzzkdacaxxasvpqc";
	int gkqndyy = 3198;
	if (string("noqgmoziiudeceupefttijqlrbtxhlskjdlzzkdacaxxasvpqc") != string("noqgmoziiudeceupefttijqlrbtxhlskjdlzzkdacaxxasvpqc")) {
		int fz;
		for (fz = 18; fz > 0; fz--) {
			continue;
		}
	}
	if (true != true) {
		int ae;
		for (ae = 45; ae > 0; ae--) {
			continue;
		}
	}
	if (true != true) {
		int lgioy;
		for (lgioy = 63; lgioy > 0; lgioy--) {
			continue;
		}
	}
	if (3198 != 3198) {
		int zarhlbnro;
		for (zarhlbnro = 20; zarhlbnro > 0; zarhlbnro--) {
			continue;
		}
	}
	if (3198 == 3198) {
		int gyjja;
		for (gyjja = 72; gyjja > 0; gyjja--) {
			continue;
		}
	}
	return false;
}

string agfzbsn::oycmxswxlrkaxcisyriswxa(string bavudeecemqi, bool xhyuy, int uumhbiawun, double kyqcczdkitanw) {
	double tdsvnbilje = 64415;
	string yjmzcgnlcizy = "udqwkanlnnplpqjrycydqqufgtyjpvjppaoqufsvsunccwlixipunumbirjahcbooglljjfkkydhxghgmfgkabzepdpdbm";
	double yneqqobf = 22199;
	string ktxxdqpxkuswet = "yefvgfpxidvkvfzeghcfkzmbkbwxnrgaxytattopmhseltgetnjjyag";
	int hxrtttwyy = 4214;
	bool gzrkjrco = false;
	bool sfriiecrnok = false;
	double votwyokickjgz = 20006;
	if (64415 != 64415) {
		int zlcovl;
		for (zlcovl = 27; zlcovl > 0; zlcovl--) {
			continue;
		}
	}
	return string("ooazjiyajnfffhxj");
}

void agfzbsn::znqazuodcbibx(double chvzumpcq, string hkbytqisrcqjla, int dsizpenrqmjrtgw, string wutkuwbriftwbea, bool evewxp, string iegbvwlflr, bool lfyhfhfp, string taskwlum, double avjeaqwgudhxzui) {
	string ppjtshkmsxkexlt = "ycecywxbrdytvocbsuyulnzyecbqwxvjjvtopmqltbrghhjclifzojsbjl";
	int dhbrzpaa = 853;
	string tveccniwps = "faverwyqexzzpachgzpgxquuccfmaotiyvsvbydcqftvsklpffvvwfnurkkxwwsleeqmdbq";
	int fxhnnujbx = 2108;
	string dstnfaugcreg = "otogokswfvncjzoxbkgra";
	int kjpvqkiniseb = 2366;
	if (string("faverwyqexzzpachgzpgxquuccfmaotiyvsvbydcqftvsklpffvvwfnurkkxwwsleeqmdbq") != string("faverwyqexzzpachgzpgxquuccfmaotiyvsvbydcqftvsklpffvvwfnurkkxwwsleeqmdbq")) {
		int wkk;
		for (wkk = 83; wkk > 0; wkk--) {
			continue;
		}
	}

}

bool agfzbsn::fmkswqdtbrjkgtjryrjeaycj(double zipkulwgvoo, int haukuxaljdw, int joindfr, int frxnq) {
	string zltoudvbwwfl = "lpylcevqlgcryov";
	double gnvwbp = 62674;
	double zyxjd = 8542;
	string wwzrwurddr = "swntkiczueaopqrcbtuplurvzauznhllssjgzglbaibpcnijgdgdqmzswshnfdjrqh";
	string pljnuenugeixj = "fxcugzphafzlfcjfpvpqivdscmuvkhkqnycptpeweiucjsnyxxefidlhscgqhntrhp";
	bool dcbvyehbpvuydc = true;
	double arsegdjl = 22742;
	string ioxexp = "qcxasdwmqpuwwdgimqceqrnpixelkfgkkkc";
	return false;
}

int agfzbsn::tcomnuwrnfkgevhahp(bool oyeyxcucagtg) {
	string rltblxdhuwfmixp = "setfixngnlgxuhfbjxumbkxij";
	string zfxxzkyududg = "hxbvbqzhbfjxtnswybkccscgwevviiriakeovkjkgyxmjzh";
	string bxgcfibxf = "ndnucybhebhsbjhwgqzjvywequzqnelkatbxybhadqnnhgwrzlmdxhijkwrshblszpouxbdjeilam";
	bool rxmfxkj = false;
	int gbwxhqjmhxoagi = 2747;
	double oajfrxh = 2508;
	string ayrwsxsgx = "ebzwahotyvagldwhyjogyylyaxhqhrw";
	string hwgarwgagkkwz = "gyyrcfnthtbdttjlwfj";
	int lswhnut = 2633;
	string zinuxae = "anjdeudtiwunjtzgdbzikctivbltvnlyjpbscnozymucxztczdchlpnxbzdkpunvpspgfcscihmuyjnpcpyroh";
	if (string("ndnucybhebhsbjhwgqzjvywequzqnelkatbxybhadqnnhgwrzlmdxhijkwrshblszpouxbdjeilam") == string("ndnucybhebhsbjhwgqzjvywequzqnelkatbxybhadqnnhgwrzlmdxhijkwrshblszpouxbdjeilam")) {
		int iamsgvjsnm;
		for (iamsgvjsnm = 15; iamsgvjsnm > 0; iamsgvjsnm--) {
			continue;
		}
	}
	if (string("ndnucybhebhsbjhwgqzjvywequzqnelkatbxybhadqnnhgwrzlmdxhijkwrshblszpouxbdjeilam") != string("ndnucybhebhsbjhwgqzjvywequzqnelkatbxybhadqnnhgwrzlmdxhijkwrshblszpouxbdjeilam")) {
		int ewxheaplno;
		for (ewxheaplno = 77; ewxheaplno > 0; ewxheaplno--) {
			continue;
		}
	}
	if (false == false) {
		int ypup;
		for (ypup = 29; ypup > 0; ypup--) {
			continue;
		}
	}
	return 49777;
}

bool agfzbsn::jwxhnokqnsfreilfffdpouckg(bool fojblduhem, string mcpstyuxhi, int cpfgjn, bool hcbsleyucrlk, string elkfivnxhu, int frdcxogjmyxc, bool mpsupvvnkwirvv, bool gzgffnvpafjd, string qwztsmxnwcrnty, int flxarzmnzs) {
	string tlfiqluiuiewyi = "swmdppllttrzpgmcpkgvpehccxwqaqhnypthvkajyoqsjpgxgsbslkcpvladqhocykyocedasqpq";
	double gdcnyem = 19327;
	bool euwladvcnqvt = false;
	string oreqnecvho = "gidnfyuwjcahrxkkvhsipwiyuipscdruwxefpwhtysifniwrhxlpnjlqsbzko";
	double uiwlihjkqizluii = 9210;
	string ivikj = "ohlvanw";
	if (string("ohlvanw") != string("ohlvanw")) {
		int tbjlmcy;
		for (tbjlmcy = 52; tbjlmcy > 0; tbjlmcy--) {
			continue;
		}
	}
	if (false == false) {
		int vlwfumgthv;
		for (vlwfumgthv = 1; vlwfumgthv > 0; vlwfumgthv--) {
			continue;
		}
	}
	if (19327 != 19327) {
		int kqqdndg;
		for (kqqdndg = 23; kqqdndg > 0; kqqdndg--) {
			continue;
		}
	}
	if (19327 == 19327) {
		int andudokb;
		for (andudokb = 59; andudokb > 0; andudokb--) {
			continue;
		}
	}
	if (string("ohlvanw") != string("ohlvanw")) {
		int wecuth;
		for (wecuth = 91; wecuth > 0; wecuth--) {
			continue;
		}
	}
	return false;
}

int agfzbsn::qeqxiykxcuzrrisk(bool ddjpfyyobcfsil, int xfvefpcabfsmzrb, int axsvauwi, double zhjuhapibwbfpv, string yyvxvs) {
	int afivhaupxyv = 1102;
	double zdkwcnjyntc = 3025;
	bool veaaanny = false;
	string krcql = "wurbsyfr";
	string xtjon = "kwnqknrxhmohnyx";
	double zeyfsijnu = 41091;
	bool xkvwvqicqkyppn = false;
	string lesog = "vxd";
	if (string("kwnqknrxhmohnyx") != string("kwnqknrxhmohnyx")) {
		int kzbj;
		for (kzbj = 11; kzbj > 0; kzbj--) {
			continue;
		}
	}
	if (1102 != 1102) {
		int dwbzchvn;
		for (dwbzchvn = 55; dwbzchvn > 0; dwbzchvn--) {
			continue;
		}
	}
	if (1102 != 1102) {
		int cwbjgz;
		for (cwbjgz = 32; cwbjgz > 0; cwbjgz--) {
			continue;
		}
	}
	return 30734;
}

void agfzbsn::harxnhijffwrejze(string lpdzdluzeqiymio, string tzhndmyyhqk, bool ztxwtyxxtnmfo) {
	string xcdeqevikxutnbk = "euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry";
	if (string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry") != string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry")) {
		int xfrwfz;
		for (xfrwfz = 68; xfrwfz > 0; xfrwfz--) {
			continue;
		}
	}
	if (string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry") != string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry")) {
		int hlcxa;
		for (hlcxa = 3; hlcxa > 0; hlcxa--) {
			continue;
		}
	}
	if (string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry") != string("euxesgrafmkkjhadqtvcromdpyrmpawihlovxcqpacznuwfzqlagkbrtucctnsry")) {
		int rxffddrjc;
		for (rxffddrjc = 68; rxffddrjc > 0; rxffddrjc--) {
			continue;
		}
	}

}

int agfzbsn::linvwdfppgjjcianw(string obljwktju, bool mgoallncnrv, bool onzauoudgld, string gmtawvnpogajtar, bool dmmtxgetpwzajh, string mneombdf) {
	string vcswyzloim = "ayydwjdclgnriumlpzbyvgouooiguxevxpezlbdhdpea";
	double mlugobjglq = 28255;
	string fawsdrhtqd = "txtleiyurqfnpouklmcbhqzrioqvuqnwkjgusapxgwjheigvfuktdzvxwyfdrfnhyvsufdqj";
	double mqtzwwdxk = 83760;
	return 71542;
}

bool agfzbsn::zltzbubnidxhrtfpooxpfutix(double xnkbz, bool zdjptjro, string bbxtfwnxinqkw, int fdcopdxpncfgcx, double jvqpkek, bool zbbkteboes) {
	bool yvpoklluzjhw = false;
	double uxzcokw = 15024;
	double nsdse = 18324;
	bool dgmauipdgkvw = true;
	string dtqfivkkvvehopc = "heuhzxmdtdzckswnfe";
	string jmbgjbt = "chojtbfznfiqjgyktlp";
	double ncveuulala = 48813;
	string caufncqxyihtmpy = "bucg";
	double ehsgyirxwhu = 2757;
	if (18324 != 18324) {
		int rkwkpj;
		for (rkwkpj = 72; rkwkpj > 0; rkwkpj--) {
			continue;
		}
	}
	if (string("heuhzxmdtdzckswnfe") != string("heuhzxmdtdzckswnfe")) {
		int vega;
		for (vega = 15; vega > 0; vega--) {
			continue;
		}
	}
	if (15024 == 15024) {
		int rdcamrujl;
		for (rdcamrujl = 85; rdcamrujl > 0; rdcamrujl--) {
			continue;
		}
	}
	return false;
}

string agfzbsn::utnqjikilphehxjajn() {
	string ltfacstgzjzbgw = "hsjxrmajqirbljpwnrloujsylgfxzlysblkbhjiayltjbfmcxmanhjyvfdmqnajptyplhoriaxbmwvotbhuruqncmqtm";
	int csarxjdqng = 6002;
	string wglduu = "iinhgjccnucaqarhpveiebggzzujrdivfzfchwfnjg";
	double krqemsdoguqi = 62458;
	int nwofxgap = 984;
	int nqlbyphox = 328;
	double swgtyji = 61348;
	if (string("iinhgjccnucaqarhpveiebggzzujrdivfzfchwfnjg") != string("iinhgjccnucaqarhpveiebggzzujrdivfzfchwfnjg")) {
		int ffr;
		for (ffr = 78; ffr > 0; ffr--) {
			continue;
		}
	}
	if (61348 == 61348) {
		int rtbjps;
		for (rtbjps = 17; rtbjps > 0; rtbjps--) {
			continue;
		}
	}
	if (6002 == 6002) {
		int kuykidyt;
		for (kuykidyt = 3; kuykidyt > 0; kuykidyt--) {
			continue;
		}
	}
	return string("idmnpxmlaeoqknmgegq");
}

bool agfzbsn::xvvktysngpmddn(string hhobeifilcxuygy) {
	string hjcaomqf = "gzlwblmmbgxwkigzuozsishjrvcaosxrljtnbowwiyudrjslwxhusgfhpwvmbwmvarqjlmwzwmsdyoehrswkvhhzbyjqxnoyhjm";
	int ndqsgzsty = 3199;
	string ngqnpgkv = "rrjuhzspohzarkuvqnkkbvisxxsxwgbvropjqijfmhbdpjnlqrdxxxgkwvhauakwmvwoikudkhvz";
	bool yhxxn = true;
	double ekixihw = 5004;
	double qwyihpeedvxgpb = 54212;
	bool vhysbojqbsun = false;
	int srwncvcxrmo = 2191;
	bool lrcnolanwh = false;
	return false;
}

bool agfzbsn::eexdnlerwyblovxrynmjp(bool rswbcy, int cxjhhfozch, int krxdnkwfdjnnod, bool luonbamvpbmmtjt, bool bmmyfnymbpuciq) {
	string hzxvahjnaftmtk = "nsqwmaonmuhipwnmxkywwlfywrvnfxh";
	double gqgcvzwgz = 24273;
	double boizbbwhbbs = 10492;
	double qkxygt = 85540;
	double bnzwqc = 7956;
	double ukajjnwyxu = 31328;
	int faybinti = 7448;
	bool kdvhsg = true;
	if (7956 != 7956) {
		int rnubmkl;
		for (rnubmkl = 69; rnubmkl > 0; rnubmkl--) {
			continue;
		}
	}
	if (true == true) {
		int gy;
		for (gy = 46; gy > 0; gy--) {
			continue;
		}
	}
	if (85540 == 85540) {
		int jyixjvg;
		for (jyixjvg = 33; jyixjvg > 0; jyixjvg--) {
			continue;
		}
	}
	return false;
}

double agfzbsn::bzlutitjjykkhztqshdqrt(double puyyohkjmwdo, bool mnvpejpmtaqezpw, int wcbhvxgfg, double froaaiaqtoyjpqc, int owebgogacghz, double xlvszjzjmbda, double fzkiiwcevnzwx, int fmlftcdpq, int dyvduvwode, string htrvz) {
	bool kyrqsfoptruyflg = false;
	string qlgwxtzxknlqmj = "nko";
	int qjfujvseeyfnaqe = 1891;
	double bovhtycp = 7867;
	string rskzt = "rzykirbvwrlazflphmfzeadpuefoaqhouepyrevkhhonlrtpvdlsva";
	double ksdewqg = 13351;
	bool mrbrsmtcwtwuy = false;
	double ktzoezwt = 11880;
	return 98418;
}

string agfzbsn::rqzjwnyhvfbvqig(bool krjxlzbsevrocnt, bool lngvybwo, string rrxmnprx, int wkcegnbvszp, bool ovpud, int vciljacd, string bsatetyu, string lidgpb) {
	int pigzpjngtxy = 209;
	bool pusufvc = true;
	return string("fbzvla");
}

agfzbsn::agfzbsn() {
	this->harxnhijffwrejze(string("yecjcduetbmabwmeximgzucoyslwxcupualtuow"), string("gzztaspwmqnlrynigpbunllentfmccqbumabddrekjkoxewtzzawcbrotmfkcwtgtmuxcbkjbmmleufhu"), false);
	this->linvwdfppgjjcianw(string("qwrpajqcuxdeeiqikyigkmusndpmvunwxzazvoixycgksqewfcppegxfqmomzdwxbmxzndg"), true, false, string("vgyfecbzfzfymndhdzxjls"), false, string(""));
	this->zltzbubnidxhrtfpooxpfutix(25154, true, string("mudxp"), 1908, 63540, true);
	this->utnqjikilphehxjajn();
	this->xvvktysngpmddn(string(""));
	this->eexdnlerwyblovxrynmjp(false, 374, 2035, true, true);
	this->bzlutitjjykkhztqshdqrt(10952, false, 5947, 29747, 1599, 37165, 21133, 421, 6862, string("mhyulheauotkwjfqmhqqjfzcczhkjvsuhestcgntblgemy"));
	this->rqzjwnyhvfbvqig(true, true, string("dujqafzqksxfudddlwusufzxumrjayhgulycukdbzhpaccwnyaeirfazfubpismxeyahnadzshodumgwcucemyjxwbrvtwckme"), 1525, false, 2931, string("wxbgbvqzevmttcgptbnwxmtyeigozybqhghfeeebuenvavlitfvlzhiifvtjbfatetidztbyuwq"), string("kljillifpalhmkczvzamvllwyqsjbuawtpsluqwqzmctjqzqhoxgmifqyd"));
	this->jwxhnokqnsfreilfffdpouckg(false, string("itiok"), 1273, false, string("aeqsrxxehrdvyesikegjlsucwgjrmatcqbfoczpqrqbawrtmibanqf"), 2768, true, true, string("deqecfnfuzcmurevmgiiprjjwx"), 1340);
	this->qeqxiykxcuzrrisk(true, 5193, 5716, 14286, string("ioustqtim"));
	this->znuzkwwqawagjarwvcbuksg(33511, 43775, string("njngfdjbtuduatysjdmwxtdvbrgllrkepkozdwiffrwbbdaaulque"), true, false, 7455);
	this->sldfutpfwfrtgbnyvsp(9094);
	this->xfllxphbyzztpvytsxne(false, string("t"), 19505);
	this->oycmxswxlrkaxcisyriswxa(string("opigkrfolmnrvyvvtknwiiuvnjyogprtdtixkfqll"), false, 1632, 38408);
	this->znqazuodcbibx(21858, string("khfxuzcdqdspvijwamsqftrurzvmchjxwbhpaknfeehthqcvvjkuzdmwnbdanborkdkntnyznx"), 532, string("drdlvraerxbxfjryuqrhi"), false, string("znhmhdh"), false, string("ybmcsrfmrkuyctcupsbsq"), 10243);
	this->fmkswqdtbrjkgtjryrjeaycj(56970, 4468, 1406, 7966);
	this->tcomnuwrnfkgevhahp(false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class kehmicw {
public:
	int fzaouukiyr;
	double qqwvkcr;
	string fdyxwitnurawxm;
	int hjesckbawibb;
	bool iqmedtfv;
	kehmicw();
	bool wcpsezdhkaerpiaz(bool jxvygluxkw, bool isratmwhmfb, double jtbpmxkfrylygbk, bool rurvpfvutlwcttj);
	string ayfgpwqnlno(int gahlkllyuaokws);
	string isikwkegrscncwghlpbgvtxrn(int yivduxcc, bool jwutygoq, string vilnkhjsvwsd);
	bool kchgaeaurd(string ljacqzgjfkjxb, int gxnjkvlnwsbjfhw, string kaxgonefqag, bool runipxxbmxh, string jbmkgplvquh, bool uljzybjzz, bool wseaq, double bjynudkodmch, string vprenygmbfjhw, string eyoeqylneogt);
	int evkiigptjfbjw(double gvnorapnoev, double wlkkn, double kijtfrdw, double oeelbopyvpyot, string fljvsfxpfa, bool xzlumjtmoozbix, double ylwztbgw, string aoutwjnkuzaf);
	int vjfeaygmariq(string vxmqqcl);
	int gqzwldzchiuqymzpox(int yuixpytyfk, string dwhwkcnz, bool qaltoeio, string lfzasbwnhd, double eukdud);
	int nsklrjmqalexekywknakclnit(string iibsfusunojxv, string tdyzt, bool clmcrouggnls, int tbjreqzi, int fkwpctzv);
	void aryluzuupzgar(int hmincjqrzras, string kwlccrndjzqcse, bool vxhpf, bool nvsidwvkcxmxx, int tbqnsaesiedfzpa);

protected:
	int ecclzpdqvto;
	bool xejzx;

	double dmguvtxcatkphipqnz(double kxemnksbm, string htdnfcgqph, int xaesopvkjdrjc, bool vfpgmejltuqr);

private:
	string anacioafn;

	void wmlzgpmmyepquadxcrkxd(double xcrffnkmwule, bool nzuxvflvn, bool hwnoclcp, string iscyrizmizjgb, int coxxkstbjgr, string vbcrjwdcyi, bool xwvygn, double raodbzwwrmnyyq, int sqjuxn);
	double revtkfbkpohi(string fzpljggwmoio, int movbxxobr, string frfdeaakxbwn);
	bool pzjnblragrzkmiukzk(string whdiympzomt, int fmlignzdrldvvpf, double mymkookrlep, string srwvfrby, string zkcaodgpnhcxh, bool zgydw, string qhijpkll, int lxzcejuzynqrprg);
	double owcbuoddypmrwzhjbutx(bool gcdxufxyrgx, int nqypbz);
	string jzgfdyihpiadc(double ntahmjnizuisds, bool jtacbvw, double nuowzxtqai);

};


void kehmicw::wmlzgpmmyepquadxcrkxd(double xcrffnkmwule, bool nzuxvflvn, bool hwnoclcp, string iscyrizmizjgb, int coxxkstbjgr, string vbcrjwdcyi, bool xwvygn, double raodbzwwrmnyyq, int sqjuxn) {
	string sxebgrwggdvk = "sgdvudbqnzapacjmgrigycsaeqqgwgidtuwshtkxgpijmtaieayqdibgqngpgsszijvzumrvz";
	double ecgtwgeldcwv = 32889;
	bool zuytclwxh = false;
	string cofrqetap = "zgpzjhujsazcnmnkxcktukvfevpcrgxfssqsxwmaapbsbumjkqkyubppmyiskovqhvykpbcwqebiohjmyexjyq";
	double pxhgh = 53972;
	bool mhqcaznijdtv = false;
	double bnqqmav = 14778;
	double ntvyy = 7738;
	if (false != false) {
		int qvpkj;
		for (qvpkj = 32; qvpkj > 0; qvpkj--) {
			continue;
		}
	}
	if (string("sgdvudbqnzapacjmgrigycsaeqqgwgidtuwshtkxgpijmtaieayqdibgqngpgsszijvzumrvz") == string("sgdvudbqnzapacjmgrigycsaeqqgwgidtuwshtkxgpijmtaieayqdibgqngpgsszijvzumrvz")) {
		int fcbz;
		for (fcbz = 40; fcbz > 0; fcbz--) {
			continue;
		}
	}
	if (14778 == 14778) {
		int jcp;
		for (jcp = 25; jcp > 0; jcp--) {
			continue;
		}
	}
	if (53972 == 53972) {
		int fgofojlp;
		for (fgofojlp = 59; fgofojlp > 0; fgofojlp--) {
			continue;
		}
	}

}

double kehmicw::revtkfbkpohi(string fzpljggwmoio, int movbxxobr, string frfdeaakxbwn) {
	bool xjtzrfwirpyz = true;
	string lnoychuwdcoicr = "";
	if (true == true) {
		int qrixmjir;
		for (qrixmjir = 53; qrixmjir > 0; qrixmjir--) {
			continue;
		}
	}
	if (true != true) {
		int aovyd;
		for (aovyd = 6; aovyd > 0; aovyd--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int oyqau;
		for (oyqau = 89; oyqau > 0; oyqau--) {
			continue;
		}
	}
	if (true == true) {
		int xn;
		for (xn = 15; xn > 0; xn--) {
			continue;
		}
	}
	return 80858;
}

bool kehmicw::pzjnblragrzkmiukzk(string whdiympzomt, int fmlignzdrldvvpf, double mymkookrlep, string srwvfrby, string zkcaodgpnhcxh, bool zgydw, string qhijpkll, int lxzcejuzynqrprg) {
	double edowoqlprevb = 65449;
	double lmomkwaqwrg = 20546;
	if (20546 != 20546) {
		int htoxazm;
		for (htoxazm = 13; htoxazm > 0; htoxazm--) {
			continue;
		}
	}
	if (20546 == 20546) {
		int hb;
		for (hb = 45; hb > 0; hb--) {
			continue;
		}
	}
	if (65449 == 65449) {
		int gwkd;
		for (gwkd = 40; gwkd > 0; gwkd--) {
			continue;
		}
	}
	if (20546 == 20546) {
		int sbqwjjbaul;
		for (sbqwjjbaul = 22; sbqwjjbaul > 0; sbqwjjbaul--) {
			continue;
		}
	}
	if (65449 == 65449) {
		int kgsjbsxs;
		for (kgsjbsxs = 28; kgsjbsxs > 0; kgsjbsxs--) {
			continue;
		}
	}
	return false;
}

double kehmicw::owcbuoddypmrwzhjbutx(bool gcdxufxyrgx, int nqypbz) {
	int cbssbqgdijjxgc = 733;
	double cqpkdiikpue = 11821;
	int uktbtpc = 3085;
	string smrnobcc = "xztvptkccnwjriogpniqjednjukhhnmlzmeseychylvdufgmelkqjbizahcmqtrpfh";
	if (3085 != 3085) {
		int agjaflgd;
		for (agjaflgd = 90; agjaflgd > 0; agjaflgd--) {
			continue;
		}
	}
	if (string("xztvptkccnwjriogpniqjednjukhhnmlzmeseychylvdufgmelkqjbizahcmqtrpfh") == string("xztvptkccnwjriogpniqjednjukhhnmlzmeseychylvdufgmelkqjbizahcmqtrpfh")) {
		int chqr;
		for (chqr = 36; chqr > 0; chqr--) {
			continue;
		}
	}
	if (string("xztvptkccnwjriogpniqjednjukhhnmlzmeseychylvdufgmelkqjbizahcmqtrpfh") == string("xztvptkccnwjriogpniqjednjukhhnmlzmeseychylvdufgmelkqjbizahcmqtrpfh")) {
		int khilh;
		for (khilh = 1; khilh > 0; khilh--) {
			continue;
		}
	}
	if (733 == 733) {
		int bumxlth;
		for (bumxlth = 62; bumxlth > 0; bumxlth--) {
			continue;
		}
	}
	return 31302;
}

string kehmicw::jzgfdyihpiadc(double ntahmjnizuisds, bool jtacbvw, double nuowzxtqai) {
	int deksuscuhuhbd = 5045;
	double slbxtnbukzu = 26719;
	double emoykdxhkfpao = 17999;
	int qxholmgl = 3822;
	bool jdadmhftmdhqkz = false;
	double nbeqjoiu = 18551;
	if (26719 == 26719) {
		int mo;
		for (mo = 37; mo > 0; mo--) {
			continue;
		}
	}
	if (3822 == 3822) {
		int edbmupd;
		for (edbmupd = 98; edbmupd > 0; edbmupd--) {
			continue;
		}
	}
	if (26719 == 26719) {
		int zjwku;
		for (zjwku = 94; zjwku > 0; zjwku--) {
			continue;
		}
	}
	if (17999 != 17999) {
		int gd;
		for (gd = 59; gd > 0; gd--) {
			continue;
		}
	}
	if (17999 != 17999) {
		int waaoaoc;
		for (waaoaoc = 9; waaoaoc > 0; waaoaoc--) {
			continue;
		}
	}
	return string("crhvipfkjof");
}

double kehmicw::dmguvtxcatkphipqnz(double kxemnksbm, string htdnfcgqph, int xaesopvkjdrjc, bool vfpgmejltuqr) {
	double jryax = 7029;
	double ujvdfvbuje = 532;
	if (7029 == 7029) {
		int xtrndkan;
		for (xtrndkan = 57; xtrndkan > 0; xtrndkan--) {
			continue;
		}
	}
	if (7029 != 7029) {
		int eznjzcg;
		for (eznjzcg = 57; eznjzcg > 0; eznjzcg--) {
			continue;
		}
	}
	if (7029 != 7029) {
		int ec;
		for (ec = 64; ec > 0; ec--) {
			continue;
		}
	}
	if (7029 == 7029) {
		int pks;
		for (pks = 3; pks > 0; pks--) {
			continue;
		}
	}
	if (532 != 532) {
		int iuoaeayzc;
		for (iuoaeayzc = 59; iuoaeayzc > 0; iuoaeayzc--) {
			continue;
		}
	}
	return 95978;
}

bool kehmicw::wcpsezdhkaerpiaz(bool jxvygluxkw, bool isratmwhmfb, double jtbpmxkfrylygbk, bool rurvpfvutlwcttj) {
	return true;
}

string kehmicw::ayfgpwqnlno(int gahlkllyuaokws) {
	int nkbrgiuj = 4034;
	int lyaqoyslaoy = 3103;
	double sirutoqgkm = 10216;
	string ohfkktkzhcymkh = "ipoqwterbfqibgkvkusfcpqghlsxgyylmrbqr";
	if (10216 == 10216) {
		int rzijqczxw;
		for (rzijqczxw = 92; rzijqczxw > 0; rzijqczxw--) {
			continue;
		}
	}
	return string("ygqdo");
}

string kehmicw::isikwkegrscncwghlpbgvtxrn(int yivduxcc, bool jwutygoq, string vilnkhjsvwsd) {
	return string("pqdktqtjdtosv");
}

bool kehmicw::kchgaeaurd(string ljacqzgjfkjxb, int gxnjkvlnwsbjfhw, string kaxgonefqag, bool runipxxbmxh, string jbmkgplvquh, bool uljzybjzz, bool wseaq, double bjynudkodmch, string vprenygmbfjhw, string eyoeqylneogt) {
	string ilytxvs = "wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa";
	if (string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa") != string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa")) {
		int gdsvwff;
		for (gdsvwff = 15; gdsvwff > 0; gdsvwff--) {
			continue;
		}
	}
	if (string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa") == string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa")) {
		int lsrhygzyb;
		for (lsrhygzyb = 35; lsrhygzyb > 0; lsrhygzyb--) {
			continue;
		}
	}
	if (string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa") != string("wvjjbjrojwrzoqbuhinbmecxgrzyjeqtreyrqsmvhgniaxnklpujhrthfiamnkyhjropa")) {
		int cuzq;
		for (cuzq = 45; cuzq > 0; cuzq--) {
			continue;
		}
	}
	return false;
}

int kehmicw::evkiigptjfbjw(double gvnorapnoev, double wlkkn, double kijtfrdw, double oeelbopyvpyot, string fljvsfxpfa, bool xzlumjtmoozbix, double ylwztbgw, string aoutwjnkuzaf) {
	int vnqemzqb = 4837;
	int bcnnqpot = 3247;
	string ybxehxwruvv = "tnftenncopyggiyyvelrdvmbvkutdbrxqdfuyzvnbqjglostyindfgtxozzoxziptdyospfacjypxefyfcymebkpjmlxnuukyko";
	string onfrjerc = "nlpcmpsfvgouhnrwnbrfaeacjtglfsmvxkxlgujclejxdyjonobquwqktoaennpoocshpnfmkpyevgbwkgxsr";
	bool pmnrqpdledil = true;
	int qzxlhfo = 3766;
	double niymanfxigx = 33003;
	double pnirwk = 1893;
	double bjnddwz = 12772;
	if (3247 != 3247) {
		int ij;
		for (ij = 39; ij > 0; ij--) {
			continue;
		}
	}
	return 84584;
}

int kehmicw::vjfeaygmariq(string vxmqqcl) {
	return 43604;
}

int kehmicw::gqzwldzchiuqymzpox(int yuixpytyfk, string dwhwkcnz, bool qaltoeio, string lfzasbwnhd, double eukdud) {
	string azmlgesu = "bcgdlomxtmeicafqdmvyetgqsaptwrqdcouwdjtdftllatnytfptgqxlpamhvquncourznwpekjmffltbawlehobfzoqsva";
	bool oqxiss = true;
	bool dtwuixt = false;
	bool wshnuptx = true;
	bool wamta = false;
	bool yotbxrsrupgqsr = true;
	if (true != true) {
		int plvn;
		for (plvn = 21; plvn > 0; plvn--) {
			continue;
		}
	}
	if (false == false) {
		int po;
		for (po = 74; po > 0; po--) {
			continue;
		}
	}
	if (true != true) {
		int zaybxgllz;
		for (zaybxgllz = 32; zaybxgllz > 0; zaybxgllz--) {
			continue;
		}
	}
	if (false != false) {
		int tbhlmrld;
		for (tbhlmrld = 11; tbhlmrld > 0; tbhlmrld--) {
			continue;
		}
	}
	if (true != true) {
		int yzdojdo;
		for (yzdojdo = 19; yzdojdo > 0; yzdojdo--) {
			continue;
		}
	}
	return 41529;
}

int kehmicw::nsklrjmqalexekywknakclnit(string iibsfusunojxv, string tdyzt, bool clmcrouggnls, int tbjreqzi, int fkwpctzv) {
	bool ckacba = true;
	string hakip = "kuyxjmopxfzhfbvovdaojwv";
	string gocltblfrypxvd = "sxthmhxzczryxwddjquximoqyllujbhbwcgqpghunlqijcqyrarrbdpyvjgtnrqiwbjupdliaiahcvvncvtukbhggpbj";
	string cefkrjkbn = "cgtbjzkmunpnqjyswqkgwr";
	int qfgdqwktwqukl = 4501;
	string wzwtjbkzbvgq = "khq";
	if (string("sxthmhxzczryxwddjquximoqyllujbhbwcgqpghunlqijcqyrarrbdpyvjgtnrqiwbjupdliaiahcvvncvtukbhggpbj") != string("sxthmhxzczryxwddjquximoqyllujbhbwcgqpghunlqijcqyrarrbdpyvjgtnrqiwbjupdliaiahcvvncvtukbhggpbj")) {
		int nwdw;
		for (nwdw = 36; nwdw > 0; nwdw--) {
			continue;
		}
	}
	if (4501 == 4501) {
		int zhr;
		for (zhr = 19; zhr > 0; zhr--) {
			continue;
		}
	}
	if (true == true) {
		int jrg;
		for (jrg = 50; jrg > 0; jrg--) {
			continue;
		}
	}
	return 57772;
}

void kehmicw::aryluzuupzgar(int hmincjqrzras, string kwlccrndjzqcse, bool vxhpf, bool nvsidwvkcxmxx, int tbqnsaesiedfzpa) {
	bool hefixxhnsqddpnz = false;
	bool lchqd = true;
	if (false == false) {
		int llgbwodgfh;
		for (llgbwodgfh = 57; llgbwodgfh > 0; llgbwodgfh--) {
			continue;
		}
	}
	if (true != true) {
		int bymrduoev;
		for (bymrduoev = 89; bymrduoev > 0; bymrduoev--) {
			continue;
		}
	}

}

kehmicw::kehmicw() {
	this->wcpsezdhkaerpiaz(true, true, 27562, true);
	this->ayfgpwqnlno(1170);
	this->isikwkegrscncwghlpbgvtxrn(2582, true, string("xsidvnzzxolfazapgzfjjunkpqdelxhmycabtlafgpwmkytt"));
	this->kchgaeaurd(string("tyxhwfyvizfxsqhciqfofqelfurqhrytjtemajsgnnudzobsjrjsrsrlqsgidpxttikfkzipgywpatxdfqbdqmfyepabzjgo"), 1876, string("hctadqbpiwfxdcbgszbanbi"), true, string("feyguuujbotbcrbcqgszmcdryorrbdvhdvntqqyxiwrfyaaldcturxxlvkxvnbnuuyushhshwxtdxjzuted"), false, true, 39745, string("gtartjuchqbdjwtyfccxxjmgaqwhy"), string("sycyewysbtjyqworqdyrqqdjhbbnjtbwomqdrxjvcxpzlnwswjregipcsmfnuvcqmaupmybxgktrrcjqsyeaazwmiod"));
	this->evkiigptjfbjw(32946, 18429, 15013, 46602, string("twxabkdogpgggfsanlqxlwvokhwqzxeljdkvhdhwncoxdoyraboykjmnndqrisbreib"), false, 7198, string("ozuscsoxhbleagmgxkympbixyauverpj"));
	this->vjfeaygmariq(string("kzqhpleqxjksiwlixsfqeomwrlollpddzvhzbgpqjoeivszpgvrgjktwcvertuyncouatpaej"));
	this->gqzwldzchiuqymzpox(7444, string("sdbunnegxjhjlglsggudlsp"), false, string("qnwohnzzsrirzfytjieoclypupppvfvrdxgwnibgaswpzvuaa"), 8947);
	this->nsklrjmqalexekywknakclnit(string("szaafkxzpivydgpuixtsrwccxfgrmpjxqmjythlhjwuoyyhdyludquvnpqcauphlyneufwaeklombhzokyuezokvsnmo"), string("obuidkophxrrmdrpxktoxbgenarsspdxkxpahvseyqdccgmmpmodfggilzregogtkvcwdvlnnxfcdxtdscdghntps"), true, 1187, 5214);
	this->aryluzuupzgar(0, string("tvnrhpdmyczhbbaegoeryjmozrhiqlmfmzmzardooc"), false, false, 4888);
	this->dmguvtxcatkphipqnz(35432, string("ylzhbmkhukqpizzzdgvkuiwevuxhkmxsqfbncflxl"), 3795, true);
	this->wmlzgpmmyepquadxcrkxd(25242, true, false, string("bcvehknolgktdwwmtbiwrpqvfvtzaxmxqndwmlvosxwmnihgvhvvojubgkwvecskbmtjys"), 1208, string("afzplmovdxdkmdloielggwrhjaszzkuwszzfchlrzbnavjnmovamcjamcisydafvk"), false, 34970, 3148);
	this->revtkfbkpohi(string("tbsugerxjhxxsuzdiawoljjabbbsugylugegjszejxogbcsrcozqiz"), 3065, string("nnbnmtkhbgfigobdsrjxduamiwenxkrspclejqnwsmpysppaehrxneolxumdmtcbujablczwsbznvqlvegrbqbchiwvqjiv"));
	this->pzjnblragrzkmiukzk(string("rwedkpxypmpkslqkevltkoamxnjlhavufmlszdwxjriljnzirbzpxrocisgvzelnzdbbhngyxhjkuhnudaojwqiupoqzkneynf"), 6933, 4090, string("wbyxoajgpxx"), string("phueaxtawenxnpaosdfulfpmdfqiagoazrjdnwkammexkothoqwbvzoilvwpwrjtqyimlasuvgjvhzgvhccqgmvwnhvhjelrc"), false, string("nbspdtlhvfslvainjfkkeejbzilduyjatobgiwpyctbsemja"), 2209);
	this->owcbuoddypmrwzhjbutx(true, 484);
	this->jzgfdyihpiadc(5283, false, 22999);
}
