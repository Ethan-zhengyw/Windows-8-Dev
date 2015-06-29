#include "StartScene.h"
#include "ColorBubbleScene.h"
#include "SimpleAudioEngine.h"
#include "PassScene.h"
#include "GameOverScene.h"

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
	this->removeAllChildren();
	weapons.clear();	
	hearts.clear();
	bubbles.clear();                       
	rainbows.clear();
	hearts_speed.clear();
	bubbles_speed.clear();
	rainbows_speed.clear();

	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/StartSound.mp3");

	srand(time(NULL));
	if (!Layer::init())
	{
		return false;
	}

	switch (UserDefault::getInstance()->getIntegerForKey("diff"))
	{
	case -1:
		speed = 4;
		bf = PHYSICSBODY_MATERIAL_DEFAULT;
		speedLimit = 300;
		growRate = 80;
		break;
	case 0:
		speed = 3;
		bf = PhysicsMaterial(1, 0.1, 0.1);
		speedLimit = 500;
		growRate = 70;
		break;
	case 1:
		speed = 2;
		bf = PhysicsMaterial(0.1, 0.1, 0.1);
		speedLimit = 700;
		growRate = 50;
		break;
	default:
		break;
	}

	world_ = world;
	visibleSize = Director::getInstance()->getVisibleSize();

	weapon = NULL;
	rainbowNum = 1;
	heartNum = 1;  
	bubbleNum = 8;
	score = 0;
	life = 3;
	UserDefault::getInstance()->setIntegerForKey("life", 3);
	weaponColor = random(1, 8);
	heroTime = 2.0f;
	canMove = true;
	canHurt = false;
	handle = NULL;

	Sprite* background = Sprite::create("UI/background.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);


	levelLabel = Label::createWithTTF("LEVEL 1", "fonts/STHUPO.ttf", 35);
	levelLabel->setPosition(200, visibleSize.height - 50);
	levelLabel->setColor(Color3B(11, 59, 73));
	addChild(levelLabel);

	scoreLabel1 = Label::createWithTTF("SCORE", "fonts/STHUPO.ttf", 35);
	scoreLabel1->setPosition(800, visibleSize.height / 2 + 100);
	scoreLabel1->setColor(Color3B(11, 59, 73));
	addChild(scoreLabel1);

	scoreLabel2 = Label::createWithTTF("0", "fonts/STHUPO.ttf", 70);
	scoreLabel2->setPosition(820, visibleSize.height / 2 + 30);
	scoreLabel2->setColor(Color3B(11, 59, 73));
	addChild(scoreLabel2);

	drawBorder();
	initTopHeart();
	prepareSprite();
	spitBubbles(bubbleNum);
	addWeapon();

	/*SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bgm.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("music/shoot.mp3");
	*/

	initMouseEvent();
	initKeyEvent();

	initContactEvent();
	// 坑爹的bug，第二次进入这个场景就不正常了
	// 把碰撞检测推迟试下
	// auto callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::initContactEvent, this));
	//this->runAction(CCSequence::createWithTwoActions(
	//	CCDelayTime::create(3.0f),
	//	callSelectorAction
	//	));
	// 靠!居然还是没用，见鬼了

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
			Vec2(-w / 2, -h / 2 + border / 2 + 10),		// 左上
			Vec2(w / 2 - 10 - w / 2, h / 2 + border / 2 - 10 - w / 2),  // 右上
			Vec2(w / 2 - w / 2, h / 2 - border / 2 - w / 2),				// 右下
			Vec2(-w / 2 + 10, -h / 2 - border / 2 + 10)	// 左下
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

	// spitHeart();  // 心型
	// spitRainbow();  // 彩虹
	auto callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::spitHeart, this));
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(random(10, 50)),
		callSelectorAction
	));
	callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::spitRainbow, this));
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(random(10, 50)),
		callSelectorAction
	));
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

	Sprite* pipe = Sprite::create("UI/pipe.png");
	pipe->setPhysicsBody(PhysicsBody::createEdgeBox(pipe->getContentSize(), PhysicsMaterial(1.0, 1.0, 0.5), 10.0));
	pipe->setPosition(container->getPosition().x - container->getContentSize().width / 2 * scale,
		container->getPosition().y + container->getContentSize().height / 2 * scale);
	addChild(pipe);
}

