#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define ITEM_TAG 3
#define BRICK_TAG 4

enum class eBrickStatus;
enum class eItem;

class Brick : public Sprite
{
public:
	static Brick* create(SpriteFrame* pSpriteFrame, int32_t tagNumber, eItem item);

	Brick() = default;
	Brick(int32_t tagNumber, eItem item);

	~Brick() = default;

	void onEnter() override;
	void onExit() override;

	Sprite* GetItem() const;

private:
	int32_t mTagNumber;
	eBrickStatus mStatus;
	eItem mItemUse;
};