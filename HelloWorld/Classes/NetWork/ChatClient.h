#ifndef ChatClient_h
#define ChatClient_h

#include <deque>
#include <iostream>
//#include <thread>
#include <boost/asio.hpp>
//#include "CHAT_MESSAGE.hpp"
#include "ChatMessage.h"


using boost::asio::ip::tcp;

typedef std::deque<ChatMessage> ChatMessage_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator)
    : io_service_(io_service),
    socket_(io_service)//≥ı ºªØ
    {
        do_connect(endpoint_iterator);
    }
    
    void write(const ChatMessage& msg)
    {
        io_service_.post(//∑¢∆“Ï≤Ω ¬º˛
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
        //∑¢∆¡¨Ω”
        boost::asio::async_connect(socket_, endpoint_iterator,
                                   [this](boost::system::error_code ec, tcp::resolver::iterator)
                                   {
                                       if (!ec)
                                       {
                                           do_read_header();//∂¡»°œ˚œ¢
                                       }
                                   });
    }
    
    void doRead()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.readMessage()),
                                [this](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        std::cout.write(read_msg.readMessage());
                                        std::cout << std::endl;
                                        doRead();
                                    }
                                    else
                                    {
                                        socket_.close();
                                    }
                                });
    }
    
    void doWrite()
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
                                             doWrite();
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
    ChatMessage read_msg_;
    ChatMessage_queue write_msgs_;
};
/*
 int main(int argc, char* argv[])
 {
 //øÕªß∂À–Ë“™¡Ω∏ˆœﬂ≥Ã
 //÷˜œﬂ≥Ã∏∫‘∂¡»°±Í◊º ‰»Î
 //threadœﬂ≥Ã∏∫‘µ˜∂»“Ï≤Ω ¬º˛(io_service.run())
 try
 {
 #if 0
 if (argc != 3)
 {
 std::cerr << "Usage: chat_client <host> <port>\n";
 return 1;
 }
 #endif
 
 boost::asio::io_service io_service;
 
 tcp::resolver resolver(io_service);
 auto endpoint_iterator = resolver.resolve({ "localhost", "1024" });
 chat_client c(io_service, endpoint_iterator);//øÕªß∂À
 
 std::thread t([&io_service]() { io_service.run(); });//∆Ù∂Øœﬂ≥Ã÷¥––io_service.run()
 
 
 char line[ChatMessage::max_body_length + 1];
 while (std::cin.getline(line, ChatMessage::max_body_length + 1))
 {
 ChatMessage msg;
 msg.body_length(std::strlen(line));//ªÒ»°header–≈œ¢
 std::memcpy(msg.body(), line, msg.body_length());
 msg.encode_header();//œ˚œ¢±‡¬Î
 c.write(msg);
 }
 
 c.close();
 t.join();
 }
 catch (std::exception& e)
 {
 std::cerr << "Exception: " << e.what() << "\n";
 }
 
 return 0;
 }
 
 */










#endif /* ChatClient_h */






