#include "StartScene.h"
#include "ColorBubbleScene.h"

USING_NS_CC;

Scene* StartScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = StartScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool StartScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add "StartScene" splash screen"
    auto background = Sprite::create("UI/homeBG.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);

	//创建菜单
	auto label1 = Label::createWithTTF("Play", "fonts/STHUPO.TTF", 50);
	label1->setTextColor(Color4B::GRAY);
	auto item1 = MenuItemLabel::create(label1, CC_CALLBACK_1(StartScene::menuCallback, this));
	item1->setTag(1);
	item1->setPosition(visibleSize.width / 2, visibleSize.height / 2 - label1->getContentSize().height * 2);

	auto label2 = Label::createWithTTF("Help", "fonts/STHUPO.TTF", 50);
	label2->setTextColor(Color4B::GRAY);
	auto item2 = MenuItemLabel::create(label2, CC_CALLBACK_1(StartScene::menuCallback, this));
	item2->setTag(2);
	item2->setPosition(visibleSize.width / 2, visibleSize.height / 2 - label1->getContentSize().height * 3);

	auto label3 = Label::createWithTTF("Settings", "fonts/STHUPO.TTF", 50);
	label3->setTextColor(Color4B::GRAY);
	auto item3 = MenuItemLabel::create(label3, CC_CALLBACK_1(StartScene::menuCallback, this));
	item3->setTag(3);
	item3->setPosition(visibleSize.width / 2, visibleSize.height / 2 - label1->getContentSize().height * 4);

	auto menu = Menu::create(item1, item2, item3, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

    return true;
}

void StartScene::menuCallback(cocos2d::Ref* pSender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
	int tag = (int)pMenuItem->getTag();

	switch (tag)
	{
	case 1:
		Director::getInstance()->replaceScene(ColorBubbleScene::createScene());
		break;
	case 2: break;
	case 3: break;
	default: break;
	}
}