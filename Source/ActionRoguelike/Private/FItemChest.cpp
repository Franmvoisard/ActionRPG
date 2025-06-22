// Fill out your copyright notice in the Description page of Project Settings.


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

void AFItemChest::BeginPlay()
{
	Super::BeginPlay();
}

void AFItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

