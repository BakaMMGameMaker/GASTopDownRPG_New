// BakaMMGameMaker Learning From Druid Mechanics


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

UOverlayWidgetController::UOverlayWidgetController()
{
	MessageTag = FGameplayTag::RequestGameplayTag(TEXT("Message"));
}

// 广播给 Widgets
void UOverlayWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	// 广播所有初始值
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	// 监听 Model 中的数值变化，GAS 内置的属性变化广播系统
	// 与头文件中的自定义 delegates 不是一回事
	HealthDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                                             AuraAttributeSet->GetHealthAttribute())
	                                             .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) {
		                                             OnHealthChanged.Broadcast(Data.NewValue);
	                                             });

	MaxHealthDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                                                AuraAttributeSet->GetMaxHealthAttribute())
	                                                .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) {
		                                                OnMaxHealthChanged.Broadcast(Data.NewValue);
	                                                });

	ManaDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                                           AuraAttributeSet->GetManaAttribute())
	                                           .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) {
		                                           OnManaChanged.Broadcast(Data.NewValue);
	                                           });

	MaxManaDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		                                              AuraAttributeSet->GetMaxManaAttribute())
	                                              .AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) {
		                                              OnMaxManaChanged.Broadcast(Data.NewValue);
	                                              });

	MessageDelegateHandle = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddWeakLambda(
		this,
		[this](const FGameplayTagContainer& AssetTags) {
			for (const FGameplayTag& Tag : AssetTags)
			{
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					if (!Row) continue;
					MessageWidgetRow.Broadcast(*Row);
				}
			}
		}
	);
}

void UOverlayWidgetController::BeginDestroy()
{
	// 解绑对 Delegates 的监听，防止僵尸委托累积
	if (IsValid(AbilitySystemComponent))
	{
		if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
			                        Remove(HealthDelegateHandle);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
			                        Remove(MaxHealthDelegateHandle);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).
			                        Remove(ManaDelegateHandle);
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).
			                        Remove(MaxManaDelegateHandle);
		}
		Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.Remove(MessageDelegateHandle);
	}

	Super::BeginDestroy();
}
