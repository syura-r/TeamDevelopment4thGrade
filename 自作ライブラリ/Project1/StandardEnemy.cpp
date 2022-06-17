#include "StandardEnemy.h"

#include "Timer.h"
#include "FBXModel.h"
#include "FBXManager.h"
#include "BoxCollider.h"
#include "CollisionAttribute.h"
#include "ActorManager.h"
#include "Player.h"
#include "Input.h"
#include "Field.h"
#include "FieldPiece.h"
#include "EnergyItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "PanelCountBoard.h"

const float INTERVAL_ACTIONTIMER = 180.0f;
const float WALKING = 90.0f;

StandardEnemy::StandardEnemy(Vector3 arg_position, float arg_hitWeight)
{
	initPos = arg_position;
	initWeight = arg_hitWeight;

	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("GamePlay_Enemy");
	//モデルの生成
	Create(myModel);

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("StandardEnemy", this);

	panelCountUI = new PanelCountUI();

	ObjectManager::GetInstance()->Add(new PanelCountBoard(this));

	Initialize();
}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	delete walkingTimer;
	delete panelCountUI;
	ActorManager::GetInstance()->DeleteObject(this);
}

void StandardEnemy::Initialize()
{
	object->Initialize();
	position = initPos;
	prePos = position;
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	velocity = { 0,0,0 };
	aceel = { 0,0,0 };
	color = { 1,1,1,1 };

	actionTimer->Initialize();
	walkingTimer->Initialize();

	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	weight = initWeight;
	state = EnemyState::Wait;

	isMoved = true;
	speed = 0.2f;
	isStraddle = false;
	isControl = false;
	isAttacked = false;
	cuttingFlag = false;
	cutPower = 0;
	gottenPanel = 0;
	playerPos = { 0,0,0 };
	itemPos = { 0,0,0 };

	panelCountUI->Initialize();
}

void StandardEnemy::Update()
{
	// タイマー更新
	actionTimer->Update();
	walkingTimer->Update();
	// position保存
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	// 自分以外の位置を更新
	ConfirmPlayerPos();
	ConfirmItemPos();

	//滑り落ちる処理
	float fallSpeed = 0.05f;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	// フィールド上にとどまる処理
	StayInTheField();
	StayOnRemainPanels();

	// 当たり判定系
	HitCheckItems();

	// 自分で操作したい時用(imguiで選択)
	if (isControl)
	{
		DebugControl();
	}

	if (isBlow)
	{
		virtualityPlanePosition += velocity * speed;
		StayInTheField();
		StayOnRemainPanels();

		blowTime--;
		if (blowTime <= 0)
		{
			isBlow = false;
		}
	}
	else
	{
		// 行動パターンの選択
		
		//if (/*タックルが終了して何にも当たらなかったら*/)
		//{
		//	state = EnemyState::CoolAfterRushAttack;
		//}
		if (RangeCheckPlayer())
		{
			state = EnemyState::RushAttack;
		}
		if (actionTimer->IsTime())
		{
			state = EnemyState::RandomMove;
		}
		//if (/*何もなく時間経過で*/)
		//{
		//	state = EnemyState::Wait;
		//}
		// 丸のこ所持数を見て切り抜き
		if (cutPower >= cuttingSowNum && (!isAttacked && !cuttingFlag))
		{
			state = EnemyState::Cutting;
		}

		// 決まったパターンによって分岐
		switch (state)
		{
		case EnemyState::Wait:						// 待機

			break;

		case EnemyState::RandomMove:				// ランダム移動
			Move();

			break;

		case EnemyState::RushAttack:				// タックル
			Tackle();

			break;

		case EnemyState::CoolAfterRushAttack:		// 後隙
			// タックル後の後隙

			break;

		case EnemyState::Straddle:					// 踏ん張り
			// 踏ん張り開始

			break;

		case EnemyState::Cutting:
			Cutting();

			break;
		default:
			break;
		}
	}

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));

	//Vector3 p = field->/*GetPlayerCuttingStartPos()*/;	// ←敵版作ろう

	//if (!cuttingFlag)
	//{
	//	panelCutLocus->SetCutPower(cutPower);
	//	panelCutLocus->Move(p, field->GetPlayerCuttingAngle());
	//}

	object->Update();

	panelCountUI->Update(0);//playerのgottenPanelの役割をする変数を入れる
}

void StandardEnemy::Draw()
{
	ImGui::Begin("enemy");
	ImGui::Checkbox("isControl", &isControl);
	ImGui::End();

	PipelineState::SetPipeline("FBX");

	object->Draw(true);

	panelCountUI->Draw(GAMEOBJECT_TYPE::ENEMY);
}

void StandardEnemy::DrawReady()
{
	pipelineName = "FBX";
}

void StandardEnemy::IsBlow()
{
	isBlow = true;
}

