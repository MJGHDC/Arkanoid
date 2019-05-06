#pragma once

#include "cocos2d.h"
USING_NS_CC;

#define BOGIE_TAG 1

enum class eItem;

class Bogie final : public Sprite
{
public:
	static Bogie* create(SpriteFrame* pSpriteFrame, int32_t tagNumber);

	Bogie() = default;
	Bogie(int32_t tagNumber);

	~Bogie() = default;

	void onEnter() override;
	void onExit() override;

	bool onContactSeparate(PhysicsContact& contact);
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);

	std::queue<eItem>& GetItemQueue();

private:
	void setItemQueueAndItemClear(Node* bogie, Node* item);

private:
	int32_t mTagNumber;
	std::queue<eItem> mItemQueue;

	EventListenerTouchOneByOne* mpTouchListener;
	EventListenerPhysicsContact* mpContactListener;
};

