#include "StandardEnemy.h"

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
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "FieldPiece.h"
#include "ItemEmitter.h"
#include "Player.h"
#include "Audio.h"
#include "ScoreManager.h"
#include "ObjectRegistType.h"
#include "IActionState.h"
#include "ActionStateMove.h"

const float INTERVAL_ACTIONTIMER = 180.0f;

StandardEnemy::StandardEnemy(const Vector3& arg_pos)
	:BaseGameActor(arg_pos)
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("GamePlay_Enemy");
	//モデルの生成
	Create(myModel);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::STANDARD_ENEMY);

	panelCountSprite3D = new PanelCountSprite3D(position, name, gottenPanel);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);

	Initialize();
}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::STANDARD_ENEMY);
}

void StandardEnemy::Initialize()
{
	actionTimer->Initialize();

	BaseGameActor::Initialize();
}

void StandardEnemy::Update()
{
	// 位置の確認
	ConfirmPlayerPos();
	ConfirmItemPos();

	BaseGameActor::Update();
}

void StandardEnemy::Draw()
{
	BaseGameActor::Draw();
}

void StandardEnemy::DrawReady()
{
#ifdef _DEBUG
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
	cameraDirectionZ.Normalize();
	ImGui::Begin("EnemyStatus");
	ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
	ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
	ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
	ImGui::Text("Rot : {%f, %f, %f }\n", rotation.x, rotation.y, rotation.z);
	ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
	ImGui::End();

#endif

	pipelineName = "FBX";
}

void StandardEnemy::StartMove()
{
	actionTimer->Reset();
}

void StandardEnemy::OnMove(ActionStateLabel& arg_label)
{
	// タイマー更新
	actionTimer->Update();

	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	speed = WALK_SPEED;

	SlidingDown();

	//移動方向
	Vector3 moveDirection = direction;
	DecideDirection(moveDirection);

	//回転処理
	//現在の進行方向とカメラの正面と角度を求める
	direction.Normalize();
	float cosA = direction.Dot(moveDirection);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;

	float rotY = acos(cosA) * 180 / 3.14159365f;
	const Vector3 CrossVec = direction.Cross(moveDirection);

	float rotSpeed = ROTATE_SPEED;
	if (abs(rotY) < 55)
	{
		virtualityPlanePosition += moveDirection * speed;
	}

	if (rotSpeed > abs(rotY))
	{
		rotSpeed = rotY;
	}
	if (CrossVec.y < 0)
		rotSpeed *= -1;
	rotation.y += rotSpeed;
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
	XMVECTOR dir = { direction.x,direction.y,direction.z,0 };
	dir = XMVector3TransformNormal(dir, matRot);
	direction = dir;

	UpdatePos();
}

void StandardEnemy::StartTackle()
{
	//Moveからのみ実行可能
	tackleCount = 0;
	isHitDuringTackle = false;

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//スティックの向き
	ConfirmPlayerPos();
	auto vec = LocusUtility::Dim3ToDim2XZ(Vector3::Normalize(playerPos - virtualityPlanePosition));
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;
}

void StandardEnemy::StartWithstand()
{
	inputStartCount = nextInputStartCount;
	withstandTime = 0;

	preWithstandVec.y = 0;
	preWithstandVec.Normalize();

	isReturningField = false;
}

