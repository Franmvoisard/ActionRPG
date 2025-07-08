// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionRoguelike/Public/FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FAttributeComponent.h"
#include "FInteractionComponent.h"
#include "FProjectileBase.h"
#include "FTeleportProjectile.h"
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
	AttributeComponent = CreateDefaultSubobject<UFAttributeComponent>("Attribute Component");
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
	}
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

void AFCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	PlayAnimMontage(AttackAnimation);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &AFCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void AFCharacter::Dash()
{
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this, &AFCharacter::Dash_TimeElapsed, 0.2f);
	PlayAnimMontage(AttackAnimation);
}

void AFCharacter::Dash_To_Projectile(AFTeleportProjectile* Projectile)
{
	TeleportTo(Projectile->GetActorLocation(), GetActorRotation());
}

void AFCharacter::Dash_TimeElapsed()
{
	const FVector HandLocation = GetHandLocation();
	const FTransform SpawnTransform = ComputeProjectileSpawnPosition(HandLocation);
	AFTeleportProjectile* DashProjectile = GetWorld()->SpawnActor<AFTeleportProjectile>(DashProjectileClass, SpawnTransform, GetDefaultProjectileSpawnParameters());
	DashProjectile->OnNotifyTriggerEffect.AddDynamic(this, &AFCharacter::Dash_To_Projectile);
}

void AFCharacter::PrimaryAttack_TimeElapsed()
{
	const FVector HandLocation = GetHandLocation();
	const FTransform SpawnTransform = ComputeProjectileSpawnPosition(HandLocation);
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, GetDefaultProjectileSpawnParameters());
}

void AFCharacter::AOEAttack()
{
	const FVector HandLocation = GetHandLocation();
	const FTransform SpawnTransform = ComputeProjectileSpawnPosition(HandLocation);
	
	GetWorld()->SpawnActor<AFProjectileBase>(AOEProjectileClass, SpawnTransform, GetDefaultProjectileSpawnParameters());
}

// Called to bind functionality to input


FTransform AFCharacter::ComputeProjectileSpawnPosition(const FVector& ProjectileStartPosition) const
{
	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(20.0f);
	
	// Calculate the starting transform to land into the player crosshair
	FVector TraceStart = CameraComp->GetComponentLocation();
	FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * 10000;
	GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_WorldDynamic, CollisionShape, CollisionParams);
	FVector Target = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	FRotator SpawnRotation = (Target - ProjectileStartPosition).Rotation();
	return FTransform(SpawnRotation, ProjectileStartPosition);
}

FActorSpawnParameters AFCharacter::GetDefaultProjectileSpawnParameters()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	return SpawnParameters;
}

FVector AFCharacter::GetHandLocation() const
{
	return GetMesh()->GetSocketLocation("Muzzle_01");
}
