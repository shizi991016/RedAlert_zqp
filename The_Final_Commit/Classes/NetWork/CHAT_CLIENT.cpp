#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "CHAT_MESSAGE.hpp"
#include <vector>

extern std::vector<std::string> MyOrderList;
extern std::vector<std::string> EnemyOrderList;
using boost::asio::ip::tcp;

typedef std::deque<chat_massage> chat_massage_queue;

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
    
    void write(const chat_massage& msg)
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
                                           do_read_header();//∂¡»°œ˚œ¢
                                       }
                                   });
    }
    
    void do_read_header()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.data(), chat_massage::header_length),
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
                                        std::string order = read_msg_.body();
                                        EnemyOrderList.push_back(order);
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
    chat_massage read_msg_;
    chat_massage_queue write_msgs_;
};










