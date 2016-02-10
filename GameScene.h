#pragma once

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
	void update(float dt);

private:
	int x, y;
	cocos2d::Sprite* snake;
	cocos2d::Sprite* food;
	std::vector<Sprite*> snakeParts;
	enum Direction {UP, DOWN, LEFT, RIGHT };
	Direction direction = RIGHT;
	cocos2d::SEL_SCHEDULE selectSched;
	void updateDirection(EventKeyboard::KeyCode keyCode, Event* event);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif // __GAME_SCENE_H__
