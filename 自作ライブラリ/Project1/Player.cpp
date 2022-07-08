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
	:BaseGameActor(arg_pos)
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
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::PLAYER);
}

void Player::Initialize()
{	
	BaseGameActor::Initialize();
}

void Player::Update()
{
	KillRandEnem();

	if (Input::TriggerKey(DIK_RETURN))
	{
		InFever();
	}

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

	panelCountSprite3D->Draw();

	if (Object3D::GetDrawShadow())
	{
		pipelineName = "FBXShadowMap";
	}
	else
	{
		pipelineName = "FBX";
	}
}

void Player::CompleteCut()
{
	panelCutLocus->RecordCuttedPanelPos();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	int num = field->CutPanel(panelCutLocus, bonusCount);
	/*weight += num * FieldPiece::GetWeight();
	gottenPanel += num;*/
	if (targetActor)
	{
		if (targetActor->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			targetActor->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor->ForcedWeight(num, this);
		}
	}

	cutPower = 0;

	if (field->IsNewFeverPlayer())
	{
		InFever();
	}
	//ScoreManager::GetInstance()->AddScore_CutPanel(num);

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

void Player::SetTargetActor()
{
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();

	if (!targetActor)
	{
		for (int i = 0; i < actors.size(); i++)
		{
			if (actors[i] == this)
			{
				continue;
			}

			if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
				actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
			{
				targetActor = actors[i];
				targetIndex = i;
				break;
			}
		}
	}
	else
	{
		if (targetActor->GetActionState()->GetLabel() == ActionStateLabel::FALL ||
			targetActor->GetActionState()->GetLabel() == ActionStateLabel::SPAWN)
		{
			targetActor = nullptr;
			targetIndex = -1;
			for (int i = 0; i < actors.size(); i++)
			{
				if (actors[i] == this)
				{
					continue;
				}

				if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
					actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
				{
					targetActor = actors[i];
					targetIndex = i;
					break;
				}
			}
		}
	}

	if (!targetActor)
	{
		return;
	}

	if (actionState->GetLabel() != ActionStateLabel::MOVE)
	{
		return;
	}

	if (!Input::TriggerPadRightTrigger())
	{
		return;
	}

	//現在のtargetEnemyより後ろ
	for (int i = targetIndex + 1; i < actors.size(); i++)
	{
		if (actors[i] == this)
		{
			continue;
		}

		if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor = actors[i];
			targetIndex = i;
			return;
		}
	}
	//現在のtargetEnemyより前
	for (int i = 0; i < targetIndex; i++)
	{
		if (actors[i] == this)
		{
			continue;
		}

		if (actors[i]->GetActionState()->GetLabel() != ActionStateLabel::FALL &&
			actors[i]->GetActionState()->GetLabel() != ActionStateLabel::SPAWN)
		{
			targetActor = actors[i];
			targetIndex = i;
			return;
		}
	}
}
