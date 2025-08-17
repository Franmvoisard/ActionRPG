// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FInteractionComponent.h"
#include "FGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteraction(TEXT("ar.DebugInteraction"), false, TEXT("Enable interaction drawing."), ECVF_Cheat);
// Sets default values for this component's properties
UFInteractionComponent::UFInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxInteractionDistance = 400;
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
	AActor* InteractableActor = nullptr;
	for(FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<UFGameplayInterface>())
			{
				InteractableActor = HitActor;
				APawn* MyPawn = Cast<APawn>(Owner);
				IFGameplayInterface::Execute_Interact(HitActor, MyPawn);
				break;
			}	
		}
	}
	if (InteractableActor && CVarDebugDrawInteraction.GetValueOnGameThread())
	{
		DrawDebugSphere(GetWorld(), Hits.Last().ImpactPoint, Radius, 32, LineColor, false, 2.0f);
		DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f,0, 2.0f);
	}
}

