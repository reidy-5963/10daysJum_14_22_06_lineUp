#pragma once 
#include "Sprite.h"
#include "Vector2.h"
#include <memory>
#include "Input.h"


class Scroll {
public:
	enum OutDirect {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UNKNOWN
	};

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

	int IsScreenOut() { return isScreenOut_; }

	void SetIsScreenOut(int isScreenOut) { isScreenOut_ = isScreenOut; }
	
	void loopUpdate();

	void WorldUpdate2x2();

	Vector2 GetEdgePos() { return edgePos_; }

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


	int isScreenOut_ = UNKNOWN;

};