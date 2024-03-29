// Copyright PlatoSpace.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mech/MRMechTypes.h"
#include "MechRangersTypes.h"
#include "MRMechLivingComponent.generated.h"

class UMRHealthComponent;
class AMRMech;

/**
 * Models the various health containing parts of a Mech, including interactions between health components
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MECHRANGERS_API UMRMechLivingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMRMechLivingComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /** Apply damage to Health Components */
    UFUNCTION(BlueprintCallable)
    float TakeDamage(const float TakenDamage, FDamageTakenData const& DamageTakenData);

    /** Apply Point damage to Health Components */
    UFUNCTION(BlueprintCallable)
    float TakePointDamage(float TakenDamage, FPointDamageEvent const& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    float TakeRadialDamage(float TakenDamage, FRadialDamageEvent const& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    float HealthContainerTakeDamage(float TakenDamage, FHitResult const& HitResult, AController* EventInstigator, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable)
    EHealthState GetHealthState();

protected:

    /** */
    UPROPERTY(EditAnywhere, Category = Config)
    bool bDebug;

    /** Mech Health State */
    UPROPERTY(EditDefaultsOnly, Category = Config)
    EHealthState HealthState = EHealthState::EHS_Healthy;

    /** Minimum damage distance to bone used on impact calculation. If distance is less - then hit a bone and stop calculating. */
    UPROPERTY(EditDefaultsOnly, Category = Config)
    float MinNearDamageDistance;

    /** Maximum damage distance to bone used on impact calculation. If distance is bigger - then will not calculate. */
    UPROPERTY(EditDefaultsOnly, Category = Config)
    float MaxNearDamageDistance;

    /** */
    UPROPERTY(BlueprintReadWrite)
    AMRMech* OwnerMech;

    /** Health Container for each mech part */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TMap<EMechPart, UMRHealthComponent*> HealthContainerList;

    /** Contains Bone names related to mech parts */
    UPROPERTY(BlueprintReadOnly)
    TMap<FName, EMechPart> BoneNameToMechPart;

    // Called when the game starts
    virtual void BeginPlay() override;

    /** Find Damaged Health Container by Bone Name */
    UFUNCTION(BlueprintCallable)
    UMRHealthComponent* FindHealthContainerByBone(const FName BoneName);

    /** Find Damaged Health Container by Location */
    UFUNCTION(BlueprintCallable)
    UMRHealthComponent* FindHealthContainerByLocation(const FVector NearLocation);

    UFUNCTION(BlueprintCallable)
    EMechPart FindMechPartByHealthComponent(const UMRHealthComponent* HealthComponent);
    
    UFUNCTION(BlueprintCallable)
    UMRHealthComponent* GetHealthContainer(EMechPart InMechPart);

    /** Subscribed to the Health Component Health State Changed Delegate */
    UFUNCTION()
    void OnHealthContainerStateChanged(FHealthStateChangedParams Params);


};
