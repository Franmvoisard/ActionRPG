// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.

#include "FItemChest.h"

#include "Net/UnrealNetwork.h"


// Sets default values

AFItemChest::AFItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMesh;
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
	LidMesh->SetupAttachment(BaseMesh);
	bReplicates = true;
}

void AFItemChest::OnActorLoaded_Implementation()
{
	OnRep_LidOpened();
}

void AFItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bLidOpened = !bLidOpened;
	OnRep_LidOpened();
}

void AFItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFItemChest, bLidOpened);
}

void AFItemChest::OnRep_LidOpened()
{
	float CurrentPitch = bLidOpened ? 110 : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0, 0));
}

