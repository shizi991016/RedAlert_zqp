#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
//#include "extensions/GUI/CCcontrolExtension/CCControl.h"
//#include "extensions/GUI/CCcontrolExtension/CCControlButton.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
//using namespace cocos2d::extension;
class HelloWorld : public cocos2d::Scene
{
private:
    TMXTiledMap* _tileMap; // 地图
    Sprite* _player; // 玩家
    Sprite* camera;//相机
    int _screenWidth,_screenHeight; // 屏幕的宽度和高度
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
    //Menu* RightMenu;
    //MenuItemImage* RightMenuPicture;
    //Layer* RightMenuLayer;
    //MenuItemImage* closeItem;
    //Menu* menu;
    Button* botton;
    //Camera* _camera;
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setViewpointCenter(Vec2 position);
    
    //void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    //void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void update(float delta) override;
    
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);
    
    void keyPressedDuration(EventKeyboard::KeyCode code);
    
};

#endif // __HELLOWORLD_SCENE_H__
