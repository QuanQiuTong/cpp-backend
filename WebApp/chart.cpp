// 参见 boost/log/detail/config.hpp
#define BOOST_USE_WINAPI_VERSION BOOST_WINAPI_VERSION_WIN8

#include <boost/json.hpp>
#include <memory>
#include <string>

#include "bserv/common.hpp"
#include "router.h"

using std::string;
using namespace bserv;
using boost::json::object;
using dbptr = std::shared_ptr<db_connection>;

static object f(request_type &request, object &&params, dbptr conn)
{
	if (request.method() != boost::beast::http::verb::post)
	{
		throw url_not_found_exception{};
	}
	return {
		{"params", params},
		{"status", "ok"},
		{"message", "chart"}};
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
	make_db_field<string>("category"),
	make_db_field<string>("personality"),
	make_db_field<string>("history"),
	make_db_field<string>("problem"),
	make_db_field<string>("solution1"),
	make_db_field<string>("solution2"),
	make_db_field<string>("created_at")};

static boost::json::array get_problems(dbptr conn)
{
	db_transaction tx{conn};
	db_result r = tx.exec("select * from problems");
	lginfo << r.query();
	return orm_problem.convert_to_array(r);
}

#include <iostream>
using std::cout, std::endl;
#include <boost/algorithm/string.hpp>

static auto filtered_field(const object &o, const char *key)
{
	boost::json::string field = o.at(key).as_string();
	boost::algorithm::replace_all(field, "'", "''");
	return field;
}

static object add_problem(request_type &req, object &&params, dbptr conn)
{
	if (req.method() != boost::beast::http::verb::post)
		throw url_not_found_exception{};

	boost::json::string category = filtered_field(params, "category");
	boost::json::string personality = filtered_field(params, "personality");
	boost::json::string history = filtered_field(params, "history");
	boost::json::string problem = filtered_field(params, "problem");
	boost::json::string solution1 = filtered_field(params, "solution1");
	boost::json::string solution2 = filtered_field(params, "solution2");

	cout << "category: " << category << endl;
	cout << "personality: " << personality << endl;
	cout << "history: " << history << endl;
	cout << "problem: " << problem << endl;
	cout << "solution1: " << solution1 << endl;
	cout << "solution2: " << solution2 << endl;

	try // if params.at throws an exception, it will not be caught, and the server will return a 500 error
	{
		db_transaction tx{conn};
		db_result r = tx.exec(
			"insert into problems (category, personality, history, problem, solution1, solution2) "
			"values (?, ?, ?, ?, ?, ?)",
			category, personality, history, problem, solution1, solution2);
		lginfo << r.query();
		tx.commit();
	}
	catch (...)
	{
		return {
			{"code", 1},
			{"message", "insert failed"}};
	}
	cout << "insert finished" << endl;
	return {
		{"code", 0},
		{"message", "insert success"}};
}

constexpr int PAGE_SIZE = 10;
constexpr const char PAGE_SIZE_STR[] = "10";
// static boost::json::array get_problems_gpt(dbptr conn, const std::string &page_num)
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
RouterBuilder::add_path("/chart", &f, placeholders::request, placeholders::json_params, placeholders::db_connection_ptr);
RouterBuilder::add_path("/problems", &get_problems, placeholders::db_connection_ptr);
RouterBuilder::add_path("/add-problem", &add_problem, placeholders::request, placeholders::json_params, placeholders::db_connection_ptr);
// RouterBuilder::add_path("/page-problems", &get_problems_gpt, placeholders::db_connection_ptr, std::string{"1"});
// RouterBuilder::add_path("/page-problems/<int>", &get_problems_gpt, placeholders::db_connection_ptr, placeholders::_1);
INIT_END