#ifndef GameScene_h
#define GameScene_h


#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;


class GameScene : public cocos2d::Scene
{
private:
    
    TMXTiledMap* TileMap; // 地图
    
    Sprite* Camera;//相机
    int  ScreenWidth, ScreenHeight; // 屏幕的宽度和高度
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
    
    Button* Textbutton;
    Sprite* RightMenuPicture;
    
    
    
    
public:
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setViewpointCenter(Vec2 position);
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameScene);
    
    void update(float delta) override;
    
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);
    
    void keyPressedDuration(EventKeyboard::KeyCode code);
    
    //void textButtonCallBack(cocos2d::Ref* pSender);
    
};

#endif /* GameScene_h */
