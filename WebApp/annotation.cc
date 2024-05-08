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

    judgement VARCHAR(255) not null,
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

bserv::json::object count_annotations(
  std::shared_ptr<bserv::db_connection> conn)
{
	bserv::db_transaction tx{conn};
	auto res = tx.exec("select count(*) from annotation");
	return { {"count", res.front()[0].as<size_t>()}};
}

bserv::db_relation_to_object orm_annotation{
    bserv::make_db_field<int>("id"),
    bserv::make_db_field<int>("problem_id"),
    bserv::make_db_field<int>("user_id"),
    bserv::make_db_field<std::string>("judgement"),
    bserv::make_db_field<std::string>("reason"),
    bserv::make_db_field<std::string>("created_at")
};

boost::json::array annotations(
  bserv::request_type& request,
  boost::json::object&& params,
  std::shared_ptr<bserv::db_connection> conn,
  std::string page_id)
{
	if (request.method() != boost::beast::http::verb::get)
	throw bserv::url_not_found_exception{};
  
    auto id = std::stoll(page_id);

	bserv::db_transaction tx{conn};
    auto res = tx.exec("select * from annotation order by id desc limit 10 offset ?", (id - 1) * 10);

	return orm_annotation.convert_to_array(res);
}

INIT_BEGIN
using namespace bserv::placeholders;
RouterBuilder::add_path("/annotation", &annotation, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/del-annotation", &del_annotation, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/count-annotations", &count_annotations, db_connection_ptr);
RouterBuilder::add_path("/annotations", &annotations, request, json_params, db_connection_ptr, std::string{"1"});
RouterBuilder::add_path("/annotations/<int>", &annotations, request, json_params, db_connection_ptr, bserv::placeholders::_1);
INIT_END