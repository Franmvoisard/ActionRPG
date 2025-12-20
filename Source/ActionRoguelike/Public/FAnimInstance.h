// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FAnimInstance.generated.h"

class UFActionComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API UFAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Animation")
	bool bIsStunned;
	
	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	UFActionComponent* ActionComp;

	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
