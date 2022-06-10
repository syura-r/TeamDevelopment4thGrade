#pragma once
#include "Object.h"
#include "BaseLocus.h"
#include "LocusDef.h"

class CircularSaw :
    public Object
{
public:

	CircularSaw(Vector3 arg_virtualityPlanePosition, BaseLocus* arg_nowCuttingLocus);
	~CircularSaw();
	void Initialize()override;
	void Update()override;
	void DrawReady()override;

	void CopyLocus();

private:

	XMFLOAT4 predictColor = XMFLOAT4(1, 1, 0, 0.6f);

	LocusType cutLocusNum;

	BaseLocus* nowCuttingLocus;

	float length;

	float speed;

	int currentLineNum;

	Vector3 virtualityPlanePosition;

	
};

