// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseServer.h"

#include "FPSTeachBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AWeaponBaseServer::AWeaponBaseServer()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	WeaponMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	WeaponMesh->SetOwnerNoSee(true);
	WeaponMesh->SetEnableGravity(true);
	WeaponMesh->SetSimulatePhysics(true);


	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&AWeaponBaseServer::OnOtherBeginOverlap);

	bReplicates = true;
}



void AWeaponBaseServer::OnOtherBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSTeachBaseCharacter* FPSCharacter = Cast<AFPSTeachBaseCharacter>(OtherActor);
	if(FPSCharacter)
	{
		EquipWeapon();
		if(KindOfWeapon == EWeaponType::DesertEagle)
		{
			FPSCharacter->EquipSecondary(this);
		}
		else
		{
			//玩家逻辑
			FPSCharacter->EquipPrimary(this);
		}
	}
}

//被拾取的时候自己设置碰撞相关属性
void AWeaponBaseServer::EquipWeapon()
{
	WeaponMesh->SetEnableGravity(false);
	WeaponMesh->SetSimulatePhysics(false);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeaponBaseServer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBaseServer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBaseServer::MultiShootingEffect_Implementation()
{
	if(GetOwner() != UGameplayStatics::GetPlayerPawn(GetWorld(),0))
	{
		FName MuzzleFlashSocketName = TEXT("Fire_FX_Slot");
		if(KindOfWeapon == EWeaponType::M4A1)
		{
			MuzzleFlashSocketName = TEXT("MuzzleSocket");
		}
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash,WeaponMesh,MuzzleFlashSocketName,
		FVector::ZeroVector,FRotator::ZeroRotator,FVector::OneVector,
		EAttachLocation::KeepRelativeOffset,true,EPSCPoolMethod::None,
		true
		);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),FireSound,GetActorLocation());
	}
}

bool AWeaponBaseServer::MultiShootingEffect_Validate()
{
	return true;
}

void AWeaponBaseServer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AWeaponBaseServer,ClipCurrentAmmo,COND_None);
}