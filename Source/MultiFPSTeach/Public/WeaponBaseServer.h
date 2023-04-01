// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBaseClient.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponBaseServer.generated.h"


UENUM()
enum class EWeaponType :uint8
{
	Ak47 UMETA(DisplayName = "AK47"),
	M4A1 UMETA(DisplayName = "M4A1"),
	MP7 UMETA(DisplayName = "MP7"),
	DesertEagle UMETA(DisplayName = "DesertEagle"),
	Sniper UMETA(DisplayName = "Sniper"),
	EEND
};

UCLASS()
class MULTIFPSTEACH_API AWeaponBaseServer : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBaseServer();
	UPROPERTY(EditAnywhere)
	EWeaponType KindOfWeapon;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AWeaponBaseClient> ClientWeaponBaseBPClass;

	UFUNCTION()
	void OnOtherBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void EquipWeapon();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere) 
	int32 GunCurrentAmmo;// 枪体现在剩余子弹

	UPROPERTY(EditAnywhere,Replicated) //如果服务器改变了，客户端也要自动改变
	int32 ClipCurrentAmmo;// 弹夹现在剩余子弹

	UPROPERTY(EditAnywhere) 
	int32 MaxClipAmmo;// 弹夹容量

	UPROPERTY(EditAnywhere)
	UAnimMontage* ServerTPBodysShootAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* ServerTPBodysReloadAnimMontage;

	UPROPERTY(EditAnywhere)
	float BulletDistance;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* BulletDecalMaterial;

	UPROPERTY(EditAnywhere)
	float BaseDamage;

	UPROPERTY(EditAnywhere)
	bool IsAutomatic;

	UPROPERTY(EditAnywhere)
	float AutomaticFireRate;

	UPROPERTY(EditAnywhere)
	UCurveFloat* VerticalRecoilCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* HorizontalRecoilCurve;

	UPROPERTY(EditAnywhere)
	float MovingFireRandomRange;

	UPROPERTY(EditAnywhere,Category = "SpreadWeaponData")
	float SpreadWeaponCallBackRate;

	UPROPERTY(EditAnywhere,Category = "SpreadWeaponData")
	float SpreadWeaponMinIndex;

	UPROPERTY(EditAnywhere,Category = "SpreadWeaponData")
	float SpreadWeaponMaxIndex;
	
	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiShootingEffect();
	void MultiShootingEffect_Implementation();
	bool MultiShootingEffect_Validate();
};
