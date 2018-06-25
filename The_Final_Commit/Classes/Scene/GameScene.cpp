#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CountryChoiceScene.h"
#include "LoseScene.h"
#include "WinScene.h"
#include "../NetWork/CHAT_CLIENT.cpp"
#include "../NetWork/CHAT_SERVER.cpp"

int CountryChoice;
extern int ClientChoice;
extern std::string IpAddress;

void writeMsg();
void serverWork();

USING_NS_CC;
using namespace CocosDenshion;
Scene* GameScene::createScene()
{
    return GameScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}


bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    
    std::thread trdRecv(writeMsg);
    trdRecv.detach();
    
    if (ClientChoice == 1)
    {
        std::thread trdRec(serverWork);
        trdRec.detach();
    }
    
    VisibleSize = Director::getInstance()->getVisibleSize();
    Vec2 Origin = Director::getInstance()->getVisibleOrigin();
    
    MyData.MyCountryChoice = CountryChoice;
    MyData.MyClientChoice = ClientChoice;
    if (ClientChoice == 2)
    {
        MyData.TagNumber += 500;
    }
    
    ScreenWidth = VisibleSize.width;
    ScreenHeight = VisibleSize.height;
    
    TileMap = TMXTiledMap::create("RedAlertTiledMap.tmx");
    TileMap->setAnchorPoint(Vec2::ZERO);
    TileMap->setPosition(Vec2::ZERO);
    this->addChild(TileMap,-1);
    
    /*
     获得tilemap中MiningYard 并且将地图所对应位置设置为有矿井与有建筑物
     */
    auto MiningYardPoints = TileMap->getObjectGroup("MiningYard");
    auto MiningYardPositionGroup = MiningYardPoints->getObjects();
    for (auto& obj : MiningYardPositionGroup)
    {
        auto dic = obj.asValueMap();
        int x = dic.at("x").asInt();
        int y = dic.at("y").asInt();
        for (int i = 0; i < 128; i++)
        {
            for (int j = 0; j < 128; j++)
            {
                IsPositionHaveBuildings[x+i][y+j] = 1;
                IsPositionHaveMiningYard[x+i][y+j] = 1;
            }
        }
    }
    
    auto s = Size(100,20);
    auto _9Sprite = Scale9Sprite::create("Common/EditBox.png");
    IpEditBox = EditBox::create(s, _9Sprite);
    IpEditBox->setFont("fonts/Marker Felt.ttf", 15);
    IpEditBox->setPosition(Vec2(80,VisibleSize.height-200));
    this->addChild(IpEditBox,7);
    IpEditBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    IpEditBox->setReturnType(EditBox::KeyboardReturnType::DEFAULT);
    IpEditBox->setDelegate(this);
    
    ChatRoom = Text::create();
    ChatRoom->setFontSize(15);
    ChatRoom->setPosition(Vec2(80,VisibleSize.height-150));
    this->addChild(ChatRoom,7);
    
    
    Camera = Sprite::create("Common/camera.png");
    Camera->setPosition(Vec2(ScreenWidth/2,ScreenHeight/2));
    this->addChild(Camera);
    
    RightMenuPicture = Sprite::create("MenuPicture/RightMenu.png");
    RightMenuPicture->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2));
    RightMenuPicture->setScale(1, 1.279);
    this->addChild(RightMenuPicture,3);
    
    RightMenuText = Text::create(" ","fonts/Marker Felt.ttf",23);
    RightMenuText->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2+150));
    this->addChild(RightMenuText,4);
    
    PromptText = Text::create("将建筑拖到矿\n场或者已有建\n筑物上即可取\n消建筑","fonts/Marker Felt.ttf",23);
    PromptText->setPosition(Vec2(ScreenWidth-100, ScreenHeight/2-300));
    this->addChild(PromptText,4);
    
    MoneyText = Text::create(" ","fonts/Marker Felt.ttf",35);
    MoneyText->setPosition(Vec2(50, ScreenHeight-50));
    this->addChild(MoneyText,4);
    
    baseBuildBallBack();
    
    auto listener = EventListenerKeyboard::create();
    
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = true;
    };
    
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        keys[keyCode] = false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    

    commonGamePictureLoading();
    countryChoiceSwitch(CountryChoice);
    
    this->scheduleUpdate();
    
    if (ClientChoice == 2)
    {
        auto ActionMoveBy = MoveBy::create(5, Vec2(770, 800));
        Camera->runAction(ActionMoveBy);
        rightMenuMoveBy(ActionMoveBy);
    }
    
    return true;
}



