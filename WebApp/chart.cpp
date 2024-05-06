#define BOOST_USE_WINAPI_VERSION BOOST_WINAPI_VERSION_WIN8

#include <boost/json.hpp>
#include <memory>

#include "bserv/common.hpp"
#include "router.h"
using namespace bserv;
using boost::json::object;

object f(request_type& request, object&& params, std::shared_ptr<db_connection> conn)
{
	if (request.method() != boost::beast::http::verb::post) {
		throw bserv::url_not_found_exception{};
	}
	return {
		{"status", "ok"},
		{"message", "chart"}
	};
}

INIT_BEGIN
	add_router({make_path("/chart", &f, placeholders::request, placeholders::json_params, placeholders::db_connection_ptr)});
INIT_END