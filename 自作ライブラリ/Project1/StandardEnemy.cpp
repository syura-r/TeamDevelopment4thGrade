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
#include "ObjectRegistType.h"
#include "IActionState.h"
#include "ActionStateMove.h"
#include "EnemyAIPositiv.h"

const float INTERVAL_ACTIONTIMER = 10.0f;

StandardEnemy::StandardEnemy(const Vector3& arg_pos, const EnemyAILabel& arg_AILabel, const std::string& modelName)
	:BaseGameActor(arg_pos)
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel(modelName);
	//モデルの生成
	Create(myModel);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::STANDARD_ENEMY);

	panelCountSprite3D = new PanelCountSprite3D(position, name, gottenPanel);
	dropPointGetUI = new DropPointGetUI(position, name);
	//chart = new PieChart({ 1, 0, 0, 1 }, { 0, 0, 0, 1 });
	//chart->SetScale(Vector3(4, 4, 4));
	if (modelName == "GamePlay_Enemy")
	{
		actorColor = { 0.96f, 0.53f, 0.54f, 1 };
		effectColor = { 0.96f, 0.53f, 0.54f, 1 };
	}
	else
	{
		actorColor = { 0.51f, 0.92f, 0.60f, 1 };
		effectColor = { 0.51f, 0.92f, 0.60f, 1 };
	}
	//ObjectManager::GetInstance()->Add(chart);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);

	// AIラベルとポインターの取得
	enemyAILabel = arg_AILabel;		// ←ラベルによって何を呼ぶか決めるようにする
	enemyAI = EnemyAIPositiv::GetInstance();


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
	firstMoved = false;
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

	dropPointGetUI->Draw();
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
	int num = field->CutPanel(panelCutLocus, bonusCount, targetActor);
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
	totalCutCount += num;

	cutPower = MIN_CUTPOWER;

	if (field->IsNewFeverPlayer())
	{
		InFever(5 * 60);
	}

	CuttingInfo* info = field->GetCuttingInfo(this);
	virtualityPlanePosition = info->ridingPiece->GetVirtualityPlanePosition();
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	ChangeActionState(actionState, ActionStateMove::GetInstance());
	actionState = ActionStateMove::GetInstance();
}

void StandardEnemy::DecideDirection(Vector3& arg_direction)
{
	//カメラのビュー行列の逆行列を計算
	/*XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();*/

	if (actionTimer->IsTime())
	{
		//---方向決定系---
		Vector3 vel = enemyAI->ApproachEnergyItem(this, velocity);
		vel = enemyAI->KeepAwayFromFieldBorder(this, vel);
		vel = enemyAI->KeepAwayFromGottenPieces(this, vel, virtualityPlanePosition);

		Vector2 fixVel = LocusUtility::Dim3ToDim2XZ(vel);
		moveDir = fixVel;

		// 向きを指定して移動
		if (firstMoved == false)
		{
			moveDir = RandomDir();
			//moveDir = NearObjDir();
			firstMoved = true;
		}

		actionTimer->Reset();
	}
	arg_direction = LocusUtility::Dim2XZToDim3(moveDir);

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
	//cutPower確認
	if (cutPower == 0)
	{
		return false;
	}

	//ただの乱数
	int rnd = std::rand() % 30;
	if (cutPower != 6)
	{
		rnd = std::rand() % 5;
	}
	if (rnd != 0)
	{
		return false;
	}
	//cutPowerの量による判定
	int randRatio = (1 + cutPowerUpperLimit - cutPower) * 10;
	if (randRatio <= 0)
	{
		randRatio = 1;
	}
	rnd = std::rand() % randRatio;
	if (rnd != 0)
	{
		return false;
	}

	//切れるパネルが範囲内にあるか
	if (!IsExistPiecesWithinSawRange())
	{
		return false;
	}
	
	
	CuttingInfo* info = ActorManager::GetInstance()->GetFields()[0]->GetCuttingInfo(this);
	//---切り抜き判断系---
	//enemyAI->StartCutKillActorInFever();
	return cutPower >= cutPowerLowerLimit && info->ridingPiece;
}