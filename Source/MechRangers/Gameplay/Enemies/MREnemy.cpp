// Copyright PlatoSpace.com All Rights Reserved.

#include "MREnemy.h"
#include "MREnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "MechRangers/Gameplay/Mech/MRMech.h"
#include "MechRangers/MechRangers.h"

// debug includes
#include "DrawDebugHelpers.h"
#include "Log.h"

// Sets default values
AMREnemy::AMREnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup components
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->SetSphereRadius(600.f);
	AgroSphere->SetCollisionProfileName(FName("Trigger"));

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->SetSphereRadius(90.f);
	CombatSphere->SetCollisionProfileName(FName("Trigger"));

	// Setup configs
	GetCapsuleComponent()->SetCollisionProfileName(FName("MechPawn"));
	AIControllerClass = AMREnemyAIController::StaticClass();
	MaxHealth = 50.f;
	bAlive = true;
	TimeBeforeDestroy = FVector2D(3, 5);
	MovementSpeedRange = FVector2D(500.f, 600.f);
	DamageRange = FVector2D(1.f, 2.f);
}

// Called when the game starts or when spawned
void AMREnemy::BeginPlay()
{
	Super::BeginPlay();

	// Configs
	AIController = Cast<AMREnemyAIController>(GetController());
	AnimInstance = GetMesh()->GetAnimInstance();
	CurrentHealth = MaxHealth;

	// Setup overlaps
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AMREnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AMREnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AMREnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AMREnemy::CombatSphereOnOverlapEnd);

	// Setup movement
	UCharacterMovementComponent* MovementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (GetMovementComponent())
	{
		MovementComp->MaxWalkSpeed = FMath::RandRange(MovementSpeedRange.X, MovementSpeedRange.Y);
	}

	StartMovement();
}

bool AMREnemy::GetMovePoint(AActor* ToActor, FVector& OutResult)
{
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if(!NavSys)
		return false;
	 
	FNavLocation Result;

	bool const bSuccess = NavSys->GetRandomPointInNavigableRadius(ToActor->GetActorLocation(), 500.f, Result);

	//Out
	OutResult = Result.Location;
	return bSuccess;
}

void AMREnemy::MoveToPoint(FVector& Point)
{
	if (!AIController)
		return;

	MovementStatus = EEnemyMovementStatus::EMS_MoveToPoint;

	const EPathFollowingRequestResult::Type RequestResult = AIController->MoveToLocation(Point);

	if (RequestResult != EPathFollowingRequestResult::RequestSuccessful)
	{
		ULog::Error("Enemy Path Request Error!!!");
	} else
	{
		ULog::Success("Enemy Path Successfully Set!!!");
	}
}


// Called every frame
void AMREnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMREnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMREnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bAlive)
	{
		TakeHealth(Damage);
	}

	return CurrentHealth;
}

void AMREnemy::TakeHealth(const float Damage)
{
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
	{
		Death();
	}
}

void AMREnemy::Death()
{
	bAlive = false;
	MovementStatus = EEnemyMovementStatus::EMS_Dead;
	
	if (AIController)
	{
		AIController->StopMovement();
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const float DestroyTime = FMath::RandRange(TimeBeforeDestroy.X, TimeBeforeDestroy.Y);

	// Destroy actor by timer 
	GetWorldTimerManager().SetTimer(TimerHandle_Destroy, this, &AMREnemy::DestroyEnemy, DestroyTime, false);
}

void AMREnemy::DestroyEnemy()
{
	Destroy();
}

void AMREnemy::StartMovement()
{
	if (!MoveToActor)
		return;

	FVector MovePoint;
	if (GetMovePoint(MoveToActor, MovePoint))
	{
		MoveToPoint(MovePoint);
	}
	
}

void AMREnemy::MoveToTarget(AActor* Target)
{
	if (!AIController || !Alive())
		return;

	MovementStatus = EEnemyMovementStatus::EMS_MoveToTarget;

	FAIMoveRequest AIMoveRequest(Target);
	AIMoveRequest.SetAcceptanceRadius(1.f);

	FNavPathSharedPtr NavPathShared;

	AIController->MoveTo(AIMoveRequest, OUT &NavPathShared);

	/** Draw Debug line*/
	/*auto PathPoints = NavPathShared->GetPathPoints();

	FVector PreviousPoint = FVector(0, 0, 0);

	for (auto PathPoint : PathPoints)
	{
	auto EndPoint = PathPoint.Location;

	if (PreviousPoint.IsZero())
	{
	PreviousPoint = EndPoint;
	continue;
	}

	UKismetSystemLibrary::DrawDebugLine(GetWorld(), PreviousPoint, EndPoint, FLinearColor::Red, 5.f, 1.f);
	PreviousPoint = EndPoint;
	}*/
}

void AMREnemy::SetMoveToActor(AActor* NewActor)
{
	MoveToActor = NewActor;
}

bool AMREnemy::Alive()
{
	return bAlive;
}

void AMREnemy::AttackTarget(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("AEnemy::AttackTarget:Start"));
	if (!CombatMontage || !AnimInstance || !Alive())
		return;

	bAttacking = true;
	CombatTarget = Target;
	AnimInstance->Montage_Play(CombatMontage, 1.f);
}

