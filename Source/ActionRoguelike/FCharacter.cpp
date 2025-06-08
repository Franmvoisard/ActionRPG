// Fill out your copyright notice in the Description page of Project Settings.

#include "FCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Sets default values
AFCharacter::AFCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AFCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(InputMappingContext,0);
		}
	}
}

void AFCharacter::MoveForward(const FInputActionValue& Value)
{
	AddMovementInput(GetActorForwardVector(), Value.GetMagnitude());
}

void AFCharacter::MoveLateral(const FInputActionValue& Value)
{
	AddMovementInput(GetActorRightVector(), Value.GetMagnitude());
}

// Called every frame
void AFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ForwardMovementAction, ETriggerEvent::Triggered, this, &AFCharacter::MoveForward);
		EnhancedInputComponent->BindAction(LateralMovementAction, ETriggerEvent::Triggered, this,  &AFCharacter::MoveLateral);
	}
}

