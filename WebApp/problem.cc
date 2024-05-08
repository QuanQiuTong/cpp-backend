// 参见 boost/log/detail/config.hpp
#define BOOST_USE_WINAPI_VERSION BOOST_WINAPI_VERSION_WIN8

#include <boost/json.hpp>
#include <memory>
#include <string>

#include "bserv/common.hpp"
#include "router.hpp"

using namespace bserv;
using boost::json::object;
using dbptr = std::shared_ptr<db_connection>;


#include <boost/algorithm/string.hpp>

static auto filtered_field(const object& o, const char* key)
{
	std::string field = o.at(key).as_string().c_str();
	boost::algorithm::replace_all(field, "'", "''");
	return field;
}

#include <iostream>

static __int64 get_int(const boost::json::object& obj, const std::string& key)
{
	if (!obj.contains(key))
	{
		std::cerr << "missing field " << key << std::endl;
		throw std::runtime_error("missing field " + key);
	}
	auto field = obj.at(key);
	if (field.is_int64())
		return field.as_int64();
	else if (field.is_string())
		return std::atoll(field.as_string().c_str());
	else
	{
		std::cerr << "invalid type for " << key << std::endl;
		throw std::runtime_error("invalid type for " + key);
	}
}


/*create table problems(
	id SERIAL primary key,

	category varchar(255) not null,
	personality TEXT not null,
	history TEXT not null,
	problem TEXT not null,
	solution1 TEXT not null,
	solution2 TEXT not null
);
 insert into problems(catogeory, personality, history, problem, solution1, solution2)
 values('category', 'personality', 'history', 'problem', 'solution1', 'solution2');

select * from problems;
*/
db_relation_to_object orm_problem{
	make_db_field<int>("id"),
	make_db_field<std::string>("category"),
	make_db_field<std::string>("personality"),
	make_db_field<std::string>("history"),
	make_db_field<std::string>("problem"),
	make_db_field<std::string>("solution1"),
	make_db_field<std::string>("solution2"),
	// make_db_field<std::string>("created_at")
};

static boost::json::array get_problems(dbptr conn)
{
	db_transaction tx{conn};
	db_result r = tx.exec("select * from problems order by id asc");
	lginfo << r.query();
	return orm_problem.convert_to_array(r);
}

static object count_problems(dbptr conn) {
	db_transaction tx{ conn };
	db_result r = tx.exec("select count(*) from problems");
	return { { "count",r.front()[0].as<size_t>()} };
}

static object get_problem(dbptr conn, const std::string& id)
{
	db_transaction tx{ conn };
	db_result r = tx.exec("SELECT * FROM problems ORDER BY id LIMIT 1 OFFSET ?;", std::stoi(id));
	tx.commit();
	lginfo << r.query();
	if (r.empty())
		throw url_not_found_exception{};
	return orm_problem.convert_row(r.front());
}

static object set_problem(request_type& req, object&& params, dbptr conn)
{
	if (req.method() != boost::beast::http::verb::post)
		throw url_not_found_exception{};

	auto id = get_int(params, "id");
	auto category = filtered_field(params, "category");
	auto personality = filtered_field(params, "personality");
	auto history = filtered_field(params, "history");
	auto problem = filtered_field(params, "problem");
	auto solution1 = filtered_field(params, "solution1");
	auto solution2 = filtered_field(params, "solution2");

	db_transaction tx{ conn };
	db_result r = tx.exec(
		"update problems set category = ?, personality = ?, history = ?, problem = ?, solution1 = ?, solution2 = ? where id = ?",
		category, personality, history, problem, solution1, solution2, id);
	lginfo << r.query();
	tx.commit();

	return {
		{"code", 0},
		{"message", "update success"} };
}	

static object add_problem(request_type &req, object &&params, dbptr conn)
{
	if (req.method() != boost::beast::http::verb::post)
		throw url_not_found_exception{};

		auto category = filtered_field(params, "category");
		auto personality = filtered_field(params, "personality");
		auto history = filtered_field(params, "history");
		auto problem = filtered_field(params, "problem");
		auto solution1 = filtered_field(params, "solution1");
		auto solution2 = filtered_field(params, "solution2");

		db_transaction tx{conn};
		db_result r = tx.exec(
			"insert into problems (category, personality, history, problem, solution1, solution2) "
			"values (?, ?, ?, ?, ?, ?)",
			category, personality, history, problem, solution1, solution2);
		lginfo << r.query();
		tx.commit();

	return {
		{"code", 0},
		{"message", "insert success"}};
}

static object del_problem(request_type &req, object &&params, dbptr conn)
{
	if (req.method() != boost::beast::http::verb::post)
		throw url_not_found_exception{};

	std::cout<<"del_problem"<<std::endl;
	auto id = get_int(params, "id");
	std::cout<<"id:"<<id<<std::endl;

	db_transaction tx{conn};
	tx.exec("delete from annotation where problem_id = ?", id); // foreign key
	tx.exec("delete from problems where id = ?", id);
	tx.commit();

	return {
		{"code", 0},
		{"message", "delete success"}};
}

// constexpr int PAGE_SIZE = 10;
// constexpr const char PAGE_SIZE_STR[] = "10";
// static boost::json::array get_problems_page(dbptr conn, const std::string &page_num)
// {
// 	int page_id = std::stoi(page_num);
// 	db_transaction tx{conn};

// 	// 查询总的问题数量
// 	db_result total_count_res = tx.exec("select count(*) from problems;");
// 	lginfo << total_count_res.query();
// 	std::size_t total_problems = (*total_count_res.begin())[0].as<std::size_t>();
// 	lgdebug << "total problems: " << total_problems << std::endl;

// 	// 计算总页数
// 	int total_pages = int(total_problems / PAGE_SIZE + !!(total_problems % PAGE_SIZE));
// 	lgdebug << "total pages: " << total_pages << std::endl;

// 	// 查询分页后的问题列表
// 	db_result problems_res = tx.exec(
// 		"select * from problems limit ? offset ?;",
// 		PAGE_SIZE, (page_id - 1) * PAGE_SIZE);
// 	lginfo << problems_res.query();

// 	// 转换为问题对象列表
// 	return orm_problem.convert_to_array(problems_res);
// }

INIT_BEGIN
using namespace bserv::placeholders;
RouterBuilder::add_path("/count-problems", &count_problems, db_connection_ptr);
RouterBuilder::add_path("/problems", &get_problems, db_connection_ptr);
RouterBuilder::add_path("/problem", &get_problem, db_connection_ptr, std::string{"1"});
RouterBuilder::add_path("/problem/<int>", &get_problem, db_connection_ptr, _1);
RouterBuilder::add_path("/set-problem", &set_problem, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/add-problem", &add_problem, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/del-problem", &del_problem, request, json_params, db_connection_ptr);

// RouterBuilder::add_path("/page-problems", &get_problems_page, db_connection_ptr, std::string{"1"});
// RouterBuilder::add_path("/page-problems/<int>", &get_problems_page, db_connection_ptr, _1);
INIT_END