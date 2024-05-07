#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602 // Windows 8
#endif

#include "router.hpp"
#include "bserv/common.hpp"

#include <boost/json.hpp>
#include <memory>
#include <string>

/*CREATE TABLE annotation(
    id SERIAL PRIMARY KEY,
    problem_id int not null,
    user_id int not null,

    judgement VARCHAR not null,
    reason text not null,

    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (problem_id) REFERENCES problems(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);*/

static __int64 get_int(const boost::json::object& obj, const std::string& key)
{
    auto field = obj.at(key);
    if(field.is_int64())
		return field.as_int64();
	else if(field.is_string())
		return std::atoll(field.as_string().c_str());
	else
		throw std::runtime_error("invalid type for " + key);
}

boost::json::object annotation(
	bserv::request_type& request,
	boost::json::object&& params,
	std::shared_ptr<bserv::db_connection> conn)
{
    if (request.method() != boost::beast::http::verb::post)
		throw bserv::url_not_found_exception{};
	
    bserv::db_transaction tx{conn};
    tx.exec("insert into annotation(problem_id, user_id, judgement, reason) "
        "values(?, ?, ?, ?)",
        get_int(params, "problem_id"),
        get_int(params, "user_id"),
        params["judgement"].as_string(),
        params["reason"].as_string());
    tx.commit();
    return {
		{"status", "ok"},
		{"message", "annotation added"}
	};
}

boost::json::object del_annotation(
  bserv::request_type& request,
  boost::json::object&& params,
  std::shared_ptr<bserv::db_connection> conn)
{
    if (request.method() != boost::beast::http::verb::post)
    throw bserv::url_not_found_exception{};
  
    bserv::db_transaction tx{conn};
    tx.exec("delete from annotation where id = ?", get_int(params, "id"));
    tx.commit();
    return {
    {"status", "ok"},
    {"message", "annotation deleted"}
  };
}


INIT_BEGIN
using namespace bserv::placeholders;
RouterBuilder::add_path("/annotation", &annotation, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/del-annotation", &del_annotation, request, json_params, db_connection_ptr);
INIT_END