#ifndef BaseAndFlags_h
#define BaseAndFlags_h

#include "ArmyClass.h"

class BaseAndFlags
{
public:
    int IsPositionHaveBuildings[3201][3201] = {0};
    int IsPositionHaveMiningYard[3201][3201] = {0};
    
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
    std::vector<ElectricPowerPlantClass*> MyElectricPowerPlant;
    std::vector<RefineryClass*> MyRefinery;
    std::vector<BarracksClass*> MyBarracks;
    std::vector<WarFactoryClass*> MyWarFactory;
private:
    
};



#endif /* BaseAndFlags_h */
