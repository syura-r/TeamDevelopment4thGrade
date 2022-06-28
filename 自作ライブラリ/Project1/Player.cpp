#include "Player.h"

#include"OBJLoader.h"
#include"SphereCollider.h"
#include"Input.h"
#include"DebugText.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "GameSettingParam.h"
#include "ParticleEmitter.h"
#include "ActorManager.h"
#include "Field.h"
#include "StandardEnemy.h"
#include "EnergyItem.h"
#include "PanelItem.h"
#include "CircularSaw.h"
#include "PanelCutLocus.h"
#include "FieldPiece.h"
#include "ItemEmitter.h"
#include "UnableThroughEdge.h"
#include "UnableThroughBlock.h"
#include "ScoreManager.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "ObjectRegistType.h"

DebugCamera* Player::camera = nullptr;

Player::Player()
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("GamePlay_Player");
	//モデルの生成
	Create(myModel);	

	pObjectManager = ObjectManager::GetInstance();	

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);		

	panelCutLocus = new PanelCutLocus(Vector3(0, -5, 0), 90, predictColor);
	panelCutLocus->SetParentObject(this);

	name = typeid(*this).name();
	ActorManager::GetInstance()->AddObject(this, ObjectRegistType::PLAYER);

	panelCountUI = new PanelCountUI();
	panelCountSprite3D = new PanelCountSprite3D(position, name, gottenPanel);

	Initialize();

	//定数バッファの作成
	HRESULT result;
	result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstLightCameraBuff) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constCameraBuff));
	assert(SUCCEEDED(result));


	//デプスシャドウ用のカメラ情報を設定
	ConstLightCameraBuff* constMap2 = nullptr;
	result = constCameraBuff->Map(0, nullptr, (void**)&constMap2);
	assert(SUCCEEDED(result));
	constMap2->cameraPos = Object3D::GetLightCamera()->GetEye();
	constMap2->viewProjection = XMMatrixLookAtLH(Vector3(Object3D::GetLightCamera()->GetEye()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetTarget()).ConvertXMVECTOR(), Vector3(Object3D::GetLightCamera()->GetUp()).ConvertXMVECTOR()) * XMMatrixOrthographicLH(100, 100, 1.0f, 1000.0f);
	constCameraBuff->Unmap(0, nullptr);

	
}

Player::~Player()
{			
	Audio::StopWave("SE_SteppingOn");
	delete panelCountUI;
	delete panelCountSprite3D;		
	ActorManager::GetInstance()->DeleteObject(this, ObjectRegistType::PLAYER);
}

void Player::Initialize()
{	
	scale = { 0.9f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 0,0,1 };
	cameraRotCount = 0;
	rotCamera = false;
	radY = 0;
	camera->SetTarget(position + Vector3{ 0, 1, 0 });

	camera->Update();
	//----------カメラの回転角度を算出---------------
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	float cosA = Vector3(0, 0, 1).Dot(cameraDirectionZ);
	if (cosA > 1.0f)
		cosA = 1.0f;
	else if (cosA < -1.0f)
		cosA = -1.0f;
	float rad = acos(cosA);
	const Vector3 CrossVec = Vector3(0, 0, 1).Cross(cameraDirectionZ);
	if (CrossVec.y < 0)
		rad *= -1;
	//-----------------------------------------------
	//カメラの回転
	//camera->AddPhi(rad);
	camera->SetTheta(120);
	

	camera->SetDistance(100);

	drawingFlag = false;			
	virtualityPlanePosition = position;
	preVirtualityPlanePosition = virtualityPlanePosition;
	weight = 10;
	cutPower = 0;	
	gottenPanel = 0;

	panelCountUI->Initialize();
	panelCountSprite3D->Initialize();
	fallFlag = false;
	fallEasingCount = 0;
	pressFlag = false;
	nextInputStartCount = 60;
	count = 0;
	gameEnd = false;

	fallSoundFlag = false;
}

