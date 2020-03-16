// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUsage.h"

void USkillUsage::Fire(ACharacterBase* character, FString skillname, FTransform spawnParent) {
	struct FSkillInfo & skill = character->Skill;
	if (skill.Cooldown >= skill.Timer) {
		return;
	}
	if (skill.type == ESkillType::VE_Projectile) {
		FireProjectile(character, skill, spawnParent);
	}

	skill.Timer = 0;
}

void USkillUsage::ActivatePowerUp(ACharacterBase * character, EPowerUpType name)
{
	switch(name)
	{
		case EPowerUpType::EDadAttackSpeedBoost:
			character->ActivateAttackPowerUp();
			break;
		case EPowerUpType::EDadMoveSpeedBoost:
			character->ActivateSpeedPowerUp();
			break;
		case EPowerUpType::EGirlMoveSpeedBoost:
			character->ActivateSpeedPowerUp();
			break;
		case EPowerUpType::EGirlInvincibilityBoost:
			character->ActivateInvincibilityPowerUp();
			break;
	}
}

void USkillUsage::Cooldown(float deltaTime, struct FSkillInfo& skill) {
	if (skill.Timer <= skill.Cooldown) {
		skill.Timer += deltaTime;
	}
}

void USkillUsage::FireProjectile(ACharacterBase* character, struct FSkillInfo& skill, FTransform spawnParent) {
	FRotator rotator = spawnParent.GetRotation().Rotator();
	AProjectileBase* projectile = character->GetWorld()->SpawnActor<AProjectileBase>(character->Skill.projectile, spawnParent.GetLocation(), rotator);
	FVector from = character->GetActorLocation();
	FVector to = spawnParent.GetLocation();

	// set the velocity of the projectile
	FVector velocity = (to - from);
	velocity.Normalize();
	velocity = velocity * skill.Speed;

	//set projectile property
	projectile->Speed = velocity;
	projectile->Damage = skill.Damage;
	projectile->Range = skill.Range;
}
