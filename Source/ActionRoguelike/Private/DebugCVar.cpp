#include "DebugCVar.h"

static TAutoConsoleVariable CVarDebugInteraction(TEXT("ar.EnableInteractionDebug"), false, TEXT("Enable debugging for all the interaction system."), ECVF_Cheat);
static TAutoConsoleVariable CVarDebugProjectilesEnabled(TEXT("ar.Projectiles.DebugDrawEnabled"), false, TEXT("Enable debug draw for projectiles"), ECVF_Cheat);
static TAutoConsoleVariable CVarDebugActions(TEXT("ar.EnableActionsDebug"), false, TEXT("Enable debugging for all the action component system."), ECVF_Cheat);
static TAutoConsoleVariable CVarSpawnBots(TEXT("ar.SpawnBots"), true, TEXT("Enable or disable bots spawning"), ECVF_Cheat);

bool DebugCVar::IsInteractionDebugEnabled()
{
	return CVarDebugInteraction.GetValueOnGameThread();
}

bool DebugCVar::IsProjectilesDebugEnabled()
{
	return CVarDebugProjectilesEnabled.GetValueOnGameThread();
}

bool DebugCVar::IsActionsDebugEnabled()
{
	return CVarDebugActions.GetValueOnGameThread();
}

bool DebugCVar::IsBotSpawningEnabled()
{
	return CVarSpawnBots.GetValueOnGameThread();
}


