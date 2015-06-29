#include "GameOverScene.h"
#include "HelpScene.h"
#include "StartScene.h"
#include "SettingScene.h"
#include "ColorBubbleScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    // add "GameOverScene" splash screen"
    auto background = Sprite::create("UI/GameOver/gameOver.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width * 1.1);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2 * 1.1, visibleSize.height / 2);
	addChild(background);

	auto levelLabel = Label::createWithTTF("1", "fonts/STHUPO.ttf", 50);
	levelLabel->setPosition(450, 340);
	levelLabel->setColor(Color3B(11, 59, 73));
	addChild(levelLabel);

	char score_str[10];
	itoa(UserDefault::getInstance()->getIntegerForKey("score"), score_str, 10);

	auto scoreLabel1 = Label::createWithTTF(score_str, "fonts/STHUPO.ttf", 50);
	scoreLabel1->setPosition(450, 280);
	scoreLabel1->setColor(Color3B(11, 59, 73));
	addChild(scoreLabel1);

	// 创建右下角的菜单：暂停【继续】，回到主页
	auto item1 = CCMenuItemImage::create("UI/GameOver/tryAgain.png",
		"UI/action/tryAgain.png", this, menu_selector(GameOverScene::menuCallback));
	item1->setScale(0.7, 0.7);
	item1->setTag(1);
	item1->setPosition(visibleSize.width / 2 + 30 - item1->getContentSize().width * 2.1, item1->getContentSize().height / 2 + 130);

	auto item2 = CCMenuItemImage::create("UI/GameOver/home.png",
		"UI/action/home.png", this, menu_selector(GameOverScene::menuCallback));
	item2->setScale(0.7, 0.7);
	item2->setTag(2);
	item2->setPosition(visibleSize.width / 2 + 30 - item1->getContentSize().width, item1->getContentSize().height / 2 + 130);

	auto menu = Menu::create(item1, item2, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/GameOverSound.mp3");

    return true;
}

void GameOverScene::menuCallback(cocos2d::Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey("isEf"))
		SimpleAudioEngine::getInstance()->playEffect("music/HitSound.mp3");
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int tag = (int)pMenuItem->getTag();

	switch (tag)
	{
	case 1:
		Director::getInstance()->replaceScene(ColorBubbleScene::createScene());
		break;
	case 2:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	default: break;
	}
}