// BakaMMGameMaker Learning From Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCMaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class GASTOPDOWNRPG2_API UMMCMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMCMaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;
};
