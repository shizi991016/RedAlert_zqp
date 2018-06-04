#ifndef BaseAndFlags_h
#define BaseAndFlags_h

#include "ArmyClass.h"

class BaseAndFlags
{
public:
    int IsPositionHaveBuildings[3201][3201] = {0};
    int IsPositionHaveMiningYard[3201][3201] = {0};
    
    int MyEdgeSelection;
    
    int ElectricPowerPlantNumber;
    int RefineryNumber;
    int BarracksNumber;
    int WarFactoryNumber;
    
    cocos2d::Point TheLastBarracksPosition;
    cocos2d::Point TheLastWarFactoryPosition;
    
    cocos2d::Point LastTouchPosition;
    bool IsTouchPositionAvailable = 0;
    
    std::vector<BuildingsClass*> MyBuildings;//用于在update函数中遍历数组，HP<=0删除
    
private:
    
};



#endif /* BaseAndFlags_h */
