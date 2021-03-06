#include "EnemyAIPositiv.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"
#include "ActorManager.h"
#include "EnergyItem.h"
#include "IActionState.h"

EnemyAIPositiv* EnemyAIPositiv::GetInstance()
{
	static EnemyAIPositiv* instance = new EnemyAIPositiv();
	return instance;
}

Vector3 EnemyAIPositiv::KeepAwayFromGottenPieces(StandardEnemy* arg_enemy, const Vector3& arg_velocity, const Vector3& arg_currentPos, FieldPiece* arg_recentCalcPiece)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	auto gottenPanels = field->GetGottenPieces();

	// １つも切り抜かれていなかったらreturn
	if (gottenPanels.size() <= 0)
	{
		return arg_velocity;
	}
	// パネルとの距離
	Vector3 distance = { 0,0,0 };
	// 一番近いパネルとの距離
	Vector3 nearestDistance = gottenPanels[0]->GetVirtualityPlanePosition() - arg_currentPos;
	// 一番近いパネルのポインタ（再計算用）
	FieldPiece* nearestPiece = gottenPanels[0];

	// 切り抜かれたパネルの位置を走査
	for (auto panel : gottenPanels)
	{
		distance = panel->GetVirtualityPlanePosition() - arg_currentPos;

		// より自分に近い穴が見つかったら
		if (distance.Length() < nearestDistance.Length())
		{
			// 距離の更新
			nearestDistance = distance;
			nearestPiece = panel;
		}
	}

	float a = Vector2::Length(LocusUtility::Dim3ToDim2XZ(nearestDistance));
	// 最も近いパネルが規定値よりも遠かったらreturn
	if (a >= specifiedValueDistance)
	{
		return arg_velocity;
	}

	// 正規化した一番近い穴へのベクトルと現在の進行方向ベクトルの内積
	Vector2 VecA = Vector2::Normalize(LocusUtility::Dim3ToDim2XZ(arg_velocity));
	Vector2 VecB = Vector2::Normalize(LocusUtility::Dim3ToDim2XZ(nearestDistance));
	float dot = Vector2::Dot(VecA, VecB);

	// 内積の絶対値が規定値以下だったらreturn
	if (dot <= specifiedValueDot)
	{
		return arg_velocity;
	}

	//アイテムが近くにあったら処理を無効に
	if (a > FieldPiece::GetLowerTimeOffset())
	{
		auto items = ActorManager::GetInstance()->GetEnergyItems();
		for (auto item : items)
		{
			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(item->GetVirtualityPlanePosition() - arg_enemy->GetVirtualityPlanePosition())) <= FieldPiece::GetSidewaysLength() * 2)
			{
				return arg_velocity;
			}
		}
	}
	
	// 進行方向の調整（現在の進行方向とその逆方向の外積から直角なベクトルを出す）
	float cross = Vector2::Cross(VecB, VecA);
	Vector3 fixVel = Vector3();
	if (cross > 0)
	{
		fixVel = Vector3(-VecB.y, 0, VecB.x);
	}
	else
	{
		fixVel = Vector3(VecB.y, 0, -VecB.x);
	}

	// 進行方向を調整したベクトルを返す
	return Vector3::Normalize(fixVel);
}

Vector3 EnemyAIPositiv::KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// 一番近い辺の算出
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	int minLength = 100;
	int minIndex = 0;
	for (int i = 0; i < 6; i++)
	{
		int length = field->GetLengthToFieldBorder(i, arg_enemy->GetVirtualityPlanePosition());

		if (length < minLength)
		{
			minLength = length;
			minIndex = i;
		}
	}

	// 一番近い辺との距離が規定値以上だったらreturn
	if (minLength >= specifiedValueDistance) return arg_velocity;

	//最近接辺から中心に向かうベクトル
	Vector3 normalBorder = field->GetFieldBorderNormal(minIndex);

	// 進行方向と辺の法線との外積
	Vector2 VecA = LocusUtility::Dim3ToDim2XZ(arg_velocity);
	Vector2 VecB = LocusUtility::Dim3ToDim2XZ(normalBorder);

	float dot = Vector2::Dot(VecA, VecB);

	// 内積の絶対値が規定値以下だったらreturn
	if (dot <= specifiedValueDot)return arg_velocity;

	// 算出した修正ベクトルで一度確認する
	//KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_enemy->GetPosition());

	//アイテムが近くにあったら処理を無効に
	if (minLength > FieldPiece::GetLowerTimeOffset())
	{
		auto items = ActorManager::GetInstance()->GetEnergyItems();
		for (auto item : items)
		{
			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(item->GetVirtualityPlanePosition() - arg_enemy->GetVirtualityPlanePosition())) <= FieldPiece::GetSidewaysLength() * 2)
			{
				return arg_velocity;
			}
		}
	}

	return -normalBorder;
}