void AMREnemy::AttackFinish()
{
	if (bOverlappingCombatSphere && CombatTarget)
	{
		AttackTarget(CombatTarget);
	} else if (bOverlappingAgroSphere && AgroTarget)
	{
		MoveToTarget(AgroTarget);
	}
}

void AMREnemy::MakeDamage()
{
	if (!Alive())
		return;
	
	const FVector StartTrace = GetActorLocation();
	const FVector EndTrace = StartTrace + GetActorForwardVector() * 300;

	const FHitResult& HitResult = AttackTrace(StartTrace, EndTrace);

	if (HitResult.bBlockingHit)
	{
		// @TODO: Check is target attackable and valid for attack
		
		AMRMech* AttackTarget = Cast<AMRMech>(HitResult.Actor);
		if (AttackTarget)
		{
			UGameplayStatics::ApplyPointDamage(AttackTarget, 3, GetActorForwardVector(), HitResult, AIController, this, DamageTypeClass);
		}
	}	
}

FHitResult AMREnemy::AttackTrace(const FVector& TraceFrom, const FVector& TraceTo) const
{
	// Perform trace to retrieve hit info
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, this);
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bDebugQuery = true;

	FHitResult Hit(ForceInit);
	//GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, COLLISION_WEAPON_TRACE, TraceParams);

	// create a collision sphere
	FCollisionShape ColSphere = FCollisionShape::MakeSphere(80.0f);

	bool IsHit = GetWorld()->SweepSingleByChannel(Hit, TraceTo, TraceTo, FQuat::Identity, COLLISION_WEAPON_TRACE, ColSphere);

	// Draw debug line
	if (IsHit && bDebug)
	{
		DrawDebugLine(GetWorld(), TraceFrom, TraceTo, FColor::Green, false, 5, 0.f, 1.f);

		// draw collision sphere
		DrawDebugSphere(GetWorld(), TraceTo, ColSphere.GetSphereRadius(), 24, FColor::Purple, true);

		if (Hit.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 12.f, 8, FColor::Red,false,5.f,0,1.f);
		}
	}

	return Hit;
}

void AMREnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMRMech* NewTarget = Cast<AMRMech>(OtherActor);

	if (NewTarget && Alive())
	{
		bOverlappingAgroSphere = true;
		AgroTarget = NewTarget;
		MoveToTarget(NewTarget);
	}
}

void AMREnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMRMech* NewTarget = Cast<AMRMech>(OtherActor);

	if (NewTarget && Alive())
	{
		bOverlappingAgroSphere = false;
		AgroTarget = nullptr;
	}
}

void AMREnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMRMech* NewTarget = Cast<AMRMech>(OtherActor);

	if (NewTarget)
	{
		bOverlappingCombatSphere = true;
		AttackTarget(NewTarget);
	}

}

void AMREnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMRMech* NewTarget = Cast<AMRMech>(OtherActor);

	if (NewTarget)
	{
		bOverlappingCombatSphere = false;
		CombatTarget = nullptr;
	}
}

