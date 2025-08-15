// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#pragma once

#include "CoreMinimal.h"
#include "FGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "FItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API AFItemChest : public AActor, public IFGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

public:
	AFItemChest();
};
