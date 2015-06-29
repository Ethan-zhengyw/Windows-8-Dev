#include "cocos2d.h"
#include <string>
#include <vector>

USING_NS_CC;

enum FoodTag {
	weapon,
	bubble_correct,
	bubble_wrong,
	bomb,
	life
};

class ColorBubbleScene :public Layer
{
public:
	ColorBubbleScene();
	~ColorBubbleScene();

	//CREATE_FUNC(ColorBubbleScene);
	static ColorBubbleScene* create(PhysicsWorld* world);
	virtual bool init(PhysicsWorld* world);

	static cocos2d::Scene* createScene();
	void onRightPressed(Ref* sender);
	void onLeftPressed(Ref* sender);
	void onUpPressed(Ref* sender);
	void onDownPressed(Ref* sender);

	void _stdcall stopBGM();
	float distance(float x, float y);
	void playFireworks();

	void initMouseEvent();
	void onMouseDown(Event *event);
	void onMouseMove(Event *event);
	void onMouseUp(Event *event);
	void onMouseScroll(Event *event);

	void initKeyEvent();
	void processKey(EventKeyboard::KeyCode keyCode, Event* event);

	void initContactEvent();
	bool processContact(const PhysicsContact& contact);

	void prepareSprite();
	void drawBorder();
	void spitBubbles(int bubblesNum);
	void spitBubble();
	void addWeapon();

	void hurtDelay();

	void initTopHeart();
	void incTopHeart();
	void decTopHeart();

	void processHiter(Sprite*);  // 处理被武器撞击的对象
	void hitBubble(Sprite*, int);  // int型变量只是bubble包含的字
	void hitHeart(Sprite*);
	void hitRainbow(Sprite*);

	void updateScore(int);

	void menuCallback(cocos2d::Ref* pSender);
	void pause();
	void goon();

	void spitRainbow();
	void spitHeart();

	void setAllWeaponAttrs();

private:
	Size visibleSize;
	PhysicsWorld* world_;
	bool mouseDown;
	float startX, startY, endX, endY;

	int weaponColor;
	float speed;			// 武器移动的速度
	Vector<Sprite*> weapons;	// 有多种颜色的武器

	int heartNum;				// 心型小球
	int bubbleNum;				// 普通小球
	int rainbowNum;				// 彩虹小球
	Sprite* weapon;									// 当前武器            type - 0
	Vector<Sprite*> hearts;                         // type - 2
	Vector<Sprite*> bubbles;                        // type - 1   
	Vector<Sprite*> rainbows;                       // type - 3
	std::vector<Vec2> hearts_speed;
	std::vector<Vec2> bubbles_speed;
	std::vector<Vec2> rainbows_speed;
	Vec2 weapon_speed;

	Vector<Sprite*> lifes;

	Sprite* container;
	Sprite* handle;
	PhysicsJointFixed* joint;

	// PhysicsMaterial bf = PhysicsMaterial(0.1, 0.1, 0.1);
	PhysicsMaterial bf = PHYSICSBODY_MATERIAL_DEFAULT;

	int score;
	int life;
	float speedLimit; // 难度
	Label* levelLabel;
	Label* scoreLabel1;
	Label* scoreLabel2;

	bool canHurt;
	float heroTime;
	bool canMove;

	CCMenuItemImage* menuPCItem;  // pause / continue

	int growRate;  // 吐出更多气泡的可能
	int bubbleToSpitNum;
};
