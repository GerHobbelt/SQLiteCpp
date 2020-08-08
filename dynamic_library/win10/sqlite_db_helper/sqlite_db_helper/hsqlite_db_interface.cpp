#include "hsqlite_db_interface.h"
#include "hsqlite_db_imp.h"



#ifdef _WIN32
#include <windows.h>
#elif	__linux__

#elif __APPLE__

#endif // !


namespace sqlite_db
{

	/*
	* @brief: 
	*/
	hsqlite_db::~hsqlite_db()
	{
		this->uninit();
	}


	/*
	* @brief:
	*/
	hsqlite_db::hsqlite_db() : _psqlite_db_imp(nullptr)
	{
		_psqlite_db_imp = new hsqlite_db_imp;
	}

	// -------------------------------------------------------------------------------
	/*
	* @brief:
	*/
	int hsqlite_db::init_v2(const std::string db_file, std::string& out_str_err, const sqlite3_open_with open_type /*= OPEN_READONLY*/, const int busy_time_out /*= 0*/, const std::string& vfs /*= ""*/)
	{
		int ret_val = 0;
		
		if ( nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->init_v2(db_file, out_str_err, open_type, busy_time_out, vfs);
	}


	/*
	*	@brief:
	*/
	int hsqlite_db::init(const std::string db_file, std::string& out_str_exception, const sqlite3_open_with open_type /*= OPEN_READONLY*/)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->init(db_file, out_str_exception, open_type);
	}

	/*
	* @brief:
	*/
	int hsqlite_db::uninit()
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->uninit();
	}


	/*
	* @brief:
	*/
	int hsqlite_db::table_exist(const std::string table_name)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->table_exist(table_name);
	}


	/*
	* @brief:
	*/
	int hsqlite_db::get_single_value(const std::string str_query, std::string& str_result, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_single_value(str_query, str_result, out_str_err);
	}


	/*
	* @brief:
	*/
	int hsqlite_db::get_db_file_name(std::string& str_db_file_name, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_db_file_name(str_db_file_name, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::reset(std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->reset(out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_table_column_name(const unsigned int in_column_index, std::string& out_str_name, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_table_column_name(in_column_index, out_str_name, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_table_column_count(unsigned int& out_total_column_count, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_table_column_count(out_total_column_count, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_index(const std::string name, unsigned int& out_index)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_index(name, out_index);
	}


	// -------------------------------------------------------------------------------
	// 
	// bind start
	// 
	// -------------------------------------------------------------------------------

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int index, const int value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(index, value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int index, const unsigned value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(index, value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int index, const long value, std::string & out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(index, value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int in_index, const long long in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(in_index, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int in_index, const double in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(in_index, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int in_index, const std::string in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(in_index, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const int in_index, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(in_index, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const int in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const unsigned in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const long in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const long long in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const double in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, const std::string in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bind(const std::string name, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, out_str_err);
	}


	/*
	*	@brief:
	*/
	int hsqlite_db::clear_bindinds(std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->clear_bindinds(out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bindNoCopy(const int in_index, const std::string in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(in_index, in_value, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::bindNoCopy(const std::string name, const std::string in_value, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->bind(name, in_value, out_str_err);
	}

	// -------------------------------------------------------------------------------
	// 
	// bind end
	// 
	// -------------------------------------------------------------------------------

	/*
	* @brief:
	*/
	int hsqlite_db::exec_db_sql(const std::string in_str_db_sql, int& out_result_row, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->exc_db_sql(in_str_db_sql, out_result_row, out_str_err);
	}


	/*
	* @brief:
	*/
	int hsqlite_db::exec_query_sql(const std::string in_str_query_sql, std::string& out_str_exception)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->exc_query_sql(in_str_query_sql, out_str_exception);
	}


	/*
	* @brief:
	*/
	bool hsqlite_db::exec_query_step()
	{
		bool ret_val = true;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = false;

			return ret_val;
		}

		return _psqlite_db_imp->exec_query_step();
	}


	/*
	* @brief:
	*/
	int hsqlite_db::exec_query_step(std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->exec_query_step(out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_column_int(const unsigned int in_column_index, int& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_column_int(in_column_index, out_val, out_str_err);
	}


	/*
	*	@brief:
	*/
	int hsqlite_db::get_column_double(const unsigned int in_column_index, double& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_column_double(in_column_index, out_val, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_column_uint(const unsigned int in_column_index, unsigned int& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_column_uint(in_column_index, out_val, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_column_int64(const unsigned int in_column_index, long long& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_column_int64(in_column_index, out_val, out_str_err);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db::get_column_string(const unsigned int in_column_index, std::string& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->get_column_string(in_column_index, out_val, out_str_err);
	}


	/*
	*	@brief:
	*/
	int hsqlite_db::commit(std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _psqlite_db_imp)
		{
			ret_val = -1;

			return ret_val;
		}

		return _psqlite_db_imp->commit(out_str_err);
	}

	// -------------------------------------------------------------------------------
	/*
	* @brief:
	*/
	std::string util_set::get_exec_path()
	{
		char path[path_max_len_256] = { 0 };
		// 32 and 64 bits windows contain the _win32 macro
#ifdef _WIN32
		 GetCurrentDirectoryA(path_max_len_256, path);
#elif	__linux__

#elif __APPLE__

#endif // !

		return std::string(path);
	}

}