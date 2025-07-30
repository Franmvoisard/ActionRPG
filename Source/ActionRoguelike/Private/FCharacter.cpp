// Public Domain - 2025 Franco Voisard
// This code is provided for educational purposes.
// No rights reserved. Use freely.
#include "ActionRoguelike/Public/FCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FAttributeComponent.h"
#include "FInteractionComponent.h"
#include "FProjectileBase.h"
#include "FTeleportProjectile.h"
#include "ActionRoguelike/FFlashOnHitComponent.h"
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
	FlashOnHitComponent = CreateDefaultSubobject<UFFlashOnHitComponent>("Flash On Hit Component");
	ProjectileTraceSphereRadius = 20.0f;
	ProjectileMaxTraceDistance = 5000;
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

void AFCharacter::Dash_TimeElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void AFCharacter::PrimaryAttack_TimeElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void AFCharacter::AOEAttack()
{
	SpawnProjectile(AOEProjectileClass);
}

void AFCharacter::SpawnProjectile(TSubclassOf<AFProjectileBase> ProjectileClassToSpawn)
{
	if (ensureAlways(ProjectileClassToSpawn))
	{
		const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AbilitySpawnParticle, HandLocation, FQuat::Identity.Rotator());

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = this;

		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(ProjectileTraceSphereRadius);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FHitResult HitResult;
		
		FVector TraceStart = CameraComp->GetComponentLocation();
		DrawDebugSphere(GetWorld(), TraceStart, ProjectileTraceSphereRadius, 12, FColor::Red, false, 2.0f);
		FVector TraceEnd = TraceStart + CameraComp->GetForwardVector() * ProjectileMaxTraceDistance;
		GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionParams);
		FVector Target = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		FRotator SpawnRotation = (Target - HandLocation).Rotation();
		FTransform SpawnTransform = FTransform(SpawnRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClassToSpawn, SpawnTransform, SpawnParameters);		
	}
}
