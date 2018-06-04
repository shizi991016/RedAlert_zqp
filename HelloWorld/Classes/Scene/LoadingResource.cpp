#include "GameScene.h"


//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************


void GameScene::countryChoiceSwitch(int CountryChoice)
{
    /*
     BattlePlane            战斗机
     Jet                    喷气机
     RTank                  坦克
     TTank                  坦克
     SoldierX               大兵
     MissileWell            核弹井
     */
    
    switch (CountryChoice)
    {
        case 1:
        {
            OneBattlePlaneButton  = Button::create("MenuPicture/CommonMenuButton.png");
            OneBattlePlanePicture = Sprite::create("GamePicture/OneBattlePlane.png");
            OneBattlePlaneButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            OneBattlePlanePicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(OneBattlePlaneButton,5);
            this->addChild(OneBattlePlanePicture,4);
            OneJetButton  = Button::create("MenuPicture/CommonMenuButton.png");
            OneJetPicture = Sprite::create("GamePicture/OneJet.png");
            OneJetButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            OneJetPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(OneJetButton,5);
            this->addChild(OneJetPicture,4);
            break;
        }
            
        case 2:
        {
            TwoRTankButton  = Button::create("MenuPicture/CommonMenuButton.png");
            TwoRTankPicture = Sprite::create("GamePicture/TwoRtank.png");
            TwoRTankButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            TwoRTankPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(TwoRTankButton,5);
            this->addChild(TwoRTankPicture,4);
            TwoTTankButton  = Button::create("MenuPicture/CommonMenuButton.png");
            TwoTTankPicture = Sprite::create("GamePicture/TwoTtank.png");
            TwoTTankButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            TwoTTankPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(TwoTTankButton,5);
            this->addChild(TwoTTankPicture,4);
            break;
        }
            
        case 3:
        {
            ThreeSoldierXButton = Button::create("MenuPicture/CommonMenuButton.png");
            ThreeSoldierXPicture = Sprite::create("GamePicture/ThreeSoldierX.png");
            ThreeSoldierXButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            ThreeSoldierXPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 140));
            this->addChild(ThreeSoldierXPicture,4);
            this->addChild(ThreeSoldierXButton,5);
            ThreeMissileWellButton = Button::create("MenuPicture/CommonMenuButton.png");
            ThreeMissileWellPicture = Sprite::create("GamePicture/ThreeMissileWell.png");
            ThreeMissileWellButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            ThreeMissileWellPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 140));
            this->addChild(ThreeMissileWellPicture,4);
            this->addChild(ThreeMissileWellButton,5);
            break;
        }
            
        default:
        {
            break;
        }
    }
}



void GameScene::commonGamePictureLoading()
{
    
    /*
     Barracks               兵营 对应Soldier与SoldierX
     Cannon                 加农炮 对空伤害高对地伤害低
     ElectricPowerPlant     电厂 没有电厂不让建除矿场以外其他建筑 电厂越多建筑速度越快
     Refinery               矿场 没有矿场不给加钱 矿场越多加钱越快
     Soldier                大兵 对地伤害高对空伤害低
     Tank                   坦克
     WarFactory             战车工厂 对应Tank TTank RTank
     
     */
    
    CommonBarracksButton           = Button::create("MenuPicture/CommonMenuButton.png");
    CommonCannonButton             = Button::create("MenuPicture/CommonMenuButton.png");
    CommonElectricPowerPlantButton = Button::create("MenuPicture/CommonMenuButton.png");
    CommonRefineryButton           = Button::create("MenuPicture/CommonMenuButton.png");
    CommonSoldierButton            = Button::create("MenuPicture/CommonMenuButton.png");
    CommonTankButton               = Button::create("MenuPicture/CommonMenuButton.png");
    CommonWarFactoryButton         = Button::create("MenuPicture/CommonMenuButton.png");
    
    
    CommonBarracksPicture           = Sprite::create("GamePicture/CommonBarracks.png");
    CommonCannonPicture             = Sprite::create("GamePicture/CommonCannon.png");
    CommonElectricPowerPlantPicture = Sprite::create("GamePicture/CommonElectricPowerPlant.png");
    CommonRefineryPicture           = Sprite::create("GamePicture/CommonRefinery.png");
    CommonSoldierPicture            = Sprite::create("GamePicture/CommonSoldier.png");
    CommonTankPicture               = Sprite::create("GamePicture/CommonTank.png");
    CommonWarFactoryPicture         = Sprite::create("GamePicture/CommonWarFactory.png");
    
    
    CommonElectricPowerPlantButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
    CommonElectricPowerPlantPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 28));
    CommonElectricPowerPlantButton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                                          {
                                                              switch (type)
                                                              {
                                                                  case Widget::TouchEventType::ENDED:
                                                                  {
                                                                      auto ElectricPowerPlantPicture = Sprite::create
                                                                      ("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                                                                      ElectricPowerPlantPicture->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                                      ElectricPowerPlantPicture->setScale(0.25f);
                                                                      this->addChild(ElectricPowerPlantPicture,1);
                                                                      ElectricPowerPlantPicture->setOpacity(140);
                                                                      ArmyMoveOnce(ElectricPowerPlantPicture);
                                                                      break;
                                                                  }
                                                                      
                                                                  default:
                                                                      break;
                                                              }
                                                          });
    CommonRefineryButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height-28));
    CommonRefineryPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height-28));
    CommonRefineryButton->addTouchEventListener([=](Ref* pSender,Widget::TouchEventType type)
                                                {
                                                    switch (type)
                                                    {
                                                        case Widget::TouchEventType::ENDED:
                                                        {
                                                            auto TextPic = Sprite::create("CommonElectricPowerPlant_action/CommonElectricPowerPlant_action_15.png");
                                                            TextPic->setPosition(Vec2(Camera->getPosition().x,Camera->getPosition().y));
                                                            this->addChild(TextPic,1);
                                                            break;
                                                        }
                                                            
                                                        default:
                                                            break;
                                                    }
                                                });
    CommonWarFactoryButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height-28));
    CommonWarFactoryPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height-28));
    CommonSoldierButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 85));
    CommonSoldierPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 85));
    CommonTankButton->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 85));
    CommonTankPicture->setPosition(Vec2(VisibleSize.width - 98, VisibleSize.height - 85));
    CommonCannonButton->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 85));
    CommonCannonPicture->setPosition(Vec2(VisibleSize.width - 44, VisibleSize.height - 85));
    CommonBarracksButton->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 140));
    CommonBarracksPicture->setPosition(Vec2(VisibleSize.width - 150, VisibleSize.height - 140));
    
    this->addChild(CommonBarracksButton,5);
    this->addChild(CommonCannonButton,5);
    this->addChild(CommonElectricPowerPlantButton,5);
    this->addChild(CommonRefineryButton,5);
    this->addChild(CommonSoldierButton,5);
    this->addChild(CommonTankButton,5);
    this->addChild(CommonWarFactoryButton,5);
    
    
    this->addChild(CommonBarracksPicture,4);
    this->addChild(CommonCannonPicture,4);
    this->addChild(CommonElectricPowerPlantPicture,4);
    this->addChild(CommonRefineryPicture,4);
    this->addChild(CommonSoldierPicture,4);
    this->addChild(CommonTankPicture,4);
    this->addChild(CommonWarFactoryPicture,4);
}
