// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#include "ActionRoguelike/Public/FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FActionComponent.h"
#include "FAttributeComponent.h"
#include "FInteractionComponent.h"
#include "FProjectileBase.h"
#include "FFlashOnHitComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AFCharacter::AFCharacter() {
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
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
	ActionComponent = CreateDefaultSubobject<UFActionComponent>("Action Component");
	FlashOnHitComponent = CreateDefaultSubobject<UFFlashOnHitComponent>("Flash On Hit Component");
	
}

// Called when the game starts or when spawned
void AFCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if(TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ForwardMovementAction, ETriggerEvent::Triggered, this, &AFCharacter::MoveForward);
		EnhancedInputComponent->BindAction(LateralMovementAction, ETriggerEvent::Triggered, this,  &AFCharacter::MoveLateral);
		EnhancedInputComponent->BindAction(LookRotationAction, ETriggerEvent::Triggered, this, &AFCharacter::LookRotation);
		EnhancedInputComponent->BindAction(PrimaryAttackAction, ETriggerEvent::Started, this, &AFCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(AOEAttackAction, ETriggerEvent::Started, this, &AFCharacter::AOEAttack);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AFCharacter::Dash);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AFCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AFCharacter::StopJumping);
		EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, InteractionComponent, &UFInteractionComponent::PrimaryInteract);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AFCharacter::Sprint_Start);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AFCharacter::Sprint_Stop);
	}
}

void AFCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

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

FVector AFCharacter::GetPawnViewLocation() const
{
	return CameraComp->GetComponentLocation();
}

void AFCharacter::OnHealthChanged(AActor* InstigatorActor, UFAttributeComponent* AttributeOwner, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (NewHealth <= 0.0f)
		{
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			DisableInput(PlayerController);
		}
		else
		{
			FlashOnHitComponent->OnHit(GetMesh());
		}
	}
}

void AFCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChange.AddDynamic(this, &AFCharacter::OnHealthChanged);
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
	if (!ActionComponent->StartActionByName(this,"MagicProjectile"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint action not found"));
	}
}

void AFCharacter::Dash()
{
	if (!ActionComponent->StartActionByName(this,"TeleportDash"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint action not found"));
	}
}

void AFCharacter::Sprint_Start()
{
	if (!ActionComponent->StartActionByName(this,"Sprint"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint action not found"));
	}
}

void AFCharacter::Sprint_Stop()
{
	if (!ActionComponent->StopActionByName(this,"Sprint"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint action not found"));
	}
}


void AFCharacter::AOEAttack()
{
	if (!ActionComponent->StartActionByName(this,"BlackHole"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint action not found"));
	}
}
