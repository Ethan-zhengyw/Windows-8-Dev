#include "StartScene.h"
#include "ColorBubbleScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

const float SIZE_WSAD = 100.0;
const int FONT_SIZE = 100;

char getCharFromInt(int n) {
	char tmp[10];
	itoa(n, tmp, 10);
	return tmp[0];
}

Scene* ColorBubbleScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();

	// scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = ColorBubbleScene::create(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

ColorBubbleScene* ColorBubbleScene::create(PhysicsWorld* world) {
	ColorBubbleScene* pRet = new ColorBubbleScene();
	if (pRet && pRet->init(world))
	{
		return pRet;
	}
	pRet = NULL;
	return NULL;
}

ColorBubbleScene::ColorBubbleScene()
{
}
ColorBubbleScene::~ColorBubbleScene()
{
}

bool ColorBubbleScene::init(PhysicsWorld* world)
{
	srand(time(NULL));
	if (!Layer::init())
	{
		return false;
	}


	world_ = world;
	visibleSize = Director::getInstance()->getVisibleSize();

	rainbowNum = 1;
	heartNum = 2;  
	bubbleNum = 8;
	speed = 5;
	weaponColor = 2;
	speedLimit = 100;
	heroTime = 2.0f;
	canMove = true;

	Sprite* background = Sprite::create("UI/bgWithPipe.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);

	levelLabel = Label::createWithTTF("LEVEL 1", "fonts/Marker Felt.ttf", 35);
	levelLabel->setPosition(200, visibleSize.height - 50);
	levelLabel->setColor(Color3B(11, 59, 73));
	addChild(levelLabel);

	scoreLabel1 = Label::createWithTTF("SCORE", "fonts/Marker Felt.ttf", 35);
	scoreLabel1->setPosition(800, visibleSize.height / 2 + 100);
	scoreLabel1->setColor(Color3B(11, 59, 73));
	addChild(scoreLabel1);

	scoreLabel2 = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 50);
	scoreLabel2->setPosition(820, visibleSize.height / 2 + 30);
	scoreLabel2->setColor(Color3B(11, 59, 73));
	addChild(scoreLabel2);

	prepareSprite();
	drawBorder();
	spitBubbles(bubbleNum);
	addWeapon();
	initTopHeart();

	/*SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("music/shoot.mp3");
	*/

	initMouseEvent();
	initKeyEvent();
	initContactEvent();

	// 创建右下角的菜单：暂停【继续】，回到主页
	auto item1 = CCMenuItemImage::create("UI/action/pause.png",
		"UI/action/pause.png", this, menu_selector(ColorBubbleScene::menuCallback));
	item1->setScale(0.7, 0.7);
	item1->setTag(1);
	item1->setPosition(visibleSize.width - item1->getContentSize().width * 2.1, item1->getContentSize().height / 2 + 20);

	auto item2 = CCMenuItemImage::create("UI/action/home.png",
		"UI/action/home.png", this, menu_selector(ColorBubbleScene::menuCallback));
	item2->setScale(0.7, 0.7);
	item2->setTag(2);
	item2->setPosition(visibleSize.width - item1->getContentSize().width, item1->getContentSize().height / 2 + 20);

	auto menu = Menu::create(item1, item2, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	menuPCItem = item1;


	return true;
}

void ColorBubbleScene::prepareSprite()
{
	// 载入64个bubble
	std::string path, base = "UI/bubble/";
	for (int i = 1; i <= 8; i++) {
		for (int j = 1; j <= 8; j++) {
			path = base + getCharFromInt(i) + '_' + getCharFromInt(j) + ".png";

			auto bubble = Sprite::create(path);
			float scale = 0.4 + random(-0.1, 0.3);
			bubble->setScale(scale);
			auto body = PhysicsBody::createCircle(bubble->getContentSize().width * scale / 2, bf);
			// body->setVelocity(Vec2(100, 100));
			body->setLinearDamping(0);
			body->setContactTestBitmask(0x0001);
			body->setVelocityLimit(speedLimit);
			bubble->setPhysicsBody(body);
			bubble->setPosition(500, 200);
			bubble->setTag(100 * i + 10 * j + 1);   // ij1
			bubbles.pushBack(bubble);
		}
	}

	// 载入8根不同颜色的管子
	base = "UI/tube/";
	for (int i = 1; i <= 8; i++) {
		path = base + getCharFromInt(i) + ".png";

		auto tube = Sprite::create(path);
		float w = tube->getContentSize().width;
		float h = tube->getContentSize().height;
		float border = 20.0f;
		Vec2 shapePoints[4] = {
			Vec2(-w / 2, -h / 2 + border / 2 + 10),
			Vec2(w / 2 - 10, h / 2 + border / 2 - 10),
			Vec2(w / 2, h / 2 - border / 2),
			Vec2(-w / 2 + 10, -h / 2 - border / 2 + 10)
		};
		auto body = PhysicsBody::createPolygon(shapePoints, 4, bf);
		// auto body = PhysicsBody::createEdgeBox(tube->getContentSize(), PhysicsMaterial(1, 1, 1), 5);
		// body->setVelocity(Vec2(1000, -1000));
		body->setLinearDamping(0);
		body->setContactTestBitmask(0x0001);
		body->setRotationEnable(false);
		body->setVelocityLimit(speedLimit);
		tube->setPhysicsBody(body);
		tube->setPosition(200, 200);
		tube->setScale(0.5);
		tube->setTag(10 * i);             // i0
		weapons.pushBack(tube);
	 }

	// 心型
	for (int i = 0; i < heartNum; i++) {
		auto heart = Sprite::create("UI/special bubbles/life.png");
		float scale = 0.4 + random(-0.1, 0.3);
		heart->setScale(scale);
		auto body = PhysicsBody::createCircle(heart->getContentSize().width * scale / 2, bf);
		// body->setVelocity(Vec2(100, 100));
		body->setLinearDamping(0);
		body->setContactTestBitmask(0x0001);
		body->setVelocityLimit(speedLimit);
		heart->setPhysicsBody(body);
		heart->setPosition(500, 200);
		heart->setTag(2);   // 2
		hearts.pushBack(heart);
		addChild(heart);
	}

	for (int i = 0; i < rainbowNum; i++) {
		auto rainbow = Sprite::create("UI/special bubbles/rainbow.png");
		float scale = 0.4 + random(-0.1, 0.3);
		rainbow->setScale(scale);
		auto body = PhysicsBody::createCircle(rainbow->getContentSize().width * scale / 2, bf);
		// body->setVelocity(Vec2(100, 100));
		body->setLinearDamping(0);
		body->setContactTestBitmask(0x0001);
		body->setVelocityLimit(speedLimit);
		rainbow->setPhysicsBody(body);
		rainbow->setPosition(500, 200);
		rainbow->setTag(3);   // 3
		rainbows.pushBack(rainbow);
		addChild(rainbow);
	}
}
void ColorBubbleScene::drawBorder()
{
	// 地面
	auto borderBottom = Node::create();
	borderBottom->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, 0), Vec2(visibleSize.width, 0)));
	borderBottom->getPhysicsBody()->setDynamic(false);
	addChild(borderBottom);

	// 天花板
	auto borderUp = Node::create();
	borderUp->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, visibleSize.height), Vec2(visibleSize.width, visibleSize.height)));
	borderUp->getPhysicsBody()->setDynamic(false);
	addChild(borderUp);

	// 左墙壁
	auto borderLeft = Node::create();
	borderLeft->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(0, 0), Vec2(0, visibleSize.height)));
	borderLeft->getPhysicsBody()->setDynamic(false);
	addChild(borderLeft);

	// 天花板
	auto borderRight = Node::create();
	borderRight->setPhysicsBody(PhysicsBody::createEdgeSegment(Vec2(visibleSize.width, 0), Vec2(visibleSize.width, visibleSize.height)));
	borderRight->getPhysicsBody()->setDynamic(false);
	addChild(borderRight);

	float scale = 0.7;
	container = Sprite::create("UI/border.png");
	container->setPhysicsBody(PhysicsBody::createEdgeBox(container->getContentSize(), PhysicsMaterial(1.0, 1.0, 0.5), 20.0));
	container->setScale(scale);
	container->setPosition(visibleSize.width / 2 - 100, visibleSize.height / 2);
	addChild(container);
}
void ColorBubbleScene::addWeapon()
{
	// 添加武器
	weapon = weapons.at(weaponColor - 1);
	addChild(weapon);

	// 执行3秒的闪烁动画
	//weapon->runAction(Blink::create(3, 3));
	weapon->setOpacity(256 / 5);

	// 启动倒计时，3秒后取消无敌状态
	canHurt = false;
	auto callSelectorAction = CallFunc::create( std::bind(&ColorBubbleScene::hurtDelay, this) );
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(heroTime),
		callSelectorAction
	));

}
void ColorBubbleScene::spitBubbles(int bubblesNum)
{
	for (int i = 0; i < bubblesNum; i++)
		spitBubble();
}
void ColorBubbleScene::spitBubble()
{
	int color = random(0, 7);
	int text = random(0, 7);

	int target = color * 8 + text;
	weaponColor = text + 1;
	if (bubbles.at(target)->getParent() == NULL) {
		addChild(bubbles.at(target));
		bubbles.at(target)->getPhysicsBody()->setVelocity(Vec2(random(-100, 100), random(-100, 100)));
		bubbles.at(target)->setPosition(Vec2(random(300, 400), random(100, 400)));
	}
}