Vector3 EnemyAIPositiv::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// フィールドの傾きを取得
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	Vector3 tiltDir = field->GetAngleTilt();			// 傾きの向き
	float depthMagnitude = field->GetDepthMagnitude();	// 傾きの強さ

	// 傾きの強さが規定値以下だったらreturn
	if (depthMagnitude < specifiedValueInclination) return arg_velocity;

	// 傾きの正規化とフィールドの中心からの方向の正規化との内積
	Vector3 dir = arg_enemy->GetPosition() - field->GetPosition();	// フィールドの中心からの方向
	float dot = Vector3::Dot(tiltDir.Normalize(), dir.Normalize());

	// 内積の絶対値が規定値以下だったらreturn
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// 傾きに逆らうベクトルをreturn
	Vector3 fixVel = -tiltDir;

	return fixVel;
}

Vector3 EnemyAIPositiv::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// フィールド上のアイテムを近い順に並べる
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();
	// vectorが空だったらreturn
	if (items.size() <= 0)
	{
		return arg_velocity;
	}

	std::vector<ItemRange*> itemRanges;

	Vector3 itemDistance;
	// 全アイテムを走査
	for (auto item : items)
	{
		// 距離を記録
		itemDistance = item->GetVirtualityPlanePosition() - arg_enemy->GetVirtualityPlanePosition();

		ItemRange* itemRange = new ItemRange();
		itemRange->item = item;
		itemRange->range = itemDistance.Length();

		itemRanges.push_back(itemRange);
	}

	// 距離順でソート
	std::sort(itemRanges.begin(), itemRanges.end(), [](const ItemRange* x, const ItemRange* y) {return x->range < y->range; });
	
	//簡易版
	Vector2 returnVec = LocusUtility::Dim3ToDim2XZ(itemRanges[0]->item->GetVirtualityPlanePosition() - arg_enemy->GetVirtualityPlanePosition());
	return Vector3::Normalize(LocusUtility::Dim2XZToDim3(returnVec));

	// 他のActorとアイテムとの距離を見る
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// 他のActorがいなかったらreturn
	if (actors.size() <= 0)	return arg_velocity;
	// 全アイテムを走査
	for (auto itemRange : itemRanges)
	{
		// 全actorを走査
		for (auto actor : actors)
		{
			if (actor == arg_enemy)
			{
				continue;
			}

			Vector3 r = itemRange->item->GetVirtualityPlanePosition() - actor->GetVirtualityPlanePosition();
			float otherRange = r.Length();
			
			// 自分より近いActorが居たら終了
			if (otherRange < itemRange->range)
			{
				break;
			}
		}
		// 自分が一番近かった場合そこに向かう
		return itemRange->item->GetPosition() - arg_enemy->GetPosition();
	}

	// 自分が一番近いアイテムが無かったら自分に一番近いアイテムへ向かう
	Vector3 fixVel = itemRanges[0]->item->GetPosition() - arg_enemy->GetPosition();

	return fixVel;
}

Vector3 EnemyAIPositiv::ApproachCuttingActor(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// 切り抜き中のActorを探す
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// 他のActorがいなかったらリターン
	if (actors.size() <= 0)	return arg_velocity;
	// 全Actorを走査
	for (auto actor : actors)
	{
		// 切り抜き中で無かったらcontinue
		if (actor->GetActionState()->GetLabel() != ActionStateLabel::CUT) continue;

		// 距離を測る
		Vector3 v = actor->GetPosition() - arg_enemy->GetPosition();
		float distance = v.Length();

		// 距離が規定値以上だったらcontinue
		if (distance > specifiedValueDistance) continue;

		// Actorへの方向をreturn
		return v;
	}

	// 切り抜き中のActorがいないか規定距離より離れていたので元のvelocityをreturn
	return arg_velocity;
}

