# RedAlert_zqp
赵姐姐班大项目红警

最终的mac版本的Classes文件在 The_Final_Commit文件夹里
Resouce文件在HelloWorld文件夹下的Resource里

我们修改了cocos引擎源代码的CCDirector.h文件
增加了一个切换屏幕with特效的函数
请将CCDirector.h覆盖原文件
路径为/cocos2d/cocos/base/CCDirector.h

服务器启动文件为 NetText！
游戏启动文件为 RedAlert_desktop

6月25日上午更新：
将ChatServer放入程序中
具体打开方式见pdf

6月26日晚上更新
提供网络联机 client 1 启动 ip输入127.0.0.1
client 2 启动 ip输入在client 1 电脑cmd由ipconfig
（mac为 ifconfig）获得的ip地址
可进行网络联机

6月28日更新
win系统增加背景音乐调节函数
SimpleAudioEngine.cpp，MciPlayer.h ，MciPlayer.cpp  位于 cocos2d\cocos\audio\win32 
SimpleAudioEngine.h  位于 cocos2d\cocos\audio\include 
增加作弊代码，在聊天框中输入money增加500$ 

分值分配：
1753390 130%
其余 90%
