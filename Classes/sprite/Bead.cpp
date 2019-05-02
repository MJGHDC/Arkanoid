#include "Bead.h"
#include "layer/PhysicsLayer.h"

Bead* Bead::create(SpriteFrame * pSpriteFrame)
{
	Bead* pSprite = new Bead(BEAD_TAG);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Bead* Bead::create(SpriteFrame* pSpriteFrame, int32_t tagNumber)
{
	Bead* pSprite = new Bead(tagNumber);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Bead::Bead(int32_t tagNumber)
	: mTagNumber(tagNumber)
{
	this->setTag(tagNumber);
}

void Bead::onEnter()
{
	Sprite::onEnter();

	auto* contactListener = EventListenerPhysicsContact::create();
	//onContactSeparate
	contactListener->onContactBegin = [=](PhysicsContact& contact) {

		auto* bodyA = contact.getShapeA()->getBody();
		auto* nodeA = bodyA->getNode();
		auto* nodeB = contact.getShapeB()->getBody()->getNode();

		if (nodeA && nodeB && nodeA->getTag() == BEAD_TAG)
		{
			if (nodeB->getTag() == -1)
			{
				if (nodeA->getPositionY() < 80)
				{
					log("The End");
				}
			}
			else 
			{
				auto* pBrick = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bead"));
				pBrick->setPosition(nodeB->getPosition());
				log("parent Tag %d", this->getTag());
				nodeB->getParent()->addChild(pBrick);
				nodeB->removeFromParentAndCleanup(true);
			}
		}		

		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	mListener = contactListener;
}

void Bead::onExit()
{
	_eventDispatcher->removeEventListener(mListener);
	Sprite::onExit();
}
