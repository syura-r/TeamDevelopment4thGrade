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
#include "TestBoss.h"
#include "BossMissile.h"
#include "BossRangeAttack.h"
#include "Field.h"
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCountBoard.h"
#include "PanelCutLocus.h"
#include "FieldPiece.h"
#include "ItemEmitter.h"
#include "Player.h"

const float INTERVAL_ACTIONTIMER = 180.0f;
const float WALKING = 90.0f;

StandardEnemy::StandardEnemy()
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("GamePlay_Enemy");
	//モデルの生成
	Create(myModel);

	pObjectManager = ObjectManager::GetInstance();

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);
	attackSprite = new Sprite();

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("StandardEnemy", this);

	panelCountUI = new PanelCountUI();

	pObjectManager->Add(new PanelCountBoard(this));

	Initialize();

}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	delete walkingTimer;
	delete attackSprite;
	delete panelCountUI;
	ActorManager::GetInstance()->DeleteObject(this);
}

void StandardEnemy::Initialize()
{
	onGround = true;
	scale = { 0.9f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };
	cameraRotCount = 0;
	rotCamera = false;
	radY = 0;
	//----------カメラの回転角度を算出---------------
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	float cosA = Vector3(0, 0, 1).Dot(cameraDirectionZ);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;
	float rad = acos(cosA);
	const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
	if (CrossVec.y < 0)
		rad *= -1;
	//-----------------------------------------------
	drawingFlag = false;
	isExtendLine = false;
	currentLineNum = 0;
	DeleteDrawingLine();
	for (int i = 0; i < vecLocuss.size(); i++)
	{
		delete vecLocuss[i];
		vecLocuss[i] = nullptr;
	}
	vecLocuss.clear();
	feverQuota = maxFeverQuota;
	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	weight = 10;
	cutPower = 0;
	gottenPanel = 0;

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	actionTimer->Initialize();
	walkingTimer->Initialize();

	panelCountUI->Initialize();
}

void StandardEnemy::Update()
{
	// タイマー更新
	actionTimer->Update();
	walkingTimer->Update();
	// 位置の確認
	ConfirmPlayerPos();
	ConfirmItemPos();

	//移動処理
	Move();

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);

	if (blowFlag)
	{
		blowTime--;
		if (blowTime <= 0)
		{
			blowFlag = false;
		}
	}
	else
	{
		// 丸のこの所持数が一定以上だったら切り抜き
		if ((Input::TriggerKey(DIK_U) || cutPower >= cutPowerLimit) && cutPower > 0 && info->ridingPiece)
		{
			if (!tackleFlag && !drawingFlag)
			{
				drawingFlag = true;
				//線の生成
				Vector3 p = info->cuttingStartPos;
				ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus, CircularSaw::ENEMY, this));
			}

		}

		// 敵が近くに居たらタックル
		if (Input::TriggerKey(DIK_O) || RangeCheckPlayer())
		{
			Tackle();
		}
	}

	//図形の消去
	if (!drawingFlag)
	{
		if (Input::TriggerPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER) || Input::TriggerPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			DeleteLocuss();
		}
	}

	//当たり判定系
	HitCheckItems();

	//他のオブジェクトとのヒットチェック
	Object::Update();

	Vector3 p = info->cuttingStartPos;
	//SetLocus(LocusType::UNDIFINED);
	if (!drawingFlag)
	{
		panelCutLocus->SetCutPower(cutPower);
		panelCutLocus->Move(p, info->cuttingAngle);
	}
	for (auto locus : vecLocuss)
	{
		locus->Move(locus->GetVirtualityPlanePosition(), locus->GetAngle());
	}

	panelCountUI->Update(gottenPanel);
}

void StandardEnemy::Draw()
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
	ImGui::Text("inputAccuracy : {%f}\n", inputAccuracy);
	ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
	ImGui::End();

#endif

	PipelineState::SetPipeline("FBX");

	object->Draw(true);

	panelCountUI->Draw();
}

void StandardEnemy::DrawReady()
{
	pipelineName = "FBX";
}

