//**************************************************
/*
 该文档用于把冗长的精灵创建代码拉到一个助教不能轻易看见的地方
 //你的代码怎么那么乱
 */
//**************************************************
#include "../Scene/GameScene.h"


void GameScene::jetBuildCallBack()
{
    auto BuildingSprite = BuildingsClass::createWithSpriteFileName("ArmyAction/OneJet_action/OneJet_action_up_1.png");
    BuildingSprite->setPosition(Vec2(MyData.TheLastWarFactoryPosition.x, MyData.TheLastWarFactoryPosition.y));
    this->addChild(BuildingSprite,2);
    MyData.MyJet.push_back(BuildingSprite);
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
    FinishBuildAnimation->addSpriteFrameWithFile
    ("ArmyAction/OneJet_action/OneJet_action_up_1.png");
    FinishBuildAnimation->addSpriteFrameWithFile
    ("ArmyAction/OneJet_action/OneJet_action_up_2.png");
    FinishBuildAnimation->setLoops(1);
    FinishBuildAnimation->setDelayPerUnit(1.0f/2.0f);
    auto FinishBuildAnimate = Animate::create(FinishBuildAnimation);
    auto FinishBuildSpawn = Spawn::createWithTwoActions(FinishBuildAnimate, FinishBuildAction);
    BuildingSprite->runAction(FinishBuildSpawn);
    BuildingHPBar->runAction(FinishBuildAction->clone());
    
    MyData.MyMoney -= 50;
    
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
        if (MyData.IsPositionHaveArmyAndTag
            [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y])
        {
            if ((MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) *
                (MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) <= 150*150 &&
                (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) *
                (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) <= 150*150)
            {
                BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
                (MyData.IsPositionHaveArmyAndTag
                 [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
                BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-10);
                LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
                HPBar->setPercent(HPBar->getPercent()-10);
            }
        }
        else
        {
            if (MyData.IsPositionHaveBuildings
                [(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y] == 1)
            {
                if ((MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) *
                    (MyData.ArmyLastTouchPosition.x-MyData.ArmyFirstTouchPosition.x) <= 150*150 &&
                    (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) *
                    (MyData.ArmyLastTouchPosition.y-MyData.ArmyFirstTouchPosition.y) <= 150*150)
                {
                    BuildingsClass* BuildingSprite = (BuildingsClass*)this->getChildByTag
                    (MyData.PositionTag[(int)MyData.ArmyLastTouchPosition.x][(int)MyData.ArmyLastTouchPosition.y]);
                    BuildingSprite->setLifeValue(BuildingSprite->getLifeValue()-10);
                    LoadingBar* HPBar = (LoadingBar*)this->getChildByTag(BuildingSprite->getTag()+1);
                    HPBar->setPercent(HPBar->getPercent()-10);
                }
            }
            else
            {
                BuildingSprite->stopAllActions();
                BuildingHPBar->stopAllActions();
                MyData.ArmyLastTouchPosition = this->convertToNodeSpace(touch->getLocation());
                double DistanceX2 = (MyData.ArmyLastTouchPosition.x - MyData.ArmyFirstTouchPosition.x)
                * (MyData.ArmyLastTouchPosition.x - MyData.ArmyFirstTouchPosition.x);
                double DistanceY2 = (MyData.ArmyLastTouchPosition.y - MyData.ArmyFirstTouchPosition.y)
                * (MyData.ArmyLastTouchPosition.y - MyData.ArmyFirstTouchPosition.y);
                double AbsoluteDistanceX = sqrt(DistanceX2);
                double AbsoluteDistanceY = sqrt(DistanceY2);
                double DistanceX = MyData.ArmyLastTouchPosition.x - MyData.ArmyFirstTouchPosition.x;
                double DistanceY = MyData.ArmyLastTouchPosition.y - MyData.ArmyFirstTouchPosition.y;
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
    };
    //将触摸监听添加到eventDispacher中去
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ArmyListener, BuildingSprite);
}


Action* GameScene::JetMoveUp(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_up_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_up_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::JetMoveLeft(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_left_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_left_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::JetMoveRight(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_right_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_right_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}


Action* GameScene::JetMoveDown(int ActionTime)
{
    auto animation = Animation::create();
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_down_1.png");
    animation->addSpriteFrameWithFile("ArmyAction/OneJet_action/OneJet_action_down_2.png");
    animation->setLoops(ActionTime);
    animation->setDelayPerUnit(1.0f/2.0f);
    animation->setRestoreOriginalFrame(false);
    auto action = Animate::create(animation);
    return action;
}





