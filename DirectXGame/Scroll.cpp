#include "Scroll.h"
#include "WinApp.h"

Scroll* Scroll::GetInstance() {
	static Scroll instance;
	return &instance;
}

void Scroll::Initialize() {
	

}

void Scroll::Update() { 
	//
	if (target_->x >= float(WinApp::kWindowWidth) * float(widthMax) + edgePos_.x) {
		addScroll.x = float(WinApp::kWindowWidth) * float(widthMax);
	} 
	else if (target_->x >= edgePos_.x) {
		addScroll.x = target_->x - edgePos_.x;
	}
	//
	if (target_->y >= float(WinApp::kWindowHeight) * float(heightMax) + edgePos_.y) {
		addScroll.x = float(WinApp::kWindowHeight) * float(heightMax);
	} else if (target_->y >= edgePos_.y) {
		addScroll.y = target_->y - edgePos_.y;
	}



}
