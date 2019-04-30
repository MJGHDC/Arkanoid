#pragma once

#include "cocos2d.h"
USING_NS_CC;

#define BOGIE_TAG 1

class Bogie : public Sprite
{
public:
	static Bogie* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bogie() = default;
	Bogie(int32_t tagNumber);

	~Bogie() = default;

	void onEnter() override;
	void onExit() override;

private:
	int32_t mTagNumber;

	EventListener* mListener;
};

