#include "Player.h"

#include"OBJLoader.h"
#include"SphereCollider.h"
#include"Input.h"
#include"DebugText.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "GameSettingParam.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "FieldPiece.h"
#include "ItemEmitter.h"
#include "UnableThroughEdge.h"
#include "UnableThroughBlock.h"
#include "ScoreManager.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "ObjectRegistType.h"
#include "IActionState.h"
#include "ActionStateFall.h"
#include "ActionStateMove.h"

Player::Player(const Vector3& arg_pos)
	:BaseGameActor(arg_pos),
	 targetEnemy(nullptr)
{
	if (!BaseGameActor::constCameraBuff)
	{
		InitCamera(position);
	}

	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("GamePlay_Player");
	//モデルの生成
	Create(myModel);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::PLAYER);

	panelCountSprite3D = new PanelCountSprite3D(position, name, gottenPanel);

	Initialize();
}

Player::~Player()
{			
	Audio::StopWave("SE_SteppingOn");		
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::PLAYER);
}

void Player::Initialize()
{	
	BaseGameActor::Initialize();
	targetEnemy = nullptr;
}

void Player::Update()
{
	KillRandEnem();
	SetTargetEnemy();

	BaseGameActor::Update();
}

void Player::Draw()
{
	BaseGameActor::Draw();
}

void Player::DrawReady()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
		XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
		Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
		cameraDirectionZ.Normalize();
		ImGui::Begin("PlayerStatus");
		ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
		ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
		ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
		ImGui::Text("Rot : {%f, %f, %f }\n", rotation.x, rotation.y, rotation.z);
		ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
		ImGui::End();
	}
#endif

	if (Object3D::GetDrawShadow())
		pipelineName = "FBXShadowMap";
	else
	{
			pipelineName = "FBX";
	}
}

void Player::CompleteCut()
{
	panelCutLocus->RecordCuttedPanelPos();
	int num = ActorManager::GetInstance()->GetFields()[0]->CutPanel(panelCutLocus, bonusCount);
	/*weight += num * FieldPiece::GetWeight();
	gottenPanel += num;*/
	if (targetEnemy)
	{
		if (targetEnemy->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			targetEnemy->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetEnemy->ForcedWeight(num);
		}
	}

	static const int BONUS_COUNT_UNIT = 3;
	if (bonusCount > bonusCount * 3)
	{
		bonusCount = bonusCount * 3;
	}
	cutPower = bonusCount / BONUS_COUNT_UNIT;
	//cutPower = 0;

	ScoreManager::GetInstance()->AddScore_CutPanel(num);

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);
	virtualityPlanePosition = info->ridingPiece->GetVirtualityPlanePosition();
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	ChangeActionState(actionState, ActionStateMove::GetInstance());
	actionState = ActionStateMove::GetInstance();
}

void Player::KillRandEnem()
{
	if (!Input::TriggerKey(DIK_M))
	{
		return;
	}

	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	
	for (auto e : enemies)
	{
		if (e->GetActionState()->GetLabel() == ActionStateLabel::FALL)
		{
			continue;
		}
		else
		{
			e->ChangeActionState(e->GetActionState(), ActionStateFall::GetInstance());
			e->SetActionState(ActionStateFall::GetInstance());
			return;
		}
	}
}

void Player::SetTargetEnemy()
{
	auto enemies = ActorManager::GetInstance()->GetStandardEnemies();
	static int index = -1;

	if (!targetEnemy)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			if (enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
				enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
			{
				targetEnemy = enemies[i];
				index = i;
				break;
			}
		}
	}
	else
	{
		if (targetEnemy->GetActionState()->GetLabel() == ActionStateLabel::FALL ||
			targetEnemy->GetActionState()->GetLabel() == ActionStateLabel::SPAWN)
		{
			targetEnemy = nullptr;
			index = -1;
			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
					enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
				{
					targetEnemy = enemies[i];
					index = i;
					break;
				}
			}
		}
	}

	if (!targetEnemy)
	{
		return;
	}

	if (!Input::TriggerPadRightTrigger())
	{
		return;
	}

	//現在のtargetEnemyより後ろ
	for (int i = index + 1; i < enemies.size(); i++)
	{
		if (enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetEnemy = enemies[i];
			index = i;
			return;
		}
	}
	//現在のtargetEnemyより前
	for (int i = 0; i < index; i++)
	{
		if (enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			enemies[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetEnemy = enemies[i];
			index = i;
			return;
		}
	}
}
