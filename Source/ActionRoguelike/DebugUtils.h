#pragma once

#include "CoreMinimal.h"
#include "DebugCVar.h"
#include "DrawDebugHelpers.h"

namespace DebugUtils
{
	FORCEINLINE void DrawInteractionSphere(const UWorld* InWorld, const FVector& Center, float Radius, FColor Color,
	                                       float Duration = 2.0f)
	{
#if ENABLE_DRAW_DEBUG
		if (DebugCVar::IsInteractionDebugEnabled())
		{
			DrawDebugSphere(InWorld, Center, Radius, 32, Color, false, Duration);
		}
#endif
	}

	FORCEINLINE void DrawInteractionLine(const UWorld* InWorld, const FVector& Start, const FVector& End, FColor Color,
	                                     float Duration = 2.0f)
	{
#if ENABLE_DRAW_DEBUG
		if (DebugCVar::IsInteractionDebugEnabled())
		{
			DrawDebugLine(InWorld, Start, End, Color, false, Duration, 0, 1.0f);
		}
#endif
	}

	FORCEINLINE void DrawProjectilePath(const UWorld* InWorld, const FVector& Start, const FVector& End, FColor Color)
	{
#if ENABLE_DRAW_DEBUG
		if (DebugCVar::IsProjectilesDebugEnabled())
		{
			DrawDebugLine(InWorld, Start, End, Color, false, 5.0f, 0, 2.0f);
			DrawDebugPoint(InWorld, End, 10.0f, Color, false, 5.0f);
		}
#endif
	}

	FORCEINLINE void DrawDebugText(const UWorld* InWorld, const FVector& Location, const FString& Text,
	                               FColor Color = FColor::White)
	{
#if ENABLE_DRAW_DEBUG
		DrawDebugString(InWorld, Location, Text, nullptr, Color, 0.0f, true);
#endif
	}
	
	FORCEINLINE void DrawScreenText(const FString& Text, FColor Color = FColor::White, float TimeToDisplay = 1.0f)
	{
		GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, Color, Text);
	}
	
	namespace ActionSystem
	{
		FORCEINLINE void DebugLog(const FLogCategoryBase& Category, ELogVerbosity::Type Verbosity,
		                          const FString& Message)
		{
#if !UE_BUILD_SHIPPING
			if (DebugCVar::IsActionsDebugEnabled())
			{
				FMsg::Logf(nullptr, 0, Category.GetCategoryName(), Verbosity, TEXT("%s"), *Message);
			}
#endif
		}

		FORCEINLINE void DrawScreenText(const FString& Text, FColor Color = FColor::White, float TimeToDisplay = 1.0f)
		{
			if (DebugCVar::IsActionsDebugEnabled())
			{
				DebugUtils::DrawScreenText(Text, Color, TimeToDisplay);
			}
		}
		
		FORCEINLINE void DrawDebugText(const UWorld* InWorld, const FVector& Location, const FString& Text,
		                               FColor Color = FColor::White)
		{
			if (DebugCVar::IsActionsDebugEnabled())
			{
				DebugUtils::DrawDebugText(InWorld, Location, Text, Color);
			}
		}
	}
}
