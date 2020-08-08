#include "hsqlite_db_imp.h"
#include "Exception.h"
#include "hutils.h"
#include <fstream>


namespace sqlite_db
{
	hsqlite_db_imp::~hsqlite_db_imp()
	{
		// avoid the situation: somebody forgets to call the uninit to release,
		uninit();
	}

	hsqlite_db_imp::hsqlite_db_imp()
	{
		init_ptr();
	}

	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::init_v2(const std::string db_file, std::string& out_str_err, const int open_type /*= 1*/, const int busy_time_out /*= 0*/, const std::string& vfs /*= ""*/)
	{
		int ret_val = 0;

		// 1、check the param
		if (true == db_file.empty())
		{
			ret_val = 1;

			return ret_val;
		}


		// 2、check the file status
		std::fstream file(db_file.c_str());
		if (false == file.good())
		{
			// file doesnt exist
			ret_val = 2;

			return ret_val;
		}


		// 3、check the param of type.
		int get_open_type = hutils::get_sqlite_open_type(open_type);


		// 4、initial the database
		if (nullptr == _pdatabase)
		{
			ret_val = 3;
			return ret_val;
		}

		// 5、initial base params
		try
		{
			int ret_init = _pdatabase->init_v2(db_file.c_str(), out_str_err, get_open_type, busy_time_out, vfs);
			if (-1 == ret_init)
			{
				ret_val = 4;

				return ret_val;
			}

			// 6、initialize transaction
			_ptransaction.reset(new SQLite::Transaction(*_pdatabase));
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());

			ret_val = 5;
			return ret_val;
		}

