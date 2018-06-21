#ifndef BaseAndFlags_h
#define BaseAndFlags_h

#include "ArmyClass.h"

class BaseAndFlags
{
public:
    char IsPositionHaveBuildings[1601][1601] = {0};
    char IsPositionHaveMiningYard[1601][1601] = {0};
    
    int MyEdgeSelection;
    
    int MyCountryChoice = 0;
    
    int ElectricPowerPlantNumber = 0;
    int RefineryNumber = 0;
    int BarracksNumber = 0;
    int WarFactoryNumber = 0;
    
    int MyMoney = 1000;
    
    cocos2d::Point TheLastBarracksPosition;
    cocos2d::Point TheLastWarFactoryPosition;
    
    cocos2d::Point LastTouchPosition;
    bool IsTouchPositionAvailable = 0;
    
    std::vector<BuildingsClass*> MyBuildings;//用于在update函数中遍历数组，HP<=0删除
    std::vector<BuildingsClass*> MyElectricPowerPlant;
    std::vector<BuildingsClass*> MyRefinery;
    std::vector<BuildingsClass*> MyBarracks;
    std::vector<BuildingsClass*> MyWarFactory;
    std::vector<BuildingsClass*> MyMissileWell;
    
    std::vector<BuildingsClass*> MyTank;
    std::vector<BuildingsClass*> MyBattlePlane;
    std::vector<BuildingsClass*> MySoldier;
    std::vector<BuildingsClass*> MyJet;
    std::vector<BuildingsClass*> MyTTank;
    std::vector<BuildingsClass*> MyRTank;
    std::vector<BuildingsClass*> MySoldierX;
    std::vector<BuildingsClass*> MyCannon;
    
    Point ArmyFirstTouchPosition;
    Point ArmyLastTouchPosition;
    
    int TagNumber = 0;
    int PositionTag[1601][1601] ={0};
    int IsPositionHaveArmyAndTag[1601][1601] = {0};
    
    BuildingsClass* MyBase;
    
};



#endif /* BaseAndFlags_h */