void Player::Update()
{
	if (fallFlag)
	{
		Fall();
		if (virtualityPlanePosition.y <= -65)
		{
			if (!fallSoundFlag)
			{
				Audio::StopWave("SE_SteppingOn");
				Audio::PlayWave("SE_Fall", 1.0f);
				fallSoundFlag = true;
			}
			gameEnd = true;
		}
	}
	else if (pressFlag)
	{
		static int pressCount = 0;
		pressCount++;
		if (pressCount >= 30)
		{
			gameEnd = true;
		}
	}
	else
	{
		Field* field = ActorManager::GetInstance()->GetFields()[0];
		CuttingInfo* info = field->GetCuttingInfo(this);		

		//移動処理
		Move();

		//カメラのリセット処理
		MoveCamera();

		field->DecideCuttingInfo(this, virtualityPlanePosition, direction);

		if (blowFlag)
		{
			blowTime--;
			if (blowTime <= 0)
			{
				blowFlag = false;
			}
		}
		else
		{					
			if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) && cutPower > 0 && info->ridingPiece)
			{
				if (info->ridingPiece)
				{
					if (!tackleFlag && !drawingFlag)
					{
						drawingFlag = true;
						
						Vector3 p = info->cuttingStartPos;
						ObjectManager::GetInstance()->Add(new CircularSaw(p, panelCutLocus, CircularSaw::PLAYER, this));
					}
				}

			}
			if (Input::TriggerPadButton(XINPUT_GAMEPAD_B))
			{
				Tackle();
			}
		}

		//当たり判定系		
		HitCheckEnemy();
		HitCheckItems();
		HitCheckUnableThroughEdge();
		HitCheckUnableThroughBlock();

		Vector3 p = info->cuttingStartPos;		
		if (!drawingFlag)
		{
			panelCutLocus->SetCutPower(cutPower);
			panelCutLocus->Move(p, info->cuttingAngle);
		}		
	}
	
	//他のオブジェクトとのヒットチェック	
	Object::Update();	

	panelCountUI->Update(gottenPanel);
	panelCountSprite3D->Update();
}

void Player::Draw()
{
	
	if (!Object3D::GetDrawShadow())
	{
		HRESULT result;

		//定数バッファをセット
		DirectXLib::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, constCameraBuff->GetGPUVirtualAddress());
	}
	CustomDraw(true, true);
	
	panelCountUI->Draw();
	panelCountSprite3D->Draw();
}

void Player::DrawReady()
{
#ifdef _DEBUG
	if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	{
		XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
		Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
		cameraDirectionZ.Normalize();
		ImGui::Begin("PlayerStatus");
		ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
		ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
		ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
		ImGui::Text("Rot : {%f, %f, %f }\n", rotation.x, rotation.y, rotation.z);
		ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
		ImGui::End();
	}
#endif

	if (Object3D::GetDrawShadow())
		pipelineName = "FBXShadowMap";
	else
	{
			pipelineName = "FBX";
	}
}

void Player::Move()
{
	prePos = position;
	preVirtualityPlanePosition = virtualityPlanePosition;

	//走りと歩きの切り替え処理
	if (drawingFlag)
	{
		speed = drawingSpeed;
	}
	else
	{
		speed = walkSpeed;
	}

	if (standingFlag)
	{
		WithStand();
	}
	else
	{
		SlidingDown();
	}

	//フィールド端からの復帰
	if (standingFlag || returningFieldFlag)
	{
		//myModel->PlayAnimation("stand", true);
		if (returningFieldFlag)
		{
			virtualityPlanePosition = EasingMove(returningStartPos, returningEndPos, 1, moveEasingCount / 30.0f);
			moveEasingCount++;
			if (moveEasingCount >= 30)
			{
				moveEasingCount = 0;
				returningFieldFlag = false;
			}
		}
	}
	//タックルの移動　ガチで月曜にplayerリファクタリングする
	else if (tackleFlag)
	{
		virtualityPlanePosition = EasingMove(tackleStartPos, tackleEndPos, 1, tackleCount / 30.0f);
		tackleCount++;
		if (tackleCount >= 30)
		{
			tackleCount = 0;
			tackleFlag = false;
			tackleHitFlag = false;
		}
		StayInTheField();
		StayOnRemainPanels();
	}
	//通常の移動
	else
	{
		//移動方向
		Vector3 moveDirection = direction;
		DecideDirection(moveDirection);

		//回転処理
		//現在の進行方向とカメラの正面と角度を求める
		direction.Normalize();
		float cosA = direction.Dot(moveDirection);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;

		float rotY = acos(cosA) * 180 / 3.14159365f;
		const Vector3 CrossVec = direction.Cross(moveDirection);

		//入力がないので0にする
		if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
		{
			moveDirection = Vector3(0, 0, 0);
		}

		float rotSpeed = rotateSpeed;
		if (abs(rotY) < 55)
		{
			virtualityPlanePosition += moveDirection * speed;
			StayInTheField();
			StayOnRemainPanels();			
		}

		if (rotSpeed > abs(rotY))
		{
			rotSpeed = rotY;
		}
		if (CrossVec.y < 0)
			rotSpeed *= -1;
		rotation.y += rotSpeed;
		XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotSpeed));
		XMVECTOR dir = { direction.x,direction.y,direction.z,0 };
		dir = XMVector3TransformNormal(dir, matRot);
		direction = dir;

		if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
		{
			//myModel->PlayAnimation("stand", true);
		}
		else if (drawingFlag)
		{
			//myModel->PlayAnimation("run", true);
		}
		else
		{
			//myModel->PlayAnimation("walk", true);
		}
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}


