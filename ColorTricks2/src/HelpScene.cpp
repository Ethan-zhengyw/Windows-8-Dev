#include "HelpScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* HelpScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelpScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "HelpScene" splash screen"
    auto background = Sprite::create("UI/helpSecne/helpScene.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);

	// 创建右上角的菜单：回到主页
	auto item1 = CCMenuItemImage::create("UI/action/pause.png",
		"UI/action/pause.png", this, menu_selector(HelpScene::menuCallback));
	item1->setScale(0.7, 0.7);
	item1->setTag(1);
	item1->setPosition(visibleSize.width - item1->getContentSize().width * 2.1, visibleSize.height - item1->getContentSize().height / 2 - 20);

	auto item2 = CCMenuItemImage::create("UI/action/home.png",
		"UI/action/home.png", this, menu_selector(HelpScene::menuCallback));
	item2->setScale(0.7, 0.7);
	item2->setTag(2);
	item2->setPosition(visibleSize.width - item1->getContentSize().width, visibleSize.height - item1->getContentSize().height / 2 - 20);

	auto menu = Menu::create(item2, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

    return true;
}

void HelpScene::menuCallback(cocos2d::Ref* pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int tag = (int)pMenuItem->getTag();

	switch (tag)
	{
	case 1:
		break;
	case 2:
		Director::getInstance()->replaceScene(StartScene::createScene());
		break;
	case 3: break;
	default: break;
	}
}