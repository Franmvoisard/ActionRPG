// Fill out your copyright notice in the Description page of Project Settings.

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
