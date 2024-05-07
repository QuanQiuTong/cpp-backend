#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602 // Windows 8
#endif

#include "router.hpp"
#include "bserv/common.hpp"

#include <boost/json.hpp>
#include <memory>
#include <string>

using namespace bserv;
using object = boost::json::object;



/*CREATE TABLE users(
	id SERIAL PRIMARY KEY,

	username VARCHAR(255) NOT NULL UNIQUE,
	password VARCHAR(255) NOT NULL,
	email VARCHAR(255) NOT NULL
);*/

static db_relation_to_object orm_user{
	make_db_field<int>("id"),
	make_db_field<std::string>("username"),
	make_db_field<std::string>("password"),
	make_db_field<std::string>("email"),
	// make_db_field<std::string>("created_at")
};

std::optional<object> get_user(db_transaction& tx, const boost::json::string& username) {
	bserv::db_result r = tx.exec("SELECT * FROM users WHERE username = ?", username);
	return orm_user.convert_to_optional(r);
}

object signup(request_type &request, object &&params, std::shared_ptr<db_connection> conn)
{
	auto username = params.at("username").as_string();
	auto password = params.at("password").as_string();
	auto email = params.at("email").as_string();
	auto encrypted = utils::security::encode_password(password.c_str());

	db_transaction tx{conn};
	if (get_user(tx, username))
		return {{"status", "error"}, {"message", "user already exists"}};
	tx.exec("INSERT INTO users (username, password, email) VALUES (?, ?, ?)", username, encrypted, email);
	tx.commit();
	return {{"status", "ok"}};
}

object signin(request_type &request, object &&params, std::shared_ptr<db_connection> conn)
{

	auto username = params.at("username").as_string();
	auto password = params.at("password").as_string();

	db_transaction tx{conn};
	
	auto _user = get_user(tx, username);
	if (!_user)
		return {
			{"success", false},
			{"message", "user not found"}};
	auto user = _user.value();

	auto encoded_password = user["password"].as_string();
	if (!utils::security::check_password(password.c_str(), encoded_password.c_str()))
		return {
			{"success", false},
			{"message", "wrong password"}};
}

boost::json::array user_list(std::shared_ptr<db_connection> conn)
{
	db_transaction tx{conn};
	db_result r = tx.exec("SELECT * FROM users");
	tx.commit();
	return orm_user.convert_to_array(r);
}

INIT_BEGIN
using namespace placeholders;
RouterBuilder::add_path("/signup", &signup, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/signin", &signin, request, json_params, db_connection_ptr);
RouterBuilder::add_path("/user-list", &user_list, db_connection_ptr);
INIT_END