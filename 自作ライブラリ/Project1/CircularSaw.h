#pragma once
#include "Object.h"
#include "BaseLocus.h"
#include "LocusDef.h"

class CircularSaw :
    public Object
{
	
public:
	enum GAMEOBJECTTYPE
	{
		PLAYER,
		ENEMY,
	};

	CircularSaw(Vector3 arg_virtualityPlanePosition, BaseLocus* arg_nowCuttingLocus, GAMEOBJECTTYPE arg_objecType, Object* arg_object);
	~CircularSaw();
	void Initialize()override;
	void Update()override;
	void DrawReady()override;

	void CopyLocus();
	Object* GetParentObject();
	
private:

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);

	LocusType cutLocusNum;

	GAMEOBJECTTYPE objectType;

	BaseLocus* nowCuttingLocus;

	float length;

	float speed;

	int currentLineNum;

	Vector3 virtualityPlanePosition;

	Object* parentObj;
};

