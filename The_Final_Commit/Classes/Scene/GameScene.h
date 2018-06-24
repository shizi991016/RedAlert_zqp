#ifndef GameScene_h
#define GameScene_h

#include "../Army/BaseAndFlags.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
//#include "../NetWork/CHAT_CLIENT.cpp"

USING_NS_CC;
using namespace ui;

extern std::vector<std::string> MyOrderList;
extern std::vector<std::string> EnemyOrderList;
extern char IsPositionHaveBuildings[1601][1601];
extern char IsPositionHaveMiningYard[1601][1601];
extern int PositionTag[1601][1601];
extern int IsPositionHaveArmyAndTag[1601][1601];


class GameScene : public cocos2d::Scene , public cocos2d::ui::EditBoxDelegate
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
    Text* ChatRoom;
    
    EditBox* IpEditBox;
    
    int TimeCount = 0;
    
    //NetNode* MyNetNode;
    
    //chat_client* Client;
    
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
    
    //void armyMoveOnce(Sprite* ArmyName);
    //void armyMoveOnce(Sprite* ArmyName,Action* ArmyAction);
    //我曾经想把所有的东西都由这一个函数加进去，但是函数参数传递中Action的Target会出现错误，所以我放弃了这个方法
    //迫不得已只能多次重写armyMoveOnce函数来支持不同的建筑物动画
    //以及多次重写loadingAction加载建筑物动画
    
    
    
    //void armyBuildCallBack(Action* BuildingAction,const std::string& FileName);
    
    
    void electricPowerPlantMoveOnce(Sprite* ArmyName);
    void electricPowerPlantBuildCallBack();
    void emenyElectricPowerPlantBuildCallBack(int Tag);
    Action* loadingElectricPowerPlantAction();
    
    
    void refineryMoveOnce(Sprite* ArmyName);
    void refineryBuildCallBack();
    void emenyRefineryBuildCallBack(int Tag);
    Action* loadingRefinerytAction();
    
    void barracksMoveOnce(Sprite* ArmyName);
    void barracksBuildCallBack();
    void emenyBarracksBuildCallBack(int Tag);
    Action* loadingBarracksAction();
    
    void warFactoryMoveOnce(Sprite* ArmyName);
    void warFactoryBuildCallBack();
    void emenyWarFactoryBuildCallBack(int Tag);
    Action* loadingWarFactoryAction();
    
    void missileWellMoveOnce(Sprite* ArmyName);
    void missileWellBuildCallBack();
    void emenyMissileWellBuildCallBack(int Tag);
    Action* loadingMissileWellAction();
    
    void cannonMoveOnce(Sprite* ArmyName);
    void cannonBuildCallBcak();
    void emenyCannonBuildCallBack(int Tag);
    Action* loadingCannonAction();
    
    void baseBuildBallBack();
    Action* loadingBaseAction();
    void emenyBaseBuildCallBack(int Tag);
    
    void soldierBuildCallBack();
    Action* SoldierMoveUp(int ActionTime);
    Action* SoldierMoveDown(int ActionTime);
    Action* SoldierMoveLeft(int ActionTime);
    Action* SoldierMoveRight(int ActionTime);
    void emenySoldierBuildCallBack(int Tag);
    
    void tankBuildCallBack();
    Action* TankMoveUp(int ActionTime);
    Action* TankMoveLeft(int ActionTime);
    Action* TankMoveRight(int ActionTime);
    Action* TankMoveDown(int ActionTime);
    void emenyTankBuildCallBack(int Tag);
    
    void rTankBuildCallBack();
    Action* RTankMoveUp(int ActionTime);
    Action* RTankMoveLeft(int ActionTime);
    Action* RTankMoveRight(int ActionTime);
    Action* RTankMoveDown(int ActionTime);
    void emenyRTankBuildCallBack(int Tag);
    
    void tTankBuildCallBack();
    Action* TTankMoveUp(int ActionTime);
    Action* TTankMoveLeft(int ActionTime);
    Action* TTankMoveRight(int ActionTime);
    Action* TTankMoveDown(int ActionTime);
    void emenyTTankBuildCallBack(int Tag);
    
    void battlePlaneBuildCallBack();
    Action* BattlePlaneMoveUp(int ActionTime);
    Action* BattlePlaneMoveLeft(int ActionTime);
    Action* BattlePlaneMoveRight(int ActionTime);
    Action* BattlePlaneMoveDown(int ActionTime);
    void emenyBattlePlaneBuildCallBack(int Tag);
    
    void jetBuildCallBack();
    Action* JetMoveUp(int ActionTime);
    Action* JetMoveLeft(int ActionTime);
    Action* JetMoveRight(int ActionTime);
    Action* JetMoveDown(int ActionTime);
    void emenyJetBuildCallBack(int Tag);
    
    void soldierXBuildCallBack();
    Action* SoldierXMoveUp(int ActionTime);
    Action* SoldierXMoveLeft(int ActionTime);
    Action* SoldierXMoveRight(int ActionTime);
    Action* SoldierXMoveDown(int ActionTime);
    void emenySoldierXBuildCallBack(int Tag);
    
    
    void rightMenuMoveBy(MoveBy* SpriteMoveBy);

    
    
    virtual void editBoxReturn(EditBox* editBox)
    {
        MyData.MyChat = editBox->getText();
        std::stringstream stream1;
        std::string MyOrder;
        std::string MySide;
        stream1 << MyData.MyClientChoice;
        stream1 >> MySide;
        std::stringstream stream2;
        stream2 << "c" << "%" << MyData.MyChat << "%";
        stream2 >> MyOrder;
        MySide += MyOrder;
        MyOrderList.push_back(MySide);
        
    }
    
    
    
};



#endif /* GameScene_h */





