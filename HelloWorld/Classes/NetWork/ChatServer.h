#ifndef ChatServer_h
#define ChatServer_h


//#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "ChatMessage.h"

using boost::asio::ip::tcp;


typedef std::deque<ChatMessage> ChatMessage_queue;//载有chat_message的双端队列


class chat_participant // 一个抽象类，用于提供聊天室成员的接口
{
public:
    virtual ~chat_participant(){}
    virtual void deliver(const ChatMessage& msg) = 0;
    //每一个chat_session都要重载这个函数
};


typedef std::shared_ptr<chat_participant> chat_participant_ptr;//指向class chat_participant的智能指针  typedef 简化变量声明




//==================================chat_room===============================

class chat_room //聊天室
//将新加入的会话加入房间内以便以 广播 的方式发送数据
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);//insert set容器的函数，insert(key_value); 将key_value插入到set中,将智能指针插入容器中
        for (auto msg: recent_msgs_)//遍历双端队列
        {
            participant->deliver(msg);//调用class chat_participant的deliver函数
        }
    }
    
    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);//调用set的函数erase从set中擦除这个智能指针
    }
    
    void deliver(const ChatMessage& msg)
    //将从某个客户端收到的消息msg 挂到 每一个客户端的write_msgs队尾 具体见chat_participant::deliver
    {
        recent_msgs_.push_back(msg);//将class ChatMessage的一个实例加入双端队列中 实际就是将一个用户的message存入双端队列的队尾
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();//将最前端的一个实例释放掉，保证当前队列最多存在100条信息
        for (auto participant: participants_)//遍历set容器， 将msg发给每个聊天室成员
            participant->deliver(msg);//调用class chat_participant的deliver函数
    }
private:
    std::set<chat_participant_ptr> participants_;//set容器 类似于vector 一个载有智能指针的set容器  用set来保存用户信息 保存房间内所有参与的会话
    //即储存所有class chat_participant的实例
    enum { max_recent_msgs = 20 };//当前最大信息条数100条  改为20条
    ChatMessage_queue recent_msgs_;//载有chat_message的双端队列  用来保存从某个客户端接收到的信息 通过deliver向set队列中所有用户
    //即所有的class chat_participant实例 发送消息
};


//==================================chat_room===============================






//==================================chat_session===============================

/**
 *   @brief  会话类\n
 *           继承自chat_participant类，需要自定义发送消息的方法
 */

// 在聊天室环境下，一个session就是一个成员
class chat_session : public chat_participant, public std::enable_shared_from_this<chat_session>
//public之后的为继承，session继承了participant的deliver 用来重载   std::enable_shared_from_this 智能指针 用来返回一个指向自身的指针
{
public:
    /**
     * 构造函数
     * 初始化会话，此时并不加入房间，只有成功链接时才加入
     * socket    直接使用socket实例，不在用io_service创建socket
     * room      房间对象
     * std::move 函数可以以非常简单的方式将左值引用转换为右值引用
     * 左值是可以放在赋值号左边可以被赋值的值；左值必须要在内存中有实体；
     * 右值当在赋值号右边取出值赋给其他变量的值；右值可以在内存也可以在CPU寄存器。
     * 一个对象被用作右值时，使用的是它的内容(值)，被当作左值时，使用的是它的地址。
     * std::move是将对象的状态或者所有权从一个对象转移到另一个对象，只是转移，没有内存的搬迁或者内存拷贝。
     */
    chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)) , room_(room)
    {
    }
    
    void start()
    {
        room_.join(shared_from_this());//将指向session的指针加入room的客户双端队列
        doRead();//读取报头
    }
    
    void deliver(const ChatMessage& msg)//重载deliver 发送信息 msg 需要发送的信息
    // 有几个客户端调用几次
    {
        bool write_in_progress = !write_msgs_.empty();//队列是否为空 空的则in_progress为0
        write_msgs_.push_back(msg);//将msg加入双端队列  //把room中保存的消息挂到write_msgs队尾
        if (!write_in_progress)//如果队列是空的
        {
            doWrite();//进行写操作
        }
    }
    
    
private:
    tcp::socket socket_;            //socket实例
    chat_room& room_;               //聊天室
    ChatMessage read_msg_;         //class chat_message的实例  存从buffer读出的数据
    ChatMessage_queue write_msgs_; //class chat_message的双端队列  欲写入buffer的数据队列，deque
    
    void doRead()
    {
        auto self(shared_from_this());//返回指向session的指针
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(read_msg_.readMessage()),
                                 //队列中第一个元素调用data()
                                 [this, self](boost::system::error_code ec, std::size_t)
                                 //异步写操作，将message的内容写进socket
                                 {
                                     if (!ec)
                                     {
                                         doRead();
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }
    
    
    void doWrite()
    {
        auto self(shared_from_this());//返回指向session的指针
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().readMessage()),
                                 //队列中第一个元素调用data()
                                 [this, self](boost::system::error_code ec, std::size_t)
                                 //异步写操作，将message的内容写进socket
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();//释放第一个元素
                                         if (!write_msgs_.empty())//如果队列中还有元素，继续进行写操作
                                         {
                                             doWrite();
                                         }
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }
};


class chat_server
{
public:
    chat_server(boost::asio::io_context& io_context,
                const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
    {
        do_accept();
    }
    
private:
    void do_accept()
    {
        acceptor_.async_accept(
                               [this](boost::system::error_code ec, tcp::socket socket)
                               {
                                   if (!ec)
                                   {
                                       std::make_shared<chat_session>(std::move(socket), room_)->start();
                                   }
                                   
                                   do_accept();
                               });
    }
    
    tcp::acceptor acceptor_;
    chat_room room_;
};

class Server : public cocos2d::Node
{
public:
    void runConnection()
    {
        tcp::endpoint endpoint(tcp::v4(), 6688);
        servers.emplace_back(io_context, endpoint);
        io_context.run();
    }
private:
    boost::asio::io_context io_context;
    std::list<chat_server> servers;
};






#endif /* ChatServer_h */










