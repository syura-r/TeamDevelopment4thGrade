#pragma once
#include <array>

#include "BoxCollider.h"
#include "DebugCamera.h"
#include "FBXModel.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Line.h"
#include "BaseLocus.h"
#include "Timer.h"
#include "NumberSprite.h"
#include "PanelCountUI.h"
#include "PanelCountSprite3D.h"
#include "BaseGameActor.h"

class StandardEnemy;
class EnergyItem;
class PanelItem;
class PanelCutLocus;

class Player : public BaseGameActor
{
public:
	Player(const Vector3& arg_pos);
	~Player();
	void Initialize()override;
	void Update()override;
	void Draw() override;
	void DrawReady() override;

	//---Cut---
	//Ø‚è”²‚«I—¹CircularSaw‚©‚çŒÄ‚Î‚ê‚é
	virtual void CompleteCut()override;
	
private:
	void KillRandEnem();

	//---Cut---
	virtual void SetTargetActor()override;
};

