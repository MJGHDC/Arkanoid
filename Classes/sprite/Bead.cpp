#include "Bead.h"
#include "Brick.h"
#include "sprite/properties/SpriteStatus.h"

int32_t Bead::s_mDestroyedBrickCount = 0;

Bead* Bead::create(SpriteFrame* pSpriteFrame)
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
	, mbBrickCheckList{false}
	, mStatus(eBeadStatus::default)
	, mItemTime(0.0f)
	, mSpeed(600)
{
	this->setTag(tagNumber);
	log("msDestroyedBrickCount : %d", s_mDestroyedBrickCount);
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


	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == BEAD_TAG)
		{
			PhysicsBody* pPhysicsBodyA = nodeA->getPhysicsBody();
			pPhysicsBodyA->setVelocity(pPhysicsBodyA->getVelocity().getNormalized() * mSpeed);
		}
		if (nodeB->getTag() == BEAD_TAG)
		{
			PhysicsBody* pPhysicsBodyB = nodeB->getPhysicsBody();
			pPhysicsBodyB->setVelocity(pPhysicsBodyB->getVelocity().getNormalized() * mSpeed);
		}

		if (nodeB->getTag() == -1)
		{
			if (nodeA->getPositionY() < 80)
			{
				nodeA->removeFromParentAndCleanup(true);
			}
		}
		else if (nodeA->getTag() == BEAD_TAG)
		{
			if (nodeB->getTag() >= BRICK_TAG)
			{
				mbBrickCheckList[nodeB->getTag() - BRICK_TAG] = true;
				//log("%d", nodeB->getPhysicsBody()->getCollisionBitmask());
				//log("%d", nodeA->getPhysicsBody()->getCollisionBitmask());
				nodeB->removeFromParentAndCleanup(true);
			}
		}
	}

	return true;
}

Sprite* Bead::Processing(Vector<Brick*> bricks)
{
	for (int32_t i = 0; i < bricks.size(); ++i)
	{
		if (mbBrickCheckList[i] == true)
		{
			mbBrickCheckList[i] = false;
			auto* pBrick = bricks.at(i);
			//log("destroyBrickTag : %d", pBrick->getTag());
			s_mDestroyedBrickCount++;
			log("msDestroyedBrickCount : %d", s_mDestroyedBrickCount);
			if (s_mDestroyedBrickCount >= bricks.size())
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
	if (mStatus == eBeadStatus::default)
	{
		mStatus = eBeadStatus::power;
		this->getPhysicsBody()->setCollisionBitmask(0x02);
		mItemTime = 5.f;
	}
	else if(mStatus == eBeadStatus::power)
	{
		mItemTime = 5.f;
	}
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