void ColorBubbleScene::setAllWeaponAttrs()
{
	// 设置所有武器的位置和旋转角度以及速度
	for (int i = 0; i < weapons.size(); i++) {
		if (weapons.at(i) == weapon)
			continue;
		weapons.at(i)->setPosition(weapon->getPosition());
		weapons.at(i)->setRotation(weapon->getRotation());
		weapons.at(i)->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity());
	}
}

void ColorBubbleScene::addWeapon()
{
	if (weapon)
		setAllWeaponAttrs();
	// 添加武器
	weapon = weapons.at(weaponColor - 1);
	addChild(weapon);

	// 执行3秒的闪烁动画
	//weapon->runAction(Blink::create(3, 3));
	weapon->setOpacity(256 / 4);

	// 启动倒计时，3秒后取消无敌状态
	canHurt = false;
	auto callSelectorAction = CallFunc::create( std::bind(&ColorBubbleScene::hurtDelay, this) );
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(heroTime),
		callSelectorAction
	));

	if (handle == NULL) {
		float scale = 0.25;
		handle = Sprite::create("UI/special bubbles/rainbow.png");
		handle->setScale(scale);
		handle->setOpacity(0.0f);
		handle->setPosition(weapon->getPosition().x + handle->getContentSize().width, weapon->getPosition().y + handle->getContentSize().height);
		float w = handle->getPosition().x - 300;
		float h = handle->getPosition().y - 300;
		Vec2 shapePoints[4] = {
			Vec2(w, h + 10),		// 左上
			Vec2(w + weapon->getContentSize().width / 2 * 0.5 + 10, h + 10),  // 右上
			Vec2(w + weapon->getContentSize().width / 2 * 0.5 + 10, h - 5),				// 右下
			Vec2(w, h - 5)	// 左下
		};
		auto body = PhysicsBody::createPolygon(shapePoints, 4, bf);
		body->setContactTestBitmask(0x0001);
		body->setVelocityLimit(speedLimit);
		body->setLinearDamping(0);
		body->setRotationEnable(false);

		handle->setPhysicsBody(body);
		addChild(handle);
		handle->setRotation(weapon->getRotation());
	}
	
}
void ColorBubbleScene::spitBubbles(int bubblesNum)
{
	speedLimit = 800;
	bubbleToSpitNum = bubblesNum;
	// 新增吐出多个气泡的时间间隔
	spitBubble();
}
void ColorBubbleScene::spitBubble()
{
	int text = random(0, 7);
	int color, target;

	do {
		color = random(0, 7);
		target = color * 8 + text;
	} while (bubbles.at(target)->getParent() != NULL);

	for (int i = 0; i < 64; i++) {
		if (bubbles.at(i)->getParent() != NULL) {
			weaponColor = bubbles.at(i)->getTag() % 100 / 10;
			break;
		}
	}

	if (bubbles.at(target)->getParent() == NULL) {
		if (UserDefault::getInstance()->getBoolForKey("isEf"))
			SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");

		addChild(bubbles.at(target));
		bubbles.at(target)->getPhysicsBody()->setVelocity(Vec2(random(10, int(speedLimit)), random(int(-speedLimit), 0)));
		bubbles.at(target)->setPosition(container->getPosition().x - container->getContentSize().width / 2 * 0.7 + 120,
			container->getPosition().y + container->getContentSize().height / 2 * 0.7 - 120);
	}

	bubbleToSpitNum--;
	if (bubbleToSpitNum > 0) {
		auto callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::spitBubble, this));
		this->runAction(CCSequence::createWithTwoActions(
			CCDelayTime::create(0.5f),
			callSelectorAction
		));
	} else {
		speedLimit = 300;
		if (weapon != NULL && weapon->getParent() != NULL) {
			weapon->removeFromParent();
			addWeapon();
		}
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
	if (!canHurt)
		return true;

	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	if (!spriteA || !spriteB)
		return false;

	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();

	int typeA = tagA % 10;
	int typeB = tagB % 10;

	if (handle != NULL && weapon != NULL) {
		handle->setPosition(weapon->getPosition().x + handle->getContentSize().width, weapon->getPosition().y + handle->getContentSize().height);
		handle->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity());
		handle->setRotation(weapon->getRotation());
	}

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

	if (handle != NULL && weapon != NULL) {
		handle->setPosition(weapon->getPosition().x + handle->getContentSize().width, weapon->getPosition().y + handle->getContentSize().height);
		handle->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity());
		handle->setRotation(weapon->getRotation());
	}

}

