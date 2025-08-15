// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FItemChest.h"


// Sets default values
AFItemChest::AFItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
}

void AFItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	LidMesh->SetRelativeRotation(FRotator(110, 0, 0));
}

