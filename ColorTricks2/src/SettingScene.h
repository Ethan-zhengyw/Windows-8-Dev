#ifndef __SETTING_SCENE_H__
#define __SETTING_SCENE_H__

#include "cocos2d.h"

class SettingScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SettingScene);

	void menuCallback(cocos2d::Ref* pSender);
	void initTouchEvent();
	void toggleBgMusic(Ref* sender);
	void toggleEffect(Ref* sender);
	void incDiff(Ref* sender);
	void decDiff(Ref* sender);
	void setDiffLabel(int diff);

private:
    cocos2d::Label* label_bg;
	cocos2d::Label* label_ef;

	cocos2d::Label* label_diff;


	cocos2d::UserDefault* ud;
};

#endif // __HELP_SCENE_H__
