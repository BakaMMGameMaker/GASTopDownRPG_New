// BakaMMGameMaker Learning From Druid Mechanics


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();

	// 主要属性
	GameplayTags.Attributes_Primary_Strength = TagsManager.AddNativeGameplayTag(FName("Attributes.Primary.Strength"));
	GameplayTags.Attributes_Primary_Intelligence = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"));
	GameplayTags.Attributes_Primary_Resilience = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"));
	GameplayTags.Attributes_Primary_Vigor = TagsManager.AddNativeGameplayTag(FName("Attributes.Primary.Vigor"));

	// 次要属性
	GameplayTags.Attributes_Secondary_MaxHealth = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"));
	GameplayTags.Attributes_Secondary_MaxMana = TagsManager.AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"));

	GameplayTags.Attributes_Secondary_Armor = TagsManager.AddNativeGameplayTag(FName("Attributes.Secondary.Armor"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"));
	GameplayTags.Attributes_Secondary_BlockChance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"));
}
