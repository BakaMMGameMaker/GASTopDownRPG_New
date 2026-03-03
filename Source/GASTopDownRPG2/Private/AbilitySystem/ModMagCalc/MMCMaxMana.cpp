// BakaMMGameMaker Learning From Druid Mechanics


#include "AbilitySystem/ModMagCalc/MMCMaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluationParameters, Intelligence);

	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 40.f + 2 * Intelligence + 15 * PlayerLevel;
}
