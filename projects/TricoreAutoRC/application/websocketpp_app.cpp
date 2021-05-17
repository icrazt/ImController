#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>

#include "tinyframe_app.h"
#include "imc_data.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

IMC_DataClass imcdata;
// Create a server endpoint
server echo_server;

bool ws_target_connection = false;
websocketpp::connection_hdl client_hdl;

// Echo Server ,Used for connection test.
// Define a callback to handle incoming messages
//void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
//    std::cout << "on_message called with hdl: " << hdl.lock().get()
//        << " and message: " << msg->get_payload()
//        << std::endl;
//
//    // check for a special command to instruct the server to stop listening so
//    // it can be cleanly exited.
//    if (msg->get_payload() == "stop-listening") {
//        s->stop_listening();
//        return;
//    }
//
//    try {
//        s->send(hdl, msg->get_payload(), msg->get_opcode());
//    }
//    catch (websocketpp::exception const& e) {
//        std::cout << "Echo failed because: "
//            << "(" << e.what() << ")" << std::endl;
//    }
//}
void TF_WriteImpl(TinyFrame* tf, const uint8_t* buff, uint32_t len)
{
    // send by websocket
    if(ws_target_connection)
        echo_server.send(client_hdl,buff,len,websocketpp::frame::opcode::binary);
    
}

void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    ws_target_connection = true;
    client_hdl = hdl;
    //std::cout << "on_message called with hdl: " << hdl.lock().get()<< std::endl;
    imcdata.receive(msg->get_raw_payload());
    //printf("receive msg:\n");
    
}

void on_close(websocketpp::connection_hdl) {
    ws_target_connection = false;
    std::cout << "Close handler" << std::endl;
}

void ws_echo_server(void) {
    
    try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        echo_server.init_asio();

        // Register our message handler
        echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));
        //echo_server.set_close_handler(&on_close);

        // Listen on port 
        echo_server.listen(80);

        // Start the server accept loop
        echo_server.start_accept();

        // Start the ASIO io_service run loop
        echo_server.run();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