void StandardEnemy::Move()
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	//走りと歩きの切り替え処理
	if (drawingFlag)
	{
		speed = drawingSpeed;
	}
	else
	{
		speed = walkSpeed;
	}

	if (standingFlag)
	{
		WithStand();
	}
	else
	{
		SlidingDown();
	}

	//フィールド端からの復帰
	if (standingFlag || returningFieldFlag)
	{
		//myModel->PlayAnimation("stand", true);
		if (returningFieldFlag)
		{
			virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
			moveEasingCount++;
			if (moveEasingCount >= 30)
			{
				moveEasingCount = 0;
				returningFieldFlag = false;
			}
		}
	}
	//タックルの移動　ガチで月曜にplayerリファクタリングする
	else if (tackleFlag)
	{
		virtualityPlanePosition = EasingMove(tackleStartPos, tackleEndPos, 1, tackleCount / 30.0f);
		tackleCount++;
		if (tackleCount >= 30)
		{
			tackleCount = 0;
			tackleFlag = false;
			tackleHitFlag = false;
		}
		StayInTheField();
		StayOnRemainPanels();

	}
	//通常の移動
	else
	{
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

		////入力がないので0にする
		//if (!Input::DownKey(DIK_I) && !Input::DownKey(DIK_K) && !Input::DownKey(DIK_J) && !Input::DownKey(DIK_L))
		//{
		//	moveDirection = Vector3(0, 0, 0);
		//}

		float rotSpeed = rotateSpeed;
		if (abs(rotY) < 55)
		{
			virtualityPlanePosition += moveDirection * (speed * inputAccuracy);
			StayInTheField();
			StayOnRemainPanels();
			isExtendLine = true;
		}
		else
		{
			isExtendLine = false;
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

		if (!Input::DownKey(DIK_I) && !Input::DownKey(DIK_K) && !Input::DownKey(DIK_J) && !Input::DownKey(DIK_L))
		{
			//myModel->PlayAnimation("stand", true);
		}
		else if (drawingFlag)
		{
			//myModel->PlayAnimation("run", true);
		}
		else
		{
			//myModel->PlayAnimation("walk", true);
		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void StandardEnemy::SlidingDown()
{
	if (drawingFlag)
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.05f;
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();
	StayOnRemainPanels();
}

void StandardEnemy::DecideDirection(Vector3& arg_direction)
{
	//ふっとばされ中
	if (blowFlag)
	{
		//velocityに入っている値に進むように
		arg_direction = velocity;
		inputAccuracy = 1;
		speed = blowSpeed;
		return;
	}

	//if (!Input::DownKey(DIK_I) && !Input::DownKey(DIK_K) && !Input::DownKey(DIK_J) && !Input::DownKey(DIK_L))
	//{
	//	return;
	//}

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//くり抜き動作中
	if (drawingFlag)
	{
		arg_direction = { 0,0,0 };
	}
	else
	{
		
		//if (Input::DownKey(DIK_J))
		//	arg_direction += cameraDirectionX * -1;
		//if (Input::DownKey(DIK_L))
		//	arg_direction += cameraDirectionX;
		//if (Input::DownKey(DIK_K))
		//	arg_direction += cameraDirectionZ * -1;
		//if (Input::DownKey(DIK_I))
		//	arg_direction += cameraDirectionZ;
		
		if (actionTimer->IsTime())
		{
			// 向きを指定して移動
			//moveDir = RandomDir();
			moveDir = NearObjDir();
			actionTimer->Reset();
		}
		arg_direction = cameraDirectionX * moveDir.x + cameraDirectionZ * moveDir.y;

		//if (Input::CheckPadLStickAnythingDir())
		//{
		//	auto vec = Input::GetLStickDirection();

		//	arg_direction = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
		//}
		inputAccuracy = 1;
	}

	arg_direction.Normalize();
	//反発用に代入
	velocity = arg_direction;
}

void StandardEnemy::DeleteDrawingLine()
{
	for (int i = 0; i < vecDrawingLines.size(); i++)
	{
		vecDrawingLines[i]->Dead();
	}
	vecDrawingLines.clear();
}

void StandardEnemy::SuspendDrawing()
{
	drawingFlag = false;
	currentLineNum = 0;
	DeleteDrawingLine();
	pNowDrawingLine = nullptr;
}

void StandardEnemy::DeleteLocuss()
{
	auto end = vecLocuss.size();

	for (int i = 0; i < end; i++)
	{
		delete vecLocuss[i];
		vecLocuss[i] = nullptr;
	}
	vecLocuss.clear();

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	if (field)
	{
		field->ResetInfluences();
	}
	weight = 5;
	gottenPanel = 0;
}

void StandardEnemy::MoveEndDrawing(BaseLocus* arg_locus)
{
	Vector3 vec = LocusUtility::AngleToVector2(arg_locus->GetAngle() + 180);
	virtualityPlanePosition = arg_locus->GetLine(arg_locus->GetMaxNumLine() - 1)->GetVirtualityPlaneEndPos();
	virtualityPlanePosition += vec * 2.0f;
	StayInTheField();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

void StandardEnemy::StayInTheField()
{
	if (standingFlag || returningFieldFlag)
	{
		return;
	}

	std::vector<Vector2> fieldEdges = Field::GetEdges();

	for (int i = 0; i < fieldEdges.size(); i++)
	{
		Vector2 A = fieldEdges[i];
		Vector2 B = fieldEdges[(i + 1) % fieldEdges.size()];
		Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;
		Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - B;
		Vector2 AB = B - A;
		Vector2 normalAB = Vector2::Normalize(AB);

		//今当たっているか
		float cross = Vector2::Cross(AO, normalAB);
		if (fabsf(cross) > RADIUS)
		{
			continue;
		}

		float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
		if (multiDot <= 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			StartStand();
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			StartStand();
			break;
		}

		//通り過ぎたか
		Vector2 start = A;
		Vector2 end = B;
		Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
		Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

		if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
			LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			StartStand();
			break;
		}
	}

	if (!standingFlag)
	{
		return;
	}

	if (drawingFlag)
	{
		SuspendDrawing();
	}
}

void StandardEnemy::StayOnRemainPanels()
{

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	FieldPiece* piece = field->IsRideGottenPanel(virtualityPlanePosition, preVirtualityPlanePosition, RADIUS);

	if (piece)
	{
		virtualityPlanePosition = preVirtualityPlanePosition;

		Vector3 outPieceVec = {};
		outPieceVec = position - piece->GetVirtualityPlanePosition();
		outPieceVec.Normalize();
		StartStand(false, outPieceVec);

		if (tackleFlag)
		{
			SuspendTackle();
		}
	}
}

void StandardEnemy::HitCheckItems()
{
	if (cutPower >= 6)
	{
		return;
	}

	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();

	for (auto item : items)
	{
		if (!item->IsAppeared())
		{
			continue;
		}

		float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - item->GetVirtualityPlanePosition()));

		if (length <= RADIUS + EnergyItem::GetRadius())
		{
			HitItem(item);
		}
	}
}

void StandardEnemy::HitItem(EnergyItem* arg_item)
{
	arg_item->Dead();
	if (cutPower < 6)
	{
		cutPower++;
	}
}

void StandardEnemy::StartStand(bool arg_outField, Vector3 arg_velocity)
{
	standingFlag = true;
	//タックル終了
	SuspendTackle();

	standTime = 120;

	//場外か図形か
	if (arg_outField)
	{
		preStandVec = -position; //中央に向かう
	}
	else
	{
		preStandVec = arg_velocity; //パネルの中心からプレイヤーに向かうベクトル
	}

	preStandVec.y = 0;
	preStandVec.Normalize();
}

void StandardEnemy::WithStand()
{
	if (drawingFlag)
	{
		return;
	}

	//踏ん張り中　赤
	Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	if (BGColor >= 0)
	{
		BGColor -= 0.02f;
	}
	else
	{
		BGColor = 1;
	}

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	//スティックの向き
	auto vec = Input::GetLStickDirection();
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	float accuracy = 0;

	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	accuracy = Vector2::Dot(stickDirection, correctVec);

	if (accuracy <= 0)
	{
		accuracy = 0;
	}

	if (accuracy >= 0.55f)
	{
		Vector3 moveDirection = preStandVec;
		Object::SetColor({ 1,1,1,1 });
		returningFieldFlag = true;
		standingFlag = false;
		returningStartPos = virtualityPlanePosition;
		returningEndPos = virtualityPlanePosition + moveDirection * 3;
		return;
	}
}

void StandardEnemy::Tackle()
{
	if (tackleFlag || blowFlag || drawingFlag || standingFlag || returningFieldFlag)
	{
		return;
	}

	tackleFlag = true;

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, Object3D::GetCamera()->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//スティックの向き
	auto vec = Input::GetLStickDirection();
	stickDirection.x = (cameraDirectionX * vec.x).x;
	stickDirection.y = (cameraDirectionZ * vec.y).z;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;
}

void StandardEnemy::SuspendTackle()
{
	if (standingFlag || returningFieldFlag)
	{
		tackleFlag = false;
		tackleCount = 0;
		speed = walkSpeed;
	}
	//タックル中
	else
	{
		tackleEndPos = virtualityPlanePosition;
		//tackleCount = 0;
		speed = walkSpeed;
	}
}

void StandardEnemy::DischargeGottenPanel(StandardEnemy* arg_enemy)
{
	ItemEmitter* itemEmitter = ItemEmitter::GetInstance();
	int maxEmit = 0;
	if (gottenPanel == 1)
	{
		maxEmit = 1;
	}
	else
	{
		maxEmit = gottenPanel / 2;
	}

	for (int i = 0; i < maxEmit; i++)
	{
		itemEmitter->EmitPanelItem(virtualityPlanePosition);
		gottenPanel--;
		weight -= FieldPiece::GetWeight();
	}

}

void StandardEnemy::IsBlow()
{
	blowFlag = true;
}

Vector3 StandardEnemy::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	Vector3 result = {};
	result.x = Easing::EaseOutCubic(arg_startPos.x, arg_endPos.x, arg_maxTime, arg_nowTime);
	result.y = Easing::EaseOutCubic(arg_startPos.y, arg_endPos.y, arg_maxTime, arg_nowTime);
	result.z = Easing::EaseOutCubic(arg_startPos.z, arg_endPos.z, arg_maxTime, arg_nowTime);
	return result;
}

bool StandardEnemy::IsAlive()
{
	return true;
}

void StandardEnemy::EndDrawing()
{
	drawingFlag = false;
	panelCutLocus->RecordCuttedPanelPos();
	int num = ActorManager::GetInstance()->GetFields()[0]->CutPanel(panelCutLocus->GetCuttedPanelPos());
	weight += num * FieldPiece::GetWeight();
	gottenPanel += num;
	cutPower = 0;
}

Vector3 StandardEnemy::GetDirection() const
{
	return direction;
}

PanelCutLocus* StandardEnemy::GetPanelCutLocus()
{
	return panelCutLocus;
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
	if (blowFlag) { return false; }
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