void ColorBubbleScene::hitHeart(Sprite* hiter) {
	life++;

	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");
	incTopHeart();

	hiter->removeFromParent();

	updateScore(score);
	auto callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::spitHeart, this));
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(random(25, 50)),
		callSelectorAction
	));

	if (handle != NULL && weapon != NULL) {
		handle->setPosition(weapon->getPosition().x + handle->getContentSize().width, weapon->getPosition().y + handle->getContentSize().height);
		handle->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity());
		handle->setRotation(weapon->getRotation());
	}

}

void ColorBubbleScene::hitRainbow(Sprite* hiter) {
	if (rainbows.size() <= 0)
		return;

	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");

	rainbows.at(rainbows.size() - 1)->removeFromParent();
	rainbows.popBack();

	score += 20;
	updateScore(score);
	auto callSelectorAction = CallFunc::create(std::bind(&ColorBubbleScene::spitRainbow, this));
	this->runAction(CCSequence::createWithTwoActions(
		CCDelayTime::create(random(10, 50)),
		callSelectorAction
	));
}

void ColorBubbleScene::spitHeart()
{
	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");

	for (int i = 0; i < heartNum; i++) {
		auto heart = Sprite::create("UI/special bubbles/life.png");
		float scale = 0.4 + random(-0.1, 0.3);
		heart->setScale(scale);
		auto body = PhysicsBody::createCircle(heart->getContentSize().width * scale / 2, bf);
		// body->setVelocity(Vec2(100, 100));
		body->setLinearDamping(0);
		body->setContactTestBitmask(0x0001);
		body->setVelocityLimit(speedLimit);
		body->setVelocity(Vec2(1000, -1000));
		heart->setPhysicsBody(body);
		heart->setPosition(container->getPosition().x - container->getContentSize().width / 2 * 0.7 + 120,
			container->getPosition().y + container->getContentSize().height / 2 * 0.7 - 120);
		heart->setTag(2);   // 2
		hearts.pushBack(heart);
		addChild(heart);
	}
}

void ColorBubbleScene::spitRainbow()
{
	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");

	for (int i = 0; i < rainbowNum; i++) {
		auto rainbow = Sprite::create("UI/special bubbles/rainbow.png");
		float scale = 0.4 + random(-0.1, 0.3);
		rainbow->setScale(scale);
		auto body = PhysicsBody::createCircle(rainbow->getContentSize().width * scale / 2, bf);
		body->setLinearDamping(0);
		body->setContactTestBitmask(0x0001);
		body->setVelocityLimit(speedLimit);
		body->setVelocity(Vec2(1000, -1000));
		rainbow->setPhysicsBody(body);
		rainbow->setPosition(container->getPosition().x - container->getContentSize().width / 2 * 0.7 + 120,
			container->getPosition().y + container->getContentSize().height / 2 * 0.7 - 120);
		rainbow->setTag(3);   // 3
		rainbows.pushBack(rainbow);
		addChild(rainbow);
	}
}

