#pragma once

#include <DirectXMath.h>

class Object;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(Object* object, BaseCollider* collider, const DirectX::XMVECTOR& inter, const DirectX::XMVECTOR& reject = {}) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
		this->reject = reject;
	}

	// 衝突相手のオブジェクト
	Object* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
	//排斥ベクトル
	DirectX::XMVECTOR reject;

};

