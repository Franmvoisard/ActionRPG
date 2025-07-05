// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

class AFTeleportProjectile;
class AFProjectileBase;
class UFInteractionComponent;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
UCLASS()
class ACTIONROGUELIKE_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

	FTransform ComputeProjectileSpawnPosition(const FVector& ProjectileStartPosition) const;
	FVector GetHandLocation() const;

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	FTimerHandle TimerHandle_PrimaryAttack;
	FTimerHandle TimerHandle_Dash;
	
	UPROPERTY(EditAnywhere, Category = Input)
	const UInputAction* AOEAttackAction;

	UPROPERTY(EditAnywhere, Category = Attack);
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input);
	const UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere)
	UFInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* ForwardMovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* LateralMovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* LookRotationAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* PrimaryAttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* PrimaryInteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<AFProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<AFProjectileBase> AOEProjectileClass;

	UPROPERTY(EditAnywhere, Category = Abilities)
	TSubclassOf<AFProjectileBase> DashProjectileClass;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(const FInputActionValue& InputActionValue);
	void MoveLateral(const FInputActionValue& InputActionValue);
	void LookRotation(const FInputActionValue& InputActionValue);
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();
	void AOEAttack();
	FActorSpawnParameters GetDefaultProjectileSpawnParameters();
	void Dash();
	void Dash_TimeElapsed();
	
	UFUNCTION()
	void Dash_To_Projectile(AFTeleportProjectile* Projectile);

public:
	AFCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
