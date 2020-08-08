#ifndef _hsqlite_db_imp_h
#define _hsqlite_db_imp_h

// add sqlitecppÎÄ¼þ
#include "SQLiteCpp.h"
#include <memory>
#include "hutils.h"


namespace sqlite_db
{
	class hsqlite_db_imp
	{
	public:
		explicit hsqlite_db_imp();
		virtual ~hsqlite_db_imp();
		hsqlite_db_imp(const hsqlite_db_imp &instance) = delete;
		hsqlite_db_imp &operator =(const hsqlite_db_imp &instance) = delete;
		// -------------------------------------------------------------------------------

		

		
		/* @ brief: initial some params 
		*  @ const std::string & db_file - where is the .db file. such as: "c:/sqlite/demo.db"
		*  @ const int open_type - 1:read_only, 2 - read_write, 4 - create, if this param cannot be found the definition, it will reset 1;
		*  @ const int busy_time_out - busy time out, default is 0;
		*  @ const std::string & vfs - // std::string& out_str_err
		*  @ std::string& out_str_err - error msg. out_str_err will record the error information if initial database occur error
		*  @ return - int
				0 - success
				1 - db_file is empty  
				2 - db_file is not exist
				3 - internal error, create _pdatabase object failed
				4,5 - initial error, check the param [out_str_err] to get error information
		*/
		int init_v2(const std::string db_file, std::string& out_str_err, const int open_type = 1, const int busy_time_out = 0, const std::string& vfs = "");

		// call sqlite_open 
		int init(const std::string& db_file, std::string& out_str_err, const int open_type = 1);

		/* @ brief: call this function to release resource before quitting
		*  @ return - int
				0 - success , default result is 0;
		*
		*/
		int uninit();


		/* @ brief: check if the table_me exists
		*  @ const std::string & tabl_name - table name 
		*  @ return - int
				0 - table is contained in the database
				1 - false, the database doesnt have that table
				2 - failed, the param [tabl_name] is empty
				3 - failed, internal error£¨_pdatabase is nullptr£©
		*
		*/
		int table_exist(const std::string& tabl_name) ;


		
		/* @ brief: Get a single value result with an easy to use shortcut
		*  @ const std::string& str_query - sql string to query, for example, "SELECT value FROM test WHERE id=2"
		*  @ std::string str_result - the query's result
		*  @ std::string out_str_err - error msg
		*  @ return - int
				0 - querry success
				1 - failed, the param [str_query] is empty
				2 - failed, internal error(_pdatabase is nullptr)
		*
		*/
		int get_single_value(const std::string& str_query, std::string& str_result, std::string& out_str_err) ;


		/* @ brief: query the database's name of current operation 
		*  @ std::string & str_db_file_name - return the name of db file's name
		*  @ std::string & out_str_err - save error msg if sth gets wrong
		*  @ return - int
				0 - success 
				1 - failed, internal error, the object of the db file is nullptr
		*/
		int get_db_file_name(std::string& str_db_file_name, std::string& out_str_err);



		// -------------------------------------------------------------------------------

		/* @ brief: the following function has the these functioms:  create and drop tables, insert and update a row.
						and it will not work with querying data			
		*  @ const std::string & in_str_db_sql - sql string
		*  @ std::string & out_str_err - exceptional string 
		*  @ int& out_result_row - Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
		*  @ return - int
				0 - success
				-2 - failed, there is no database file from initialising 
				-3 - falied, [in_str_db_sql] is empty
				-4 - failed, internal error, the object to communicate with database file was created failed
		*/
		int exc_db_sql(const std::string& in_str_db_sql, int& out_result_row, std::string& out_str_err);



