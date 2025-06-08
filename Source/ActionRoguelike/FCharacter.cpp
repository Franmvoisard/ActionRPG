// Fill out your copyright notice in the Description page of Project Settings.

#include "FCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AFCharacter::AFCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArm) ;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
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

void AFCharacter::MoveForward(const FInputActionValue& InputActionValue)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Pitch = 0.0f;
	ControlRotator.Roll = 0.0f;
	AddMovementInput(ControlRotator.Vector(), InputActionValue.GetMagnitude());
}

void AFCharacter::MoveLateral(const FInputActionValue& InputActionValue)
{
	FRotator ControlRotator = GetControlRotation();
	ControlRotator.Pitch = 0.0f;
	ControlRotator.Roll = 0.0f;
	FVector RightVector = FRotationMatrix(ControlRotator).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, InputActionValue.GetMagnitude());
}

void AFCharacter::LookRotation(const FInputActionValue& InputActionValue)
{
	FVector2D LookRotationInput = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(LookRotationInput.X);
	AddControllerPitchInput(-LookRotationInput.Y);
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
		EnhancedInputComponent->BindAction(LookRotationAction, ETriggerEvent::Triggered, this, &AFCharacter::LookRotation);
	}
}

