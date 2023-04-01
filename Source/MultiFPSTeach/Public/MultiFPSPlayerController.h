// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MultiFPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MULTIFPSTEACH_API AMultiFPSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void PlayerCameraShake(TSubclassOf<UCameraShakeBase> CameraShake);

	UFUNCTION(BlueprintImplementableEvent,Category="PlayerUI")
	void CreatePlayerUI();

	UFUNCTION(BlueprintImplementableEvent,Category="PlayerUI")
	void DoCrosshairRecoil();

	UFUNCTION(BlueprintImplementableEvent,Category="PlayerUI")
	void UpdateAmmoUI(int32 ClipCurrentAmmo,int32 GunCurrentAmmo);

	UFUNCTION(BlueprintImplementableEvent,Category="PlayerUI")
	void UpdateHealthUI(float NewHealth);

	UFUNCTION(BlueprintImplementableEvent,Category = "Health")
	void DeathMatchDeath(AActor* DamageCauser);
};