void ColorBubbleScene::hitBubble(Sprite* hiter, int text) {
	if (!canHurt)
		return;

	if (text == weaponColor) {
		hiter->removeFromParent();
		score += 10;
		updateScore(score);

		//for (int i = 0; i < 64; i++) {
		// 	if (bubbles.at(i)->getParent() != NULL)
		//		bubbles.at(i)->removeFromParent();
		//}

		weapon->removeFromParent();
		if (random(1, 100) > growRate)
			spitBubbles(2);
		else
			// 吐出两个的可能性较小，设置这个的目的是增加小球个数，逐渐提高游戏难度
			spitBubbles(1);  
		addWeapon();

	}
	else {

		// 碰错了泡泡，接受惩罚
		if (life > 0 && canHurt) {
			life--;
			decTopHeart();
		}
		if (life <= 0) {
			// 分数扣完，游戏结束
			int highestScore = UserDefault::getInstance()->getIntegerForKey("highestScore");

			if (score > highestScore)
				UserDefault::getInstance()->setIntegerForKey("highestScore", score);

			UserDefault::getInstance()->setIntegerForKey("score", score);

			if (score > highestScore) {
				Director::getInstance()->replaceScene(PassScene::createScene());  
				init(world_);  // 我勒个去，，为什么走之前还要init才可以保证下次进来的时候和第一次进来的时候一样！！
			}
			else {
				Director::getInstance()->replaceScene(GameOverScene::createScene());
				init(world_);  
			}
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

	if (handle != NULL && weapon != NULL) {
		handle->setPosition(weapon->getPosition().x, weapon->getPosition().y);
		handle->getPhysicsBody()->setVelocity(Vec2(speedX * speed, speedY * speed));
		handle->setRotation(weapon->getRotation() - 45);
	}
}
void ColorBubbleScene::onMouseUp(Event *event)
{
	EventMouse *em = (EventMouse*)event;
	mouseDown = false;
}
void ColorBubbleScene::onMouseScroll(Event *event) {
	EventMouse *em = (EventMouse*)event;
	weapon->setRotation(weapon->getRotation() + em->getScrollY() * 10);
	if (handle != NULL && weapon != NULL) {
		handle->setPosition(weapon->getPosition().x, weapon->getPosition().y);
		handle->getPhysicsBody()->setVelocity(weapon->getPhysicsBody()->getVelocity());
		handle->setRotation(weapon->getRotation() - 45);
	}
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
	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.wav");
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
		hearts.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}
	for (int i = 0; i < bubbles.size(); i++) {
		bubbles_speed.push_back(bubbles.at(i)->getPhysicsBody()->getVelocity());
		bubbles.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}
	for (int i = 0; i < rainbows.size(); i++) {
		rainbows_speed.push_back(rainbows.at(i)->getPhysicsBody()->getVelocity());
		rainbows.at(i)->getPhysicsBody()->setVelocity(Vec2::ZERO);
	}

	weapon_speed = weapon->getPhysicsBody()->getVelocity();
	weapon->getPhysicsBody()->setVelocity(Vec2::ZERO);

	canMove = false;
}

void ColorBubbleScene::goon()
{

	for (int i = 0; i < hearts.size() && i < hearts_speed.size(); i++) {
		hearts.at(i)->getPhysicsBody()->setVelocity(hearts_speed[i]);
	}
	for (int i = 0; i < bubbles.size() && i < bubbles_speed.size(); i++) {
		bubbles.at(i)->getPhysicsBody()->setVelocity(bubbles_speed[i]);
	}
	for (int i = 0; i < rainbows.size() && i < rainbows_speed.size(); i++) {
		rainbows.at(i)->getPhysicsBody()->setVelocity(rainbows_speed[i]);
	}

	weapon->getPhysicsBody()->setVelocity(weapon_speed);

	canMove = true;
}