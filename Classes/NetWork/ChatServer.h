#ifndef ChatServer_h
#define ChatServer_h

#include "cocos2d.h"
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread/thread.hpp>
#include "ChatMessage.h"
#pragma warning(disable:4996)
using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);
        
        for (auto msg : recent_msgs_)
            participant->deliver(msg);
    }
    
    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }
    
    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();
        
        for (auto participant : participants_)
            participant->deliver(msg);
    }
    
private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_queue recent_msgs_;
};

class chat_session :
public chat_participant,
public std::enable_shared_from_this<chat_session>
{
public:
    chat_session(tcp::socket socket, chat_room& room)
    : socket_(std::move(socket)),
    room_(room)
    {
    }
    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }
    void deliver(const chat_message& msg)
    {
        //
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }
private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        auto ep_ = socket_.remote_endpoint();
                                        std::cout << "client : " << ep_.port() << " leave this room" << std::endl;
                                        
                                        room_.leave(shared_from_this());
                                    }
                                });
    }
    
    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);
                                        do_read_header();//
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }
    
    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         std::string msg(write_msgs_.front().data(), write_msgs_.front().length());
                                         std::cout << msg << std::endl;
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         room_.leave(shared_from_this());
                                     }
                                 });
    }
    
    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
    : acceptor_(io_service, endpoint),
    socket_(io_service)
    {
        do_accept();
    }
    
private:
    void do_accept()
    {
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       auto ep_ = socket_.remote_endpoint();
                                       std::cout << "client : " << ep_.port() << " enter this room" << std::endl;
                                       std::make_shared<chat_session>(std::move(socket_), room_)->start();//session
                                       
                                   }
                                   
                                   do_accept();
                               });
    }
    
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    chat_room room_;
};

//==================================LocalServer===============================
typedef boost::shared_ptr<chat_server>  chat_server_ptr;
typedef std::list<chat_server_ptr>      chat_server_list;

/**
 *   @brief  LocalServer类，继承自Node类，可被cocos内存管理系统自动管理\n
 */
class LocalServer : public cocos2d::Node
{
public:
    /**
     * @brief    创建LocalServer对象并运行server
     */
    static LocalServer* create(void)
    {
        LocalServer* sprite = new LocalServer();
        if (sprite)
        {
            sprite->autorelease();
            sprite->runServer();
            
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    /**
     * @brief    开启一个新线程，使server驻留后台运行
     */
    void runServer(void)
    {
        std::thread t(&LocalServer::server, this);
        t.detach();
    }
    
    /**
     * @brief    在分离线程中运行server\n
     *           只要io_service不被关闭，该线程就会在后台持续运行，直到主线程结束后被清理
     */
    int server(void)
    {
        try
        {
            boost::asio::io_service io_service;
            chat_server_list servers;
            
            using namespace std;
            tcp::endpoint endpoint(tcp::v4(), 11332);
            chat_server_ptr server(new chat_server(io_service, endpoint));
            servers.push_back(server);
            
            io_service.run();
            
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
        }
        
        return 0;
    }
    
};


#endif

