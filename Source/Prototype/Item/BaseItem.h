// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h" 
#include "BaseItem.generated.h"

enum class StatType;

UENUM()
enum class ItemType
{
	Equipment,
	Consume,
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _Code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ItemType _Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	StatType _ModTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString _Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int _Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* _Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* _Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* _Skeletal;
};

UCLASS()
class PROTOTYPE_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

	virtual void SetItemWithCode(int32 itemCode);

	void Init();
	void Disable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	virtual void GetItem();
	virtual void UseItem();
	virtual void DropItem(FVector location, FRotator rotation = FRotator::ZeroRotator);

	virtual void SetPlayer();

	int GetCode() { return ItemData._Code; }
	FString GetName() { return ItemData._Name; }
	ItemType GetType() { return ItemData._Type; }
	StatType GetModStat() { return ItemData._ModTarget; }
	FString GetDesc() { return ItemData._Description; }
	int32 GetPrice() { return ItemData._Price; }
	int32 GetValue() { return ItemData._Value; }
	UStaticMesh* GetSkeletalMesh() { return ItemData._Mesh; }
	UTexture2D* GetTexture() { return ItemData._Texture; }
	int32 GetEquip() { return ItemData._Equip; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent *_meshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent *_trigger;

	UPROPERTY()
	class AMyPlayer *_player;

    UPROPERTY()
    FItemData ItemData;
};
