#include "DebugCVar.h"

namespace DebugCVar
{
	bool bEnableInteractionDebug = false;
	bool bEnableProjectilesDebug = false;
	bool bEnableActionsDebug = false;
	bool bEnableBotSpawning = true;

	FAutoConsoleVariableRef CVarInteractionDebug(
		TEXT("ar.EnableInteractionDebug"),
		bEnableInteractionDebug,
		TEXT("Enable debugging for the interaction system."),
		ECVF_Cheat
	);

	FAutoConsoleVariableRef CVarProjectileDebug(
		TEXT("ar.Projectiles.DebugDrawEnabled"),
		bEnableProjectilesDebug,
		TEXT("Enable debug draw for projectiles"),
		ECVF_Cheat
	);

	FAutoConsoleVariableRef CVarActionsDebug(
		TEXT("ar.EnableActionsDebug"),
		bEnableActionsDebug,
		TEXT("Enable debugging for the action component system."),
		ECVF_Cheat
	);

	FAutoConsoleVariableRef CVarSpawnBots(
		TEXT("ar.SpawnBots"),
		bEnableBotSpawning,
		TEXT("Enable or disable bots spawning"),
		ECVF_Cheat
	);
}
