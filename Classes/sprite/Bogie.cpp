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

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [=](Touch* touch, Event* event) {

		Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
		Size s = this->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("touch Bogie");
			//this->setColor(Color3B::RED);
			return true;
		}
		return false;
	};

	listener->onTouchMoved = [=](Touch* touch, Event* event) {

		Vec2 move = Vec2(touch->getLocation().x, this->getPosition().y);
		if (move.x < 64)
		{
			move.x = 64;
		}
		else if (move.x > 960)
		{
			move.x = 960;
		}
		log("x : %f, y : %f", move.x, move.y);
		this->setPosition(move);
	};

	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		//this->setColor(Color3B::WHITE);
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//_eventDispatcher->addEventListenerWithFixedPriority(listener, _fixedPriority);

	mListener = listener;
}

void Bogie::onExit()
{
	_eventDispatcher->removeEventListener(mListener);

	Sprite::onExit();
}
