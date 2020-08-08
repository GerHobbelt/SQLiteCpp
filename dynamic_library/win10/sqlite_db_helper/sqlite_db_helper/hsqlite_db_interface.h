#ifndef _hsqlite_db_h
#define _hsqlite_db_h


#include <iostream>
#include <memory>

#if defined (_WIN32) 

	#ifndef _sqlite_db_api_export_
		#define _sqlite_db_api_export_	__declspec(dllexport)
	#else
		#define _sqlite_db_api_export_	__declspec(dllimport)
	#endif // !_sqlite_db_api_export_

#else
	
	#define _sqlite_db_api_export_ __attribute__((visibility ("default")))
#endif // !



namespace sqlite_db
{

	class hsqlite_db_imp;

	/*
	*	@ brief: a helper to use sqlite database. It bases on SQLitecpp , 
				which is an openning source on github [https://github.com/SRombauts/SQLiteCpp]
	*/
	class _sqlite_db_api_export_ hsqlite_db
	{

	public:
		// 
		enum sqlite3_open_with
		{
			OPEN_READONLY		 = 0x00000001,  /* Ok for sqlite3_open_v2() */
			OPEN_READWRITE       = 0x00000002,  /* Ok for sqlite3_open_v2() */
			OPEN_CREATE          = 0x00000004,  /* Ok for sqlite3_open_v2() */
			OPEN_DELETEONCLOSE   = 0x00000008,  /* VFS only */
			OPEN_EXCLUSIVE       = 0x00000010,  /* VFS only */
			OPEN_AUTOPROXY       = 0x00000020,  /* VFS only */
			OPEN_URI             = 0x00000040,  /* Ok for sqlite3_open_v2() */
			OPEN_MEMORY          = 0x00000080,  /* Ok for sqlite3_open_v2() */
			OPEN_MAIN_DB         = 0x00000100,  /* VFS only */
			OPEN_TEMP_DB         = 0x00000200,  /* VFS only */
			OPEN_TRANSIENT_DB    = 0x00000400,  /* VFS only */
			OPEN_MAIN_JOURNAL    = 0x00000800,  /* VFS only */
			OPEN_TEMP_JOURNAL    = 0x00001000,  /* VFS only */
			OPEN_SUBJOURNAL      = 0x00002000,  /* VFS only */
			OPEN_MASTER_JOURNAL  = 0x00004000,  /* VFS only */
			OPEN_NOMUTEX         = 0x00008000,  /* Ok for sqlite3_open_v2() */
			OPEN_FULLMUTEX       = 0x00010000,  /* Ok for sqlite3_open_v2() */
			OPEN_SHAREDCACHE     = 0x00020000,  /* Ok for sqlite3_open_v2() */
			OPEN_PRIVATECACHE    = 0x00040000,  /* Ok for sqlite3_open_v2() */
			OPEN_WAL             = 0x00080000,  /* VFS only */
		};

	public:
		explicit hsqlite_db();

		// it will call  uninit to ensure that _psqlite_db_imp releases
		virtual ~hsqlite_db();

		hsqlite_db(const hsqlite_db &instance) = delete;
		hsqlite_db & operator = (const hsqlite_db &instance) = delete;
		// -------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------
		//
		// initialization
		//
		// -------------------------------------------------------------------------------

		/* @ brief: initial some params, and it will call sqlite_open_v2 to open db file
		*  @ const std::string & db_file - where is the .db file. such as: "c:/sqlite/demo.db"
		*  @ const sqlite3_open_with open_type - 1:read_only, 2 - read_write, 4 - create, if this param cannot be found the definition, it will reset 1;
		*  @ const int busy_time_out - busy time out, default is 0;
		*  @ const std::string & vfs - // std::string& out_str_exception
		*  @ std::string& out_str_exception - error msg. out_str_exception will record the error information if initial database occur error
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - db_file is empty
				2 - db_file is not exist
				3 - internal error, create _pdatabase object failed
				4 - initial error, check the param [out_str_exception] to get error information
		*/
		int init_v2(const std::string db_file, std::string& out_str_exception, const sqlite3_open_with open_type = OPEN_READONLY, const int busy_time_out = 0, const std::string& vfs = "");
		// this function will call sqlite_open to open db file
		int init(const std::string db_file, std::string& out_str_exception, const sqlite3_open_with open_type = OPEN_READONLY);



		/* @ brief: call this function to release resource before quitting
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success , the default result is 0;
		*/
		int uninit();


