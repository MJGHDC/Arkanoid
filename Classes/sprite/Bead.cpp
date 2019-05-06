#include "Bead.h"
#include "Brick.h"
#include "sprite/properties/SpriteStatus.h"

Bead* Bead::create(SpriteFrame* pSpriteFrame)
{
	Bead* pSprite = new Bead(BEAD_TAG, 128);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Bead* Bead::create(SpriteFrame* pSpriteFrame, int32_t tagNumber, int32_t stageBrickCount)
{
	Bead* pSprite = new Bead(tagNumber, stageBrickCount);
	if (pSprite && pSprite->initWithSpriteFrame(pSpriteFrame))
	{
		pSprite->autorelease();

		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);

	return nullptr;
}

Bead::Bead(int32_t tagNumber, int32_t stageBrickCount)
	: mTagNumber(tagNumber)
	, mDestroyedBrickCount(0)
	, mStageBrickCount(stageBrickCount)
	, mbBrickCheckList{false}
	, mStatus(eBeadStatus::default)
	, mItemTime(0.0f)
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

bool Bead::onContactSeparate(PhysicsContact& contact)
{
	auto* nodeA = contact.getShapeA()->getBody()->getNode();
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
			if (nodeB->getTag() >= BRICK_TAG)
			{
				mbBrickCheckList[nodeB->getTag() - BRICK_TAG] = true;
				log("%d", nodeB->getPhysicsBody()->getCollisionBitmask());
				log("%d", nodeA->getPhysicsBody()->getCollisionBitmask());
				nodeB->removeFromParentAndCleanup(true);
			}
		}
	}

	return true;
}

Sprite* Bead::Processing(Vector<Brick*> bricks)
{
	for (int32_t i = 0; i < mStageBrickCount; ++i)
	{
		if (mbBrickCheckList[i] == true)
		{
			mbBrickCheckList[i] = false;
			auto* pBrick = bricks.at(i);
			log("destroyBrickTag : %d", pBrick->getTag());
			mDestroyedBrickCount++;
			
			if (mDestroyedBrickCount == mStageBrickCount)
			{
				log("game clear");
			}

			return pBrick->GetItem();
		}
	}

	return nullptr;
}

void Bead::SetPowerBall()
{
	//int32_t collisionBitmask = this->getPhysicsBody()->getCollisionBitmask();
	if (mStatus == eBeadStatus::default)
	{
		this->getPhysicsBody()->setCollisionBitmask(0x02);
		mItemTime = 3.f;
	}
	else if(mStatus == eBeadStatus::power)
	{
		mItemTime = 3.f;
	}

	//this->runAction(Sequence::create(DelayTime::create(5.0f), nullptr));
}

float Bead::Item(float deltaTime)
{
	if (mItemTime > 0)
	{
		mItemTime -= deltaTime;
	}
	else
	{
		mItemTime = 0.f;
		statusReset();
	}

	return mItemTime;
}

void Bead::statusReset()
{
	mStatus = eBeadStatus::default;
	this->getPhysicsBody()->setCollisionBitmask(0x03);
}
