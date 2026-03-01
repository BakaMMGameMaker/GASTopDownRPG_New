// BakaMMGameMaker Learning From Druid Mechanics


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass,
                                           EEffectRemovalPolicy RemovalPolicy)
{
	if (!IsValid(TargetActor) || !GameplayEffectClass) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!IsValid(TargetASC)) return;

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(
		GameplayEffectClass, 1.f, EffectContextHandle);

	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(
		*EffectSpecHandle.Data);

	// 对于 Infinite 效果，记录 Actor 和句柄以便后期去除某 Actor 身上的 Effect
	if (EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite &&
		RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap
	)
	{
		ActiveEffectHandles.FindOrAdd(TargetASC).Add(ActiveGameplayEffectHandle);
	}
}

// 重叠时应用各种效果
void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	for (const FAuraGameplayEffect& EffectInfo : AppliedGameplayEffects)
	{
		if (EffectInfo.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass, EffectInfo.RemovalPolicy);
		}
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		IsValid(TargetASC))
	{
		if (TArray<FActiveGameplayEffectHandle>* Handles = ActiveEffectHandles.Find(TargetASC))
		{
			for (FActiveGameplayEffectHandle& Handle : *Handles)
			{
				if (Handle.IsValid()) TargetASC->RemoveActiveGameplayEffect(Handle, 1);
			}
			ActiveEffectHandles.Remove(TargetASC);
		}
	}

	for (const FAuraGameplayEffect& EffectInfo : AppliedGameplayEffects)
	{
		if (EffectInfo.ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectInfo.GameplayEffectClass, EffectInfo.RemovalPolicy);
		}
	}

	if (bDestroyOnEffectRemoval && ActiveEffectHandles.IsEmpty()) Destroy();
}
