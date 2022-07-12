#include "CircularSaw.h"
#include "OBJLoader.h"
#include "ActorManager.h"
#include "Field.h"
#include "Player.h"
#include "StandardEnemy.h"
#include "PanelCutLocus.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Audio.h"
#include "ObjectRegistType.h"
#include "IActionState.h"
#include "BaseGameActor.h"

CircularSaw::CircularSaw(Vector3 arg_virtualityPlanePosition, BaseLocus* arg_nowCuttingLocus, BaseGameActor* arg_object)
	:parentObj(arg_object)
{
	Create(OBJLoader::GetModel("Saw"));
	virtualityPlanePosition = arg_virtualityPlanePosition;
	nowCuttingLocus = arg_nowCuttingLocus;
	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::CIRCULAR_SAW);
	Initialize();
}

CircularSaw::~CircularSaw()
{
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::CIRCULAR_SAW);
	Audio::StopSE("SE_SawCutNow");
	Audio::PlaySE("SE_GetSaw", 1.0f * Audio::volume_se);

	ParticleManager::GetInstance()->DeleteFeverCutEffect();
}

void CircularSaw::Initialize()
{
	speed = 0.3f;
	if (parentObj->IsInFever())
	{
		speed *= 5;

		ParticleEmitter::FeverCut(nowCuttingLocus->GetLine(0));
		Audio::PlaySE("SE_FeverCut", Audio::volume_se);
	}
	length = 0;
	currentLineNum = 0;
	scale = { 5,5,5 };
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
	Object::Update();

}

void CircularSaw::Update()
{
	length += speed;

	//線の向き
	velocity = nowCuttingLocus->GetLine(currentLineNum)->GetDirection();


	virtualityPlanePosition = virtualityPlanePosition + velocity * speed;

	float lengthLocusLine = nowCuttingLocus->GetLine(currentLineNum)->GetLength();

	float angle = nowCuttingLocus->GetLine(currentLineNum)->GetAngle();

	rotation.y = angle + 90;
	rotation.x += 3;

	Audio::PlaySE("SE_SawCutNow", 1.0f * Audio::volume_se, true);

	if (lengthLocusLine - length <= 0.05f) //マジックサイコー
	{
		currentLineNum++;
		length = 0;

		if (currentLineNum >= nowCuttingLocus->GetMaxNumLine())
		{
			Dead();

			parentObj->CompleteCut();

			ParticleManager::GetInstance()->DeleteFeverCutEffect();
			return;
		}

		virtualityPlanePosition = nowCuttingLocus->GetLine(currentLineNum)->GetVirtualityPlaneStartPos();
		if (parentObj->IsInFever())
		{
			ParticleEmitter::FeverCut(nowCuttingLocus->GetLine(currentLineNum));
			Audio::PlaySE("SE_FeverCut", Audio::volume_se);
		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());


	ParticleManager::GetInstance()->UpdateFeverCutEffect();


	Object::Update();

	//Particle
	ParticleEmitter::CutEffect(position, velocity);
}

void CircularSaw::DrawReady()
{
}

void CircularSaw::Draw()
{


	Object::Draw();
}

void CircularSaw::CopyLocus()
{
	/*BaseLocus* copyLocus = nullptr;
	switch (nowCuttingLocus->GetType())
	{
	case LocusType::UNDIFINED:
		break;
	case LocusType::TRIANGLE:
		copyLocus = new TestTriangle(*predictTriangle, copyColor);
		break;
	case LocusType::RIBBON:
		copyLocus = new TestRibbon(*predictRibbon, copyColor);
		break;
	case LocusType::PENTAGON:
		copyLocus = new TestPentagon(*predictPentagon, copyColor);
		break;
	case LocusType::STAR:
		copyLocus = new TestStar(*predictStar, copyColor);
		break;
	case LocusType::HEXAGRAM:
		copyLocus = new TestHexagram(*predictHexagram, copyColor);
		break;
	case LocusType::TRIFORCE:
		copyLocus = new TestTriforce(*predictTriforce, copyColor);
		break;
	default:
		break;
	}

	switch (nowCuttingLocus->GetType())
	{
	case LocusType::UNDIFINED:
		nowCuttingLocus = nullptr;
		break;
	case LocusType::TRIANGLE:
		nowCuttingLocus = new TestTriangle(Vector3(0, -5, 0), 90, predictColor);;
		break;
	case LocusType::RIBBON:
		nowCuttingLocus = new TestRibbon(Vector3(0, -5, 0), 90, predictColor);;
		break;
	case LocusType::PENTAGON:
		nowCuttingLocus = new TestPentagon(Vector3(0, -5, 0), 90, predictColor);;
		break;
	case LocusType::STAR:
		nowCuttingLocus = new TestStar(Vector3(0, -5, 0), 90, predictColor);;
		break;
	case LocusType::HEXAGRAM:
		nowCuttingLocus = new TestHexagram(Vector3(0, -5, 0), 90, predictColor);;
		break;
	case LocusType::TRIFORCE:
		nowCuttingLocus = new TestTriforce(Vector3(0, -5, 0), 90, predictColor);;
		break;
	default:
		break;
	}*/
}

Object* CircularSaw::GetParentObject()
{
	return parentObj;
}