void Player::MoveCamera()
{
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();

	//カメラのリセット処理
#ifdef _DEBUG
	if (Input::TriggerKey(DIK_C) && !rotCamera)
	{
		rotCamera = true;
		float cosA = direction.Dot(cameraDirectionZ);
		if (cosA > 1.0f)
			cosA = 1.0f;
		else if (cosA < -1.0f)
			cosA = -1.0f;
		radY = acos(cosA);
		const Vector3 CrossVec = direction.Cross(cameraDirectionZ);
		if (CrossVec.y < 0)
			radY *= -1;
		cameraRotCount = 0;
		//camera->AddPhi(radY);
	}
#endif // _DEBUG
	
	//カメラの回転処理
	if (rotCamera)
	{
		cameraRotCount++;
		float rad = radY / RotTime;
		camera->AddPhi(rad);
		if (cameraRotCount >= RotTime)
			rotCamera = false;
	}
}

void Player::SlidingDown()
{
	if (drawingFlag)
	{
		return;
	}	

	Field* field = ActorManager::GetInstance()->GetFields()[0];

	float fallSpeed = 0.01f * (gottenPanel / 8 + 1);
	virtualityPlanePosition += field->GetTilt() * fallSpeed * field->GetDepthMagnitude();
	StayInTheField();
	StayOnRemainPanels();

	if (nextInputStartCount > 60)
	{
		count++;
		if (count >= 60)
		{
			count = 0;
			nextInputStartCount -= 2;
		}
	}
	
}

void Player::DecideDirection(Vector3& arg_direction)
{
	//ふっとばされ中
	if (blowFlag)
	{
		//velocityに入っている値に進むように
		arg_direction = velocity;		
		speed = blowSpeed;
		return;
	}

	if (!Input::DownWASD() && !Input::CheckPadLStickAnythingDir())
	{
		return;
	}

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();

	//くり抜き動作中
	if (drawingFlag)
	{
		arg_direction = { 0,0,0 };		
	}
	else
	{
		if (Input::DownKey(DIK_A))
			arg_direction += cameraDirectionX * -1;
		if (Input::DownKey(DIK_D))
			arg_direction += cameraDirectionX;
		if (Input::DownKey(DIK_S))
			arg_direction += cameraDirectionZ * -1;
		if (Input::DownKey(DIK_W))
			arg_direction += cameraDirectionZ;
		if (Input::CheckPadLStickAnythingDir())
		{
			auto vec = Input::GetLStickDirection();

			arg_direction = cameraDirectionX * vec.x + cameraDirectionZ * vec.y;
		}		
	}

	arg_direction.Normalize();
	//反発用に代入
	velocity = arg_direction;
}

