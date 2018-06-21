
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>
#include "CHAT_MESSAGE.hpp"

using boost::asio::ip::tcp;


typedef std::deque<chat_massage> chat_massage_queue;//载有chat_message的双端队列


class chat_participant // 一个抽象类，用于提供聊天室成员的接口
{
public:
    virtual ~chat_participant(){}
    virtual void deliver(const chat_massage& msg) = 0;
    //每一个chat_session都要重载这个函数
};


typedef std::shared_ptr<chat_participant> chat_participant_ptr;//指向class chat_participant的智能指针  typedef 简化变量声明




//==================================chat_room===============================

/**
 *   @brief  chart_session的基类\n
 *           保证每个chart_session都需要重载自己的deliver方法
 */

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
    
    void deliver(const chat_massage& msg)
    //将从某个客户端收到的消息msg 挂到 每一个客户端的write_msgs队尾 具体见chat_participant::deliver
    {
        recent_msgs_.push_back(msg);//将class chat_massage的一个实例加入双端队列中 实际就是将一个用户的message存入双端队列的队尾
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();//将最前端的一个实例释放掉，保证当前队列最多存在100条信息
        for (auto participant: participants_)//遍历set容器， 将msg发给每个聊天室成员
            participant->deliver(msg);//调用class chat_participant的deliver函数
    }
private:
    std::set<chat_participant_ptr> participants_;//set容器 类似于vector 一个载有智能指针的set容器  用set来保存用户信息 保存房间内所有参与的会话
                                                 //即储存所有class chat_participant的实例
    enum { max_recent_msgs = 100 };//当前最大信息条数100条
    chat_massage_queue recent_msgs_;//载有chat_message的双端队列  用来保存从某个客户端接收到的信息 通过deliver向set队列中所有用户
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
        do_read_header();//读取报头
    }
    
    void deliver(const chat_massage& msg)//重载deliver 发送信息 msg 需要发送的信息
    // 有几个客户端调用几次
    {
        bool write_in_progress = !write_msgs_.empty();//队列是否为空 空的则in_progress为0
        write_msgs_.push_back(msg);//将msg加入双端队列  //把room中保存的消息挂到write_msgs队尾
        if (!write_in_progress)//如果队列是空的
        {
            do_write();//进行写操作
        }
    }
    
    
private:
    tcp::socket socket_;            //socket实例
    chat_room& room_;               //聊天室
    chat_massage read_msg_;         //class chat_message的实例  存从buffer读出的数据
    chat_massage_queue write_msgs_; //class chat_message的双端队列  欲写入buffer的数据队列，deque
    
    void do_read_header()
    // 存buffer中的数据到read_msg_：header部分
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_massage::header_length),
                                //取buffer文本部分到read_msg_
                                //read_msg_.data() 调用class chat_messgae的data函数 返回class chat_messgae的data_ 即chat_message的内容
                                //异步读操作，读取buffer中message的报头内容 即开头的4位数字，表示内容共有多少
                                [this, self](boost::system::error_code ec, std::size_t)
                                //将内容写进socket
                                {
                                    if (!ec && read_msg_.decode_header())
                                        //调用decode_header保证内容长度不能超过最大长度
                                    {
                                        do_read_body();//开始读取内容
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                        //如果出错或者内容长度过大
                                        //将这条信息剔除room
                                    }
                                });
    }
    
    void do_read_body()
    // 存buffer中的数据到read_msg_：body部分
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                // 取buffer头部，正文字符数到read_msg_
                                //返回内容长度
                                [this, self](boost::system::error_code ec, std::size_t)
                                //将内容写进socket
                                {
                                    if (!ec)
                                    {
                                        room_.deliver(read_msg_);//将message实例发送
                                        do_read_header();        //重复调用，保持message一直进行
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }
    
    void do_write()
    //存入数据到write_msgs_，送队列的最开始一条发言到buffer
    {
        auto self(shared_from_this());//返回指向session的指针
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(),//队列中第一个元素调用data()
                                                     write_msgs_.front().length()),//第一个元素的报头加内容的长度
                                 [this, self](boost::system::error_code ec, std::size_t)
                                 //异步写操作，将message的内容写进socket
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();//释放第一个元素
                                         if (!write_msgs_.empty())//如果队列中还有元素，继续进行写操作
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

/*
int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: chat_server <port> [<port> ...]\n";
            return 1;
        }
        
        boost::asio::io_context io_context;
        
        std::list<chat_server> servers;
        for (int i = 1; i < argc; ++i)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }
        
        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    return 0;
}
*/