		/* @ brief: query data only
		*  @ const std::string & in_str_query_sql - query string of sql
		*  @ std::string & out_str_exception - if an excpetion occurs, you could check the param to get error's detail
		*  @ return - int
				0 - success
		*/
		int exc_query_sql(const std::string& in_str_query_sql, std::string& out_str_exception);

		
		/* @ brief: you must run the function [exc_query_sql] , then you could call the following function ,
					it will get the next column after  returnning true;
					Note: if the object do not created, it also returns false 
		*  @ return - bool
				true - youg could call [] to get column value
				false - it gets end, or dont initialise the db file and call the [exc_query_sql] function
		*/
		bool exec_query_step();

		/* @ brief: to query the data from the next column
		*  @ std::string & out_str_err - if an error occurs, you could check the param [out_str_err] to get details
		*  @ return - int
				0 - success
				1 - failed, dont initialise the db file and call the [exc_query_sql] function firstly
				2 - failed, please to check the param out_str_err to get details
		*/
		int exec_query_step(std::string& out_str_err);


		/* @ brief: Reset the statement to make it ready for a new execution
		*  @ std::string & out_str_err - error msg 
		*  @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, please check [out_str_err] to get more details
		*/
		int reset(std::string& out_str_err);


		/* @ brief: get column's name
		*  @ const unsigned int & in_column_index - index of column
		*  @ std::string & out_str_name - column's name
		*  @ std::string & out_str_err - error msg
		*  @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - an error occurs, please check [out_str_err] to get more details
		*/
		int get_table_column_name(const unsigned int& in_column_index, std::string& out_str_name, std::string& out_str_err);


		/* @ brief: get total rows of the table.
		*  @ unsigned int & out_total_row_count - the total row count 
		*  @ std::string & out_str_err - error msg
		*  @ return - int
				 0 - success 
				 1 - failed, it doesnt initialize db file
				 2 - an error occur, please check [out_str_err] to get more details
		*/
		int get_table_column_count(unsigned int& out_total_column_count, std::string& out_str_err);

		/* @ brief: get name's index
		*  @ const std::string & name - 
		*  @ unsigned int& out_index - the result of index
		*  @ return - int
				0 - success
				1 - falied, it doesnt initialize db file
				2 - failed, name is empty
		*/
		int get_index(const std::string& name, unsigned int& out_index);


		///------------------------ bind start---------------------------------------------


		// 145
		/* @ brief: Bind an int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*  @ const int index - 
		*  @ const int value - 
		*  std::string& out_str_erre - 
		*  @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int index, const int value, std::string& out_str_err);

		/**  149
		* @brief Bind a 32bits unsigned int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		  @ return - int
			  0 - success
			  1 - failed, it doesnt initialize db file
			  2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int index, const unsigned  value, std::string& out_str_err);
		
		/**151
		* @brief Bind a 32bits long value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		* @ return - int
			  0 - success
			  1 - failed, it doesnt initialize db file
			  2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int index, const long  value, std::string & out_str_err)
		{
#if (LONG_MAX == INT_MAX) // 4 bytes "long" type means the data model is ILP32 or LLP64 (Win64 Visual C++ and MinGW)
			return this->bind(index, static_cast<int>(value), out_str_err);
#else
			return bind(aIndex, static_cast<long long>(aValue), out_str_err);
#endif // 
		}

		/** 172
		* @brief Bind a 64bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		* @ return - int
			  0 - success
			  1 - failed, it doesnt initialize db file
			  2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int aIndex, const long long aValue, std::string& out_str_err);


		/** 176
		* @brief Bind a double (64bits float) value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		* @ return - int
			  0 - success
			  1 - failed, it doesnt initialize db file
			  2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int aIndex, const double   aValue, std::string& out_str_err);

		/** 182
		* @brief Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int aIndex, const std::string&  aValue, std::string& out_str_err);



		// *  188
		/*
		* @brief Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		*/
		// void bind(const int aIndex, const char*         apValue);




		/** --blob.  194
		* @brief Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		*/
		// void bind(const int aIndex, const void*         apValue, const int aSize);




