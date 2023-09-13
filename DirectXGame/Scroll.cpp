#include "Scroll.h"
#include "WinApp.h"

Scroll* Scroll::GetInstance() {
	static Scroll instance;
	return &instance;
}

void Scroll::Initialize() { 
	target_ = nullptr;
	addScroll.x = 0.0f;
	addScroll.y = 0.0f;
}

void Scroll::Update() { 
	
	WorldUpdate2x2();

}

void Scroll::loopUpdate() {
	if (isScreenIn_ == UNKNOWN) {
		if (target_->x >= float(WinApp::kWindowWidth)) {
			addScroll.x -= float(WinApp::kWindowWidth);
			isScreenIn_ = LEFT;
		} else if (target_->x <= 0) {
			addScroll.x += float(WinApp::kWindowWidth);
			isScreenIn_ = RIGHT;
		}

		if (target_->y >= float(WinApp::kWindowHeight)) {
			addScroll.y -= float(WinApp::kWindowHeight);
			isScreenIn_ = UP;
		} else if (target_->y <= 0) {
			addScroll.y += float(WinApp::kWindowHeight);
			isScreenIn_ = DOWN;
		}
	}
}

void Scroll::WorldUpdate2x2() {
	 if (target_->x >= float(WinApp::kWindowWidth) * float(widthMax) + edgePos_.x) {
		addScroll.x = float(WinApp::kWindowWidth) * float(widthMax);
	 }
	 else if (target_->x >= edgePos_.x) {
		addScroll.x = target_->x - edgePos_.x;
	 }
	//
	 if (target_->y >= float(WinApp::kWindowHeight) * float(heightMax) + edgePos_.y) {
		addScroll.y = float(WinApp::kWindowHeight) * float(heightMax);
	 } else if (target_->y >= edgePos_.y) {
		addScroll.y = target_->y - edgePos_.y;
	 }
}
