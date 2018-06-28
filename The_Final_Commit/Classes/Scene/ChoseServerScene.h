#ifndef ChoseServerScene_h
#define ChoseServerScene_h

#include "extensions/cocos-ext.h"
#include "cocos2d.h"

using namespace cocos2d::extension;
USING_NS_CC;

extern std::string IpAddress;


class ChoseServerScene : public cocos2d::Scene , public cocos2d::ui::EditBoxDelegate
{
public:
    
    Size VisibleSize;
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void clientOneCallBack(cocos2d::Ref* pSender);
    
    void clientTwoCallBack(cocos2d::Ref* pSender);
    
    CREATE_FUNC(ChoseServerScene);
    
    virtual void editBoxReturn(EditBox* editBox)
    {
        IpAddress = editBox->getText();
    }
    
    
};

#endif /* ChoseServerScene_h */
