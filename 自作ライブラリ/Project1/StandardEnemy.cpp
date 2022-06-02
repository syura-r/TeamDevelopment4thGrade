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
	//当たり判定(Box)の生成
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale({ 0.2f,0.5f,0.2f });
	boxCollider->SetOffset({ 0,0.5f,0 ,0 });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);

	actionTimer = new Timer(INTERVAL_ACTIONTIMER);
	walkingTimer = new Timer(WALKING);

	Initialize();
}

StandardEnemy::~StandardEnemy()
{
	delete actionTimer;
	delete walkingTimer;

}

void StandardEnemy::Initialize()
{
	object->Initialize();
	position = initPos;
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	velocity = { 0,0,0 };
	aceel = { 0,0,0 };
	color = { 1,0,0.1f,1 };

	actionTimer->Initialize();
	walkingTimer->Initialize();

	virtualityPlanePosition = position;

	weight = initWeight;
	state = EnemyState::Wait;

	isMoved = true;
	speed = 0.2f;
	isStraddle = false;
	isControl = false;
}

void StandardEnemy::Update()
{
	myModel->PlayAnimation("stand", true, 1, false);
	actionTimer->Update();
	walkingTimer->Update();

	// 自分で操作したい時用(imguiで選択)
	if (isControl)
	{
		DebugControl();
	}

	// 行動パターンの選択
	if (HitCheckLoci())
	{
		state = EnemyState::Straddle;
	}
	//if (/*タックルが終了して何にも当たらなかったら*/)
	//{
	//	state = EnemyState::CoolAfterRushAttack;
	//}
	//if (/*プレイヤーがタックルの射程に入ったら*/)
	//{
	//	state = EnemyState::RushAttack;
	//}
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

	object->Update();
}

void StandardEnemy::Draw()
{
	ImGui::Begin("enemy");
	ImGui::Checkbox("isControl", &isControl);
	ImGui::End();

	PipelineState::SetPipeline("FBX");

	object->Draw(true);
}

void StandardEnemy::DrawReady()
{
	pipelineName = "FBX";
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
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetField()->GetAngleTilt(), Vector3(0, -5, 0));

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
	// プレイヤーの位置を見て予備動作→突進
	// プレイヤーに当たった場合→後隙
	// プレイヤーにも当たらず縁にも立たなかった場合→後隙
	// 縁に到達した場合→踏ん張り状態
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

	// プレイヤーから書いた線のデータを引っ張ってくる
	std::vector<BaseLocus*> vecLocuss = ActorManager::GetInstance()->GetPlayer()->GetVecLocuss();

	for (auto locus : vecLocuss)
	{
		for (int i = 0; i < locus->GetMaxNumLine(); i++)
		{
			Line* line = locus->GetLine(i);
			Vector2 AO = LocusUtility::Dim3ToDim2XZ(position - line->GetStartPos());
			Vector2 BO = LocusUtility::Dim3ToDim2XZ(position - line->GetEndPos());
			Vector2 AB = LocusUtility::Dim3ToDim2XZ(line->GetEndPos() - line->GetStartPos());
			Vector2 normalAB = Vector2::Normalize(AB);

			float cross = Vector2::Cross(AO, normalAB);
			if (fabsf(cross) > radius)
			{
				continue;
			}

			float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
			if (multiDot <= 0.0f)
			{
				return true;
			}

			if (Vector2::Length(AO) < radius || Vector2::Length(BO) < radius)
			{
				return true;
			}
		}
	}

	return false;
}

bool StandardEnemy::IsOnField()
{
	return false;
}

void StandardEnemy::StayInTheField()
{
	//X軸
	if (virtualityPlanePosition.x > 45)
	{
		virtualityPlanePosition.x = 45;
	}
	else if (virtualityPlanePosition.x < -45)
	{
		virtualityPlanePosition.x = -45;
	}

	//Z軸
	if (virtualityPlanePosition.z > 45)
	{
		virtualityPlanePosition.z = 45;
	}
	else if (virtualityPlanePosition.z < -45)
	{
		virtualityPlanePosition.z = -45;
	}	
}

void StandardEnemy::DebugControl()
{
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
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, ActorManager::GetInstance()->GetField()->GetAngleTilt(), Vector3(0, -5, 0));
}