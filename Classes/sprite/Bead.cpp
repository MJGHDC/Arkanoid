#include "Bead.h"
#include "Brick.h"
#include "sprite/properties/SpriteStatus.h"

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
	, mDestroyedBrickCount(0)
	, mbBrickCheckList{false}
	, mStatus(eBeadStatus::default)
{
	this->setTag(tagNumber);
}

void Bead::onEnter()
{
	Sprite::onEnter();

	//onContactSeparate
	mpContactListener = EventListenerPhysicsContact::create();
	mpContactListener->onContactSeparate = CC_CALLBACK_1(Bead::onContactSeparate, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpContactListener, this);
}

void Bead::onExit()
{
	_eventDispatcher->removeEventListener(mpContactListener);

	Sprite::onExit();
}

bool Bead::onContactSeparate(PhysicsContact & contact)
{
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
			if (nodeB->getTag() > BEAD_TAG)
			{
				mbBrickCheckList[nodeB->getTag() - 4] = true;

				nodeB->removeFromParentAndCleanup(true);
			}

		}
	}

	return true;
}

void Bead::Processing(Vector<Brick*> bricks)
{
	for (int32_t i = 0; i < 114; ++i)
	{
		if (mbBrickCheckList[i] == true)
		{
			mbBrickCheckList[i] = false;
			auto* pBrick = bricks.at(i);
			log("destroyBrickTag : %d", pBrick->getTag());
			mDestroyedBrickCount++;
		}
	}

	if (mDestroyedBrickCount == 114)
	{
		log("game clear");
	}

	//if (mDestroyedBrickTag != 0)
	//{
	//	auto* pBrick = bricks.at(mDestroyedBrickTag - 4);
	//	log("destroyBrickTag : %d", pBrick->getTag());
	//	log("destroyBrickPosition : x = %f , y = %f", pBrick->getPosition().x, pBrick->getPosition().y);
	//	mDestroyedBrickTag = 0;
	//	//mDestroyedBrickCount++;
	//	//log("%d", mDestroyedBrickCount);
	//}
}
