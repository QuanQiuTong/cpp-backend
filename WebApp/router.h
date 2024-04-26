/**
 * @file router.h
 * @author QuanQiuTong (fqt1550887523@outlook.com)
 * @brief A singleton class to build the router simply
 * @date 2024-04-27
 * 
 * @copyright Copyright (c) 2024
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

void add_router(const Router &router)
{
	auto &builder = RouterBuilder::getInstance();
	for (auto &path : router)
		builder._router.push_back(path);
}

Router get_router() { return RouterBuilder::getInstance()._router; }