		return ret_val;
	}


	/*
	*	@brief:
	*/
	int hsqlite_db_imp::init(const std::string& db_file, std::string& out_str_err, const int open_type /*= 1*/)
	{
		int ret_val = 0;

		// 1、check the param
		if (true == db_file.empty())
		{
			ret_val = 1;

			return ret_val;
		}

		// 2、check the file status
		std::fstream file(db_file.c_str());
		if (false == file.good())
		{
			// file doesnt exist
			ret_val = 2;

			return ret_val;
		}


		// 3、check the param of type.
		int get_open_type = hutils::get_sqlite_open_type(open_type);


		// 4、initial the database
		if (nullptr == _pdatabase)
		{
			ret_val = 3;
			return ret_val;
		}


		// 5、initial base params
		try
		{
			int ret_init = _pdatabase->init(db_file.c_str(), out_str_err, get_open_type);
			if (-1 == ret_init)
			{
				ret_val = 4;

				return ret_val;
			}

			// 6、initialize transaction
			_ptransaction.reset(new SQLite::Transaction(*_pdatabase));
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());

			ret_val = 5;
			return ret_val;
		}

		return ret_val;
	}

	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::uninit()
	{
		// because it defined std::unique_ptr
		// -------------------------------------------------------------------------------
		//if (nullptr != _pdatabase)
		//{ 
		//	delete _pdatabase;
		//	_pdatabase = nullptr;
		//}

		return 0;
	}


	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::table_exist(const std::string& tabl_name) 
	{
		int ret_val = 0;

		// 1、
		if (nullptr == _pdatabase)
		{
			ret_val = -3;
			return ret_val;
		}

		// 2、
		if (tabl_name.empty())
		{
			ret_val = -2;
			return ret_val;
		}

		// 3、
		_pdatabase->tableExists(tabl_name) ? ret_val = 0 : ret_val = 1;


		return ret_val;
	}


	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::get_single_value(const std::string& str_query, std::string& str_result, std::string& out_str_err)
	{
		int ret_val = 0;


		// 1、
		if (str_query.empty())
		{
			ret_val = 1;
			return ret_val;
		}

		// 2、
		if (nullptr == _pdatabase)
		{
			ret_val = 2;
			return ret_val;
		}

		try
		{
			const std::string str = _pdatabase->execAndGet(str_query);
			str_result = str;
		}
		catch (SQLite::Exception& ex)
		{
			std::string str(ex.getErrorStr());
			out_str_err = str;
			ret_val = 3;
		}

		return ret_val;
	}


	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::get_db_file_name(std::string& str_db_file_name, std::string& out_str_err)
	{
		int ret_val = 0;

		if (nullptr == _pdatabase)
		{
			ret_val = -1;
			return ret_val;
		}
		
		try
		{
			str_db_file_name = _pdatabase->getFilename();
		}
		catch (SQLite::Exception& ex)
		{
			std::string str(ex.getErrorStr());
			out_str_err = str;
		}

		return ret_val;
	}


	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::exc_db_sql(const std::string& in_str_db_sql, int& out_result_row, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1、
		if (nullptr == _pdatabase)
		{
			ret_val = -4;
			return ret_val;
		}

		try
		{
			// 2、if not initialise the file
			if (true == _pdatabase->getFilename().empty())
			{
				ret_val = -2;
				return ret_val;
			}

			// 3. if sql string is empty
			if (true == in_str_db_sql.empty())
			{
				ret_val = -3;
				return ret_val;
			}

			// 4、execute the sql 
			out_result_row = _pdatabase->exec(in_str_db_sql);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = ex.getErrorStr();

			ret_val = -4;
			return ret_val;
		}

		return ret_val;
	}


	// -------------------------------------------------------------------------------
	int hsqlite_db_imp::exc_query_sql(const std::string& in_str_query_sql, std::string& out_str_exception)
	{
		int ret_val = 0;

		// 1. in_str_query_sql is empty
		if (in_str_query_sql.empty())
		{
			ret_val = 1;

			return ret_val;
		}

		// 2._pdatabase is nullptr
		if (!_pdatabase || !_pstatement)
		{
			ret_val = 2;
			return ret_val;
		}

		// 3. if  you not initialise the db file
		if (_pdatabase->getFilename().empty())
		{
			ret_val = 4;
			return ret_val;
		}

		try
		{
			// 4. 
			_pstatement->init_params(_pdatabase.get(), in_str_query_sql);
		}
		
		catch (SQLite::Exception& ex)
		{
			out_str_exception = std::string(ex.getErrorStr());
			ret_val = 5;
			return ret_val;
		}

		return ret_val;
	}


	// -------------------------------------------------------------------------------
	bool hsqlite_db_imp::exec_query_step()
	{
		bool ret_val = true;

		// 2._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = false;
			return ret_val;
		}

		try
		{
			ret_val = _pstatement->executeStep();
		}
		catch (SQLite::Exception& ex)
		{
			std::string str = std::string(ex.getErrorStr());
			ret_val = false;	
		}

		return ret_val;
	}


	int hsqlite_db_imp::exec_query_step(std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		// 2.call the function
		try
		{
			ret_val = _pstatement->executeStep();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::reset(std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->reset();
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}


	/*
	*	@brief: gets column's name
	*/
	int hsqlite_db_imp::get_table_column_name(const unsigned int& in_column_index, std::string& out_str_name, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			out_str_name = _pstatement->getColumnName(in_column_index);
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_table_column_count(unsigned int& out_total_column_count, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			out_total_column_count = _pstatement->getColumnCount();
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}


	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_index(const std::string& name, unsigned int& out_index)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		// 2. name is empty,  error, return 2
		if (name.empty())
		{
			ret_val = 2;
			return ret_val;
		}

		// 3.
		out_index = _pstatement->getIndex(name.c_str());

		return ret_val;
	}

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// --  bind start
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int index, const int value, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(index, value);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int index, const unsigned value, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(index, value);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int index, const long long aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(index, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int aIndex, const double aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(aIndex, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}



	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int aIndex, const std::string& aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(aIndex, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const int aIndex, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(aIndex);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& name, const unsigned aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(name.c_str(), aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}



	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& apName, const long aValue, std::string& out_str_err)
	{
		// bind(apName.c_str(), static_cast<int>(aValue));

		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
#if (LONG_MAX == INT_MAX)
			_pstatement->bind(apName.c_str(), static_cast<int>(aValue));
#else
			_pstatement->bind(apName.c_str(), static_cast<long long>(aValue));
#endif 
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& apName, const long long aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(apName.c_str(), aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}




	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& name, const int aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(name.c_str(), aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& apName, const double aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(apName.c_str(), aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& aName, const std::string& aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(aName, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bind(const std::string& aName, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bind(aName);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::clear_bindinds(std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->clearBindings();
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bindNoCopy(const int aIndex, const std::string& aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bindNoCopy(aIndex, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}


	/*
	*	@brief:
	*/
	int hsqlite_db_imp::bindNoCopy(const std::string& aName, const std::string& aValue, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_pstatement->bindNoCopy(aName, aValue);
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------
	// --  bind end
	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------




	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_column_string(const unsigned int in_column_index, std::string& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		if (0 != this->pre_query(in_column_index))
		{
			ret_val = 3;
			return ret_val;
		}


		try
		{
			out_val = _pstatement->getColumn(in_column_index).getString();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_val = std::string("");
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_column_int(const unsigned int in_column_index, int& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		if (0 != this->pre_query(in_column_index))
		{
			ret_val = 3;
			return ret_val;
		}

		try
		{
			out_val = _pstatement->getColumn(in_column_index).getInt();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_val = 0;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_column_double(const unsigned int in_column_index, double& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		if (0 != this->pre_query(in_column_index))
		{
			ret_val = 3;
			return ret_val;
		}

		try
		{
			out_val = _pstatement->getColumn(in_column_index).getDouble();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_val = 0.0;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_column_uint(const unsigned int in_column_index, unsigned int& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		if (0 != this->pre_query(in_column_index))
		{
			ret_val = 3;
			return ret_val;
		}

		try
		{
			out_val = _pstatement->getColumn(in_column_index).getUInt();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_val = 0;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::get_column_int64(const unsigned int in_column_index, long long& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		if (0 != this->pre_query(in_column_index))
		{
			ret_val = 3;
			return ret_val;
		}

		try
		{
			out_val = _pstatement->getColumn(in_column_index).getInt64();
		}
		catch (SQLite::Exception& ex)
		{
			ret_val = 2;
			out_val = 0;
			out_str_err = std::string(ex.getErrorStr());
		}

		return ret_val;
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::commit(std::string& out_str_err)
	{
		int ret_val = 0;
		// 1._pdatabase is nullptr
		if (!_pdatabase || !_ptransaction || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			_ptransaction->commit();
		}
		catch (SQLite::Exception& ex)
		{
			out_str_err = std::string(ex.getErrorStr());
			ret_val = 2;
		}

		return ret_val;
	}

	// -------------------------------------------------------------------------------
	void hsqlite_db_imp::init_ptr()
	{
		_pdatabase.reset(new SQLite::Database);
		_pstatement.reset(new SQLite::Statement);
		//_ptransaction.reset(new SQLite::Transaction);
	}

	/*
	*	@brief:
	*/
	int hsqlite_db_imp::pre_query(const unsigned int& column_index)
	{
		int ret_val = 0;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		unsigned int count_column_total = _pstatement->getColumnCount();
		
		if ( (0 >= count_column_total) || (column_index >= count_column_total) )
		{
			ret_val = 2;
			return ret_val;
		}

		return ret_val;
	}

}