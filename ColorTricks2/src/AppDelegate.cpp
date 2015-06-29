#include "AppDelegate.h"
#include "ColorBubbleScene.h"
#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "PassScene.h"
#include "GameOverScene.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
	glview->setFrameSize(director->getVisibleSize().width, director->getVisibleSize().height);

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/bg.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/bg.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("music/StartSound.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("music/HitSound.wav");

	UserDefault::getInstance()->setIntegerForKey("life", 3);
	int diff = UserDefault::getInstance()->getIntegerForKey("diff");
	if (diff == 0)
		UserDefault::getInstance()->setIntegerForKey("diff", 0);  // -1 0 1 : 易 中 难

    // create a scene. it's an autorelease object
	// auto scene = ColorBubbleScene::createScene();
	auto scene = StartScene::createScene();
	//auto scene = PassScene::createScene();
	//auto scene = GameOverScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