void GameScene::setViewpointCenter(Vec2 position)
{
    Point actualPosition = Vec2(position.x,position.y);
    Point centerofView = Vec2(ScreenWidth/2,ScreenHeight/2);
    Point viewPoint = centerofView - actualPosition;
    this->setPosition(viewPoint);
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


void GameScene::update(float delta)
{
    Node::update(delta);
    
    TimeCount++;
    if (TimeCount % 60 == 0)
    {
        MyData.MyMoney += 1 * MyData.RefineryNumber;
        std::string value = StringUtils::format("%d $",MyData.MyMoney);
        MoneyText->setString(value);
        if (MyData.MyBase->getLifeValue() <= 0)
        {
            std::stringstream stream;
            std::string MyOrder;
            stream << MyData.MyClientChoice;
            stream >> MyOrder;
            MyOrder += "w";
            MyOrderList.push_back(MyOrder);
            Director::getInstance()->replaceScene(TransitionFade::create(2.0f, LoseScene::createScene()));
        }
    }
    
    if (TimeCount % 10 == 0)
    {
        if (!EnemyOrderList.empty())
        {
            for (int i = 0; i < EnemyOrderList.size(); i++)
            {
                std::string Side = EnemyOrderList[i].substr(0,1);
                std::stringstream stream4;
                stream4 << Side;
                int Side_;
                stream4 >> Side_;
                if (Side_ != MyData.MyClientChoice)
                {
                    std::string Action = EnemyOrderList[i].substr(1,1);
                    if (Action == "b")
                    {
                        std::stringstream stream1;
                        
                        int Tag,x,y;
                        std::string TagStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of(" ")+1,
                               EnemyOrderList[i].find_last_of(" ")-
                               EnemyOrderList[i].find_first_of(" ")-1);
                        stream1 << TagStr;
                        stream1 >> Tag;
                        
                        std::stringstream stream2;
                        std::string xStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("*")+1,
                               EnemyOrderList[i].find_last_of("*")-
                               EnemyOrderList[i].find_first_of("*")-1);
                        stream2 << xStr;
                        stream2 >> x;
                        
                        std::stringstream stream3;
                        std::string yStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("/")+1,
                               EnemyOrderList[i].find_last_of("/")-
                               EnemyOrderList[i].find_first_of("/")-1);
                        stream3 << yStr;
                        stream3 >> y;
                        
                        MyData.LastTouchPosition.x = x;
                        MyData.LastTouchPosition.y = y;
                        
                        std::string Type = EnemyOrderList[i].substr(2,1);
                        if (Type == "a")
                        {
                            emenyRefineryBuildCallBack(Tag);
                        }
                        else if (Type == "b")
                        {
                            emenyElectricPowerPlantBuildCallBack(Tag);
                        }
                        else if (Type == "c")
                        {
                            emenyWarFactoryBuildCallBack(Tag);
                        }
                        else if (Type == "d")
                        {
                            emenyBarracksBuildCallBack(Tag);
                        }
                        else if (Type == "e")
                        {
                            emenyCannonBuildCallBack(Tag);
                        }
                        else if (Type == "f")
                        {
                            emenyTankBuildCallBack(Tag);
                        }
                        else if (Type == "g")
                        {
                            emenyRTankBuildCallBack(Tag);
                        }
                        else if (Type == "h")
                        {
                            emenyTTankBuildCallBack(Tag);
                        }
                        else if (Type == "i")
                        {
                            emenySoldierXBuildCallBack(Tag);
                        }
                        else if (Type == "j")
                        {
                            emenySoldierXBuildCallBack(Tag);
                        }
                        else if (Type == "k")
                        {
                            emenyBattlePlaneBuildCallBack(Tag);
                        }
                        else if (Type == "l")
                        {
                            emenyJetBuildCallBack(Tag);
                        }
                        else if (Type == "m")
                        {
                            emenyMissileWellBuildCallBack(Tag);
                        }
                        else if (Type == "n")
                        {
                            emenyBaseBuildCallBack(Tag);
                        }
                    }
                    if (Action == "m")
                    {
                        std::stringstream stream1;
                        
                        int Tag,x,y;
                        std::string TagStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of(" ")+1,
                               EnemyOrderList[i].find_last_of(" ")-
                               EnemyOrderList[i].find_first_of(" ")-1);
                        stream1 << TagStr;
                        stream1 >> Tag;
                        
                        std::stringstream stream2;
                        std::string xStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("*")+1,
                               EnemyOrderList[i].find_last_of("*")-
                               EnemyOrderList[i].find_first_of("*")-1);
                        stream2 << xStr;
                        stream2 >> x;
                        
                        std::stringstream stream3;
                        std::string yStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("/")+1,
                               EnemyOrderList[i].find_last_of("/")-
                               EnemyOrderList[i].find_first_of("/")-1);
                        stream3 << yStr;
                        stream3 >> y;
                        
                        MyData.LastTouchPosition.x = x;
                        MyData.LastTouchPosition.y = y;
                        std::string Type = EnemyOrderList[i].substr(2,1);
                        if (Type == "f")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/60, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/60, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)TankMoveLeft(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)TankMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)TankMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)TankMoveRight(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)TankMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)TankMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "g")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/60, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/60, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)RTankMoveLeft(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)RTankMoveRight(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "h")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/40, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/40, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)RTankMoveLeft(AbsoluteDistanceX/40));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveDown(AbsoluteDistanceY/40));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveUp(AbsoluteDistanceY/40));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)RTankMoveRight(AbsoluteDistanceX/40));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveDown(AbsoluteDistanceY/40));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)RTankMoveUp(AbsoluteDistanceY/40));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "i")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/30, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/30, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)SoldierMoveLeft(AbsoluteDistanceX/30*2));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierMoveDown(AbsoluteDistanceY/30*2));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierMoveUp(AbsoluteDistanceY/30*2));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)SoldierMoveRight(AbsoluteDistanceX/30*2));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierMoveDown(AbsoluteDistanceY/30*2));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierMoveUp(AbsoluteDistanceY/30*2));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "j")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/60, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/60, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)SoldierXMoveLeft(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierXMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierXMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)SoldierXMoveRight(AbsoluteDistanceX/60));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierXMoveDown(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)SoldierXMoveUp(AbsoluteDistanceY/60));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "k")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/110, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/110, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)BattlePlaneMoveLeft(AbsoluteDistanceX/110));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)BattlePlaneMoveDown(AbsoluteDistanceY/110));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)BattlePlaneMoveUp(AbsoluteDistanceY/110));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)BattlePlaneMoveRight(AbsoluteDistanceX/110));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)BattlePlaneMoveDown(AbsoluteDistanceY/110));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)BattlePlaneMoveUp(AbsoluteDistanceY/110));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                        else if (Type == "l")
                        {
                            auto BuildingSprite = this->getChildByTag(Tag-1);
                            auto BuildingHPBar = this->getChildByTag(Tag);
                            BuildingSprite->stopAllActions();
                            BuildingHPBar->stopAllActions();
                            auto Position = BuildingSprite->getPosition();
                            double DistanceX2 = (x-Position.x)*(x-Position.x);
                            double DistanceY2 = (y-Position.y)*(y-Position.y);
                            double AbsoluteDistanceX = sqrt(DistanceX2);
                            double AbsoluteDistanceY = sqrt(DistanceY2);
                            double DistanceX = x-Position.x;
                            double DistanceY = y-Position.y;
                            auto MoveActionX = MoveBy::create(AbsoluteDistanceX/90, Vec2(DistanceX, 0));
                            auto MoveActionY = MoveBy::create(AbsoluteDistanceY/90, Vec2(0, DistanceY));
                            if (DistanceX <= 0)
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)JetMoveLeft(AbsoluteDistanceX/90));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)JetMoveDown(AbsoluteDistanceY/90));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)JetMoveUp(AbsoluteDistanceY/90));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                            else
                            {
                                auto SpawnActionX = Spawn::createWithTwoActions(MoveActionX, (FiniteTimeAction*)JetMoveRight(AbsoluteDistanceX/90));
                                if (DistanceY <= 0)
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)JetMoveDown(AbsoluteDistanceY/90));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                                else
                                {
                                    auto SpawnActionY = Spawn::createWithTwoActions(MoveActionY, (FiniteTimeAction*)JetMoveUp(AbsoluteDistanceY/90));
                                    auto SequenceAction = Sequence::createWithTwoActions(SpawnActionX, SpawnActionY);
                                    BuildingSprite->runAction(SequenceAction);
                                    BuildingHPBar->runAction(Sequence::createWithTwoActions(MoveActionX->clone(), MoveActionY->clone()));
                                }
                            }
                        }
                    }
                    if (Action == "a")
                    {
                        std::stringstream stream1;
                        
                        int Tag,Attack;
                        std::string TagStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of(" ")+1,
                               EnemyOrderList[i].find_last_of(" ")-
                               EnemyOrderList[i].find_first_of(" ")-1);
                        stream1 << TagStr;
                        stream1 >> Tag;
                        
                        std::stringstream stream2;
                        std::string AttackStr =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("*")+1,
                               EnemyOrderList[i].find_last_of("*")-
                               EnemyOrderList[i].find_first_of("*")-1);
                        stream2 << AttackStr;
                        stream2 >> Attack;
                        LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(Tag);
                        HPBar->setPercent(HPBar->getPercent()-Attack);
                        BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag(Tag-1);
                        BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-Attack);
                    }
                    
                    if (Action == "w")
                    {
                        Director::getInstance()->replaceScene(TransitionFade::create(2.0f, WinScene::createScene()));
                    }
                    
                    if (Action == "c")
                    {
                        std::stringstream stream;
                        stream << "Player2: ";
                        std::string AllChat;
                        stream >> AllChat;
                        std::string Chat =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("%")+1,
                               EnemyOrderList[i].find_last_of("%")-
                               EnemyOrderList[i].find_first_of("%")-1);
                        AllChat += Chat;
                        ChatRoom->setString(AllChat);
                    }
                }
                else
                {
                    std::string Action = EnemyOrderList[i].substr(1,1);
                    if (Action == "c")
                    {
                        std::stringstream stream;
                        stream << "Player1: ";
                        std::string AllChat;
                        stream >> AllChat;
                        std::string Chat =
                        EnemyOrderList[i].
                        substr(EnemyOrderList[i].find_first_of("*")+1,
                               EnemyOrderList[i].find_last_of("*")-
                               EnemyOrderList[i].find_first_of("*")-1);
                        AllChat += Chat;
                        ChatRoom->setString(AllChat);
                    }
                }
            }
        }
        EnemyOrderList.clear();
    }
    
    
    
    
    
    if (TimeCount % 5 == 0)
    {
        MyData.WarFactoryNumber         = MyData.MyWarFactory.size();
        MyData.RefineryNumber           = MyData.MyRefinery.size();
        MyData.BarracksNumber           = MyData.MyBarracks.size();
        MyData.ElectricPowerPlantNumber = MyData.MyElectricPowerPlant.size();
        
        for (int i = 0; i < 1601; i++)
        {
            for (int j = 0; j < 1601; j++)
            {
                IsPositionHaveArmyAndTag[i][j] = 0;
            }
        }
        for (int q = 0; q < MyData.MyTank.size(); q++)
        {
            int SpriteWidth = MyData.MyTank[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyTank[q]->getContentSize().height;
            int x = MyData.MyTank[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyTank[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyTank[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MyTTank.size(); q++)
        {
            int SpriteWidth = MyData.MyTTank[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyTTank[q]->getContentSize().height;
            int x = MyData.MyTTank[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyTTank[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyTTank[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MyRTank.size(); q++)
        {
            int SpriteWidth = MyData.MyRTank[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyRTank[q]->getContentSize().height;
            int x = MyData.MyRTank[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyRTank[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyRTank[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MyBattlePlane.size(); q++)
        {
            int SpriteWidth = MyData.MyBattlePlane[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyBattlePlane[q]->getContentSize().height;
            int x = MyData.MyBattlePlane[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyBattlePlane[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyBattlePlane[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MyJet.size(); q++)
        {
            int SpriteWidth = MyData.MyJet[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyJet[q]->getContentSize().height;
            int x = MyData.MyJet[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyJet[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyJet[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MySoldier.size(); q++)
        {
            int SpriteWidth = MyData.MySoldier[q]->getContentSize().width;
            int SpriteHeigth = MyData.MySoldier[q]->getContentSize().height;
            int x = MyData.MySoldier[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MySoldier[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MySoldier[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MySoldierX.size(); q++)
        {
            int SpriteWidth = MyData.MySoldierX[q]->getContentSize().width;
            int SpriteHeigth = MyData.MySoldierX[q]->getContentSize().height;
            int x = MyData.MySoldierX[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MySoldierX[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MySoldierX[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }
        for (int q = 0; q < MyData.MyCannon.size(); q++)
        {
            int SpriteWidth = MyData.MyCannon[q]->getContentSize().width;
            int SpriteHeigth = MyData.MyCannon[q]->getContentSize().height;
            int x = MyData.MyCannon[q]->getPosition().x-SpriteWidth/2;
            int y = MyData.MyCannon[q]->getPosition().y-SpriteHeigth/2;
            int Tag = MyData.MyCannon[q]->getTag();
            for (int i = 0; i <= SpriteWidth; i++)
            {
                for (int j = 0; j <= SpriteHeigth; j++)
                {
                    IsPositionHaveArmyAndTag[x+i][y+j] = Tag;
                }
            }
        }

        for (int i = 0; i < MyData.EmenyRefinery.size(); i++)
        {
            if (MyData.EmenyRefinery[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.EmenyRefinery[i]->getContentSize().width;
                int SpriteHeight = MyData.EmenyRefinery[i]->getContentSize().height;
                int x = MyData.EmenyRefinery[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.EmenyRefinery[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.EmenyRefinery[i]);
                MyData.EmenyRefinery.erase(MyData.EmenyRefinery.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyTank.size(); i++)
        {
            if (MyData.MyTank[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MyTank[i],true);
                MyData.MyTank.erase(MyData.MyTank.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MyTTank.size(); i++)
        {
            if (MyData.MyTTank[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MyTTank[i],true);
                MyData.MyTTank.erase(MyData.MyTTank.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MyRTank.size(); i++)
        {
            if (MyData.MyRTank[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MyRTank[i],true);
                MyData.MyRTank.erase(MyData.MyRTank.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MyBattlePlane.size(); i++)
        {
            if (MyData.MyBattlePlane[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MyBattlePlane[i],true);
                MyData.MyBattlePlane.erase(MyData.MyBattlePlane.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MyJet.size(); i++)
        {
            if (MyData.MyJet[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MyJet[i],true);
                MyData.MyJet.erase(MyData.MyJet.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MySoldier.size(); i++)
        {
            if (MyData.MySoldier[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MySoldier[i],true);
                MyData.MySoldier.erase(MyData.MySoldier.begin()+i);
            }
        }
        for (int i = 0; i < MyData.MySoldierX.size(); i++)
        {
            if (MyData.MySoldierX[i]->getLifeValue() <= 0)
            {
                this->removeChild(MyData.MySoldierX[i],true);
                MyData.MySoldierX.erase(MyData.MySoldierX.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyWarFactory.size(); i++)
        {
            if (MyData.MyWarFactory[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.MyWarFactory[i]->getContentSize().width;
                int SpriteHeight = MyData.MyWarFactory[i]->getContentSize().height;
                int x = MyData.MyWarFactory[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.MyWarFactory[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.MyWarFactory[i],true);
                MyData.MyWarFactory.erase(MyData.MyWarFactory.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyBarracks.size(); i++)
        {
            if (MyData.MyBarracks[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.MyBarracks[i]->getContentSize().width;
                int SpriteHeight = MyData.MyBarracks[i]->getContentSize().height;
                int x = MyData.MyBarracks[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.MyBarracks[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.MyBarracks[i],true);
                MyData.MyBarracks.erase(MyData.MyBarracks.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyElectricPowerPlant.size(); i++)
        {
            if (MyData.MyElectricPowerPlant[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.MyElectricPowerPlant[i]->getContentSize().width;
                int SpriteHeight = MyData.MyElectricPowerPlant[i]->getContentSize().height;
                int x = MyData.MyElectricPowerPlant[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.MyElectricPowerPlant[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.MyElectricPowerPlant[i],true);
                MyData.MyElectricPowerPlant.erase(MyData.MyElectricPowerPlant.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyRefinery.size(); i++)
        {
            if (MyData.MyRefinery[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.MyRefinery[i]->getContentSize().width;
                int SpriteHeight = MyData.MyRefinery[i]->getContentSize().height;
                int x = MyData.MyRefinery[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.MyRefinery[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.MyRefinery[i],true);
                MyData.MyRefinery.erase(MyData.MyRefinery.begin()+i);
            }
        }
        
        for (int i = 0; i < MyData.MyCannon.size(); i++)
        {
            if (MyData.MyCannon[i]->getLifeValue() <= 0)
            {
                int SpriteWidth = MyData.MyCannon[i]->getContentSize().width;
                int SpriteHeight = MyData.MyCannon[i]->getContentSize().height;
                int x = MyData.MyCannon[i]->getPosition().x-SpriteWidth/2;
                int y = MyData.MyCannon[i]->getPosition().y-SpriteHeight/2;
                for (int i = 0; i <= SpriteWidth; i++)
                {
                    for (int j = 0; j <= SpriteHeight; j++)
                    {
                        if (x+i >= 0 && y+j >= 0 && x+i <= 1600 && y+j <= 1600)
                        {
                            IsPositionHaveBuildings[x+i][y+j] = 0;
                        }
                    }
                }
                this->removeChild(MyData.MyCannon[i],true);
                MyData.MyCannon.erase(MyData.MyCannon.begin()+i);
            }
        }
    }
    
    
    auto leftArrow =  EventKeyboard::KeyCode::KEY_A,  rightArrow =  EventKeyboard::KeyCode::KEY_D,
         upArrow   =  EventKeyboard::KeyCode::KEY_W,  downArrow  =  EventKeyboard::KeyCode::KEY_S;
    if(isKeyPressed(leftArrow))
    {
        keyPressedDuration(leftArrow);
    }
    else if(isKeyPressed(rightArrow))
    {
        keyPressedDuration(rightArrow);
    }
    else if(isKeyPressed(upArrow))
    {
        keyPressedDuration(upArrow);
    }
    else if(isKeyPressed(downArrow))
    {
        keyPressedDuration(downArrow);
    }
    this->setViewpointCenter(Camera->getPosition());
    
    if (MyData.MyMoney < 60)
    {
        CommonRefineryButton->setTouchEnabled(false);
    }
    else
    {
        CommonRefineryButton->setTouchEnabled(true);
    }
    
    if (MyData.RefineryNumber <= 0 || MyData.ElectricPowerPlantNumber <= 0)
    {
        CommonCannonButton->setTouchEnabled(false);
        CommonBarracksButton->setTouchEnabled(false);
        CommonSoldierButton->setTouchEnabled(false);
        CommonTankButton->setTouchEnabled(false);
        CommonWarFactoryButton->setTouchEnabled(false);
        switch (MyData.MyCountryChoice)
        {
            case 1:
                OneJetButton->setTouchEnabled(false);
                OneBattlePlaneButton->setTouchEnabled(false);
                break;
                
            case 2:
                TwoRTankButton->setTouchEnabled(false);
                TwoTTankButton->setTouchEnabled(false);
                break;
                
            case 3:
                ThreeSoldierXButton->setTouchEnabled(false);
                ThreeMissileWellButton->setTouchEnabled(false);
                break;
                
            default:
                break;
        }
    }
    
    if (MyData.RefineryNumber <= 0)
        CommonElectricPowerPlantButton->setTouchEnabled(false);
    
    if (MyData.RefineryNumber > 0)
    {
        if (MyData.MyMoney > 50)
        {
            CommonElectricPowerPlantButton->setTouchEnabled(true);
        }
        else
            CommonElectricPowerPlantButton->setTouchEnabled(false);
    }
    
    
    if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0)
    {
        if (MyData.MyMoney > 40)
        {
            CommonCannonButton->setTouchEnabled(true);
        }
        else
        {
            CommonCannonButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 100)
        {
            CommonBarracksButton->setTouchEnabled(true);
        }
        else
        {
            CommonBarracksButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 150)
        {
            CommonWarFactoryButton->setTouchEnabled(true);
        }
        else
        {
            CommonWarFactoryButton->setTouchEnabled(false);
        }
        
        if (MyData.MyMoney > 400)
        {
            switch (MyData.MyCountryChoice)
            {
                case 3:
                    ThreeMissileWellButton->setTouchEnabled(true);
                    break;
                    
                default:
                    break;
            }
        }
        else
        {
            switch (MyData.MyCountryChoice)
            {
                case 3:
                    ThreeMissileWellButton->setTouchEnabled(false);
                    break;
                    
                default:
                break;
            }
        }
    
        if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0 && MyData.WarFactoryNumber > 0)
        {
            if (MyData.MyMoney > 50)
            {
                CommonTankButton->setTouchEnabled(true);
            }
            else
            {
                CommonTankButton->setTouchEnabled(false);
            }
        
            if (MyData.MyMoney > 40)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneJetButton->setTouchEnabled(true);
                        break;
                    
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneJetButton->setTouchEnabled(false);
                        break;
                        
                    default:
                        break;
                }
            }
        
            if (MyData.MyMoney > 60)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneBattlePlaneButton->setTouchEnabled(true);
                        break;
                    
                    case 2:
                        TwoTTankButton->setTouchEnabled(true);
                        TwoRTankButton->setTouchEnabled(true);
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 1:
                        OneBattlePlaneButton->setTouchEnabled(false);
                        break;
                    
                    case 2:
                        TwoTTankButton->setTouchEnabled(false);
                        TwoRTankButton->setTouchEnabled(false);
                    default:
                        break;
                }
            }
        }
    
        if (MyData.RefineryNumber > 0 && MyData.ElectricPowerPlantNumber > 0 && MyData.BarracksNumber > 0)
        {
            if (MyData.MyMoney > 20)
            {
                CommonSoldierButton->setTouchEnabled(true);
            }
            else
            {
                CommonSoldierButton->setTouchEnabled(false);
            }
        
            if (MyData.MyMoney > 30)
            {
                switch (MyData.MyCountryChoice)
                {
                    case 3:
                        ThreeSoldierXButton->setTouchEnabled(true);
                        break;
                    
                    default:
                        break;
                }
            }
            else
            {
                switch (MyData.MyCountryChoice)
                {
                    case 3:
                        ThreeSoldierXButton->setTouchEnabled(false);
                        break;
                    
                    default:
                        break;
                }
            }
        }
    }
}

bool GameScene::isKeyPressed(EventKeyboard::KeyCode keyCode)
{
    if(keys[keyCode])
    {
        return true;
    }
    else
    {
        return false;
    }
}

void GameScene::keyPressedDuration(EventKeyboard::KeyCode code)
{
    int offsetX = 0, offsetY = 0;
    switch (code) {
        case EventKeyboard::KeyCode::KEY_A:
            if ((int)Camera->getPositionX() > ScreenWidth/2)
            {
                offsetX = -5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if ((int)Camera->getPositionX() < TileMap->getMapSize().width * TileMap->getTileSize().width - ScreenWidth/2 + 190)
            {
                offsetX = 5;
            }
            else
            {
                offsetX = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_W:
            if ((int)Camera->getPositionY() < TileMap->getMapSize().height * TileMap->getTileSize().height - ScreenHeight/2 - 10)
            {
                offsetY = 5;
            }
            else
            {
                offsetY = 0;
            }
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if ((int)Camera->getPositionY() > ScreenHeight/2)
            {
                offsetY = -5;
            }
            else
            {
                offsetY = 0;
            }
            break;
        default:
            offsetY = offsetX = 0;
            break;
    }
    auto CameraMoveBy = MoveBy::create(0, Vec2(offsetX, offsetY));
    Camera->runAction(CameraMoveBy);
    auto RightMenuMoveByAction = MoveBy::create(0, Vec2(offsetX, offsetY));
    rightMenuMoveBy(RightMenuMoveByAction);
}


void writeMsg()
{
    try
    {
        
        boost::asio::io_service io_service;
        
        tcp::resolver resolver(io_service);
        auto endpoint_iterator = resolver.resolve({ IpAddress, "1024" });
        chat_client c(io_service, endpoint_iterator);
        
        std::thread t([&io_service]() { io_service.run(); });
        
        for (; ; )
        {
            if (!MyOrderList.empty())
            {
                for (int i = 0; i < MyOrderList.size(); i++)
                {
                    const char* line;
                    line = MyOrderList[i].c_str();
                    
                    chat_massage msg;
                    msg.body_length(std::strlen(line));
                    std::memcpy(msg.body(), line, msg.body_length());
                    msg.encode_header();
                    c.write(msg);
                }
                MyOrderList.clear();
            }
            
        }
    
        c.close();
        t.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
}


void serverWork()
{
    try
    {
        
        boost::asio::io_service io_service;
        
        std::list<chat_server> servers;
        
        tcp::endpoint endpoint(tcp::v4(), 1024);//listen 1024 port
        servers.emplace_back(io_service, endpoint);//constructor
        
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
}





