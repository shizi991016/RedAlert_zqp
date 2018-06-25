#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "CHAT_MESSAGE.hpp"

extern std::vector<std::string> MessageCode;

using boost::asio::ip::tcp;

typedef std::deque<chat_massage> chat_massage_queue;

class chat_participant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const chat_massage& msg) = 0; //participant should deliver massage
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;//shared ptr

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.insert(participant);//add a client
        for (auto msg : recent_msgs_)
            participant->deliver(msg);
    }
    
    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);//remove a client
    }
    
    void deliver(const chat_massage& msg)
    {
        recent_msgs_.push_back(msg);
        
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();//delete expired massage
        //修改 尝试直接pop掉msg
        //尝试失败 不知道当前msg的具体位置无法擦除
        for (auto participant : participants_)
            participant->deliver(msg);//Ω´œ˚œ¢œÚÀ˘”–µƒ∂‘œÛ◊™∑¢“ª±È
    }
    
private:
    std::set<chat_participant_ptr> participants_;//all client
    enum { max_recent_msgs = 100 };
    chat_massage_queue recent_msgs_;
};//chat room

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
        //
        room_.join(shared_from_this());//add a participant
        
        do_read_header();
    }
    void deliver(const chat_massage& msg)
    {
        //
        bool write_in_progress = !write_msgs_.empty();//ø’
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
                                boost::asio::buffer(read_msg_.data(), chat_massage::header_length),
                                [this, self](boost::system::error_code ec, std::size_t )//lambada
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        //auto ep_ = socket_.remote_endpoint();
                                        //std::cout << "client : " << ep_.port() << " enter this room" << std::endl;
                                        do_read_body();// read body
                                    }
                                    else
                                    {
                                        auto ep_ = socket_.remote_endpoint();
                                        std::cout << "client : " << ep_.port() << " leave this room" << std::endl;
                                        room_.leave(shared_from_this());//read a eof
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
                                         //std::cout << msg << std::endl;
                                         MessageCode.push_back(msg);
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
    chat_massage read_msg_;
    chat_massage_queue write_msgs_;
};//seesion

class chat_server
{
public:
    //constructor
    chat_server(boost::asio::io_service& io_service,
                const tcp::endpoint& endpoint)
    : acceptor_(io_service, endpoint),//listen fd
    socket_(io_service)//conn fd
    {
        do_accept();
    }
    
private:
    void do_accept()
    {
        //a new connection
        acceptor_.async_accept(socket_,
                               [this](boost::system::error_code ec)
                               {
                                   if (!ec)
                                   {
                                       std::make_shared<chat_session>(std::move(socket_), room_)->start();//session
                                   }
                                   
                                   do_accept();
                               });
    }
    
    tcp::acceptor acceptor_;
    tcp::socket socket_;
    //server should keep all client connected
    chat_room room_;//chat_room class//±£¥ÊÀ˘”–µƒclient
};

/*
int main(int argc, char* argv[])
{
    try
    {
        
        boost::asio::io_service io_service;
        
        std::list<chat_server> servers;
        
        tcp::endpoint endpoint(tcp::v4(), 1024);//listen 1024 port
        servers.emplace_back(io_service, endpoint);//constructor
        
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    
    
    
    return 0;
}
*/
