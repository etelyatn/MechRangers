// Copyright PlatoSpace.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MREnemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MREnemySpawner.generated.h"

UCLASS()
class MECHRANGERS_API AMREnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMREnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Is Enemy Spawner Active */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	bool bSpawnerActive;

	/** Spawning area visual representation */
	UPROPERTY(BlueprintReadWrite)
	USphereComponent* VisualComp;

	/** Radius on NavMesh where enemies can be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	float Radius;

	/** How many enemies will be spawned */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	int32 MaxEnemies;

	/** Min and Max number can be spawned at the same time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	FIntPoint SingleSpawnEnemiesRange;

	/** Random between min and max scale */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SpawnScaleRange;

	/** How many spawned already enemies */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Spawn)
	int32 SpawnedEnemies;

	/** Min and Max time between spawning */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	FVector2D SpawnIntervalRange;

	/** Enemy for spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	TSubclassOf<AMREnemy> EnemyClass;

	/** Enemy for spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawn)
	AActor* EnemyMainTarget;

	/** Timer for the Next Spawn */
	FTimerHandle TimerHandle_NextSpawn;

	/** One time Spawn process */
	UFUNCTION(BlueprintCallable)
	void SpawnEnemies();

	/** Get random point in navigation mesh for spawning */
	UFUNCTION(BlueprintCallable)
	bool GetRandomPoint(FVector& OutResult);

	/** Spawn one enemy at location */
	UFUNCTION(BlueprintCallable)
	bool SpawnEnemy(FVector& Location);


public:

	/** Radius visualization */
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Activate spawning process */
	UFUNCTION(BlueprintCallable)
	void Activate();

	/** Deactivate spawning process */
	UFUNCTION(BlueprintCallable)
	void Deactivate();

	/** Reset number of spawned enemies. So it will be spawned again MaxEnemies  */
	UFUNCTION(BlueprintCallable)
	void Reset();

};
