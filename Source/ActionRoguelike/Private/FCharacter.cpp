// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionRoguelike/Public/FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FInteractionComponent.h"
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
	InteractionComponent = CreateDefaultSubobject<UFInteractionComponent>(TEXT("Interaction Component"));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AFCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext,0);
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
	FVector RightVector = FRotationMatrix(ControlRotator).
	 GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, InputActionValue.GetMagnitude());
}

void AFCharacter::LookRotation(const FInputActionValue& InputActionValue)
{
	FVector2D LookRotationInput = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(LookRotationInput.X);
	AddControllerPitchInput(-LookRotationInput.Y);
}

void AFCharacter::PrimaryAttack()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTransform = FTransform(GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);
}

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
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AFCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFCharacter::StopJumping);
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Triggered, InteractionComponent, &UFInteractionComponent::PrimaryInteract);
	}
}

