#include <iostream>
#include <cstdlib>
#include <string>

#include <boost/json.hpp>
#include "bserv/common.hpp"

#include "router.h"
#include "rendering.h"
#include "handlers.h"

using namespace bserv;

void show_usage(const server_config &config)
{
	std::cout << "Usage: " << config.get_name() << " [config.json]\n"
			  << config.get_name() << " is a C++ HTTP server.\n\n"
									  "Example:\n"
			  << "  " << config.get_name() << " config.json\n\n"
			  << std::endl;
}

void show_config(const server_config &config)
{
	std::cout << config.get_name() << " config:"
			  << "\nport: " << config.get_port()
			  << "\nthreads: " << config.get_num_threads()
			  << "\nrotation: " << config.get_log_rotation_size() / 1024 / 1024
			  << "\nlog path: " << config.get_log_path()
			  << "\ndb-conn: " << config.get_num_db_conn()
			  << "\nconn-str: " << config.get_db_conn_str() << std::endl;
}

int main(int argc, char *argv[])
{
	server_config config;

	if (argc != 2)
	{
		show_usage(config);
		return EXIT_FAILURE;
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
				config.set_log_path(std::string{config_obj["log-dir"].as_string()});
			if (!config_obj.contains("template_root"))
			{
				std::cerr << "`template_root` must be specified" << std::endl;
				return EXIT_FAILURE;
			}
			else
				init_rendering(config_obj["template_root"].as_string().c_str());
			if (!config_obj.contains("static_root"))
			{
				std::cerr << "`static_root` must be specified" << std::endl;
				return EXIT_FAILURE;
			}
			else
				init_static_root(config_obj["static_root"].as_string().c_str());
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
			return EXIT_FAILURE;
		}
	}
	show_config(config);

	RouterBuilder::add({
		// rest api example
		make_path("/hello", &hello,
				  placeholders::response,
				  placeholders::session),

				  // GET request
		make_path("/register", &user_register,
				  placeholders::request,
				  placeholders::json_params,
				  placeholders::db_connection_ptr),

				  // POST request. Just ignore "session".
		make_path("/login", &user_login,
				  placeholders::request,
				  placeholders::json_params,
				  placeholders::db_connection_ptr,
				  placeholders::session),
		make_path("/logout", &user_logout,
				  placeholders::session),
		make_path("/find/<str>", &find_user,
				  placeholders::db_connection_ptr,
				  placeholders::_1),
		make_path("/send", &send_request,
				  placeholders::session,
				  placeholders::http_client_ptr,
				  placeholders::json_params),
		make_path("/echo", &echo,
				  placeholders::json_params),

		// serving static files
		make_path("/statics/<path>", &serve_static_files,
				  placeholders::response,
				  placeholders::_1),

		// serving html template files
		make_path("/", &index_page,
				  placeholders::session,
				  placeholders::response),
		make_path("/form_login", &form_login,
				  placeholders::request,
				  placeholders::response,
				  placeholders::json_params,
				  placeholders::db_connection_ptr,
				  placeholders::session),
		make_path("/form_logout", &form_logout,
				  placeholders::session,
				  placeholders::response),
		make_path("/users", &view_users,
				  placeholders::db_connection_ptr,
				  placeholders::session,
				  placeholders::response,
				  std::string{"1"}),
		make_path("/users/<int>", &view_users,
				  placeholders::db_connection_ptr,
				  placeholders::session,
				  placeholders::response,
				  placeholders::_1),
		make_path("/form_add_user", &form_add_user,
				  placeholders::request,
				  placeholders::response,
				  placeholders::json_params,
				  placeholders::db_connection_ptr,
				  placeholders::session),
	});

	auto _ = server{config, std::move(RouterBuilder::getInstance()._router),
										   {/*websocket example*/make_path("/echo", &ws_echo, placeholders::session, placeholders::websocket_server_ptr)}};

	return EXIT_SUCCESS;
}
