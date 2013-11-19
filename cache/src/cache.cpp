/*
* 2013+ Copyright (c) Alexander Ponomarev <noname@yandex-team.ru>
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*/

#include "cocaine/cache.hpp"

#include <cocaine/traits/tuple.hpp>

using namespace cocaine;
using namespace cocaine::service;

using namespace std::placeholders;

cache_t::cache_t(context_t& context, io::reactor_t& reactor, const std::string& name, const dynamic_t& args):
    service_t(context, reactor, name, args),
    implements<io::cache_tag>(context, name),
	cache_(args.as_object().at("max-size", 1000000).to<size_t>())
{
    on<io::cache::get>(std::bind(&cache_t::get, this, _1));
    on<io::cache::put>(std::bind(&cache_t::put, this, _1, _2));
}

void
cache_t::put(const std::string& key, const std::string& value) {
	cache_.put(key, value);
}

auto
cache_t::get(const std::string& key) -> get_result_type {
    if(cache_.exists(key)) {
		return std::make_tuple(true, cache_.get(key));
	} else {
		return std::make_tuple(false, std::string(""));
	}
}