		/** 202
		* @brief Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1).
		*
		* The string can contain null characters as it is binded using its size.
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		// int bindNoCopy(const int aIndex, const std::string&    aValue, std::string& out_str_err);


		/**  210
		* @brief Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* Main usage is with null-terminated literal text (aka in code static strings)
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bindNoCopy(const int aIndex, const std::string& apValue, std::string& out_str_err);



		/** blob 216
		* @brief Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		// int bindNoCopy(const int aIndex, const void*           apValue, const int aSize, std::string& out_str_err);
		

		/** 222
		* @brief Bind a NULL value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @see clearBindings() to set all bound parameters to NULL.
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const int aIndex, std::string& out_str_err);

		// 227 void bind(const char* apName, const int             aValue)

		// 234 void bind(const char* apName, const unsigned        aValue)

		// 259 void bind(const char* apName, const long long       aValue)

		// 266 void bind(const char* apName, const double          aValue)

		// 275 void bind(const char* apName, const std::string&    aValue)

		// 284 void bind(const char* apName, const char*           apValue)

		// 293 void bind(const char* apName, const void*           apValue, const int aSize)

		// 304 void bindNoCopy(const char* apName, const std::string&  aValue)

		// 315 void bindNoCopy(const char* apName, const char*         apValue)

		// 324 void bindNoCopy(const char* apName, const void*         apValue, const int aSize)

		// 333 void bind(const char* apName) // bind NULL value



		
		/**   342
		* @brief Bind an int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const std::string& name, const int aValue, std::string& out_str_err);

		/** 349
		* @brief Bind a 32bits unsigned int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		* @ return - int
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_err] to get more details
		*/
		int bind(const std::string& name, const unsigned   aValue, std::string& out_str_err);

		/** 358
		* @brief Bind a 32bits long value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*/
		int bind(const std::string& apName, const long   aValue, std::string& out_str_err);

		// 374 @brief Bind a 64bits int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		int bind(const std::string& apName, const long long       aValue, std::string& out_str_err);

		// 381 @brief Bind a double (64bits float) value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		int bind(const std::string& apName, const double          aValue, std::string& out_str_err);
		
		/** 390
		* @brief Bind a string value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		*/
		int bind(const std::string& aName, const std::string&    aValue, std::string& out_str_err);

		//  399 void bind(const std::string& aName, const char*           apValue) 

		//  408 void bind(const std::string& aName, const void*           apValue, const int aSize)


		/** 419
		* @brief Bind a string value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* The string can contain null characters as it is binded using its size.
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		*/
		int bindNoCopy(const std::string& aName, const std::string& aValue, std::string& out_str_err);

		// 430 void bindNoCopy(const std::string& aName, const char*        apValue);

		// 439 void bindNoCopy(const std::string& aName, const void*        apValue, const int aSize)


		/** 448 
		* @brief Bind a NULL value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (aIndex >= 1)
		*
		* @see clearBindings() to set all bound parameters to NULL.
		*/
		int bind(const std::string& aName, std::string& out_str_err); // bind NULL value

		///------------------------ bind end---------------------------------------------


		/* @ brief: clear bindings
		*  @ std::string & out_str_err - error msg
		*  @ return - int
				0 - success
				1 - failed,  it doesnt initialize db file
				2 - failed, an error occurs, please check [out_str_err] to get more details
		*/
		int clear_bindinds(std::string& out_str_err);


		
#ifdef xxxxxxxxxxx
		/* @ brief: get the column's data
		*  @ const unsigned int in_column_index - which column do you wanna get . such 0, 1, 3, . it starts 0
		*  @ T & out_val - get column's data
		*  @ std::string & out_str_err - if an error occurs, you could check the param to get details
		*  @ return - int
				0 - success 
				1 - failed, dont initialise the db file and call the [exc_query_sql] function firstly
				2 - failed, an error occured, please check the [out_str_err] to check the details
				3 - failed, it cannot work with this type of T
		*/
		template<typename T>
		int get_column(const unsigned int in_column_index, T& out_val, std::string& out_str_err);

