#include "app.h"
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <functional>
#include <thread>

#include "World.h"

using namespace node;
using namespace std;

bool app::run() {
	//sConfig load config

	//start_db
	//enable db async process

	//register hook

	///////////////////////////////////////////////////////////////
	//new WorldThread
	std::thread worldThread(std::bind(&app::RunWorldThread, this));

	//new RemoteAccessThread if enabled

	//new SOAP thread

	//new AntiFreezeThread


	//wait worldThread

	//wait EventPool
	//delete cliThread;
	//delete freezeThread;
	//delete soapThread;
	//delete worldThread;

	server_.reset(new tcp_server(pool_, "0.0.0.0", boost::lexical_cast<std::string>(8080)));
	server_->msgHandler = std::bind(&app::on_msg, this, std::placeholders::_1, std::placeholders::_2);
	server_->connectedHandler = std::bind(&app::on_connect, this, std::placeholders::_1);
	server_->errorHandler = std::bind(&app::on_error, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	server_->start_accept();
	return true;
}

app::app(io_service_pool& pool):
	pool_(pool)
{
}

void app::stop()
{
	pool_.stop();
}

app::~app() 
{
	server_.reset();
}

void app::on_connect(tcp_connection_ptr) {
	logger::log("%s", __func__);
}

void app::on_msg(tcp_connection_ptr conn, std::string& msg) {
	//logger::log("%s", __func__);
	//cout<<*msg<<endl;
	conn->send_msg(msg.data(), msg.size());
}

void app::on_error(tcp_connection_ptr, const boost::system::error_code& error, const std::string& info) {
	logger::log("%s", __func__);
}
void app::RunWorldThread()
{
	logger::log("%s", "start world thread");

	uint32_t prevSleepTime = 0;

	while (!World::IsStopped()) {
		//++Wrold::m_worldLoopCounter;

		//std::chorn
		uint32_t diff = 0;//WorldTimer::tick();
		sWorld.Update(diff);
		if (diff <= WORLD_SLEEP_CONST + prevSleepTime) {
			prevSleepTime = WORLD_SLEEP_CONST + prevSleepTime - diff;
			//sleep;
		}
		else
			prevSleepTime = 0;
	}

	sWorld.KickAll();
	sWorld.UpdateSessions(1);
	//stop network

	//sMapMgr.UnloadAll();

	logger::log("%","stop WorldThread");
}
