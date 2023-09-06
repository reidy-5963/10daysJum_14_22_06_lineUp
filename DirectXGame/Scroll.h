#pragma once 
#include "Sprite.h"
#include "Vector2.h"
#include <memory>
#include "Input.h"


class Scroll {
public:
	static Scroll* GetInstance();

	void Initialize();

	void Update();

	Vector2 GetAddScroll() { return addScroll; }

	void SetEdgePos(Vector2 edge) { edgePos_ = edge; }

	void SetTarget(const Vector2* target) {
		//
		target_ = target;
	}

private:
	Scroll() = default;
	~Scroll() = default;
	Scroll(const Scroll&) = delete;
	const Scroll& operator=(const Scroll&) = delete;

	Vector2 addScroll{};
	const Vector2* target_;

	Vector2 edgePos_{};

	int widthMax = 2;
	int heightMax = 2;
};