		template <typename T, typename U>
		bool is_same_type(T, U)
		{
			return std::is_same<typename std::decay<T>::type, U>::value;
		}
#endif // !xxxxxxxxxxx


		/* @ brief: get a string 
		*  @ const unsigned int in_column_index - which column do you wanna get . such 0, 1, 3, . it starts 0
		*  @ std::string & out_val - save the string
		*  @ std::string & out_str_err - error msg 
		*  @ return - int
				0 - success
				1 - failed, dont initialise the db file and call the [exc_query_sql] function firstly
				2 - failed, an error occured, please check the [out_str_err] to check the details
				3 - failed, it doesnt get any column
		*/
		int get_column_string(const unsigned int in_column_index, std::string& out_val, std::string& out_str_err);
		int get_column_int(const unsigned int in_column_index, int& out_val, std::string& out_str_err);
		int get_column_double(const unsigned int in_column_index, double& out_val, std::string& out_str_err);
		int get_column_uint(const unsigned int in_column_index, unsigned int& out_val, std::string& out_str_err);
		int get_column_int64(const unsigned int in_column_index, long long & out_val, std::string& out_str_err);
		


		// -------------------------------------------------------------------------------
		//
		// transaction
		//
		// -------------------------------------------------------------------------------
		/* @ brief: Commit the transaction
		*  @ std::string & out_str_err - if an error occurs, check this param to get more details
		*  @ return - int
				0 - success 
				1 - failed, do not initialize database file
				2 - failed, an error occurs, please check the [out_str_err] to get more details
		*/
		int commit(std::string& out_str_err);

	private:
		/* @ brief: initialise the internal pointers
		*  @ return - void
				
		*/
		void init_ptr();


	private:
		/* @ brief: get column count
		*  @ return - int
				0 - success, it gets columns
				1 - failed, it doesnt initialise
				2 - failed, it doesnt have columns
		*/
		int pre_query(const unsigned int& column_index);

	private:

		//SQLite::Database	*_pdatabase = nullptr;
		//SQLite::Statement	*_pstatement = nullptr;s


		std::unique_ptr<SQLite::Database >		_pdatabase	= nullptr;
		std::unique_ptr<SQLite::Statement>		_pstatement = nullptr;
		std::unique_ptr<SQLite::Transaction>	_ptransaction = nullptr;



	};


#ifndef xxxxxxxxxxx
	template<typename T>
	int sqlite_db::hsqlite_db_imp::get_column(const unsigned int in_column_index, T& out_val, std::string& out_str_err)
	{
		int ret_val = 0;

		static int xx_int = 0;
		static double xx_double = 0.0;
		static unsigned int xx_uint = 0;
		static __int64 xx_int64	= 0;
		static std::string xx_string;

		// 1._pdatabase is nullptr
		if (!_pdatabase || !_pstatement || _pdatabase->getFilename().empty())
		{
			ret_val = 1;
			return ret_val;
		}

		try
		{
			auto val = _pstatement->getColumn(in_column_index);
			// T is int
			

			//if (std::is_same<typename std::decay<T>::type, int>::value)
			if (hutils::is_same_type<T,		int>(out_val, xx_int))
				out_val = val.getInt();
			
			// T is  double
			else if (hutils::is_same_type<T, double>(out_val, xx_double))
				out_val = val.getDouble();
			// T is UINT
			else if (hutils::is_same_type<T, unsigned int>(out_val, xx_uint))
				out_val = val.getUInt();
			// T is int64
			else if (hutils::is_same_type<T, __int64>(out_val, xx_int64))
				out_val = val.getInt64();
			else
				ret_val = 3;

		}
		catch (SQLite::Exception *e)
		{
			out_str_err = e->getErrorStr();
			ret_val = 2;
		}

		return ret_val;
	}
#endif ! xxxxxxxxxxx

}



#endif // !_hsqlite_db_imp_h
