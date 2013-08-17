
/*
 * buddy manager - manager/update/cache the group/buddies information
 *
 * copyright 2013 microcai
 * copyright 2012 - 2013 avplayer.org
 */

#pragma once

#include <boost/asio.hpp>

#include <soci-sqlite3.h>
#include <boost-optional.h>
#include <boost-tuple.h>
#include <boost-gregorian-date.h>
#include <soci.h>

namespace webqq {
namespace qqimpl {
namespace detail {

} // nsmespace detail

class buddy_mgr
{
public:
	buddy_mgr(std::string dbname  = ":memory:")
	{
		sqlite_api::sqlite3_enable_shared_cache(1);
		m_sql.open(soci::sqlite3, dbname);
		db_initialize();
	}

	void update_group_list(std::string gid, std::string name, std::string code);

	bool group_has_qqnum(std::string code);
	void map_group_qqnum(std::string code, std::string qqnum);

private:

	void db_initialize()
	{
		soci::transaction trans(m_sql);
		// 初始化存储数据库
		m_sql << ""
			"create table if not exists groups ( "
				"`gid` TEXT not null,"
				"`group_code` TEXT not null,"
				"`name` TEXT not null, "
				"`qqnum` TEXT, "
				"`owner` TEXT, "
				// last time that this group information retrived from TX
				// libwebqq will remove outdated one
				"`generate_time` TEXT not null"
			");";

		m_sql << ""
			"create table if not exists group_buddies ( "
				"`gid` TEXT not null,"
				"`uid` TEXT not null,"
				"`nick` TEXT,"
				"`card` TEXT,"
				"`mflag` INTEGER,"
				"`qqnum` TEXT"
			");";

		trans.commit();
	}

private:
	soci::session	m_sql;

};


} // nsmespace qqimpl
} // namespace webqq