Vector3 EnemyAIPositiv::ApproachActorInFever(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// 他のActorがいなかったらリターン
	if (actors.size() <= 0)	return arg_velocity;
	// Actorへのベクトル
	Vector3 vector = { 0,0,0 };
	// 一番近いActorへのベクトル
	Vector3 nearestVector = actors[0]->GetPosition() - arg_enemy->GetPosition();
	// 全Actorを走査
	for (auto actor : actors)
	{
		vector = actor->GetPosition() - arg_enemy->GetPosition();

		// より自分に近いActorが見つかったら
		if (vector.Length() < nearestVector.Length())
		{
			// 一番近いActorへのベクトルの更新
			nearestVector = vector;
		}
	}

	return nearestVector;
}

bool EnemyAIPositiv::StartCutOnSafeTiming(StandardEnemy* arg_enemy)
{
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// Actorへのベクトル
	Vector3 vector = { 0,0,0 };
	for (auto actor : actors)
	{
		vector = actor->GetPosition() - arg_enemy->GetPosition();

		// 規定値より近いActorが見つかったら
		if (vector.Length() < specifiedValueDistance)
		{
			return false;
		}
	}

	return true;
}

bool EnemyAIPositiv::StartCutEnoughActivePieces(StandardEnemy* arg_enemy)
{
	static std::vector<FieldPiece*> vecPieces;
	vecPieces.clear();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	field->GetPiecesWithinSawRange(arg_enemy->GetPanelCutLocus(), vecPieces);
	int cutNum = 0;
	for (auto p : vecPieces)
	{
		if (p->IsActive())
		{
			cutNum++;
		}
	}
	return cutNum >= (arg_enemy->cutPowerUpperLimit + arg_enemy->cutPowerLowerLimit) / 2;
}

bool EnemyAIPositiv::StartCutIncludeBonus(StandardEnemy* arg_enemy)
{
	static std::vector<FieldPiece*> vecPieces;
	vecPieces.clear();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	field->GetPiecesWithinSawRange(arg_enemy->GetPanelCutLocus(), vecPieces);
	int bonusNum = 0;
	for (auto p : vecPieces)
	{
		if (p->IsBonus())
		{
			bonusNum++;
		}
	}
	return bonusNum > 0;
}

bool EnemyAIPositiv::StartCutReachFever(StandardEnemy* arg_enemy)
{
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	int feverNolma = field->GetFeverNolma();	// フィーバーまでのノルマ
	int gottenCount = field->GetGottenCount();	// 現在の獲得数

	// cutPowerとフィーバーまでの残り枚数を比較
	if (arg_enemy->GetCutPower() > feverNolma - gottenCount) return true;

	return false;
}

bool EnemyAIPositiv::StartCutKillActorInFever(StandardEnemy* arg_enemy)
{
	static const float R = 2.0f;
	static std::vector<FieldPiece*> vecPieces;
	vecPieces.clear();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	field->GetPiecesWithinSawRange(arg_enemy->GetPanelCutLocus(), vecPieces);

	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	for (auto a : actors)
	{
		if (a == arg_enemy)
		{
			continue;
		}

		CuttingInfo* info = field->GetCuttingInfo(a);
		for (auto p : vecPieces)
		{
			if (info->ridingPiece != p)
			{
				continue;
			}

			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(a->GetVirtualityPlanePosition() - info->ridingPiece->GetVirtualityPlanePosition())) < R)
			{
				return true;
			}
		}
	}

	return false;
}

EnemyAILabel EnemyAIPositiv::GetLabel() const
{
	return EnemyAILabel::POSITIVE;
}

float EnemyAIPositiv::PointToLineDistance(Vector2 arg_point, Vector2 arg_linestart, Vector2 arg_lineend)
{
	float x0 = arg_point.x, y0 = arg_point.y;
	float x1 = arg_linestart.x, y1 = arg_linestart.y;
	float x2 = arg_lineend.x, y2 = arg_lineend.y;

	float a = x2 - x1;
	float b = y2 - y1;
	float a2 = a * a;
	float b2 = b * b;
	float r2 = a2 + b2;
	float tt = -(a * (x1 - x0) + b * (y1 - y0));

	if (tt < 0)
		return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
	else if (tt > r2)
		return sqrt((x2 - x0) * (x2 - x0) + (y2 - y0) * (y2 - y0));

	double f1 = a * (y1 - y0) - b * (x1 - x0);
	return sqrt((f1 * f1) / r2);
}