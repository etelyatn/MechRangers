// Copyright PlatoSpace.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/Weapon/MRWeapon.h"
#include "MRWeapon_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AMRProjectile;

USTRUCT()
struct FProjectileWeaponData
{
	GENERATED_USTRUCT_BODY()

    /** projectile class */
    UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<AMRProjectile> ProjectileClass;

	/** life time */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	float ProjectileLife;

	/** damage at impact point */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	int32 ExplosionDamage;

	/** radius of damage */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	float ExplosionRadius;

	/** type of damage */
	UPROPERTY(EditDefaultsOnly, Category=WeaponStat)
	TSubclassOf<UDamageType> DamageType;

	/** defaults */
	FProjectileWeaponData()
	{
		ProjectileClass = nullptr;
		ProjectileLife = 10.0f;
		ExplosionDamage = 100;
		ExplosionRadius = 300.0f;
		DamageType = UDamageType::StaticClass();
	}
};

UCLASS(Abstract, Blueprintable)
class MECHRANGERS_API AMRWeapon_Projectile : public AMRWeapon
{
	GENERATED_BODY()

protected:

	virtual EAmmoType GetAmmoType() const override
	{
		return EAmmoType::ERocket;
	}

	/** weapon config */
	UPROPERTY(EditDefaultsOnly, Category=Config)
	FProjectileWeaponData ProjectileConfig;

public:
	
	/** apply config on projectile */
	void ApplyWeaponConfig(FProjectileWeaponData& Data);

//----------------------------------------------------------------------------------------------------------------------
// Weapon Usage
//----------------------------------------------------------------------------------------------------------------------
protected:
	
	/** [local] weapon specific fire implementation */
	virtual void FireWeapon() override;

	/** spawn projectile on server */
	UFUNCTION(reliable, server, WithValidation)
    void ServerFireProjectile(FVector Origin, FVector_NetQuantizeNormal ShootDir);
};
