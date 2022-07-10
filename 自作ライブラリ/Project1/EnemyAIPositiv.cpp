#include "EnemyAIPositiv.h"
#include "ActorManager.h"
#include "Field.h"
#include "FieldPiece.h"
#include "ActorManager.h"
#include "EnergyItem.h"

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

	float a = nearestDistance.Length();
	// 最も近いパネルが規定値よりも遠かったらリターン
	if (nearestDistance.Length() >= specifiedValueDistance) return arg_velocity;

	// 正規化した一番近い穴へのベクトルと現在の進行方向ベクトルの内積
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = Vector3::Normalize(nearestDistance);
	float dot = Vector3::Dot(VecA, VecB);

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
	// 一番近い辺の算出
	float distance = 100;	// 辺との距離
	Vector2 nearyEdgS;		// 最も近い辺
	Vector2 nearyEdgE;
	auto edgs = Field::GetEdges();

	for (int i = 0; i < edgs.size(); i++)
	{
		Vector2 point = { arg_enemy->GetPosition().x,arg_enemy->GetPosition().y };
		Vector2 start = edgs[i];
		Vector2 end;
		if (i == edgs.size())
		{
			end = edgs[0];
		}
		else
		{
			end = edgs[i + 1];
		}

		float d = PointToLineDistance(point, start, end);

		if (d <= distance)
		{
			distance = d;
			nearyEdgS = start;
			nearyEdgE = end;
		}
	}

	// 一番近い辺との距離が規定値以上だったらリターン
	if (distance >= specifiedValueDistance) return arg_velocity;

	// 進行方向と辺の法線との外積
	Vector2 n = (nearyEdgS + nearyEdgE) / 2;	//フィールドの中心から辺の中心へのベクトル
	n = Vector2::Normalize(n);
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = { n.x, 0, n.y };

	float dot = Vector3::Dot(VecA, VecB);

	// 内積の絶対値が規定値以下だったらリターン
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// 進行方向の調整（とりあえず真反対に）
	Vector3 fixVel = -VecB;

	// 算出した修正ベクトルで一度確認する
	KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_enemy->GetPosition());

	return fixVel;
}

Vector3 EnemyAIPositiv::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// フィールド上のアイテムを近い順に並べる
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();
	// vectorが空だったら
	if (items.size() <= 0) return arg_velocity;

	std::vector<ItemRange*> itemRanges;

	Vector3 itemDistance;
	// 全アイテムを走査
	for (auto item : items)
	{
		// 距離を記録
		itemDistance = item->GetPosition() - arg_enemy->GetPosition();

		ItemRange* itemRange;
		itemRange->item = item;
		itemRange->range = itemDistance.Length();

		itemRanges.push_back(itemRange);
	}

	// 距離順でソート
	std::sort(itemRanges.begin(), itemRanges.end(), [](const ItemRange* x, const ItemRange* y) {return x->range < y->range; });

	// 他のActorとアイテムとの距離を見る
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// 他のActorがいなかったらリターン
	if (actors.size() <= 0)	return arg_velocity;
	// 全アイテムを走査
	for (auto itemRange : itemRanges)
	{
		// 全actorを走査
		for (auto actor : actors)
		{
			Vector3 r = itemRange->item->GetPosition() - actor->GetPosition();
			float otherRange = r.Length();
			
			// 自分より近いActorが居たら終了
			if (otherRange < itemRange->range)
			{
				break;
			}

			// 自分が一番近かった場合そこに向かう
			return itemRange->item->GetPosition() - arg_enemy->GetPosition();
		}
	}

	// 自分が一番近いアイテムが無かったら自分に一番近いアイテムへ向かう
	Vector3 fixVel = itemRanges[0]->item->GetPosition() - arg_enemy->GetPosition();

	return fixVel;
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