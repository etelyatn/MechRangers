// Copyright PlatoSpace.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MechComponents/MRMechLivingComponent.h"
#include "GameFramework/Character.h"
#include "MechDataAssets/MRMechLoadoutDataAsset.h"
#include "MechDataAssets/MRMechModelDataAsset.h"

#include "MRMech.generated.h"

UCLASS()
class MECHRANGERS_API AMRMech : public ACharacter
{
	GENERATED_BODY()

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Sets default values for this character's properties
	AMRMech(const class FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Setup Mech from Loadout Asset */
	UFUNCTION(BlueprintCallable)
	void ConstructMech();

	/** Current Mechs Cockpit actor instance */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AMRMechCockpit* Cockpit;

	/** Setup Cockpit for mech from Mech Model Data */
	UFUNCTION(BlueprintCallable)
	AMRMechCockpit* SpawnCockpit(FMechCockpit CockpitData);

//----------------------------------------------------------------------------------------------------------------------
// Components
//----------------------------------------------------------------------------------------------------------------------
protected:
	
	/** Models the various health containing parts of a Mech, including interactions between health components. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMRMechLivingComponent* LivingComponent;

//----------------------------------------------------------------------------------------------------------------------
// Configs
//----------------------------------------------------------------------------------------------------------------------
protected:

	/** Loadout asset for the Mech. Every Mech must have it! */ 
	UPROPERTY(Category=MRMech, BlueprintReadOnly, EditAnywhere)
	UMRMechLoadoutDataAsset* MechLoadoutAsset;

	/** Active Loadout structure for the Mech */
	UPROPERTY(Category=MRMech, BlueprintReadWrite, VisibleInstanceOnly)
	FMechLoadout MechLoadout;

	/** Active Mech model data structure */
	UPROPERTY(Category=MRMech, BlueprintReadWrite, VisibleInstanceOnly)
	FMechModelData MechModelData;

	
	/** Is Combat mode or Movement mode. */
	UPROPERTY(Category=MRMech, VisibleInstanceOnly, BlueprintReadWrite)
	bool bIsCombatMode;

public:

	/** Set Mech loadout */
	UFUNCTION(BlueprintCallable)
	void SetLoadout(FMechLoadout NewLoadout);
	
	/** Checks is Mech in combat mode */
	UFUNCTION(BlueprintCallable)
    FORCEINLINE bool IsCombatMode() const { return bIsCombatMode; }

	/** Set Mech Combat Mode state */
	UFUNCTION(BlueprintCallable)
    void SetCombatMode(bool const Val);

//----------------------------------------------------------------------------------------------------------------------
// Movement controls
//----------------------------------------------------------------------------------------------------------------------
public:
	/** Move forward/back */
	void MoveForward(float Val);

	/** Frame rate independent turn */
	void TurnAtRate(float Val);

//----------------------------------------------------------------------------------------------------------------------
// Arm controls
//----------------------------------------------------------------------------------------------------------------------
protected:
	UPROPERTY(BlueprintReadWrite)
	FRotator ArmLeftRotator;

	UPROPERTY(BlueprintReadWrite)
	FRotator ArmRightRotator;

	/** Add rotation to arms with limitation */
	void AddArmRotator(FRotator& ArmRotator, const FRotator& AddRot);
	
public:

	FORCEINLINE FRotator GetArmLeftRotator() const { return ArmLeftRotator; }
	FORCEINLINE FRotator GetArmRightRotator() const { return ArmRightRotator; }

	/** Add Left Arm Rotation */
	void AddArmLeftRotator(const FRotator& Rot);

	/** Add Right Arm Rotation */
	void AddArmRightRotator(const FRotator& Rot);

	
};
