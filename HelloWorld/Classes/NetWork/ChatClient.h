#ifndef ChatClient_h
#define ChatClient_h

#include "cocos2d.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <fstream>
#include "ChatMessage.h"
#include <vector>
#include <string>



using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
    socket_(io_service)
    {
        do_connect(endpoint_iterator);
    }
    
    void write(const chat_message& msg)
    {
        io_service_.post(
                         [this, msg]()
                         {
                             bool write_in_progress = !write_msgs_.empty();
                             write_msgs_.push_back(msg);
                             if (!write_in_progress)
                             {
                                 do_write();
                             }
                         });
    }
    
    void close()
    {
        io_service_.post([this]() { socket_.close(); });
    }
    
private:
    void do_connect(tcp::resolver::iterator endpoint_iterator)
    {
        boost::asio::async_connect(socket_, endpoint_iterator,
                                   [this](boost::system::error_code ec, tcp::resolver::iterator)
                                   {
                                       if (!ec)
                                       {
                                           do_read_header();
                                       }
                                   });
    }
    
    void do_read_header()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }
    
    void do_read_body()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        std::cout.write(read_msg_.body(), read_msg_.body_length());
                                        std::cout << "\n";
                                        do_read_header();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }
    
    void do_write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),
                                                     write_msgs_.front().length()),
                                 [this](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                         }
                                     }
                                     else
                                     {
                                         socket_.close();
                                     }
                                 });
    }
    
private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};

//----------------------------------------------------------------------

/**
 *   @brief  搜索并连接server\n
 *           分两种工作模式，在局域网下负责搜索并连接局域网内\n
 *           的server，在互联网模式下则直接连接远程server
 */
class Client : public cocos2d::Node
{
public:
    
    bool                        _search_finished;       ///是否完成搜索并于server建立起连接
    bool                        _filter_mode;           ///是否开启filter mode
    bool                        _with_server;           ///client启动时是否有server同步启动
    chat_client*                _clientInstance;        ///用于处理数据的接受和发送
    std::mutex                  t_lock;                 ///线程锁，避免多个线程下对数据进行同时操作
    std::string                 sensitive_word;         ///filter mode下用于甄别指令的敏感词
    std::deque<std::string>     _orderList;             ///读取到的正确指令
    
    /**
     * @brief    创建client对象并运行
     */
    static Client* create(int mode)
    {
        Client* sprite = new Client();
        if (sprite)
        {
            sprite->autorelease();
            sprite->runClient(mode);
            
            return sprite;
        }
        CC_SAFE_DELETE(sprite);
        return nullptr;
    }
    
    /**
     * @brief    运行client
     *
     * @param    mode         运行client的模式
     *        -<em>1</em>    同时与server启动
     *        -<em>2/em>     只启动client
     *
     */
    void runClient(int mode);
    
    /**
     * @brief    取出读取到的指令
     *
     * @return   读取到并经过过滤的指令
     *        -<em>"no"</em>             没有更多指令
     *        -<em>std::string</em>      正确指令
     */
    std::string executeOrder (void);
    
    /**
     * @brief    运行client
     *
     * @param    code        指令码
     * @param    message     要发送的指令
     *
     */
    void sendMessage(const std::string & code, const std::string & message);
    
    /**
     * @brief    启动client并搜索、连接server
     *
     * @return   是否正常结束
     */
    int client(void);
};


#endif /* ChatClient_h */
