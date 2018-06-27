//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"


void GameScene::tankBuildCallBack()
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("ArmyAction/CommonTank_action/CommonTank_action_up_1.png");
    BuildingSprite->setPosition(Vec2(MyData.TheLastWarFactoryPosition.x, MyData.TheLastWarFactoryPosition.y));
    this->addChild(BuildingSprite,2);
    MyData.MyTank.push_back(BuildingSprite);
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.TheLastWarFactoryPosition.x,MyData.TheLastWarFactoryPosition.y+30));
    this->addChild(BuildingHPBar,2);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    MyData.TagNumber += 2;
    BuildingSprite->setTag(MyData.TagNumber-1);
    BuildingHPBar->setTag(MyData.TagNumber);
    
    auto FinishBuildAction = MoveBy::create(1.0f, Vec2(0, 100));
    auto FinishBuildAnimation = Animation::create();
    FinishBuildAnimation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_up_1.png");
    FinishBuildAnimation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_up_2.png");
    FinishBuildAnimation->setLoops(1);
    FinishBuildAnimation->setDelayPerUnit(1.0f/2.0f);
    auto FinishBuildAnimate = Animate::create(FinishBuildAnimation);
    auto FinishBuildSpawn = Spawn::createWithTwoActions(FinishBuildAnimate, FinishBuildAction);
    BuildingSprite->runAction(FinishBuildSpawn);
    BuildingHPBar->runAction(FinishBuildAction->clone());
    
    MyData.MyMoney -= 40;
    
    std::stringstream stream1;
    stream1 << MyData.MyClientChoice;
    std::string MyOrder;
    stream1 >> MyOrder;
    MyOrder += "bf ";
    std::stringstream stream;
    stream << MyData.TagNumber << " " << "*" << (int)MyData.TheLastWarFactoryPosition.x << "*" << "/" << (int)MyData.TheLastWarFactoryPosition.y+100 << "/";
    std::string order = stream.str();
    //std::string MyOrder = "1bf ";
    MyOrder += order;
    MyOrderList.push_back(MyOrder);
    
    auto ArmyListener = EventListenerTouchOneByOne::create();//创建一个触摸监听
    ArmyListener->setSwallowTouches(true);
    ArmyListener->onTouchBegan = [=](Touch* touch, Event* event)
    {
        MyData.ArmyFirstTouchPosition = this->convertToNodeSpace(touch->getLocation());
        auto ArmyTarget = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
        Point locationInNode = ArmyTarget->convertToNodeSpace(touch->getLocation());//获得触摸点相对于触摸目标的坐标
        Size ArmySize = ArmyTarget->getContentSize();//获得触摸目标的大小
        Rect ArmyRect = Rect(0, 0, ArmySize.width, ArmySize.height);//创建一个坐标在左下角的相对于触摸目标的坐标系
        if (ArmyRect.containsPoint(locationInNode))//判断触摸点是否在目标的范围内
            return true;
        else
            return false;
    };
    
    ArmyListener->onTouchEnded = [=](Touch* touch, Event* event)
    {
        MyData.ArmyLastTouchPosition = this->convertToNodeSpace(touch->getLocation());
        if (IsPositionHaveArmyAndTag
            [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y])
        {
            if ((MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) *
                (MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) <= 150*150 &&
                (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) *
                (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) <= 150*150)
            {
                BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
                (IsPositionHaveArmyAndTag
                 [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
                BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-20);
                LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
                HPBar->setPercent(HPBar->getPercent()-20);
                
                int Tag = HPBar->getTag();
                std::stringstream stream1;
                stream1 << MyData.MyClientChoice;
                std::string MyOrder;
                std::stringstream stream;
                stream << Tag << " " << "*" << 20 << "*";
                std::string order = stream.str();
                stream1 >> MyOrder;
                MyOrder += "a ";
                MyOrder += order;
                MyOrderList.push_back(MyOrder);
            }
        }
        else
        {
            if (IsPositionHaveBuildings
                [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y] == 1)
            {
                if ((MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) *
                    (MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) <= 150*150 &&
                    (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) *
                    (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) <= 150*150)
                {
                    BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
                    (PositionTag[(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
                    BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-20);
                    LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
                    HPBar->setPercent(HPBar->getPercent()-20);
                    
                    int Tag = HPBar->getTag();
                    std::stringstream stream1;
                    stream1 << MyData.MyClientChoice;
                    std::string MyOrder;
                    std::stringstream stream;
                    stream << Tag << " " << "*" << 20 << "*";
                    std::string order = stream.str();
                    stream1 >> MyOrder;
                    MyOrder += "a ";
                    MyOrder += order;
                    MyOrderList.push_back(MyOrder);
                }
            }
            else
            {
                BuildingSprite->stopAllActions();
                BuildingHPBar->stopAllActions();
                int Tag = BuildingHPBar->getTag();
                auto Position = BuildingSprite->getPosition();
                MyData.ArmyLastTouchPosition = this->convertToNodeSpace(touch->getLocation());
                double DistanceX2 = (MyData.ArmyLastTouchPosition.x - Position.x)
                * (MyData.ArmyLastTouchPosition.x - Position.x);
                double DistanceY2 = (MyData.ArmyLastTouchPosition.y - Position.y)
                * (MyData.ArmyLastTouchPosition.y - Position.y);
                double AbsoluteDistanceX = sqrt(DistanceX2);
                double AbsoluteDistanceY = sqrt(DistanceY2);
                double DistanceX = MyData.ArmyLastTouchPosition.x - Position.x;
                double DistanceY = MyData.ArmyLastTouchPosition.y - Position.y;
                auto MoveActionX = MoveBy::create(AbsoluteDistanceX/60, Vec2(DistanceX, 0));
                auto MoveActionY = MoveBy::create(AbsoluteDistanceY/60, Vec2(0, DistanceY));
                
                
                std::stringstream stream1;
                stream1 << MyData.MyClientChoice;
                std::string MyOrder;
                stream1 >> MyOrder;
                MyOrder += "mf ";
                std::stringstream stream;
                stream << Tag << " " << "*" << (int)MyData.ArmyLastTouchPosition.x << "*" << "/" << (int)MyData.ArmyLastTouchPosition.y+100 << "/";
                std::string order = stream.str();
                //std::string MyOrder = "1af ";
                MyOrder += order;
                MyOrderList.push_back(MyOrder);
                
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
        }
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, BuildingSprite);
}


Action* GameScene::TankMoveUp(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_up_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_up_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::TankMoveLeft(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_left_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_left_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::TankMoveRight(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_right_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_right_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::TankMoveDown(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_down_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/CommonTank_action/CommonTank_action_down_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


void GameScene::emenyTankBuildCallBack(int Tag)
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("ArmyAction/CommonTank_action/CommonTank_action_up_1.png");
    BuildingSprite->setPosition(MyData.LastTouchPosition);
    this->addChild(BuildingSprite,2);
    MyData.MyRTank.push_back(BuildingSprite);
    auto BuildingHPBar = LoadingBar::create("GamePicture/HPBar.png");
    BuildingHPBar->setDirection(LoadingBar::Direction::LEFT);
    BuildingHPBar->setScale(0.07f);
    BuildingHPBar->setPercent(100);
    BuildingHPBar->setPosition(Vec2(MyData.LastTouchPosition.x,MyData.LastTouchPosition.y+30));
    this->addChild(BuildingHPBar,2);
    BuildingSprite->setHP(BuildingHPBar);
    BuildingSprite->setHPInterval(100.0f/BuildingSprite->getLifeValue());
    BuildingSprite->setTag(Tag-1);
    BuildingHPBar->setTag(Tag);
}




