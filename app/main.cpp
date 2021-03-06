#include "node/log.h"
#include "node/io_service_pool.h"
#include "node/program_options.h"

#include "app.h"

using namespace node;

int main(int argc, char** argv) {
	program_options::parse(argc, argv);

	logger::init(argv);
	logger::log("log test");

	io_service_pool pool(4);
	auto service(new app(pool));
	service->run();
	/*
	if (service->init())
		pool.run();
		*/
	delete service;
	return 0;
}
