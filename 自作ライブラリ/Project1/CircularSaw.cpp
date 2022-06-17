#include "CircularSaw.h"
#include "OBJLoader.h"
#include "TestStar.h"
#include "TestTriangle.h"
#include "TestRibbon.h"
#include "TestPentagon.h"
#include "TestHexagram.h"
#include "TestTriforce.h"
#include "ActorManager.h"
#include "Field.h"
#include "Player.h"
#include "StandardEnemy.h"
#include "PanelCutLocus.h"

#include "ParticleEmitter.h"

CircularSaw::CircularSaw(Vector3 arg_virtualityPlanePosition, BaseLocus* arg_nowCuttingLocus, GAMEOBJECTTYPE arg_objecType, Object* arg_object)
	:parentObj(arg_object)
{
	Create(OBJLoader::GetModel("Saw"));
	virtualityPlanePosition = arg_virtualityPlanePosition;
	nowCuttingLocus = arg_nowCuttingLocus;
	objectType = arg_objecType;
	Initialize();
}

CircularSaw::~CircularSaw()
{
}

void CircularSaw::Initialize()
{
	speed = 0.3f;
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

	if (lengthLocusLine - length <= 0.05f) //マジックサイコー
	{
		currentLineNum++;
		length = 0;
		
		if (currentLineNum >= nowCuttingLocus->GetMaxNumLine())
		{
			Dead();
		
			switch (objectType)
			{
			case GAMEOBJECTTYPE::PLAYER:
				static_cast<Player*>(parentObj)->EndDrawing();
				break;
			case GAMEOBJECTTYPE::ENEMY:
				static_cast<StandardEnemy*>(parentObj)->EndDrawing();
				break;
			default:
				break;
			}
		

		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());

	Object::Update();

	//Particle
	ParticleEmitter::CutEffect(position, velocity);
}

void CircularSaw::DrawReady()
{
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


