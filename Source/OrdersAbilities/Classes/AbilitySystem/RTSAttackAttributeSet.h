#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "RTSAttackAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Contains attributes about the attack of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSAttackAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

    //  YOU ARE NOT ALLOWED TO CHANGE THE ATTRIBUTE VALUES!
    //
    //  This attributes are only PUBLIC because of the 'DEFINE_ATTRIBUTE_CAPTUREDEF' macro that is used when
    //  implementing a 'UGameplayEffectExecutionCalculation'.This attribute values should only be changed using
    //  GameplayEffects.
public:
    static const float MAX_DAMAGE;
    static const float MAX_COOLDOWN;
    static const float MAX_RANGE;
    static const float MAX_OUTGOING_DAMAGE_MULTIPLIER;

    /** Default attack damage. */
    UPROPERTY(Category = "Attributes|Attack", BlueprintReadOnly, ReplicatedUsing = OnRep_Damage)
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Damage)

    UFUNCTION()
    void OnRep_Damage(const FGameplayAttributeData& OldDamage);

    /** Time before the attack can be used again, in seconds. */
    UPROPERTY(Category = "Attributes|Attack", BlueprintReadOnly, ReplicatedUsing = OnRep_Cooldown)
    FGameplayAttributeData Cooldown;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Cooldown)
    UFUNCTION()
    void OnRep_Cooldown(const FGameplayAttributeData& OldCoolDown);

    /** Attack range, in cm. */
    UPROPERTY(Category = "Attributes|Attack", BlueprintReadOnly, ReplicatedUsing = OnRep_Range)
    FGameplayAttributeData Range;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Range)
    UFUNCTION()
    void OnRep_Range(const FGameplayAttributeData& OldRange);

    /**
     * The OutgoingDamageMultiplier of a target is multiplied with incoming damage to modify the loss of hit points of
     * the attacked unit.
     */
    UPROPERTY(Category = "Attributes|Attack", BlueprintReadOnly, ReplicatedUsing = OnRep_OutgoingDamageMultiplier)
    FGameplayAttributeData OutgoingDamageMultiplier;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, OutgoingDamageMultiplier)
    UFUNCTION()
    void OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldOutgoingDamageMultiplier);

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void PostInitializeProperties(bool bInitialInit) override;
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface
};