		/* @ brief: check if the table_me exists
		*  @ const std::string table_name - table name
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - table is contained in the database
				1 - false, the database doesnt have that table
				2 - failed, the param [tabl_name] is empty
				3 - failed, internal error£¨_pdatabase is nullptr£©
		*/
		int table_exist(const std::string table_name);



		/* @ brief: Get a single value result with an easy to use shortcut
		*  @ const std::string str_query - sql string to query, for example, "SELECT value FROM test WHERE id=2"
		*  @ std::string& str_result - the query's result
		*  @ std::string out_str_exception - error msg
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - querry success
				1 - failed, the param [str_query] is empty
				2 - failed, internal error(_pdatabase is nullptr)
		*/
		int get_single_value(const std::string str_query, std::string& str_result, std::string& out_str_exception);


		/* @ brief: query the database's name of current operation
		*  @ std::string & str_db_file_name - return the name of db file's name
		*  @ std::string & out_str_exception - save error msg if sth gets wrong
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success, and this is a flag to open db file successfully
				1 - failed, internal error, the object of the db file is nullptr
		*/
		int get_db_file_name(std::string& str_db_file_name, std::string& out_str_exception);


		// -------------------------------------------------------------------------------

		/* @ brief: Reset the statement to make it ready for a new execution
		*  @ std::string & out_str_exception - error msg
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, please check [out_str_exception] to get more details
		*/
		int reset(std::string& out_str_exception);


		/* @ brief: get column's name
		*  @ const unsigned int & in_column_index - index of column
		*  @ std::string & out_str_name - column's name
		*  @ std::string & out_str_exception - error msg
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - an error occurs, please check [out_str_exception] to get more details
		*/
		int get_table_column_name(const unsigned int in_column_index, std::string& out_str_name, std::string& out_str_exception);

		/* @ brief: get total rows of the table.
		*  @ unsigned int & out_total_column_count - the total row count
		*  @ std::string & out_str_exception - error msg
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - an error occur, please check [out_str_exception] to get more details
		*/
		int get_table_column_count(unsigned int& out_total_column_count, std::string& out_str_err);

		/* @ brief: get name's index
		*  @ const std::string & name - the name of idex 
		*  @ unsigned int& out_index - the result of index
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - falied, it doesnt initialize db file
				2 - failed, name is empty
		*/
		int get_index(const std::string name, unsigned int& out_index);


		///------------------------ bind start---------------------------------------------

		/* @ brief: Bind an int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*  std::string& out_str_erre -
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const int in_value, std::string& out_str_exception);

		/**  
		* @brief Bind a 32bits unsigned int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		@ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const unsigned  in_value, std::string& out_str_exception);

		/**
		* @brief Bind a 32bits long value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const long  in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a 64bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const long long in_value, std::string& out_str_exception);


		/** 
		* @brief Bind a double (64bits float) value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const double   in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, const std::string  in_value, std::string& out_str_exception);


		/** 
		* @brief Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* Main usage is with null-terminated literal text (aka in code static strings)
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bindNoCopy(const int in_index, const std::string in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a NULL value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* @see clearBindings() to set all bound parameters to NULL.
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const int in_index, std::string& out_str_exception);


		/**   
		* @brief Bind an int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const int in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a 32bits unsigned int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const unsigned   in_value, std::string& out_str_exception);


		/** 
		* @brief Bind a 32bits long value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const long   in_value, std::string& out_str_exception);

		/* @brief Bind a 64bits int value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const long long       in_value, std::string& out_str_exception);

		/* @brief Bind a double (64bits float) value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const double          in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a string value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* @note Uses the SQLITE_TRANSIENT flag, making a copy of the data, for SQLite internal use
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, const std::string    in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a string value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* The string can contain null characters as it is binded using its size.
		*
		* @warning Uses the SQLITE_STATIC flag, avoiding a copy of the data. The string must remains unchanged while executing the statement.
		* @ return - int
			-1 - failed, _psqlite_db_imp is nullptr
			0 - success
			1 - failed, it doesnt initialize db file
			2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bindNoCopy(const std::string name, const std::string in_value, std::string& out_str_exception);

		/** 
		* @brief Bind a NULL value to a named parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement (in_index >= 1)
		*
		* @see clearBindings() to set all bound parameters to NULL.
		* @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, it doesnt initialize db file
				2 - failed, an error occurs, check [out_str_exception] to get more details
		*/
		int bind(const std::string name, std::string& out_str_exception); // bind NULL value

