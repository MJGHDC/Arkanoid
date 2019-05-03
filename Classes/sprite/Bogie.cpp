#include "Bogie.h"

Bogie* Bogie::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Bogie* pSprite = new Bogie(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Bogie::Bogie(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

void Bogie::onEnter()
{
	Sprite::onEnter();

	mpTouchListener = EventListenerTouchOneByOne::create();
	mpTouchListener->setSwallowTouches(true);
	mpTouchListener->onTouchBegan = CC_CALLBACK_2(Bogie::onTouchBegan, this);
	mpTouchListener->onTouchMoved = CC_CALLBACK_2(Bogie::onTouchMoved, this);
	
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpTouchListener, this);
	//_eventDispatcher->addEventListenerWithFixedPriority(listener, _fixedPriority);

}

void Bogie::onExit()
{
	_eventDispatcher->removeEventListener(mpTouchListener);

	Sprite::onExit();
}

bool Bogie::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);

	if (rect.containsPoint(locationInNode))
	{
		log("touch Bogie");

		return true;
	}

	return false;
}

void Bogie::onTouchMoved(Touch * touch, Event * event)
{
	Vec2 move = Vec2(touch->getLocation().x, this->getPosition().y);
	if (move.x < 64)
	{
		move.x = 64;
	}
	else if (move.x > 960)
	{
		move.x = 960;
	}
	this->setPosition(move);
}
