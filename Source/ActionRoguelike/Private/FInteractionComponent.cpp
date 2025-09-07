// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FInteractionComponent.h"
#include "DebugCVar.h"
#include "FGameplayInterface.h"

// Sets default values for this component's properties
UFInteractionComponent::UFInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxInteractionDistance = 400;
	TraceRadius = 30;
}

void UFInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void UFInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		DEBUG_ONLY(
			if (DebugCVar::IsInteractionDebugEnabled())
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("No interactable found!"));
			}
		)
	}
	else
	{
		APawn* MyPawn = Cast<APawn>(GetOwner());
		IFGameplayInterface::Execute_Interact(InFocus, MyPawn);
	}
}

void UFInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}

void UFInteractionComponent::FindBestInteractable()
{
	FocusedActor = nullptr;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);
	AActor* Owner = GetOwner();

	FVector EyeLocation;
	FRotator EyeRotation;
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	FVector End = EyeLocation + EyeRotation.Vector() * MaxInteractionDistance;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	TArray<FHitResult> Hits;
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	 
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	for(FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<UFGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<UFWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{ 
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	
	if (FocusedActor)
	{
		DEBUG_ONLY(
			if (DebugCVar::IsInteractionDebugEnabled())
			{
				DrawDebugSphere(GetWorld(), Hits.Last().ImpactPoint, TraceRadius, 32, LineColor, false, 2.0f);
				DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.0f,0, 2.0f);
			}
		)
	}
}