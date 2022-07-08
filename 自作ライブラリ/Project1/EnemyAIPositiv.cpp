#include "EnemyAIPositiv.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"

EnemyAIPositiv* EnemyAIPositiv::GetInstance()
{
	static EnemyAIPositiv* instance = new EnemyAIPositiv();
	return instance;
}

Vector3 EnemyAIPositiv::KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	auto gottenPanels = field->GetGottenPieces();

	// １つも切り抜かれていなかったらリターン
	if (gottenPanels.size() <= 0) return arg_velocity;
	// パネルとの距離
	Vector3 distance = { 0,0,0 };
	// 一番近いパネルとの距離
	Vector3 nearestDistance = gottenPanels[0]->GetPosition() - arg_currentPos;
	// 一番近いパネルのポインタ（再計算用）
	FieldPiece* nearestPiece = gottenPanels[0];

	// 切り抜かれたパネルの位置を走査
	for (auto panel : gottenPanels)
	{
		distance = panel->GetPosition() - arg_currentPos;

		// より自分に近い穴が見つかったら
		if (distance.Length() < nearestDistance.Length())
		{
			// 距離の更新
			nearestDistance = distance;
			nearestPiece = panel;
		}
	}

	// 距離の規定値←後で避ける
	float specifiedValueDistance = nearestPiece->GetSidewaysLength() * 8;
	float a = nearestDistance.Length();
	// 最も近いパネルが規定値よりも遠かったらリターン
	if (nearestDistance.Length() >= specifiedValueDistance) return arg_velocity;

	// 正規化した一番近い穴へのベクトルと現在の進行方向ベクトルの内積
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = Vector3::Normalize(nearestDistance);
	float dot = Vector3::Dot(VecA, VecB);

	// 内積の規定値
	float specifiedValueDot = 0.5f;
	// 内積の絶対値が規定値以下だったらリターン
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// 進行方向の調整（現在の進行方向とその逆方向の外積から直角なベクトルを出す）
	Vector3 reverseVec = { -arg_velocity.x,-arg_velocity.y,-arg_velocity.z };
	reverseVec = Vector3::Normalize(reverseVec);
	Vector3 fixVel = reverseVec.Cross(Vector3::Normalize(arg_velocity));

	// 算出した修正ベクトルで一度確認する
	KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_currentPos, nearestPiece);

	// 進行方向を調整したベクトルを返す
	return fixVel;
}

Vector3 EnemyAIPositiv::KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

bool EnemyAIPositiv::StartCutOnSafeTiming(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutEnoughActivePieces(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutIncludeBonus(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutReachFever(StandardEnemy* arg_enemy)
{
	return false;
}

bool EnemyAIPositiv::StartCutKillActorInFever(StandardEnemy* arg_enemy)
{
	return false;
}

EnemyAILabel EnemyAIPositiv::GetLabel() const
{
	return EnemyAILabel();
}