void Player::StayInTheField()
{
	if (standingFlag || returningFieldFlag)
	{
		return;
	}	

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
			//StartStand();
			SuspendTackle();
			break;
		}

		if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			//StartStand();
			SuspendTackle();
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
			//StartStand();
			SuspendTackle();
			break;
		}
	}

	if (!standingFlag)
	{
		return;
	}

	if (drawingFlag)
	{
		HitOnDrawing();
	}
}

void Player::StayOnRemainPanels()
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

		if (tackleFlag)
		{
			SuspendTackle();
		}
		return;
	}

	//即落ち
	if (!standingFlag)
	{
		auto gottenPanels = field->GetGottenPieces();
		for (auto p : gottenPanels)
		{
			bool isFall = true;

			if (Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - p->GetVirtualityPlanePosition())) > RADIUS + FieldPiece::GetLowerTimeOffset())
			{
				continue;
			}

			auto points = p->GetPoints();
			for (int i = 0; i < points.size(); i++)
			{
				Vector2 A = points[i];
				Vector2 B = points[(i + 1) % points.size()];
				Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition) - A;
				Vector2 AB = B - A;
				Vector2 normalAB = Vector2::Normalize(AB);

				//今当たっているか
				float cross = Vector2::Cross(AO, normalAB);
				if (cross < 0)
				{
					isFall = false;
					break;
				}
			}

			if (isFall)
			{
				fallFlag = true;
				fallStartPos = virtualityPlanePosition;
				fallEndPos = p->GetVirtualityPlanePosition();
				return;
			}
		}
	}
}

void Player::HitCheckEnemy()
{
	if (blowFlag || tackleHitFlag)
	{
		return;
	}

	std::vector<StandardEnemy*> enemies = ActorManager::GetInstance()->GetStandardEnemies();	

	for (auto itr = enemies.begin(); itr != enemies.end(); itr++)
	{
		float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ((*itr)->GetVirtualityPlanePosition() - virtualityPlanePosition));
		if (length <= 2.0f)
		{
			HitEnemy(*itr);
		}
	}
}

void Player::HitEnemy(StandardEnemy* arg_enemy)
{
	static const float weightCoefficient = 0.3f;
	
	if (arg_enemy->IsFall())
	{
		return;
	}

	Audio::PlayWave("SE_Collision", 1.0f);


	//汎用化	
	if (arg_enemy->GetStanding() && tackleFlag)
	{
		arg_enemy->StartFall();

	}
	else if (standingFlag && arg_enemy->IsTackle())
	{
		Object::SetColor({ 1,1,1,1 });
		standingFlag = false;
		fallFlag = true;
		fallStartPos = virtualityPlanePosition;
		fallEndPos = virtualityPlanePosition + (-preStandVec * 4);

		Audio::StopWave("SE_SteppingOn");
	}
	else
	{
		arg_enemy->StartBlow();
		arg_enemy->SetBlowTime(40);
	}



	blowFlag = true;
	
	blowTime = 40;
	

	Vector3 enemyPos = arg_enemy->GetVirtualityPlanePosition();
	Vector3 enemyVel = arg_enemy->GetVelocity();
	float enemyWeight = arg_enemy->GetWeight() * weightCoefficient;

	float playerWeight = weight;

	if (tackleFlag)
	{
		playerWeight += 20;
	}

	float totalWeight = (playerWeight * weightCoefficient) + enemyWeight;
	float refRate = (1 + 1 * 1); //反発率をプレイヤー、エネミーそれぞれ持たせる
	Vector3 c = virtualityPlanePosition - enemyPos;
	c.Normalize();

	float dot = Vector3::Dot((velocity - enemyVel), c);
	Vector3 constVec = refRate * dot / totalWeight * c;

	//衝突後速度ベクトル
	Vector3  playerAfterVel = -enemyWeight * constVec + velocity;
	Vector3  enemyAfterVel = (playerWeight * weightCoefficient) * constVec + enemyVel;

	if (!fallFlag && !arg_enemy->IsFall())
	{
		DischargeGottenPanel(arg_enemy);
		arg_enemy->DischargeGottenPanel(this);
	}

	if (tackleFlag)
	{
		//タックルで敵だけ飛ばす
		blowFlag = false;
		blowTime = 0;
		tackleHitFlag = true;
		SuspendTackle();
		arg_enemy->SetVelocity(enemyAfterVel.Normalize());

		//パーティクル
		ParticleEmitter::ShockEffect((arg_enemy->GetPosition() + position) / 2.0f, Vector3(255.0f, 255.0f, 255.0f));
	}
	else
	{
		velocity = playerAfterVel.Normalize();
		arg_enemy->SetVelocity(enemyAfterVel.Normalize());
	}
	
	if (drawingFlag)
	{
		HitOnDrawing();
	}
	if (arg_enemy->IsDrawing())
	{
		arg_enemy->HitOnDrawing();
	}

	////衝突後位置
	//Vector3 playerAfterPos = position + blowTime * playerAfterVel;
	//Vector3 enemyAfterPos = enemyPos + standardEnemy->GetBlowTime() * enemyAfterVel;
}

