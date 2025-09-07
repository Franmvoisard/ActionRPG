#pragma once

// Helper macro to avoid shipping debug code
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#define DEBUG_ONLY(x) x
#else
#define DEBUG_ONLY(x)
#endif

class DebugCVar
{
public:
	static bool IsInteractionDebugEnabled();
	static bool IsProjectilesDebugEnabled();
	static bool IsActionsDebugEnabled();
	static bool IsBotSpawningEnabled();
};
