// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.#pragma once
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FFlashOnHitComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API UFFlashOnHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFFlashOnHitComponent();

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	float FlashHitSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	FColor FlashColor;

	void OnHit(UMeshComponent* MeshComponent);
};