void Player::HitCheckItems()
{
	std::vector<PanelItem*> panelItems = ActorManager::GetInstance()->GetPanelItems();

	for (auto panelItem : panelItems)
	{
		if (!panelItem->IsEndBounce())
		{
			continue;
		}

		float length = Vector2::Length(LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - panelItem->GetVirtualityPlanePosition()));

		if (length <= RADIUS + PanelItem::GetRadius())
		{
			HitPanelItem(panelItem);
		}
	}

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

void Player::HitItem(EnergyItem* arg_item)
{
	switch (arg_item->GetRank())
	{
	case RankEnergyItem::NORMAL:
		cutPower++;
		break;
	case RankEnergyItem::SILVER:
		cutPower += 2;
		break;
	case RankEnergyItem::GOLD:
		cutPower += 3;
		break;
	default:
		cutPower++;
		break;
	}	

	if (cutPower > 6)
	{
		cutPower = 6;
	}

	Audio::PlayWave("SE_GetTriangle");
	arg_item->Dead();
}

void Player::HitPanelItem(PanelItem* arg_panelItem)
{
	arg_panelItem->Dead();	
	weight += FieldPiece::GetWeight();
	gottenPanel++;
}

void Player::HitCheckUnableThroughEdge()
{
	auto edges = ActorManager::GetInstance()->GetUnableThroughEdges();
	if (edges.empty())
	{
		return;
	}

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);	

	if (!info->ridingPiece)
	{
		if (edges[0]->IsEndFallDown())
		{
			virtualityPlanePosition = preVirtualityPlanePosition;
			position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
		}
		else if (edges[0]->GetPosition().y <= field->GetPosition().y + UnableThroughEdge::GetHeightMagnification() * 2)
		{
			pressFlag = true;
		}
	}	
}

void Player::HitUnableThroughEdge()
{
}

void Player::HitCheckUnableThroughBlock()
{
	auto blocks = ActorManager::GetInstance()->GetUnableThroughBlocks();
	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);

	for (auto b : blocks)
	{
		if (info->ridingPiece == b->GetParentPiece())
		{
			if (b->IsEndFallDown())
			{
				virtualityPlanePosition = preVirtualityPlanePosition;
				position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
			}
			else if (b->GetVirtualityPlanePosition().y <= field->GetPosition().y + UnableThroughBlock::GetHeightMagnification() * 2)
			{
				pressFlag = true;
			}
		}
	}
}

void Player::HitUnableThroughBlock()
{
}

void Player::StartStand(bool arg_outField, Vector3 arg_velocity)
{
	standingFlag = true;
	//タックル終了
	SuspendTackle();

	inputStartCount = nextInputStartCount;
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

	Audio::PlayWave("SE_SteppingOn", 0.25f, true);
}

