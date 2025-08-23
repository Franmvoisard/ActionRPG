// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.


#include "FAction_ProjectileAttack.h"

#include "FCharacter.h"
#include "FProjectileBase.h"
#include "Kismet/GameplayStatics.h"

static FAutoConsoleVariable CVarDebugDrawEnabled(TEXT("ar.Projectiles.DebugDrawEnabled"), false, TEXT("Enable debug draw for projectiles"), ECVF_Cheat);

UFAction_ProjectileAttack::UFAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	ProjectileTraceSphereRadius = 20.0f;
	ProjectileMaxTraceDistance = 5000;
	AttackAnimationDelay = 0.2f;
}

void UFAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);
	if (AFCharacter* Character = Cast<AFCharacter>(Instigator))
	{
		Character->PlayAnimMontage(AttackAnimation);
		
		FTimerHandle TimerHandle_ProjectileAttack;
		FTimerDelegate Delegate;

		// Use a weak pointer for safety, in case the Character is destroyed before the timer fires.
		TWeakObjectPtr<ACharacter> WeakCharacter = Character;

		Delegate.BindLambda([this, WeakCharacter]()
		{
			if (WeakCharacter.IsValid())
			{
				ProjectileAttack_TimeElapsed(WeakCharacter.Get());
			}
		});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ProjectileAttack, Delegate, AttackAnimationDelay, false);
	}
	
}

void UFAction_ProjectileAttack::ProjectileAttack_TimeElapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		const FVector HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AbilitySpawnParticle, HandLocation, FQuat::Identity.Rotator());

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;

		FCollisionShape CollisionShape;
		CollisionShape.SetSphere(ProjectileTraceSphereRadius);

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FHitResult HitResult;
		
		FVector TraceStart = InstigatorCharacter->GetPawnViewLocation();
		if (CVarDebugDrawEnabled->GetBool())
		{
			DrawDebugSphere(GetWorld(), TraceStart, ProjectileTraceSphereRadius, 12, FColor::Red, false, 2.0f);
		}
		FVector TraceEnd = TraceStart + InstigatorCharacter->GetControlRotation().Vector() * ProjectileMaxTraceDistance;
		GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionParams);
		FVector Target = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
		FRotator SpawnRotation = (Target - HandLocation).Rotation();
		FTransform SpawnTransform = FTransform(SpawnRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransform, SpawnParameters);		
	}
	
	StopAction(InstigatorCharacter);
}
