#ifndef BaseAndFlags_h
#define BaseAndFlags_h


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
    
private:
    
};



#endif /* BaseAndFlags_h */
