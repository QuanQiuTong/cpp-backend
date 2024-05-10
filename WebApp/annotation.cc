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
		{"code", 0},
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
    {"code", 0},
    {"message", "annotation deleted"}
  };
}

bserv::json::object count_annotations(
  bserv::request_type& request,
  boost::json::object&& params,
  std::shared_ptr<bserv::db_connection> conn)
{
    auto pid = get_int(params, "problem_id");
    auto uid = get_int(params, "user_id");
    std::string order_by = params.at("order_by").as_string().c_str();
    std::string order = params.at("order").as_string().c_str();

    std::string query = "select count(*) from annotation ";
    if (pid != 0 && uid != 0)
        query += "where problem_id = " + std::to_string(pid) + " and user_id = " + std::to_string(uid);
    else if (pid != 0)
        query += "where problem_id = " + std::to_string(pid);
    else if (uid != 0)
        query += "where user_id = " + std::to_string(uid);

    bserv::db_transaction tx{ conn };
    auto count = tx.exec(query).front()[0].as<size_t>();
    auto count1 = tx.exec(
        "select count(*) from annotation "
        "where judgement like '%1%' "
        + (pid != 0 ? "and problem_id = " + std::to_string(pid) : std::string{" "})
        + (uid != 0 ? "and user_id = " + std::to_string(uid) : std::string{" "})
    ).front()[0].as<size_t>();
    auto count2 = tx.exec(
        "select count(*) from annotation "
        "where judgement like '%2%' "
        + (pid != 0 ? "and problem_id = " + std::to_string(pid) : std::string{ " " })
        + (uid != 0 ? "and user_id = " + std::to_string(uid) : std::string{ " " })
    ).front()[0].as<size_t>();

    return { {"count", count}, {"count1", count1}, {"count2", count2} };
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
    auto id = std::stoll(page_id);

    auto pid = get_int(params, "problem_id");
    auto uid = get_int(params, "user_id");
    std::string order_by = params.at("order_by").as_string().c_str();
    std::string order = params.at("order").as_string().c_str();

    std::string query = "select * from annotation ";
    if(pid != 0 && uid != 0)
		query += "where problem_id = " + std::to_string(pid) + " and user_id = " + std::to_string(uid);
	else if(pid != 0)
		query += "where problem_id = " + std::to_string(pid);
	else if(uid != 0)
		query += "where user_id = " + std::to_string(uid);

    query += " order by " + order_by + " " + order + " limit 10 offset " + std::to_string((id - 1) * 10);

	bserv::db_transaction tx{conn};
    auto res = tx.exec(query);

	return orm_annotation.convert_to_array(res);
}

INIT_BEGIN
using namespace bserv::placeholders;
RouterBuilder::add_path("/annotation", &annotation, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/del-annotation", &del_annotation, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/count-annotations", &count_annotations, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/annotations", &annotations, request, json_params, db_connection_ptr, std::string{"1"});
RouterBuilder::add_path("/annotations/<int>", &annotations, request, json_params, db_connection_ptr, bserv::placeholders::_1);
INIT_END