void Player::WithStand()
{
	if (drawingFlag)
	{
		return;
	}	

	//踏ん張り中　赤
	Object::SetColor({ 0.6f,BGColor,BGColor,1 });
	if (BGColor >= 0)
	{
		BGColor -= 0.02f;
	}
	else
	{
		BGColor = 1;
	}

	Object::SetColor({ 1,1,1,1 });
	standingFlag = false;
	fallFlag = true;
	fallStartPos = virtualityPlanePosition;
	fallEndPos = virtualityPlanePosition + (-preStandVec * 4);

	Audio::StopWave("SE_SteppingOn");


	//if (inputStartCount <= 0)
	//{
	//	//カメラのビュー行列の逆行列を計算
	//	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	//	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	//	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	//	Vector2 stickDirection = {};
	//	//スティックの向き
	//	auto vec = Input::GetLStickDirection();
	//	stickDirection.x = (cameraDirectionX * vec.x).x;
	//	stickDirection.y = (cameraDirectionZ * vec.y).z;
	//	stickDirection = Vector2::Normalize(stickDirection);

	//	float accuracy = 0;

	//	Vector2 correctVec = LocusUtility::Dim3ToDim2XZ(preStandVec);

	//	accuracy = Vector2::Dot(stickDirection, correctVec);

	//	if (accuracy <= 0)
	//	{
	//		accuracy = 0;
	//	}

	//	if (accuracy >= 0.55f)
	//	{
	//		Vector3 moveDirection = preStandVec;
	//		Object::SetColor({ 1,1,1,1 });
	//		returningFieldFlag = true;
	//		standingFlag = false;
	//		returningStartPos = virtualityPlanePosition;
	//		returningEndPos = virtualityPlanePosition + moveDirection * 3;
	//		nextInputStartCount = nextInputStartCount + 30;

	//		Audio::StopWave("SE_SteppingOn");

	//		return;
	//	}

	//	standTime--;
	//	if (standTime <= 0)
	//	{
	//		Object::SetColor({ 1,1,1,1 });
	//		standingFlag = false;
	//		fallFlag = true;
	//		fallStartPos = virtualityPlanePosition;
	//		fallEndPos = virtualityPlanePosition + (-preStandVec * 4);

	//		Audio::StopWave("SE_SteppingOn");
	//	}
	//}
	//else
	//{
	//	inputStartCount--;
	//}

	
}

void Player::Tackle()
{
	if (tackleFlag || blowFlag || drawingFlag || standingFlag || returningFieldFlag )
	{
		return;
	}

	tackleFlag = true;

	//カメラのビュー行列の逆行列を計算
	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	const Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]).Normalize();
	const Vector3 cameraDirectionX = Vector3(camMatWorld.r[0].m128_f32[0], 0, camMatWorld.r[0].m128_f32[2]).Normalize();
	Vector2 stickDirection = {};
	Vector3 moveDirection = {};
	//スティックの向き
	auto vec = Input::GetLStickDirection();
	stickDirection.x = vec.x;
	stickDirection.y = vec.y;
	stickDirection = Vector2::Normalize(stickDirection);

	tackleStartPos = virtualityPlanePosition;

	moveDirection = cameraDirectionX * stickDirection.x + cameraDirectionZ * stickDirection.y;
	moveDirection.Normalize();
	tackleEndPos = virtualityPlanePosition + moveDirection * 8;

	Audio::PlayWave("SE_Dash");
}

void Player::SuspendTackle()
{
	if (standingFlag || returningFieldFlag)
	{
		tackleFlag = false;
		tackleCount = 0;
		speed = walkSpeed;
	}
	//タックル中
	else
	{
		tackleEndPos = virtualityPlanePosition;
		//tackleCount = 0;
		speed = walkSpeed;
	}
	
}

void Player::DischargeGottenPanel(StandardEnemy* arg_enemy)
{
	if (arg_enemy->IsFall())
	{
		return;
	}

	ItemEmitter* itemEmitter = ItemEmitter::GetInstance();
	int maxEmit = 0;
	if (gottenPanel == 1)
	{
		maxEmit = 1;
	}
	else
	{
		maxEmit = gottenPanel / 2;
	}
	
	for (int i = 0; i < maxEmit; i++)
	{
		itemEmitter->EmitPanelItem(virtualityPlanePosition);
		gottenPanel--;
		weight -= FieldPiece::GetWeight();
	}

	if (maxEmit != 0)
	{
		//Audio::PlayWave("SE_TriangleLost");
	}
}