void StandardEnemy::Move()
{
	if (isControl)
	{
		return;
	}

	if (isStraddle)
	{
		WithStand();
	}

	if (isMoved == true)
	{
		//ランダムな向きを決定
		//Vector2 moveDir = RandomDir();
		Vector2 moveDir = { ThinkDir().x,ThinkDir().z };

		velocity = { moveDir.x,0,moveDir.y };

		speed = 0.2f;

		walkingTimer->Reset();
		isMoved = false;
	}

	virtualityPlanePosition += velocity * speed;
	StayInTheField();
	StayOnRemainPanels();

	MatchDir();

	// 移動しきるか他の行動に移ったらactionTimerをリセット
	if (walkingTimer->IsTime())
	{
		state = EnemyState::Wait;
		actionTimer->Reset();
		isMoved = true;
	}
}

void StandardEnemy::StartStand(bool arg_outField, Vector3 arg_velocity)
{
	isStraddle = true;
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
	if (isCutting)
	{
		return;
	}

	Vector2 moveDir = RandomDir();

	float accuracy = 0;

	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	accuracy = Vector2::Dot(moveDir, correctVec);

	if (accuracy <= 0)
	{
		accuracy = 0;
	}

	if (accuracy >= 0.55f)
	{
		Vector3 moveDirection = preStandVec;
		Object::SetColor({ 1,1,1,1 });
		returningFieldFlag = true;
		isStraddle = false;
		returningStartPos = virtualityPlanePosition;
		returningEndPos = virtualityPlanePosition + moveDirection * 3;
		return;
	}
}

void StandardEnemy::Tackle()
{
	if (isControl)
	{
		return;
	}

	// プレイヤーの位置を見て予備動作→突進
	// プレイヤーに当たった場合→後隙
	// プレイヤーにも当たらず縁にも立たなかった場合→後隙
	// 縁に到達した場合→踏ん張り状態

	if (isAttacked == true)
	{
		state = EnemyState::Wait;
		isAttacked = false;
	}

	if (isAttacked == false)
	{
		// 向きの決定
		Vector3 attackDir = ActorManager::GetInstance()->GetPlayer()->GetVirtualityPlanePosition() - virtualityPlanePosition;
		attackDir.Normalize();

		velocity = attackDir * AttackPower;
		isAttacked = true;

		virtualityPlanePosition += velocity * speed;
	}
}

void StandardEnemy::SuspendTackle()
{
}

void StandardEnemy::HitCheck()
{

}

bool StandardEnemy::RangeCheckPlayer()
{
	if (isBlow) { return false; }
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

bool StandardEnemy::IsOnField()
{
	return false;
}

void StandardEnemy::StayInTheField()
{ 
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
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;			
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
			break;
		}
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

		if (isAttacked)
		{
			SuspendTackle();
		}
	}
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

Vector2 StandardEnemy::RandomDir()
{
	Vector2 dir = { 0,0 };

	dir.x = (float)((rand() % 20 - 10) / 10.0f);
	dir.y = (float)((rand() % 20 - 10) / 10.0f);
	dir = Vector2::Normalize(dir);

	return dir;
}

Vector3 StandardEnemy::ThinkDir()
{
	Vector3 itemRange = itemPos - position;
	Vector3 playerRange = playerPos - position;
	Vector3 dir;
	// アイテムが近かったらアイテムの方へ
	// プレイヤーが近かったらプレイヤーの方へ
	if (itemRange.Length() < playerRange.Length())
	{
		dir = itemPos - position;
		dir.Normalize();
		velocity = dir;
	}
	else
	{
		dir = playerPos - position;
		dir.Normalize();
		velocity = dir;
	}
	
	return velocity;
}

void StandardEnemy::Cutting()
{
	cuttingFlag = true;
	Field* field = ActorManager::GetInstance()->GetFields()[0];

	//丸のこをオブジェクトマネージャーに追加
	//Vector3 p = field->GetPlayerCuttingStartPos();	// ←敵版作ろう
	//ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus));

	state = EnemyState::Wait;
}

void StandardEnemy::SuspendCutting()
{
	cuttingFlag = false;
}

void StandardEnemy::DebugControl()
{
	if (isBlow) { return; }
	velocity = { 0,0,0 };

	if (Input::DownKey(DIK_J))
		velocity.x = -1;
	if (Input::DownKey(DIK_L))
		velocity.x = 1;
	if (Input::DownKey(DIK_K))
		velocity.z = -1;
	if (Input::DownKey(DIK_I))
		velocity.z = 1;

	velocity.Normalize();

	virtualityPlanePosition += velocity * speed;
	StayInTheField();
	StayOnRemainPanels();
}

void StandardEnemy::MatchDir()
{
	rotation.y = atan2(velocity.x, velocity.z) * 180 / PI;
}