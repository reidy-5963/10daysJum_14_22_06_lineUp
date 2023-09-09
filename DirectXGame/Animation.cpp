#include "Animation.h"
#include "TextureManager.h"

void Animation::Anime(int& count, int& number, const int scene, const int oneTime) {
	// カウント
	count++;

	// もしアニメーションの枚数 * 1枚にかけるフレーム数
	if (count > scene * oneTime) {
		// カウントを0に
		count = 0;
	}

	// アニメーションの枚数分
	for (int i = 0; i < scene; ++i) {
		//
		if (count == i * oneTime) {
			number = count / oneTime;
		}
	}
}
void Animation::DrawAnimation(
    Sprite* sprite, int texX, int texY, int srcX, int srcY, int srcW, int srcH, uint32_t texture) {
	const D3D12_RESOURCE_DESC& texDesc = TextureManager::GetInstance()->GetResoureDesc(texture);
	texX;
	texY;
	if (srcX < 0 || srcY < 0 || srcW < 0 || srcH < 0) {
		sprite->SetTextureRect({0.0f, 0.0f}, {(float)texDesc.Width, (float)texDesc.Height});
	} else {
		sprite->SetTextureRect({(float)srcX, (float)srcY}, {(float)srcW, (float)srcH});
	}
	sprite->Draw();
}

void Animation::DrawAnimation(Sprite* sprite, Vector2 pos, int animationNum, uint32_t tex) {
	const D3D12_RESOURCE_DESC& texDesc = TextureManager::GetInstance()->GetResoureDesc(tex);
	Animation::DrawAnimation(
	    sprite, int(pos.x), int(pos.y), int(animationNum * int(texDesc.Height)), 0,
	    int(texDesc.Height), int(texDesc.Height), tex);
}
