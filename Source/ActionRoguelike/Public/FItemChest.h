// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFItemChest : public AActor, public IFGameplayInterface
{
	GENERATED_BODY()

	void Interact_Implementation(APawn* InstigatorPawn) override;
protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LidMesh;
	
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	AFItemChest();
};
