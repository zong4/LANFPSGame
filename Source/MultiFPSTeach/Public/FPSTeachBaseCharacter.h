// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiFPSPlayerController.h"
#include "WeaponBaseServer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "FPSTeachBaseCharacter.generated.h"

UCLASS()
class MULTIFPSTEACH_API AFPSTeachBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSTeachBaseCharacter();

	UFUNCTION()
	void DelayBeginPlayCallBack();

private:
#pragma region Component
	UPROPERTY(Category=Character,VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(Category=Character,VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FPArmsMesh;

	UPROPERTY(Category=Character,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	UAnimInstance* ClientArmsAnimBP;
	
	UPROPERTY(Category=Character,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	UAnimInstance* ServerBodysAnimBP;

	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	AMultiFPSPlayerController* FPSPlayerController;

	UPROPERTY(EditAnywhere)
	EWeaponType TestStartWeapon;

	
#pragma endregion 

protected:
	virtual void BeginPlay() override;
	
#pragma region InputEvent
	void MoveRight(float AxisValue);
	void MoveForward(float AxisValue);

	void JumpAction();
	void StopJumpAction();

	void InputFirePressed();
	void InputFireReleased();

	void InputAimingPressed();
	void InputAimingReleased();

	void LowSpeedWalkAction();
	void NormalSpeedWalkAction();

	void InputReload();

#pragma endregion

	

#pragma region Weapon
public:
	
	void EquipPrimary(AWeaponBaseServer* WeaponBaseServer);

	void EquipSecondary(AWeaponBaseServer* WeaponBaseServer);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFPArmsBlendPose(int NewIndex);
private:
	UPROPERTY(meta=(AllowPrivateAccess = "true"),Replicated)
	EWeaponType ActiveWeapon;
	
	UPROPERTY(meta=(AllowPrivateAccess = "true"))
	AWeaponBaseServer* ServerPrimaryWeapon;

	UPROPERTY(meta=(AllowPrivateAccess = "true"))
	AWeaponBaseServer* ServerSecondaryWeapon;

	UPROPERTY(meta=(AllowPrivateAccess = "true"))
	AWeaponBaseClient* ClientPrimaryWeapon;

	UPROPERTY(meta=(AllowPrivateAccess = "true"))
	AWeaponBaseClient* ClientSecondaryWeapon;
	
	void StartWithKindOfWeapon();

	void PurchaseWeapon(EWeaponType WeaponType);

	AWeaponBaseClient* GetCurrentClientFPArmsWeaponAtcor();
	AWeaponBaseServer* GetCurrentServerTPBodysWeaponAtcor();
	
#pragma endregion
private:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma region Fire
public:
	//计时器
	FTimerHandle AutomaticFireTimerHandle;
	void AutomaticFire();

	//后坐力
	float NewVerticalRecoilAmount;
	float OldVerticalRecoilAmount;
	float VerticalRecoilAmount;
	float RecoilXCoordPerShoot;
	void ResetRecoil();
	float NewHorizontalRecoilAmount;
	float OldHorizontalRecoilAmount;
	float HorizontalRecoilAmount;

	float PistolSpreadMin = 0;
	float PistolSpreadMax = 0;
	
	//步枪的射击相关
	void FireWeaponPrimary();
	void StopFirePrimary();
	void RifleLineTrace(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	
	//手枪的射击相关
	void FireWeaponSecondary();
	void StopFireSecondary();
	void PistolLineTrace(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);

	UFUNCTION()
	void DelaySpreadWeaponShootCallBack();
	
	//狙击枪的射击相关
	void FireWeaponSniper();
	void StopFireSniper();
	void SniperLineTrace(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);

	UPROPERTY(Replicated)
	bool IsAiming;

	UPROPERTY(VisibleAnywhere,Category= "SniperUI")
	UUserWidget* WidgetScope;

	UPROPERTY(EditAnywhere,Category= "SniperUI")
	TSubclassOf<UUserWidget> SniperScopeBPClass;

	UFUNCTION()
	void DelaySniperShootCallBack();


	//Reload
	UPROPERTY(Replicated)
	bool IsFiring;
	
	UPROPERTY(Replicated)
	bool IsReloading;

	UFUNCTION()
	void DelayPlayArmReloadCallBack();

	
	
	void DamagePlayer(UPhysicalMaterial* PhysicalMaterial,AActor* DamagedActor, FVector& HitFromDirection,FHitResult& HitInfo);

	UFUNCTION()
	void OnHit(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser );

	float Health;

	void DeathMatchDeath(AActor* DamageActor);
#pragma endregion 

public:
	
#pragma region NetWorking

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerLowSpeedWalkAction();
	void ServerLowSpeedWalkAction_Implementation();
	bool ServerLowSpeedWalkAction_Validate();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerNormalSpeedWalkAction();
	void ServerNormalSpeedWalkAction_Implementation();
	bool ServerNormalSpeedWalkAction_Validate();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFireRifleWeapon(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	void ServerFireRifleWeapon_Implementation(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	bool ServerFireRifleWeapon_Validate(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFireSniperWeapon(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	void ServerFireSniperWeapon_Implementation(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	bool ServerFireSniperWeapon_Validate(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerFirePistolWeapon(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	void ServerFirePistolWeapon_Implementation(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	bool ServerFirePistolWeapon_Validate(FVector CameraLocation,FRotator CameraRotation,bool IsMoving);
	
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerReloadPrimary();
	void ServerReloadPrimary_Implementation();
	bool ServerReloadPrimary_Validate();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerReloadSecondary();
	void ServerReloadSecondary_Implementation();
	bool ServerReloadSecondary_Validate();

	UFUNCTION(Server,Reliable,WithValidation)
	void ServerStopFiring();
	void ServerStopFiring_Implementation();
	bool ServerStopFiring_Validate();
	
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerSetAiming(bool AimingState);
	void ServerSetAiming_Implementation(bool AimingState);
	bool ServerSetAiming_Validate(bool AimingState);

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiShooting();
	void MultiShooting_Implementation();
	bool MultiShooting_Validate();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiReloadAnimation();
	void MultiReloadAnimation_Implementation();
	bool MultiReloadAnimation_Validate();

	UFUNCTION(NetMulticast,Reliable,WithValidation)
	void MultiSpawnBulletDecal(FVector Location, FRotator Rotation);
	void MultiSpawnBulletDecal_Implementation(FVector Location, FRotator Rotation);
	bool MultiSpawnBulletDecal_Validate(FVector Location, FRotator Rotation);
	
	UFUNCTION(Client,Reliable)
	void ClientEquipFPArmsPrimary();

	UFUNCTION(Client,Reliable)
	void ClientEquipFPArmsSecondary();

	UFUNCTION(Client,Reliable)
	void ClientFire();
	
	UFUNCTION(Client,Reliable)
	void ClientUpdateAmmoUI(int32 ClipCurrentAmmo,int32 GunCurrentAmmo);

	UFUNCTION(Client,Reliable)
	void ClientUpdatHealthUI(float NewHealth);

	UFUNCTION(Client,Reliable)
	void ClientRecoil();

	UFUNCTION(Client,Reliable)
	void ClientReload();
	
	UFUNCTION(Client,Reliable)
	void ClientAiming();

	UFUNCTION(Client,Reliable)
	void ClientEndAiming();

	UFUNCTION(Client,Reliable)
	void ClientDeathMatchDeath();

	
#pragma endregion 
	
};