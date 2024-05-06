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
	personality TEXT,
	history TEXT,
	problem TEXT not null,
	solution1 TEXT not null,
	solution2 TEXT not null,
	created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);*/
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
	orm_problem.convert_to_optional(r);
	boost::json::array problems;
	for (object &p : orm_problem.convert_to_vector(r))
		problems.push_back(p);
	return problems;
}

constexpr int PAGE_SIZE = 10;
constexpr const char PAGE_SIZE_STR[] = "10";
static boost::json::array get_problems_gpt(dbptr conn, const std::string &page_num)
{
	int page_id = std::stoi(page_num);
	db_transaction tx{conn};

	// 查询总的问题数量
	db_result total_count_res = tx.exec("select count(*) from problems;");
	lginfo << total_count_res.query();
	std::size_t total_problems = (*total_count_res.begin())[0].as<std::size_t>();
	lgdebug << "total problems: " << total_problems << std::endl;

	// 计算总页数
	int total_pages = (int)total_problems / PAGE_SIZE + !!(total_problems % PAGE_SIZE);
	lgdebug << "total pages: " << total_pages << std::endl;

	// 查询分页后的问题列表
	db_result problems_res = tx.exec(
		"select * from problems limit ? offset ?;",
		PAGE_SIZE, (page_id - 1) * PAGE_SIZE);
	lginfo << problems_res.query();

	// 转换为问题对象列表
	boost::json::array problems;
	for (object &p : orm_problem.convert_to_vector(problems_res))
		problems.push_back(p);
	return problems;
}

INIT_BEGIN
RouterBuilder::add_path("/chart", &f, placeholders::request, placeholders::json_params, placeholders::db_connection_ptr);
RouterBuilder::add_path("/problems", &get_problems, placeholders::db_connection_ptr);
RouterBuilder::add_path("/page-problems", &get_problems_gpt, placeholders::db_connection_ptr, std::string{"1"});
RouterBuilder::add_path("/page-problems/<int>", &get_problems_gpt, placeholders::db_connection_ptr, placeholders::_1);
INIT_END