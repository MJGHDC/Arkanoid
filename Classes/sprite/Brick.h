#pragma once

#include "cocos2d.h"

USING_NS_CC;

class Brick : public Sprite
{
public:
	static Brick* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Brick() = default;
	Brick(int32_t tagNumber);

	~Brick() = default;

	void onEnter() override;
	void onExit() override;

private:
	int32_t mTagNumber;
};