// Public Domain - 2025 Franco Voisard. This code is provided for skill and knowledge demo purposes. No rights reserved. Use freely.

/*
* This file, originally forked from the BTDecoratorCooldown files of the Unreal Engine source code,
 * contains modifications made to rectify an issue where cooldown would prematurely commence even without invocation.
 * 
 * The applied solution closely mirrors the implementation of BTDecoratorCooldown in Unreal Engine version 5.4.
 * All digital rights of this file are hereby attributed to the original sources.
 *
 * PLEASE NOTE: This file is provided "as is", without warranty of any kind, express or implied. 
 * The author of this file does not hold any digital rights over it. 
 */
#include "AI/FBTDecorator_Cooldown.h"

UFBTDecorator_Cooldown::UFBTDecorator_Cooldown()
{
	NodeName = "Cooldown";
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
	CooldownTime = 5.0f;

	// aborting child nodes doesn't make sense, cooldown starts after leaving this branch
	bAllowAbortChildNodes = false;
}

void UFBTDecorator_Cooldown::PostLoad()
{
	Super::PostLoad();
	bNotifyTick = (FlowAbortMode != EBTFlowAbortMode::None);
}

void UFBTDecorator_Cooldown::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	FFBTCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FFBTCooldownDecoratorMemory>(NodeMemory);
	if (InitType == EBTMemoryInit::Initialize) DecoratorMemory->LastUseTimestamp = TNumericLimits<double>::Lowest();
	DecoratorMemory->bRequestedRestart = false;
}

bool UFBTDecorator_Cooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const FFBTCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FFBTCooldownDecoratorMemory>(NodeMemory);
	const double RecalculateTimeStamp = (OwnerComp.GetWorld()->GetTimeSeconds() - CooldownTime);
	return RecalculateTimeStamp >= DecoratorMemory->LastUseTimestamp;
}

void UFBTDecorator_Cooldown::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	const FFBTCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FFBTCooldownDecoratorMemory>(NodeMemory);

	if (const double TimePassed = OwnerComp.GetWorld()->GetTimeSeconds() - DecoratorMemory->LastUseTimestamp; TimePassed < CooldownTime)
	{
		Values.Add(FString::Printf(TEXT("%s in %ss"), (FlowAbortMode == EBTFlowAbortMode::None) ? TEXT("unlock") : TEXT("restart"), *FString::SanitizeFloat(CooldownTime - TimePassed)));
	}
}

uint16 UFBTDecorator_Cooldown::GetInstanceMemorySize() const
{
	return sizeof(FFBTCooldownDecoratorMemory);
}

FString UFBTDecorator_Cooldown::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: lock for %.1fs after execution and return %s"), *Super::GetStaticDescription(), CooldownTime, *UBehaviorTreeTypes::DescribeNodeResult(EBTNodeResult::Failed));
}

void UFBTDecorator_Cooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FFBTCooldownDecoratorMemory* DecoratorMemory = CastInstanceNodeMemory<FFBTCooldownDecoratorMemory>(NodeMemory);

	if (!DecoratorMemory->bRequestedRestart)
	{
		const double RecalculateTimeStamp = (OwnerComp.GetWorld()->GetTimeSeconds() - CooldownTime);

		if (RecalculateTimeStamp >= DecoratorMemory->LastUseTimestamp)
		{
			DecoratorMemory->bRequestedRestart = true;
			OwnerComp.RequestExecution(this);
		}
	}
}

void UFBTDecorator_Cooldown::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
	FFBTCooldownDecoratorMemory* DecoratorMemory = GetNodeMemory<FFBTCooldownDecoratorMemory>(SearchData);
	DecoratorMemory->LastUseTimestamp = SearchData.OwnerComp.GetWorld()->GetTimeSeconds();
	DecoratorMemory->bRequestedRestart = false;
}

#if WITH_EDITOR
FName UFBTDecorator_Cooldown::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Decorator.Cooldown.Icon");
}
#endif