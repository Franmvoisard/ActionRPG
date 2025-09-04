// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "FWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "FInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API UFInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UFInteractionComponent();
	void PrimaryInteract();

protected:
	UPROPERTY()
	AActor* FocusedActor;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	float TraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	int MaxInteractionDistance;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TSubclassOf<UFWorldUserWidget> DefaultWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel;
	
	UPROPERTY() 
	UFWorldUserWidget* DefaultWidgetInstance;


	void FindBestInteractable();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