		// ------------- bind end	----------------------------------------------------



		/* @ brief: clear bindings
		*  @ std::string & out_str_exception - error msg
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed,  it doesnt initialize db file
				2 - failed, an error occurs, please check [out_str_exception] to get more details
		*/
		int clear_bindinds(std::string& out_str_exception);




		// -------------------------------------------------------------------------------
		//
		// excutes sql string
		//
		// -------------------------------------------------------------------------------

		/* @ brief: the following function has the these functioms:  create and drop tables, insert and update a row.
		and it will not work with querying data
		*  @ const std::string  in_str_db_sql - sql string
		*  @ std::string & out_str_exception - exceptional string
		*  @ int& out_result_row - Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE only)
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				-2 - failed, there is no database file from initialising
				-3 - falied, [in_str_db_sql] is empty
				-4 - failed, internal error, the object to communicate with database file was created failed
		*/
		int exec_db_sql(const std::string in_str_db_sql, int& out_result_row, std::string& out_str_exception);



		/* @ brief: query data only
		*  @ const std::string in_str_query_sql - query string of sql
		*  @ std::string & out_str_exception - if an excpetion occurs, you could check the param to get error's detail
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
		*/
		int exec_query_sql(const std::string in_str_query_sql, std::string& out_str_exception);


		/* @ brief: you must run the function [exc_query_sql] , then you could call the following function ,
		it will get the next column after  returnning true;
		Note: if the object do not created, it also returns false
		*  @ return - bool
				true 
				false - failed, _psqlite_db_imp is nullptr
				false - it gets end, or dont initialise the db file and call the [exc_query_sql] function
		*/
		bool exec_query_step();

		/* @ brief: to query the data from the next column
		*  @ std::string & out_str_exception - if an error occurs, you could check the param [out_str_exception] to get details
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, dont initialise the db file and call the [exc_query_sql] function firstly
				2 - failed, please to check the param out_str_exception to get details
		*/
		int exec_query_step(std::string& out_str_exception);

		/*	@ brief: get the column value
		*	@ const unsigned int in_column_index - which column do you wanna get, it starts zero
		*	@ T& out_val - the value
		*	@ std::string& out_str_exception - if an error occurs, check it to get more details
		*	return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, dont initialise the db file and call the [exc_query_sql] function firstly
				2 - failed, an error occured, please check the [out_str_exception] to get details
				3 - failed, it doesnt get any column or in_column_index is out of this range
		*/
		int get_column_int(const unsigned int in_column_index,		int& out_val,			std::string& out_str_exception);
		int get_column_double(const unsigned int in_column_index,	double& out_val,		std::string& out_str_exception);
		int get_column_uint(const unsigned int in_column_index,		unsigned int& out_val,	std::string& out_str_exception);
		int get_column_int64(const unsigned int in_column_index,	long long& out_val,		std::string& out_str_exception);
		int get_column_string(const unsigned int in_column_index,	std::string& out_val,	std::string& out_str_exception);


		// -------------------------------------------------------------------------------
		//
		//  transaction
		//
		// -------------------------------------------------------------------------------

		/* @ brief: Commit the transaction
		*  @ std::string & out_str_exception - if an error occurs, check this param to get more details
		*  @ return - int
				-1 - failed, _psqlite_db_imp is nullptr
				0 - success
				1 - failed, do not initialize database file
				2 - failed, an error occurs, please check the [out_str_exception] to get more details
		*/
		int commit(std::string& out_str_exception);

	private:
		hsqlite_db_imp *_psqlite_db_imp = nullptr;
		//std::unique_ptr<hsqlite_db_imp > _psqlite_db_imp;

	};





	// -------------------------------------------------------------------------------
	//
	// util's set 
	//
	// -------------------------------------------------------------------------------

	class _sqlite_db_api_export_ util_set
	{
	public:
		enum
		{
			// the max length of path
			path_max_len_256 = 256,
		};

		util_set(const util_set& instance) = delete;
		util_set& operator = (const util_set& instance) = delete;
		// -------------------------------------------------------------------------------
		
		/* @ brief: it will return the path of executables, such as on windows: c:\demo\sqlite3
					on linux or mac: Users/xx/Desktop/sqlite.
		*  @ return - std::string
				the  executables' path. the max lenth is 255 chars.
		*/
		static std::string get_exec_path();


	private:
		explicit util_set();
		virtual ~util_set() {};
	};

}

#endif // !_hsqlite_db_h