void Player::Fall()
{
	if (!fallFlag)
	{
		return;
	}

	if (fallEasingCount <= 30)
	{
		fallEasingCount++;
		virtualityPlanePosition = EasingMove(fallStartPos, fallEndPos, 1, fallEasingCount / 30.0f);
	}
	else
	{
		virtualityPlanePosition.y -= 2;
	}
	

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition()); 
	
	
}

Vector3 Player::EasingMove(Vector3 arg_startPos, Vector3 arg_endPos, int arg_maxTime, float arg_nowTime)
{
	Vector3 result = {};
	result.x = Easing::EaseOutCubic(arg_startPos.x, arg_endPos.x, arg_maxTime, arg_nowTime);
	result.y = Easing::EaseOutCubic(arg_startPos.y, arg_endPos.y, arg_maxTime, arg_nowTime);
	result.z = Easing::EaseOutCubic(arg_startPos.z, arg_endPos.z, arg_maxTime, arg_nowTime);
	return result;
}

void Player::EndDrawing()
{
	drawingFlag = false;
	panelCutLocus->RecordCuttedPanelPos();
	int num = ActorManager::GetInstance()->GetFields()[0]->CutPanel(panelCutLocus);
	weight += num * FieldPiece::GetWeight();
	gottenPanel += num;
	cutPower = 0;

	ScoreManager::GetInstance()->AddScore_CutPanel(num);

	Field* field = ActorManager::GetInstance()->GetFields()[0];
	CuttingInfo* info = field->GetCuttingInfo(this);
	virtualityPlanePosition = info->ridingPiece->GetVirtualityPlanePosition();
	position = LocusUtility::RotateForFieldTilt(virtualityPlanePosition, field->GetAngleTilt(), field->GetPosition());
}

Vector3 Player::GetDirection() const
{
	return direction;
}

PanelCutLocus* Player::GetPanelCutLocus()
{
	return panelCutLocus;
}

void Player::HitOnDrawing()
{
	auto c = ActorManager::GetInstance()->GetCircularSaw(this);
	if (c)
	{
		c->Dead();
	}
	drawingFlag = false;
}

//void Player::HitCheckLoci()
//{
//	if (virtualityPlanePosition == preVirtualityPlanePosition)
//	{
//		return;
//	}
//
//	for (auto locus : vecLocuss)
//	{
//		for (int i = 0; i < locus->GetMaxNumLine(); i++)
//		{
//			Line* line = locus->GetLine(i);
//			Vector2 AO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneStartPos());
//			Vector2 BO = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition - line->GetVirtualityPlaneEndPos());
//			Vector2 AB = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos() - line->GetVirtualityPlaneStartPos());
//			Vector2 normalAB = Vector2::Normalize(AB);
//
//			//今当たっているか
//			float cross = Vector2::Cross(AO, normalAB);
//			if (fabsf(cross) > RADIUS)
//			{
//				continue;
//			}
//
//			float multiDot = Vector2::Dot(AO, AB) * Vector2::Dot(BO, AB);
//			if (multiDot <= 0.0f)
//			{
//				HitLoci(line);
//				continue;
//			}
//
//			if (Vector2::Length(AO) < RADIUS || Vector2::Length(BO) < RADIUS)
//			{
//				HitLoci(line);
//				continue;
//			}
//
//			//通り過ぎたか
//			Vector2 start = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneStartPos());
//			Vector2 end = LocusUtility::Dim3ToDim2XZ(line->GetVirtualityPlaneEndPos());
//			Vector2 pos = LocusUtility::Dim3ToDim2XZ(virtualityPlanePosition);
//			Vector2 pre = LocusUtility::Dim3ToDim2XZ(preVirtualityPlanePosition);
//
//			if (LocusUtility::Cross3p(start, end, pos) * LocusUtility::Cross3p(start, end, pre) < 0.0f &&
//				LocusUtility::Cross3p(pos, pre, start) * LocusUtility::Cross3p(pos, pre, end) < 0.0f)
//			{
//				HitLoci(line);
//			}
//		}
//	}
//}

//void Player::HitLoci(Line* arg_line)
//{
//	position = prePos;
//	virtualityPlanePosition = preVirtualityPlanePosition;
//
//	if (drawingFlag)
//	{
//		SuspendDrawing();
//	}
//}
