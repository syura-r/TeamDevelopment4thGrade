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

	// �P���؂蔲����Ă��Ȃ������烊�^�[��
	if (gottenPanels.size() <= 0) return arg_velocity;
	// �p�l���Ƃ̋���
	Vector3 distance = { 0,0,0 };
	// ��ԋ߂��p�l���Ƃ̋���
	Vector3 nearestDistance = gottenPanels[0]->GetPosition() - arg_currentPos;
	// ��ԋ߂��p�l���̃|�C���^�i�Čv�Z�p�j
	FieldPiece* nearestPiece = gottenPanels[0];

	// �؂蔲���ꂽ�p�l���̈ʒu�𑖍�
	for (auto panel : gottenPanels)
	{
		distance = panel->GetPosition() - arg_currentPos;

		// ��莩���ɋ߂���������������
		if (distance.Length() < nearestDistance.Length())
		{
			// �����̍X�V
			nearestDistance = distance;
			nearestPiece = panel;
		}
	}

	float a = nearestDistance.Length();
	// �ł��߂��p�l�����K��l�������������烊�^�[��
	if (nearestDistance.Length() >= specifiedValueDistance) return arg_velocity;

	// ���K��������ԋ߂����ւ̃x�N�g���ƌ��݂̐i�s�����x�N�g���̓���
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = Vector3::Normalize(nearestDistance);
	float dot = Vector3::Dot(VecA, VecB);

	// ���ς̐�Βl���K��l�ȉ��������烊�^�[��
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// �i�s�����̒����i���݂̐i�s�����Ƃ��̋t�����̊O�ς��璼�p�ȃx�N�g�����o���j
	Vector3 reverseVec = { -arg_velocity.x,-arg_velocity.y,-arg_velocity.z };
	reverseVec = Vector3::Normalize(reverseVec);
	Vector3 fixVel = reverseVec.Cross(Vector3::Normalize(arg_velocity));

	// �Z�o�����C���x�N�g���ň�x�m�F����
	KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_currentPos, nearestPiece);

	// �i�s�����𒲐������x�N�g����Ԃ�
	return fixVel;
}

Vector3 EnemyAIPositiv::KeepAwayFromFieldBorder(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// ��ԋ߂��ӂ̎Z�o
	float distance = 100;	// �ӂƂ̋���
	Vector2 nearyEdgS;		// �ł��߂���
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

	// ��ԋ߂��ӂƂ̋������K��l�ȏゾ�����烊�^�[��
	if (distance >= specifiedValueDistance) return arg_velocity;

	// �i�s�����ƕӂ̖@���Ƃ̊O��
	Vector2 n = (nearyEdgS + nearyEdgE) / 2;	//�t�B�[���h�̒��S����ӂ̒��S�ւ̃x�N�g��
	n = Vector2::Normalize(n);
	Vector3 VecA = Vector3::Normalize(arg_velocity);
	Vector3 VecB = { n.x, 0, n.y };

	float dot = Vector3::Dot(VecA, VecB);

	// ���ς̐�Βl���K��l�ȉ��������烊�^�[��
	if (abs(dot) <= specifiedValueDot)return arg_velocity;

	// �i�s�����̒����i�Ƃ肠�����^���΂Ɂj
	Vector3 fixVel = -VecB;

	// �Z�o�����C���x�N�g���ň�x�m�F����
	KeepAwayFromGottenPieces(arg_enemy, fixVel, arg_enemy->GetPosition());

	return fixVel;
}

Vector3 EnemyAIPositiv::AgainstFieldTilt(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	return Vector3();
}

Vector3 EnemyAIPositiv::ApproachEnergyItem(StandardEnemy* arg_enemy, const Vector3& arg_velocity)
{
	// �t�B�[���h��̃A�C�e�����߂����ɕ��ׂ�
	std::vector<EnergyItem*> items = ActorManager::GetInstance()->GetEnergyItems();
	// vector���󂾂�����
	if (items.size() <= 0) return arg_velocity;

	std::vector<ItemRange*> itemRanges;

	Vector3 itemDistance;
	// �S�A�C�e���𑖍�
	for (auto item : items)
	{
		// �������L�^
		itemDistance = item->GetPosition() - arg_enemy->GetPosition();

		ItemRange* itemRange;
		itemRange->item = item;
		itemRange->range = itemDistance.Length();

		itemRanges.push_back(itemRange);
	}

	// �������Ń\�[�g
	std::sort(itemRanges.begin(), itemRanges.end(), [](const ItemRange* x, const ItemRange* y) {return x->range < y->range; });

	// ����Actor�ƃA�C�e���Ƃ̋���������
	auto actors = ActorManager::GetInstance()->GetBaseGameActors();
	// ����Actor�����Ȃ������烊�^�[��
	if (actors.size() <= 0)	return arg_velocity;
	// �S�A�C�e���𑖍�
	for (auto itemRange : itemRanges)
	{
		// �Sactor�𑖍�
		for (auto actor : actors)
		{
			Vector3 r = itemRange->item->GetPosition() - actor->GetPosition();
			float otherRange = r.Length();
			
			// �������߂�Actor��������I��
			if (otherRange < itemRange->range)
			{
				break;
			}

			// ��������ԋ߂������ꍇ�����Ɍ�����
			return itemRange->item->GetPosition() - arg_enemy->GetPosition();
		}
	}

	// ��������ԋ߂��A�C�e�������������玩���Ɉ�ԋ߂��A�C�e���֌�����
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