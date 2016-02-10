#include "GameScene.h"
#include "Definitions.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//set background
	auto backgroundSprite = Sprite::create("Background.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

	//set food
	food = Sprite::create("Food.png");
	srand(time(NULL));
	auto random = rand() % 370 + 30;
	auto random2 = rand() % 270 + 30;
	food->setPosition(Point(random, random2));
	auto foodBody = PhysicsBody::createCircle(food->getContentSize().width / 2);
	foodBody->setCollisionBitmask(FOOD_COLLISION_BITMASK);
	foodBody->setContactTestBitmask(true);
	food->setPhysicsBody(foodBody);

	//set snake particle
	snake = Sprite::create("snake.png");
	snake->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
	auto snakeBody = PhysicsBody::createCircle(snake->getContentSize().width / 2);
	snakeBody->setCollisionBitmask(SNAKE_COLLISION_BITMASK);
	snakeBody->setContactTestBitmask(true);
	snake->setPhysicsBody(snakeBody);
	snakeParts.push_back(snake);
	snakeParts.front()->setPosition(100, 100);

	selectSched = schedule_selector(GameScene::update);

	//create the sprites
	this->addChild(backgroundSprite, 0);
	this->addChild(food, 1);
	this->addChild(snakeParts.front(), 2);

	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = CC_CALLBACK_2(GameScene::updateDirection, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->schedule(selectSched, 0.1);
	
	return true;
}

void GameScene::update(float dt) {
	x = snakeParts.front()->getPositionX();
	y = snakeParts.front()->getPositionY();

	
	for (unsigned int i = snakeParts.size() - 1; i > 0; i--) {
		snakeParts[i]->setPosition(snakeParts[i - 1]->getPositionX(), snakeParts[i - 1]->getPositionY());
		snakeParts[i]->setTexture(snakeParts[i - 1]->getTexture());
	}
	switch (direction) {
	case LEFT:
		snakeParts.front()->setPosition(x - 15, y);
		break;
	case RIGHT:
		snakeParts.front()->setPosition(x + 15, y);
		break;
	case UP:
		snakeParts.front()->setPosition(x, y + 15);
		break;
	case DOWN:
		snakeParts.front()->setPosition(x, y - 15);
		break;
	}
	snakeParts.front()->setTexture(CCTextureCache::sharedTextureCache()->addImage("snake.png"));
	
	for (unsigned int i = 1; i < snakeParts.size(); i++) {
		if (snakeParts[i]->getPosition() == snakeParts.front()->getPosition()) {
			Director::getInstance()->end();
		}
	}

	if (snakeParts.front()->getPositionX() >= 450 || snakeParts.front()->getPositionX() <= 20 || snakeParts.front()->getPositionY() >= 320 || snakeParts.front()->getPositionY() <= 0) {
		Director::getInstance()->end();
	}

}

void GameScene::updateDirection(EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			if (direction != RIGHT)
			direction = LEFT;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			if (direction != LEFT)
			direction = RIGHT;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:
			if (direction != DOWN)
			direction = UP;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:
			if (direction != UP)
			direction = DOWN;
			break;
		}
};

bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact) {
	PhysicsBody *a = contact.getShapeA()->getBody();
	PhysicsBody *b = contact.getShapeB()->getBody();
	if ( (SNAKE_COLLISION_BITMASK == a->getCollisionBitmask() && FOOD_COLLISION_BITMASK == b->getCollisionBitmask()) ||
		(SNAKE_COLLISION_BITMASK == b->getCollisionBitmask() && FOOD_COLLISION_BITMASK == a->getCollisionBitmask()) ) {
		srand(time(NULL));
		auto random = rand() % 370 + 30;
		auto random2 = rand() % 270 + 30;
		auto moveTo = MoveTo::create(0, Vec2(random, random2));
		food->runAction(moveTo);

		int tempX = snakeParts.back()->getPositionX();
		int tempY = snakeParts.back()->getPositionY();
		Sprite* tempSprite = Sprite::create("snake.png");
		switch (direction) {
		case LEFT:
			tempSprite->setPosition(tempX + 10, tempY);
			break;
		case RIGHT:
			tempSprite->setPosition(tempX - 10, tempY);
			break;
		case UP:
			tempSprite->setPosition(tempX, tempY + 10);
			break;
		case DOWN:
			tempSprite->setPosition(tempX, tempY - 10);
			break;
		}

		this->addChild(tempSprite);
		snakeParts.push_back(tempSprite);
	}
	return true;
}