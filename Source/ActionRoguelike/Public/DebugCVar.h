#pragma once

#include "CoreMinimal.h"

namespace DebugCVar
{
	extern bool bEnableInteractionDebug;
	extern bool bEnableProjectilesDebug;
	extern bool bEnableActionsDebug;
	extern bool bEnableBotSpawning;

	FORCEINLINE bool IsInteractionDebugEnabled() { return bEnableInteractionDebug; }
	FORCEINLINE bool IsProjectilesDebugEnabled() { return bEnableProjectilesDebug; }
	FORCEINLINE bool IsActionsDebugEnabled() { return bEnableActionsDebug; }
	FORCEINLINE bool IsBotSpawningEnabled() { return bEnableBotSpawning; }
}
