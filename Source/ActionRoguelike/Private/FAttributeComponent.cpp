// Fill out your copyright notice in the Description page of Project Settings.


#include "FAttributeComponent.h"

UFAttributeComponent::UFAttributeComponent()
{
	Health = 100.0f;
}

bool UFAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	return true;
}
