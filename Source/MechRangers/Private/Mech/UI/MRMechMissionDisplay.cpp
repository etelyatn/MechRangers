// Copyright PlatoSpace.com All Rights Reserved.


#include "Mech/UI/MRMechMissionDisplay.h"
#include "Components/WidgetComponent.h"
#include "UI/MRDisplayWidgetComponent.h"

// Sets default values
AMRMechMissionDisplay::AMRMechMissionDisplay()
{
	PrimaryActorTick.bCanEverTick = false;

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DisplayMesh->SetGenerateOverlapEvents(false);
	DisplayMesh->SetCollisionProfileName(FName("NoCollision"));
	RootComponent = DisplayMesh;

	DisplayWidget = CreateDefaultSubobject<UMRDisplayWidgetComponent>(TEXT("DisplayWidget"));
	DisplayWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DisplayWidget->SetGenerateOverlapEvents(false);
	DisplayWidget->SetCollisionProfileName(FName("NoCollision"));
	DisplayWidget->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMRMechMissionDisplay::BeginPlay()
{
	Super::BeginPlay();
	
}
