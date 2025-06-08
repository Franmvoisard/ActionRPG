// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
UCLASS()
class ACTIONROGUELIKE_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input);
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* ForwardMovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LateralMovementAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	const UInputAction* LookRotationAction;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(const FInputActionValue& InputActionValue);
	void MoveLateral(const FInputActionValue& InputActionValue);
	void LookRotation(const FInputActionValue& InputActionValue);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
