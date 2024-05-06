/**
 * @file router.h
 * @author QuanQiuTong (fqt1550887523@outlook.com)
 * @brief A singleton class to build the router simply
 * @date 2024-04-27
 *
 * @copyright Copyright (c) 2024
 *
 *
 * The singleton class RouterBuilder is used to build the router simply--just like the snippet below:
 * 
 * ```cpp
 * #include "router.h"
 * 
 * INIT_BEGIN
 * add_router({
 *     make_path("/hello", GET, hello_handler),
 *     make_path("/json", POST, json_handler),
 * });
 * INIT_END
 * 
 * boost::json::object json_handler( ... ) { ... }
 * ```
 * So you can add router configs in any file, and the router will be built before main function.
 * Thus you can modularize your code and make it more readable.
 *
 */

#pragma once
#include <vector>
#include <memory>

#include "bserv/router.hpp"

using Router = std::vector<std::shared_ptr<bserv::router_internal::path_holder>>;

class RouterBuilder // Singleton class
{
public:
	static RouterBuilder &getInstance()
	{
		static RouterBuilder instance; // Guaranteed to be destroyed.
		return instance;               // Instantiated on first use.
	}
	
	Router _router;

private:
	RouterBuilder() {}                              // Private constructor
	RouterBuilder(RouterBuilder const &) = delete;	// Delete copy constructor
	void operator=(RouterBuilder const &) = delete; // Delete assignment operator
};

void add_router(const Router& router);

Router get_router();

#define INIT_BEGIN   \
	namespace        \
	{                \
		struct _init \
		{            \
			_init()  \
			{
/* Code here will be executed before main. */
#define INIT_END  \
	}             \
	}             \
	_before_main; \
	}