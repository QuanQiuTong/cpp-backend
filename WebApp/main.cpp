#include <iostream>
#include <string>

#include <boost/json.hpp>
#include "bserv/common.hpp"
#include "router.hpp"

using namespace bserv;

static void show_config(const server_config& config)
{
	std::cout << config.get_name() << " config:"
		<< "\nport: " << config.get_port()
		<< "\nthreads: " << config.get_num_threads()
		<< "\nrotation: " << config.get_log_rotation_size() / 1024 / 1024
		<< "\nlog path: " << config.get_log_path()
		<< "\ndb-conn: " << config.get_num_db_conn()
		<< "\nconn-str: " << config.get_db_conn_str() << std::endl;
}

int main(int argc, char* argv[])
{
	server_config config;

	if (argc != 2)
	{
		std::cout << "Usage: " << config.get_name() << " [config.json]\n"
			<< config.get_name() << " is a C++ HTTP server.\n\n"
			"Example:\n  " << config.get_name() << " config.json\n" << std::endl;
		return 1;
	}
	if (argc == 2)
	{
		try
		{
			std::string config_content = utils::file::read_bin(argv[1]);
			// std::cout << config_content << std::endl;
			boost::json::object config_obj = boost::json::parse(config_content).as_object();
			if (config_obj.contains("port"))
				config.set_port((unsigned short)config_obj["port"].as_int64());
			if (config_obj.contains("thread-num"))
				config.set_num_threads((int)config_obj["thread-num"].as_int64());
			if (config_obj.contains("conn-num"))
				config.set_num_db_conn((int)config_obj["conn-num"].as_int64());
			if (config_obj.contains("conn-str"))
				config.set_db_conn_str(config_obj["conn-str"].as_string().c_str());
			if (config_obj.contains("log-dir"))
				config.set_log_path(std::string{ config_obj["log-dir"].as_string() });
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	show_config(config);

	server _(config, std::move(RouterBuilder::getInstance()._router), {});

	return 0;
}
