#define BOOST_USE_WINAPI_VERSION BOOST_WINAPI_VERSION_WIN8

#include "router.h"
#include "handlers.h"
using namespace bserv;
INIT_BEGIN

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

INIT_END