void StandardEnemy::OnWithstand(ActionStateLabel& arg_label)
{
	BaseGameActor::OnWithstand(arg_label);

	//prePos = position;
	//preVirtualityPlanePosition = virtualityPlanePosition;

	////踏ん張り中　赤
	//Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	//if (BGColor >= 0)
	//{
	//	BGColor -= 0.02f;
	//}
	//else
	//{
	//	BGColor = 1;
	//}

	////カメラのビュー行列の逆行列を計算
	//XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	//const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	//const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	//Vector2 stickDirection = {};
	////スティックの向き
	//auto vec = Vector2(0, 0);
	//withstandTime++;
	//{
	//	if (withstandTime >= 20)
	//	{
	//		vec = LocusUtility::Dim3ToDim2XZ(LocusUtility::AngleToVector2(std::rand() % 360));
	//		withstandTime = 0;
	//	}
	//}
	//stickDirection.x = (cameraDirectionX * vec.x).x;
	//stickDirection.y = (cameraDirectionZ * vec.y).z;
	//stickDirection = Vector2::Normalize(stickDirection);

	//float accuracy = 0;

	//Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preWithstandVec);

	//accuracy = Vector2::Dot(stickDirection, correctVec);

	//if (accuracy <= 0)
	//{
	//	accuracy = 0;
	//}

	//if (vec.x = 0 && vec.y == 0)
	//{
	//	accuracy = 0;
	//}

	//if (accuracy >= 0.55f)
	//{
	//	Vector3 moveDirection = preWithstandVec;
	//	moveEasingCount = 0;
	//	isReturningField = true;
	//	returningStartPos = virtualityPlanePosition;
	//	returningEndPos = virtualityPlanePosition + moveDirection * 3;
	//	return;
	//}

	//if (isReturningField)
	//{
	//	Object::SetColor({ 1,1,1,1 });
	//	virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
	//	moveEasingCount++;
	//	if (moveEasingCount >= 30)
	//	{
	//		arg_label = ActionStateLabel::MOVE;
	//	}
	//}

	//UpdatePos();
}

void StandardEnemy::CompleteCut()
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

void StandardEnemy::DecideDirection(Vector3& arg_direction)
{
	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	if (actionTimer->IsTime())
	{
		// 向きを指定して移動
		//moveDir = RandomDir();
		moveDir = NearObjDir();
		actionTimer->Reset();
	}
	arg_direction = cameraDirectionX * moveDir.x + cameraDirectionZ * moveDir.y;

	arg_direction.Normalize();
	//反発用に代入
	velocity = arg_direction;
}

void StandardEnemy::SetTargetActor()
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
}

Vector2 StandardEnemy::RandomDir()
{
	Vector2 dir = { 0,0 };

	dir.x = (float)((rand() % 20 - 10) / 10.0f);
	dir.y = (float)((rand() % 20 - 10) / 10.0f);
	dir = Vector2::Normalize(dir);

	return dir;
}

void StandardEnemy::ConfirmPlayerPos()
{
	playerPos = ActorManager::GetInstance()->GetPlayer()->GetVirtualityPlanePosition();
}

void StandardEnemy::ConfirmItemPos()
{
	// ActorManagerからItem vectorをもらう
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();
	// vectorが空だったら
	if (items.size() <= 0)
	{
		return;
	}

	Vector3 itemDistance;
	Vector3 nearestItemDistance = items[0]->GetPosition() - position;

	// 全アイテムを走査
	for (auto item : items)
	{
		itemDistance = item->GetPosition() - position;

		// より自分に近いアイテムが見つかったら
		if (itemDistance.Length() < nearestItemDistance.Length())
		{
			// 距離の更新
			nearestItemDistance = itemDistance;
			// 一番近いアイテムの位置を更新
			itemPos = item->GetPosition();
		}
	}
}

Vector2 StandardEnemy::NearObjDir()
{
	Vector3 itemRange = itemPos - position;
	Vector3 playerRange = playerPos - position;
	Vector2 dir;
	// アイテムが近かったらアイテムの方へ
	// プレイヤーが近かったらプレイヤーの方へ
	if (itemRange.Length() < playerRange.Length())
	{
		dir = { itemRange.x,itemRange.z };
		dir.Normalize(dir);
	}
	else
	{
		dir = { playerRange.x,playerRange.z };
		dir.Normalize(dir);
	}

	return dir;
}

bool StandardEnemy::RangeCheckPlayer()
{
	if (actionState->GetLabel() == ActionStateLabel::BLOWN) { return false; }
	Player* player = ActorManager::GetInstance()->GetPlayer();
	if (!player)
	{
		return false;
	}

	float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(player->GetVirtualityPlanePosition() - virtualityPlanePosition));
	if (length <= AttackRange)
	{
		return true;
	}

	return false;
}

bool StandardEnemy::IsChangeMoveToTackle()
{
	//int rnd = std::rand() % 30;
	return false;
}

bool StandardEnemy::IsChangeMoveToCut()
{
	int rnd = std::rand() % 30;
	if (rnd != 0)
	{
		return false;
	}
	CuttingInfo* info = ActorManager::GetInstance()->GetFields()[0]->GetCuttingInfo(this);
	return cutPower >= cutPowerLimit && cutPower > 0 && info->ridingPiece;
}