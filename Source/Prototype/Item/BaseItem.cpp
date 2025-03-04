// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/BaseItem.h"

#include "Player/MyPlayer.h"
#include "Player/MyPlayerController.h"

#include "Base/MyGameInstance.h"
#include "Base/Managers/EffectManager.h"
#include "Component/InventoryComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));

	_meshComponent->SetupAttachment(RootComponent);
	_trigger->SetupAttachment(_meshComponent);

	_trigger->SetSphereRadius(60.0f);

	_meshComponent->SetCollisionProfileName(TEXT("Item"));
	_trigger->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ABaseItem::SetItemWithCode(int32 itemCode)
{
	auto gameinstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (gameinstance != nullptr)
	{
		FItemData *data = gameinstance->GetConsumeItemData(itemCode);
		if (data == nullptr || data->_Name == TEXT(""))
		{
			return;
		}

		ItemData = *data;

		_meshComponent->SetStaticMesh(ItemData._Mesh);
	}
}

void ABaseItem::Init()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseItem::Disable()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	SetPlayer();
}

void ABaseItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_trigger->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnOverlapBegin);
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseItem::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	_player = Cast<AMyPlayer>(OtherActor);
	if (_player == nullptr)
		return;
	_player->GetItem(this);
	Disable();
}

void ABaseItem::GetItem()
{
}

void ABaseItem::UseItem()
{
	if (!_player)
		SetPlayer();

	_player->GetStatComponent()->UseItem(this);
}

void ABaseItem::DropItem(FVector location, FRotator rotation)
{
	SetActorLocationAndRotation(location, rotation);
	Init();
}

void ABaseItem::SetPlayer()
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		AMyPlayer *CurrentPlayer = Cast<AMyPlayer>(PlayerController->GetPawn());
		if (CurrentPlayer)
		{
			_player = CurrentPlayer;
		}
	}
}