void ColorBubbleScene::initContactEvent()
{
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(ColorBubbleScene::processContact, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
bool ColorBubbleScene::processContact(const PhysicsContact& contact)
{
	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	int typeA = tagA % 10;
	int typeB = tagB % 10;

	if (typeA == 0)
		processHiter(spriteA);
	else if (typeB == 0)
		processHiter(spriteA);

	return true;
}

void ColorBubbleScene::processHiter(Sprite* hiter) {
	int th = hiter->getTag();

	if (th == 2) {
		hitHeart(hiter);
	}
	if (th == 3) {
		hitRainbow(hiter);
	}
	if (th % 10 == 1) {
		int text = th / 10 % 10;
		hitBubble(hiter, text);
	}

}

void ColorBubbleScene::hitHeart(Sprite* hiter) {
	life++;

	incTopHeart();

	hiter->removeFromParent();
}

void ColorBubbleScene::hitRainbow(Sprite* hiter) {
	;
}

void ColorBubbleScene::hitBubble(Sprite* hiter, int text) {
	if (text == weaponColor) {
		hiter->removeFromParent();
		score += 10;
		updateScore(score);

		for (int i = 0; i < 64; i++) {
			if (bubbles.at(i)->getParent() != NULL)
				bubbles.at(i)->removeFromParent();
		}

		// 记忆试管原来的位置和旋转角度
		Vec2 pos = weapon->getPosition();
		float rot = weapon->getRotation();
		weapon->removeFromParent();
		spitBubbles(bubbleNum);
		weapon->setPosition(pos);
		weapon->setRotation(rot);
		addWeapon();
	} else {

		// 碰错了泡泡，接受惩罚
		if (life > 0 && canHurt) {
			life--;
			decTopHeart();
		} else {
			// 分数扣完，游戏结束

		}
	}
}

void ColorBubbleScene::updateScore(int newScore) {
	char tmp[5];
	itoa(score, tmp, 10);
	scoreLabel2->setString(tmp);
}

void ColorBubbleScene::initMouseEvent(){
	//add touch listener
	auto listener = EventListenerMouse::create();
	listener->onMouseDown = CC_CALLBACK_1(ColorBubbleScene::onMouseDown, this);
	listener->onMouseMove = CC_CALLBACK_1(ColorBubbleScene::onMouseMove, this);
	listener->onMouseUp = CC_CALLBACK_1(ColorBubbleScene::onMouseUp, this);
	listener->onMouseScroll = CC_CALLBACK_1(ColorBubbleScene::onMouseScroll, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void ColorBubbleScene::onMouseDown(Event *event)
{
	mouseDown = true;
	EventMouse *em = (EventMouse*)event;

	startX = em->getCursorX();
	startY = em->getCursorY();
}
void ColorBubbleScene::onMouseMove(Event *event)
{
	if (!canMove)
		return;
	EventMouse *em = (EventMouse*)event;
	float X = em->getCursorX();
	float Y = em->getCursorY();

	if (X < 50 || X > 700)
		return;
	if (visibleSize.height + Y < 70 || visibleSize.height + Y > 500)
		return;

	float speedX = X - weapon->getPosition().x;
	float speedY = visibleSize.height + Y - weapon->getPosition().y;
	weapon->getPhysicsBody()->setVelocity(Vec2(speedX * speed, speedY * speed));
	// weapon->setPosition(em->getCursorX(), visibleSize.height + em->getCursorY());

}
void ColorBubbleScene::onMouseUp(Event *event)
{
	EventMouse *em = (EventMouse*)event;
	mouseDown = false;
}
void ColorBubbleScene::onMouseScroll(Event *event) {
	EventMouse *em = (EventMouse*)event;
	weapon->setRotation(weapon->getRotation() + em->getScrollY() * 10);
}

void ColorBubbleScene::stopBGM()
{
	// SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void ColorBubbleScene::initKeyEvent()
{
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(ColorBubbleScene::processKey, this);
	listener->onKeyPressed = CC_CALLBACK_2(ColorBubbleScene::processKey, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}
void ColorBubbleScene::processKey(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		weapon->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity() * 0.2);
}

void ColorBubbleScene::playFireworks()
{
	auto fireworks = ParticleFireworks::create();
	fireworks->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(fireworks);
}

void ColorBubbleScene::initTopHeart()
{
	// 添加上面的代表生命的心型
	for (int i = 0; i < life; i++) {
		incTopHeart();
	}
}

void ColorBubbleScene::incTopHeart()
{
	auto tmp = Sprite::create("UI/special bubbles/heart.png");
	float scale = 0.02;
	float cScale = 0.7;
	tmp->setScale(scale, scale);
	float posX = container->getPosition().x + container->getContentSize().width / 2 * cScale - (tmp->getContentSize().width * scale + 10) * (lifes.size() + 1);
	float posY = container->getPosition().y + container->getContentSize().height / 2 * cScale + tmp->getContentSize().height * scale;

	tmp->setPosition(posX, posY);

	addChild(tmp);
	lifes.pushBack(tmp);
}

void ColorBubbleScene::decTopHeart()
{
	lifes.at(lifes.size() - 1)->removeFromParent();
	lifes.popBack();
}

void ColorBubbleScene::hurtDelay()
{
	// ((ColorBubbleScene*)Director::getInstance()->getRunningScene())->canHurt = true;
	canHurt = true;
	weapon->setOpacity(255);
}

void ColorBubbleScene::menuCallback(cocos2d::Ref* pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int tag = (int)pMenuItem->getTag();

	switch (tag)
	{
	case 1:
		if (!canMove)
			goon();
		else
			pause();
		break;
	case 2: 
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	default: break;
	}
}

void ColorBubbleScene::pause()
{
	// 暂停的时候把所有刚体速度设置为0，同时储存他们的速度用于继续时恢复
	hearts_speed.clear();
	bubbles_speed.clear();
	rainbows_speed.clear();
	
	for (int i = 0; i < hearts.size(); i++) {
		hearts_speed.push_back(hearts.at(i)->getPhysicsBody()->getVelocity());
		hearts.at(i)->getPhysicsBody()->setVelocityLimit(0);
		hearts.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles_speed.push_back(bubbles.at(i)->getPhysicsBody()->getVelocity());
		bubbles.at(i)->getPhysicsBody()->setVelocityLimit(0);
		bubbles.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}
	for (int i = 0; i < rainbows.size(); i++) {
		rainbows_speed.push_back(rainbows.at(i)->getPhysicsBody()->getVelocity());
		rainbows.at(i)->getPhysicsBody()->setVelocityLimit(0);
		rainbows.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}

	weapon_speed = weapon->getPhysicsBody()->getVelocity();
	weapon->getPhysicsBody()->setVelocityLimit(0);
	weapon->getPhysicsBody()->setVelocity(Vec2::ZERO);

	canMove = false;
}

void ColorBubbleScene::goon()
{

	for (int i = 0; i < hearts.size(); i++) {
		hearts.at(i)->getPhysicsBody()->setVelocityLimit(speedLimit);
		hearts.at(i)->getPhysicsBody()->setVelocity(hearts_speed[i]);
	}
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles.at(i)->getPhysicsBody()->setVelocityLimit(speedLimit);
		bubbles.at(i)->getPhysicsBody()->setVelocity(bubbles_speed[i]);
	}
	for (int i = 0; i < rainbows.size(); i++) {
		rainbows.at(i)->getPhysicsBody()->setVelocityLimit(speedLimit);
		rainbows.at(i)->getPhysicsBody()->setVelocity(rainbows_speed[i]);
	}

	hearts_speed.clear();
	bubbles_speed.clear();
	rainbows_speed.clear();

	weapon->getPhysicsBody()->setVelocityLimit(speedLimit);
	weapon->getPhysicsBody()->setVelocity(weapon_speed);
	canMove == true;
}