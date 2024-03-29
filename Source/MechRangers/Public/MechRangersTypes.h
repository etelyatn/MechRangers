// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ObjectMacros.h"
#include "MechRangersTypes.generated.h"

class UMaterial;
class UMRHealthComponent;

/** keep in sync with ShooterImpactEffect */
UENUM()
namespace EShooterPhysMaterialType
{
	enum Type
	{
		Unknown,
		Concrete,
		Dirt,
		Water,
		Metal,
		Wood,
		Grass,
		Glass,
		Flesh,
	};
}

#define SHOOTER_SURFACE_Default		SurfaceType_Default
#define SHOOTER_SURFACE_Concrete	SurfaceType1
#define SHOOTER_SURFACE_Dirt		SurfaceType2
#define SHOOTER_SURFACE_Water		SurfaceType3
#define SHOOTER_SURFACE_Metal		SurfaceType4
#define SHOOTER_SURFACE_Wood		SurfaceType5
#define SHOOTER_SURFACE_Grass		SurfaceType6
#define SHOOTER_SURFACE_Glass		SurfaceType7
#define SHOOTER_SURFACE_Flesh		SurfaceType8

USTRUCT()
struct FDecalData
{
	GENERATED_USTRUCT_BODY()

	/** material */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	UMaterial* DecalMaterial;

	/** quad size (width & height) */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	float DecalSize;

	/** lifespan */
	UPROPERTY(EditDefaultsOnly, Category=Decal)
	float LifeSpan;

	/** defaults */
	FDecalData()
		: DecalMaterial(nullptr)
		, DecalSize(256.f)
		, LifeSpan(10.f)
	{
	}
};

/** Grip state enum */
UENUM(BlueprintType)
enum class EGripState : uint8
{
	EGS_Open UMETA(DisplayName = "Open"),
    EGS_CanGrab UMETA(DisplayName = "CanGrab"),
    EGS_Grab UMETA(DisplayName = "Grab"),

    EGS_Max UMETA(Hidden)
};

/**
 * Gameplay Team
 */
UENUM(BlueprintType)
enum class EGameplayTeam : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Player UMETA(DisplayName = "Player"),
	Alien UMETA(DisplayName = "Alien")
};

/**
* Health State
*/
UENUM(BlueprintType)
enum class EHealthState : uint8
{
	EHS_Healthy UMETA(DisplayName = "Healthy"),
    EHS_Damaged UMETA(DisplayName = "Damaged"),
    EHS_Destroyed UMETA(DisplayName = "Destroyed"),
    EHS_Invalid UMETA(DisplayName = "Invalid")
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_USTRUCT_BODY()
};


USTRUCT(BlueprintType)
struct FDamageTakenData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	AActor* DamageDealer;

	UPROPERTY(BlueprintReadWrite)
	FDamageInfo DamageInfo;
};

/** Health Changed Parameters */
USTRUCT(BlueprintType)
struct FHealthChangedParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UMRHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite)
	float OldHealth;

	UPROPERTY(BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite)
	float DamageTaken;
	
};

/** Health State Changed Parameters */
USTRUCT(BlueprintType)
struct FHealthStateChangedParams
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UMRHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadWrite)
	EHealthState OldState;

	UPROPERTY(BlueprintReadWrite)
	EHealthState CurrentState;
};