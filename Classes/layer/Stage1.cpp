#include "layer/Stage1.h"
#include "sprite/Bead.h"
#include "sprite/Bogie.h"
#include "sprite/Brick.h"
#include "sprite/properties/SpriteStatus.h"
#include "manager/SpriteCreateManager.h"

Stage1::Stage1(const Scene* const scene)
	: mParentScene(scene)
	, mDestroyedBrickCount(0)
{
}

Stage1* Stage1::create(const Scene* const scene)
{
	Stage1* pLayer = new Stage1(scene);
	if (pLayer && pLayer->init())
	{
		pLayer->autorelease();

		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);

	return nullptr;
}

bool Stage1::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	
	int8_t brickPlacement[8][16] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	constexpr int8_t widthCount = sizeof(brickPlacement) / sizeof(brickPlacement[0]);
	constexpr int8_t lengthCount = sizeof(brickPlacement) / widthCount;
	static_assert(widthCount == 8, "세로 벽돌은 8개이여야 함");
	static_assert(lengthCount == 16, "가로 벽돌은 16개이여야 함");
	
	SpriteCreateManager spriteCreateManager;

	mBricks = spriteCreateManager.CreateBrick(brickPlacement, widthCount, lengthCount);
	for (const auto& brick : mBricks)
	{
		this->addChild(brick);
	}
	int32_t brickTagCount = mBricks.size();

	mpBogie = spriteCreateManager.CreateBogie();
	this->addChild(mpBogie);

	mpBead = spriteCreateManager.CreateBead(brickTagCount);
	this->addChild(mpBead);

	//mpBead->getPhysicsBody()->setVelocity(Vec2(300, 600));

	// 위 두 친구는 joint로 묶도록 하자.

	schedule(schedule_selector(Stage1::tick));// , 1.0f);
	
	return true;
}

void Stage1::onEnter()
{
	Layer::onEnter();

	mpTouchListener = EventListenerTouchOneByOne::create();
	mpTouchListener->setSwallowTouches(true);
	mpTouchListener->onTouchBegan = CC_CALLBACK_2(Stage1::onTouchBegan, this);
	mpTouchListener->onTouchMoved = CC_CALLBACK_2(Stage1::onTouchMoved, this);
	mpTouchListener->onTouchEnded = CC_CALLBACK_2(Stage1::onTouchEnded, this);
	mpTouchListener->onTouchCancelled = CC_CALLBACK_2(Stage1::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(mpTouchListener, this);
}

void Stage1::onExit()
{
	Layer::onExit();

	_eventDispatcher->removeAllEventListeners();
}

void Stage1::tick(float deltaTime)
{
	//log("%f", deltaTime);
	//auto begin = std::chrono::high_resolution_clock::now();

	Sprite* item = mpBead->Processing(mBricks);
	if (item != nullptr)
	{
		this->addChild(item);
	}

	mpBead->Item(deltaTime);

	std::queue<eItem>& itemQueue = mpBogie->GetItemQueue();
	if (itemQueue.empty() == false)
	{
		eItem item = itemQueue.front();
		itemQueue.pop();

		if (eItem::powerBall == item)
		{
			mpBead->SetPowerBall();
		}
		else if (eItem::multiBall == item)
		{

		}
	}
	//auto end = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0;
	//log("std::vector : %f ms", elapsed);
}

bool Stage1::onTouchBegan(Touch* touch, Event* event)
{
	auto location = touch->getLocation();
	auto physicsShapeArray = mParentScene->getPhysicsWorld()->getShapes(location);

	PhysicsBody* body = nullptr;
	for (auto& shape : physicsShapeArray)
	{
		log("%d", shape->getBody()->getTag());
		if (shape->getBody()->getTag() == BEAD_TAG || shape->getBody()->getTag() == ITEM_TAG)
		{
			body = shape->getBody();
			break;
		}
	}

	if (body != nullptr)
	{
		Node* mouse = Node::create();
		mouse->setPhysicsBody(PhysicsBody::create(0, 0));
		mouse->getPhysicsBody()->setDynamic(false);
		mouse->setPosition(location);
		this->addChild(mouse);

		PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
		joint->setMaxForce(5000.0f * body->getMass());
		mParentScene->getPhysicsWorld()->addJoint(joint);
		mMouseJoint.insert(std::make_pair(touch->getID(), mouse));

		return true;
	}
	else // Bead Collision Test
	{
		//int32_t collisionBitmask = mpBead->getPhysicsBody()->getCollisionBitmask();
		//if (collisionBitmask == 0x01)
		//{
		//	mpBead->getPhysicsBody()->setCollisionBitmask(collisionBitmask << 1);
		//}
		//else
		//{
		//	mpBead->getPhysicsBody()->setCollisionBitmask(collisionBitmask >> 1);
		//}
	}

	return false;
}

void Stage1::onTouchMoved(Touch* touch, Event* event)
{
	auto it = mMouseJoint.find(touch->getID());

	if (it != mMouseJoint.end())
	{
		it->second->setPosition(touch->getLocation());
	}
}

void Stage1::onTouchEnded(Touch* touch, Event* event)
{
	auto it = mMouseJoint.find(touch->getID());

	if (it != mMouseJoint.end())
	{
		this->removeChild(it->second);
		mMouseJoint.erase(it);
	}
}

void Stage1::A()
{
	Vec2 vec = Vec2(200, 300);
	auto* pItem = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("item"));
	pItem->setTag(1);
	pItem->setPosition(vec);

	PhysicsBody* pPhysicsBody = nullptr;

	pPhysicsBody = PhysicsBody::createBox(pItem->getContentSize(), PhysicsMaterial(0.1f, 1.0f, 0.0f));

	pPhysicsBody->setTag(pItem->getTag());
	pItem->setPhysicsBody(pPhysicsBody);

	this->addChild(pItem);
}
