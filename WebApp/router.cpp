#define BOOST_USE_WINAPI_VERSION BOOST_WINAPI_VERSION_WIN8

#include "router.h"

void add_router(const Router& router)
{
	auto& builder = RouterBuilder::getInstance();
	for (auto& path : router)
		builder._router.push_back(path);
}

Router get_router() { return RouterBuilder::getInstance()._router; }