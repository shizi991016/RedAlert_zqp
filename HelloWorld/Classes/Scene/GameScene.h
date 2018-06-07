#ifndef GameScene_h
#define GameScene_h

#include "../Army/BaseAndFlags.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "../Army/ArmyClass.cpp"

USING_NS_CC;
using namespace ui;


class GameScene : public cocos2d::Scene
{
private:
    
    TMXTiledMap* TileMap; // 地图
    
    Sprite* Camera;//相机
    int  ScreenWidth, ScreenHeight; // 屏幕的宽度和高度
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
    Size VisibleSize;
    
    Button* Textbutton;
    Sprite* RightMenuPicture;
    
    Button* CommonBarracksButton;
    Button* CommonCannonButton;
    Button* CommonElectricPowerPlantButton;
    Button* CommonRefineryButton;
    Button* CommonSoldierButton;
    Button* CommonTankButton;
    Button* CommonWarFactoryButton;
    
    Sprite* CommonBarracksPicture;
    Sprite* CommonCannonPicture;
    Sprite* CommonElectricPowerPlantPicture;
    Sprite* CommonRefineryPicture;
    Sprite* CommonSoldierPicture;
    Sprite* CommonTankPicture;
    Sprite* CommonWarFactoryPicture;

    Button* OneBattlePlaneButton;
    Button* OneJetButton;
    Button* TwoRTankButton;
    Button* TwoTTankButton;
    Button* ThreeSoldierXButton;
    Button* ThreeMissileWellButton;
    
    Sprite* OneBattlePlanePicture;
    Sprite* OneJetPicture;
    Sprite* TwoRTankPicture;
    Sprite* TwoTTankPicture;
    Sprite* ThreeSoldierXPicture;
    Sprite* ThreeMissileWellPicture;
    
    BaseAndFlags MyData;
    
    Text* RightMenuText;
    Text* PromptText;
    Text* MoneyText;
    
    int TimeCount = 0;
    
    
    
public:
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void setViewpointCenter(Vec2 position);
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(GameScene);
    
    void update(float delta) override;
    
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);
    
    void keyPressedDuration(EventKeyboard::KeyCode code);
    
    void countryChoiceSwitch(int CountryChoice);
    
    void commonGamePictureLoading();
    
    void armyMoveOnce(Sprite* ArmyName);
    void armyMoveOnce(Sprite* ArmyName,Action* ArmyAction);
    //我曾经想把所有的东西都由这一个函数加进去，但是函数参数传递中Action的Target会出现错误，所以我放弃了这个方法
    //迫不得已只能多次重写armyMoveOnce函数来支持不同的建筑物动画
    //以及多次重写loadingAction加载建筑物动画
    void electricPowerPlantMoveOnce(Sprite* ArmyName);
    void barracksMoveOnce(Sprite* ArmyName);
    void refineryMoveOnce(Sprite* ArmyName);
    void warFactoryMoveOnce(Sprite* ArmyName);
    void missileWellMoveOnce(Sprite* ArmyName);
    
    
    void armyBuildCallBack(Action* BuildingAction,const std::string& FileName);
    void electricPowerPlantBuildCallBack();
    void refineryBuildCallBack();
    void barracksBuildCallBack();
    void warFactoryBuildCallBack();
    
    
    
    
    Action* loadingElectricPowerPlantAction();
    Action* loadingBarracksAction();
    //Action* loadingCannonAction();加农炮另写
    Action* loadingRefinerytAction();
    //Action* loadingSoldierAction();
    //Action* loadingTankAction();
    Action* loadingWarFactoryAction();
    //Action* loadingBattlePlaneAction();
    //Action* loadingJetAction();
    //Action* loadingRTankAction();
   // Action* loadingTTankAction();
    //Action* loadingSoldierXAction();
    Action* loadingMissileWellAction();
    
    void rightMenuMoveBy(MoveBy* SpriteMoveBy);
  
    
    
    
    
    
    
    
    
    
    
};

#endif /* GameScene_h */
