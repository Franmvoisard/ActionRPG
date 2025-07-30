// Fill out your copyright notice in the Description page of Project Settings.
#include "FFlashOnHitComponent.h"

// Sets default values for this component's properties
UFFlashOnHitComponent::UFFlashOnHitComponent()
{
	FlashHitSpeed = 3.0f;
	FlashColor = FColor::Red;
}

void UFFlashOnHitComponent::OnHit(UMeshComponent* MeshComponent)
{
	const char* FlashHitSpeedParameterName = "TimeFlashTime";
	const char* FlashHitParameterName = "TimeToHit";
	const char* FlashColorParameterName = "Color";
	
	MeshComponent->SetScalarParameterValueOnMaterials(FlashHitParameterName, GetWorld()->TimeSeconds);
	MeshComponent->SetScalarParameterValueOnMaterials(FlashHitSpeedParameterName, FlashHitSpeed);
	MeshComponent->SetVectorParameterValueOnMaterials(FlashColorParameterName, FVector(FlashColor));
}
