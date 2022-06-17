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
#include "PanelCountBoard.h"

const float INTERVAL_ACTIONTIMER = 180.0f;
const float WALKING = 90.0f;

StandardEnemy::StandardEnemy(Vector3 arg_position, float arg_hitWeight)
{
	initPos = arg_position;
	initWeight = arg_hitWeight;

	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("enemy");
	//モデルの生成
	Create(myModel);
	////当たり判定(Box)の生成
	//BoxCollider* boxCollider = new BoxCollider();
	//boxCollider->SetObject(this);
	//boxCollider->SetScale({ 0.2f,0.5f,0.2f });
	//boxCollider->SetOffset({ 0,0.5f,0 ,0 });
	//SetCollider(boxCollider);
	//collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	//collider->SetMove(true);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject("StandardEnemy", this);

	panelCountUI = new PanelCountUI();

	ObjectManager::GetInstance()->Add(new PanelCountBoard(position, name, gottenPanel));

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
	color = { 1,0,0.1f,1 };

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

	panelCountUI->Initialize();
}

void StandardEnemy::Update()
{
	// モーション
	if (state == EnemyState::Wait)
	{
		myModel->PlayAnimation("stand", true, 1, false);
	}
	else
	{
		myModel->PlayAnimation("walk", true);
	}

	actionTimer->Update();
	walkingTimer->Update();
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	//滑り落ちる処理
	float fallSpeed = 0.05f;
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	virtualityPlanePosition += field->GetTilt() * fallSpeed;
	StayInTheField();

	// 自分で操作したい時用(imguiで選択)
	if (isControl)
	{
		DebugControl();
	}

	if (HitCheckLoci())
	{
		state = EnemyState::Straddle;
	}

	if (isBlow)
	{
		virtualityPlanePosition += velocity * speed;
		StayInTheField();
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


		// 決まったパターンによって分岐
		switch (state)
		{
		case EnemyState::Wait:						// 待機

			break;

		case EnemyState::RandomMove:				// ランダム移動
			Move();

			break;

		case EnemyState::RushAttack:				// タックル
			RushAttack();

			break;

		case EnemyState::CoolAfterRushAttack:		// 後隙
			// タックル後の後隙

			break;

		case EnemyState::Straddle:					// 踏ん張り
			Straddle();

			break;

		default:
			break;
		}
	}

	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetFields()[0]->GetAngleTilt(), Vector3(0, -5, 0));

	object->Update();

	panelCountUI->Update(gottenPanel);
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

Vector3 StandardEnemy::GetDirection() const
{
	return LocusUtility::AngleToVector2(rotation.y + 90);
}

void StandardEnemy::Move()
{
	if (isControl)
	{
		return;
	}

	if (isMoved == true)
	{
		//ランダムな向きを決定
		Vector2 moveDir = { 0,0 };
		moveDir.x = (float)((rand() % 20 - 10) / 10.0f);
		moveDir.y = (float)((rand() % 20 - 10) / 10.0f);

		velocity = { moveDir.x,0,moveDir.y };
		velocity.Normalize();

		speed = 0.2f;

		walkingTimer->Reset();
		isMoved = false;
	}

	virtualityPlanePosition += velocity * speed;
	StayInTheField();
	MatchDir();
	// 移動しきるか他の行動に移ったらactionTimerをリセット
	if (walkingTimer->IsTime())
	{
		state = EnemyState::Wait;
		actionTimer->Reset();
		isMoved = true;
	}
}

void StandardEnemy::RushAttack()
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

void StandardEnemy::Straddle()
{
	// スマブラの崖際のよろけのイメージ
	// 一定時間踏ん張った後→落下しそうな方向と反対に移動して待機へ戻る
	// 踏ん張っている途中にプレイヤーがぶつかってきた場合→飛ばされて落ちる
}

void StandardEnemy::HitCheck()
{

}

bool StandardEnemy::HitCheckLoci()
{
	static const float radius = 1.0f;
	bool b = false;

	// プレイヤーから書いた線のデータを引っ張ってくる
	std::vector<BaseLocus*> vecLocuss = ActorManager::GetInstance()->GetPlayer()->GetVecLocuss();

	if (virtualityPlanePosition == preVirtualityPlanePosition)
	{
		return b;
	}

	for (auto locus : vecLocuss)
	{
		for (int i = 0; i < locus->GetMaxNumLine(); i++)
		{
			Line* line = locus->GetLine(i);
			Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneStartPos());
			Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneEndPos());
			Vector2 AB = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos() - line->GetVirtualityPlaneStartPos());
			Vector2 normalAB = Vector2::Normalize(AB);

			//今当たっているか
			float cross = Vector2::Cross(AO, normalAB);
			if (fabsf(cross) > radius)
			{
				continue;
			}

			float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
			if (multiDot <= 0.0f)
			{
				HitLoci(line);
				b = true;
				continue;
			}

			if (Vector2::Length(AO) < radius || Vector2::Length(BO) < radius)
			{
				HitLoci(line);
				b = true;
				continue;
			}

			//通り過ぎたか
			Vector2 start = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneStartPos());
			Vector2 end = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos());
			Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
			Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);

			if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
				LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
			{
				HitLoci(line);
				b = true;
			}
		}
	}

	return b;
}

void StandardEnemy::HitLoci(Line* arg_line)
{
	position = prePos;
	virtualityPlanePosition = preVirtualityPlanePosition;
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
}

void StandardEnemy::MatchDir()
{
	rotation.y = atan2(velocity.x, velocity.z) * 180 / PI;
}
