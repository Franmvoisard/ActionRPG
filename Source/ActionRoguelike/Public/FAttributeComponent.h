// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFAttributeComponent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float Health;

public:	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	
	bool ApplyHealthChange(float Delta);
};
