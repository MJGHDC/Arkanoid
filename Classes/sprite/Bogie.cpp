#include "Bogie.h"
#include "Brick.h"

Bogie* Bogie::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Bogie* pSprite = new Bogie(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->setName("Bogie");
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
	
	mpContactListener = EventListenerPhysicsContact::create();
	mpContactListener->onContactBegin = CC_CALLBACK_1(Bogie::onContactSeparate, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpTouchListener, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpContactListener, this);
	//_eventDispatcher->addEventListenerWithFixedPriority(listener, _fixedPriority);

}

void Bogie::onExit()
{
	_eventDispatcher->removeEventListener(mpTouchListener);
	_eventDispatcher->removeEventListener(mpContactListener);

	Sprite::onExit();
}

bool Bogie::onContactSeparate(PhysicsContact & contact)
{
	auto* nodeA = contact.getShapeA()->getBody()->getNode();
	auto* nodeB = contact.getShapeB()->getBody()->getNode(); 

	log("%d, %d", nodeA->getTag(), nodeB->getTag());
	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == ITEM_TAG && nodeB->getTag() == BOGIE_TAG)
		{
			nodeA->removeFromParentAndCleanup(true);
		}
		else if (nodeA->getTag() == BOGIE_TAG && nodeB->getTag() == ITEM_TAG)
		{
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeA->getTag() == -1 && nodeB->getTag() == ITEM_TAG)
		{
			log("%f", nodeB->getPositionY());
			if (nodeB->getPositionY() < 120)
			{
				nodeB->removeFromParentAndCleanup(true);
			}
		}
	}

	return true;
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
