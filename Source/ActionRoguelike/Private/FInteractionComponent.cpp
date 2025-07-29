// Fill out your copyright notice in the Description page of Project Settings.


#include "FInteractionComponent.h"

#include "FGameplayInterface.h"

// Sets default values for this component's properties
UFInteractionComponent::UFInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxInteractionDistance = 400;
}

// Called every frame
void UFInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + EyeRotation.Vector() * MaxInteractionDistance;
	FCollisionShape Shape;
	float Radius = 30.0f;
	Shape.SetSphere(Radius);
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	 
	for(FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<UFGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(Owner);
				IFGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}	
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 2.0f);
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f,0, 2.0f);
}

