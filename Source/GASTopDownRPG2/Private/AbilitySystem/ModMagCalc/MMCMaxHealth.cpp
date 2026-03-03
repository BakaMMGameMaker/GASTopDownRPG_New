// BakaMMGameMaker Learning From Druid Mechanics


#include "AbilitySystem/ModMagCalc/MMCMaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMCMaxHealth::UMMCMaxHealth()
{
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute(); // 定义要捕获的属性
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // 从目标上捕获
	VigorDefinition.bSnapshot = false; // 时刻获取最新真实值参与计算

	RelevantAttributesToCapture.Add(VigorDefinition); // 添加到 MMC 捕获列表中
}

float UMMCMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取源与目标的标签，便于用于额外加成 or 减免等逻辑
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 准备评估参数
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluationParameters, Vigor); // 获取目标 Vigor

	Vigor = FMath::Max<float>(Vigor, 0.f);

	// 获取玩家等级
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 25.f + 2 * Vigor + 5 * PlayerLevel;
}
