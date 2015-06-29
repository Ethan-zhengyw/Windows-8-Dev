#include "SettingScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SettingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	ud = UserDefault::getInstance();

    // add "SettingScene" splash screen"
    auto background = Sprite::create("UI/SettingSecne/settingScene.jpg");
	background->setScaleX(visibleSize.width / background->getContentSize().width);
	background->setScaleY(visibleSize.height / background->getContentSize().height);
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);

	// 创建右上角的菜单：回到主页
	auto item1 = CCMenuItemImage::create("UI/action/pause.png",
		"UI/action/pause.png", this, menu_selector(SettingScene::menuCallback));
	item1->setScale(0.7, 0.7);
	item1->setTag(1);
	item1->setPosition(visibleSize.width - item1->getContentSize().width * 2.1, visibleSize.height - item1->getContentSize().height / 2 - 20);

	auto item2 = CCMenuItemImage::create("UI/action/home.png",
		"UI/action/home.png", this, menu_selector(SettingScene::menuCallback));
	item2->setScale(0.7, 0.7);
	item2->setTag(2);
	item2->setPosition(visibleSize.width - item1->getContentSize().width, visibleSize.height - item1->getContentSize().height / 2 - 20);

	auto menu = Menu::create(item2, NULL);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	initTouchEvent();

    return true;
}

void SettingScene::initTouchEvent(){
	auto menu = Menu::create();
	menu->setPosition(300, 300);
	menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	int FONT_SIZE = 40;

	bool isEf = ud->getBoolForKey("isEf");

	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		label_bg = Label::createWithTTF("ON", "fonts/STHUPO.ttf", FONT_SIZE);
	else
		label_bg = Label::createWithTTF("OFF", "fonts/STHUPO.ttf", FONT_SIZE);

	if (isEf)
		label_ef = Label::createWithTTF("ON", "fonts/STHUPO.ttf", FONT_SIZE);
	else
		label_ef = Label::createWithTTF("OFF", "fonts/STHUPO.ttf", FONT_SIZE);

	label_bg->setTextColor(Color4B::GRAY);
	label_ef->setTextColor(Color4B::GRAY);

	auto tgBg = MenuItemLabel::create(label_bg, CC_CALLBACK_1(SettingScene::toggleBgMusic, this));
	auto tgEf = MenuItemLabel::create(label_ef, CC_CALLBACK_1(SettingScene::toggleEffect, this));

	// 1代表ON，0代表OFF
	if (SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		tgBg->setTag(1);
	else
		tgBg->setTag(0);

	if (isEf)
		tgEf->setTag(1);
	else
		tgEf->setTag(0);

	tgBg->setPosition(206, 140);
	tgEf->setPosition(210, 50);

	tgBg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	tgEf->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	menu->addChild(tgBg);
	menu->addChild(tgEf);

	// 难度设置
	label_diff = Label::createWithTTF("", "fonts/STHUPO.ttf", 50);
	setDiffLabel(UserDefault::getInstance()->getIntegerForKey("diff"));

	label_diff->setPosition(480, 280);
	addChild(label_diff);

	auto label_diff_inc = Label::createWithTTF("+", "fonts/STHUPO.ttf", FONT_SIZE);
	auto label_diff_dec = Label::createWithTTF("-", "fonts/STHUPO.ttf", FONT_SIZE);
	auto btn_diff_inc = MenuItemLabel::create(label_diff_inc, CC_CALLBACK_1(SettingScene::incDiff, this));
	auto btn_diff_dec = MenuItemLabel::create(label_diff_dec, CC_CALLBACK_1(SettingScene::decDiff, this));

	btn_diff_dec->setPosition(95, -45);
	btn_diff_inc->setPosition(285, -45);

	btn_diff_dec->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	btn_diff_inc->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	label_diff->setTextColor(Color4B::GRAY);
	label_diff_inc->setTextColor(Color4B::GRAY);
	label_diff_dec->setTextColor(Color4B::GRAY);

	menu->addChild(btn_diff_dec);
	menu->addChild(btn_diff_inc);

	this->addChild(menu);
}

void SettingScene::setDiffLabel(int diff)
{
	std::string diffStr;
	switch (UserDefault::getInstance()->getIntegerForKey("diff"))
	{
	case -1: diffStr = "easy"; break;
	case 0: diffStr = "middle"; break;
	case 1: diffStr = "hard"; break;
	default:
		break;
	}
	label_diff->setString(diffStr);
	label_diff->setTextColor(Color4B::GRAY);
}

void SettingScene::incDiff(Ref* sender)
{
	int diff = UserDefault::getInstance()->getIntegerForKey("diff") + 1;
	if (diff <= 1) {
		UserDefault::getInstance()->setIntegerForKey("diff", diff);
		setDiffLabel(diff);
	}
}

void SettingScene::decDiff(Ref* sender)
{
	int diff = UserDefault::getInstance()->getIntegerForKey("diff") - 1;
	if (diff >= -1) {
		UserDefault::getInstance()->setIntegerForKey("diff", diff);
		setDiffLabel(diff);
	}
}

void SettingScene::toggleBgMusic(Ref* sender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(sender);
	int tag = (int)pMenuItem->getTag();

	if (tag == 1) {
		// 关闭背景音乐
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		pMenuItem->setTag(0);
		label_bg->setString("OFF");
	} else {
		// 开启背景音乐
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bgm.mp3", true);
		pMenuItem->setTag(1);
		label_bg->setString("ON");
	}

	return;
}

void SettingScene::toggleEffect(Ref* sender)
{
	CCMenuItem* pMenuItem = (CCMenuItem *)(sender);
	int tag = (int)pMenuItem->getTag();

	if (tag == 1) {
		// 设置关闭音效
		pMenuItem->setTag(0);
		label_ef->setString("OFF");
		UserDefault::getInstance()->setBoolForKey("isEf", false);
	}
	else {
		// 设置打开音效
		pMenuItem->setTag(1);
		label_ef->setString("ON");
		UserDefault::getInstance()->setBoolForKey("isEf", true);
	}

	return;
}

void SettingScene::menuCallback(cocos2d::Ref* pSender)
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