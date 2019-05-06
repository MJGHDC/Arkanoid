/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "ArkanoidScene.h"
#include "SimpleAudioEngine.h"
#include "layer/Stage1.h"

USING_NS_CC;

ArkanoidScene* ArkanoidScene::createScene()
{
	auto* scene = ArkanoidScene::createWithPhysics();
	if (false == scene->init())
	{
		assert(false, "리소스 로딩 실패");
		return nullptr;
	}

	// set gravity
	//scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -98.0f)); // 중력 가속도
	scene->getPhysicsWorld()->setGravity(Vec2(0.0f, -900.0f));

	// optional: set debug draw
	// scene->getPhysicsWorld()->setDebugDrawMask(0xffff);
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	auto* layer = Stage1::create(scene);
	layer->setTag(10000);
	scene->addChild(layer);

	return scene;
}

// on "init" you need to initialize your instance
bool ArkanoidScene::init()
{
	auto* pSpriteFrameCache = SpriteFrameCache::getInstance();

	// 게임에 사용되는 sprite frame 생성
	Rect rect1 = Rect(0, 0, 128, 32);
	auto* pSprite1 = SpriteFrame::create("hd/bogie.png", rect1);
	pSpriteFrameCache->addSpriteFrame(pSprite1, "bogie");

	Rect rect2 = Rect(0, 0, 64, 64);
	auto* pSprite2 = SpriteFrame::create("hd/ball.png", rect2);
	pSpriteFrameCache->addSpriteFrame(pSprite2, "bead");

	Rect rect3 = Rect(0, 0, 64, 24);
	auto* pSprite3 = SpriteFrame::create("hd/box.png", rect3);
	pSpriteFrameCache->addSpriteFrame(pSprite3, "brick");

	Rect rect4 = Rect(0, 0, 114, 114);
	auto* pSprite4 = SpriteFrame::create("hd/Icon.png", rect4);
	pSpriteFrameCache->addSpriteFrame(pSprite4, "item");
	
	// 테두리 생성
	const Size visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	const Vec2 centre = visibleOrigin + visibleSize / 2;

	auto* boundary = Node::create();
	auto* boundaryBody = PhysicsBody::createEdgeBox(visibleSize, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	boundaryBody->setContactTestBitmask(0xFFFFFFFF);
	boundary->setPhysicsBody(boundaryBody);
	boundary->setPosition(centre);
	this->addChild(boundary);

	return true;
}