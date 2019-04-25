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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

#include "cocos2d.h"
#include "sprite/Bead.h"
#include "sprite/Bogie.h"
#include "sprite/Brick.h"
//#include "Box2D/Box2D.h"
//#include <GLES-Render.h>

using namespace cocos2d;

class HelloWorld final : public cocos2d::Scene
{
	enum ePhysicsBodyType
	{
		circle,
		box,
		polygon
	};
	static Scene* scene;

public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	PhysicsBody* SpriteSetPhysicsBody(Sprite* pSprite, const float scale, const Rect& rect, const ePhysicsBodyType type, const PhysicsMaterial& material = PHYSICSBODY_MATERIAL_DEFAULT, const Vec2& offset = Vec2::ZERO);

	void onEnter() override;
	void onExit() override;
	void tick(float deltaTime);

private:

	Size mWinSize;
	Texture2D* mTexture;
	SpriteFrameCache* spriteFrameCache;

	Bogie* pBogie;
	Bead* pBead;
	//Brick* pBrick;
};

#endif // __HELLOWORLD_SCENE_H__
