
#pragma once

#define _call_dll_

#ifdef _call_dll_
	#include "../sqlite_db_helper/hsqlite_db_interface.h"
#endif // !_call_dll_
#include <iostream>
using namespace std;
#include <memory>
#include <exception>


class Aptr
{
public:
	Aptr()
	{
		std::cout << "this is constructor" << endl;
	}

	~Aptr()
	{
		cout << "this is deconstructor" << endl;
	}
};



class D : public enable_shared_from_this<D>
{
public:
	D()
	{
		cout << "构造函数" << endl;
	}

	~D()
	{
		cout << "析构函数" << endl;
	}


	int i = 0;
};




class error_demo
{
public:
	error_demo(std::string ptext) { str = ptext; }

	const std::string get_err() { return str; }

private:
	std::string str;
};


void throw_error()
{
	int xxxxx = 1;
	if (0 > xxxxx)
		throw(error_demo(std::string("0 > xxx")));
	else
		throw(error_demo(std::string("0 <= xxx")));
}


class uuuuuuuuu
{
public:
	template <typename T, typename U>
	static bool is_same_type(T, U)
	{
		return std::is_same<typename std::decay<T>::type, U>::value;
	}

	int add(int a, int b);

};




template <typename T, typename U>
struct decay_equiv : std::is_same<typename std::decay<T>::type, U>::type{ };



int main(int argc, char *argv[])
{



#ifdef _call_dll_

	unique_ptr<sqlite_db::hsqlite_db> psqlite_db(new sqlite_db::hsqlite_db);

	if (nullptr == psqlite_db)
	{
		std::cout << "error, create psqlite_db failed" << std::endl;
		return 0;
	}

	// get the current executable path	
	std::string str_path = sqlite_db::util_set::get_exec_path();
	str_path += std::string("\\mydatabase.sqlite");
	std::string str_err;

	// 2、inittial 
	int ret_val = psqlite_db->init(str_path, str_err);

#ifdef _use_example_1_

	if (0 == ret_val)
	{
		std::string str_result;

		// 3、to query table
		int get_table = psqlite_db->table_exist("alphabet");
		if (0 != get_table)
		{
			std::cout << "query table [alphabet] failed, it is not include in the database" << std::endl;
		}
		else
		{
			// 4、to call get_single_value function to get value from the database 
			int ret_val = psqlite_db->get_single_value("SELECT name FROM alphabet WHERE id = 2", str_result, str_err);
			if (0 == ret_val)
			{
				std::cout << "query's result = " << str_result.c_str() << std::endl;
			}
		}

		cout << "\n\n\n\n";

	


		system("pause");
#endif // !_use_example_1_

#ifndef _use_example_2_


	if (0 != ret_val)
	{
		cout << "initialise database failed, ret = " << ret_val << endl;
	}
	else
	{
		std::string str_query("SELECT * FROM test");
		std::string str_err;

		// 当前表的总列数
		unsigned int total_count_row = 0;
			
		// 执行查询sql
		int ret_query = psqlite_db->exec_query_sql(str_query, str_err);
		if (0 != ret_query)
		{
			cout << " query failed, ret_val = " << ret_query << endl;
		}
		else
		{
			// 获取表的列总数
			ret_val = psqlite_db->get_table_column_count(total_count_row, str_err);
			cout << "ret_val = " << ret_val << ",	 total_row_count = " << total_count_row << "\n\n\n";

			std::string str_value;
			std::string str_name;
			std::string str_mark;
			std::string str_id;
			
			// 查询表内容
			cout << "\n\n\n first time to query：\n";
			while (psqlite_db->exec_query_step())
			{
				ret_query = psqlite_db->get_column_string(0, str_id,	str_err);
				ret_query = psqlite_db->get_column_string(1, str_name,	str_err);
				ret_query = psqlite_db->get_column_string(2, str_value, str_err);
				ret_query = psqlite_db->get_column_string(3, str_mark,	str_err);


				cout << "id = " << str_id.c_str() << ",		";
				cout << "name = " << str_name.c_str() << ",		";
				cout << "value = " << str_value.c_str() << ",	";
				cout << "mark = " << str_mark.c_str() << endl;
			}

			// 重置以待再次查询该表 
			psqlite_db->reset(str_err);

			// 第二次查询
			cout << "\n\n\n second time to query：\n";
			bool ret_val2 = psqlite_db->exec_query_step();
			while (ret_val2)
			{
				ret_query = psqlite_db->get_column_string(0, str_id, str_err);
				ret_query = psqlite_db->get_column_string(1, str_name, str_err);
				ret_query = psqlite_db->get_column_string(2, str_value, str_err);
				ret_query = psqlite_db->get_column_string(3, str_mark, str_err);


				cout << "id = " << str_id.c_str() << ",		";
				cout << "name = " << str_name.c_str() << ",		";
				cout << "value = " << str_value.c_str() << ",	";
				cout << "mark = " << str_mark.c_str() << endl;

				ret_val2 = psqlite_db->exec_query_step();
			}
		}
	}

	psqlite_db->uninit();


#endif // !_use_example_2_
		


#ifdef _use_xxxxx_
	//shared_ptr<Aptr> ptr;
	//cout << "ptr = " << ptr.get() << endl;

	//ptr.reset(new Aptr);
	//cout << "ptrt = " << ptr.get() << endl;

	////ptr.reset();
	////cout << "ptr = " << ptr.get() << endl;

	//if (false == ptr)
	//{
	//	cout << "false = ptr" << endl;
	//}

	//cout << "\n\n\n\n\n\n\n";



	//Aptr *pold = ptr.get();

	//cout << "ptr = " << ptr << endl;
	//cout << "&ptr = " << &ptr << endl;
	//cout << "ptr_orign = " << ptr.get() << endl;
	//cout << "ptr_old = " << pold << endl;


	//shared_ptr<D> sp1(new D());


	//cout << "\n\n\n\n\n";

	//std::unique_ptr<D> up(new D);
	//std::unique_ptr<D> up2(up);

	//std::unique_ptr<D> up33 = nullptr;
	//up33.reset(new D);
	//up33->i = 2;

	cout << "------------------------------------------------------\n\n\n\n\n\n";


	//std::cout << std::boolalpha
	//	<< decay_equiv<int, double>::value << '\n'
	//	<< decay_equiv<int&, int>::value << '\n'
	//	<< decay_equiv<int&&, int>::value << '\n'
	//	<< decay_equiv<const int&, int>::value << '\n'
	//	<< decay_equiv<int[2], int*>::value << '\n'
	//	<< decay_equiv<int(int), int(*)(int)>::value << '\n';

	cout << "\n\n\n\n";

	int x_int = 0;
	//get_value<decltype(x_int)>(x_int);
	//double x_double = 0.0;
	//get_value<decltype(x_double)>(x_double);
	get_value<int>(x_int);
	cout << "int" << x_int << endl;




	//cout << "up33.i = " << up33->i << std::endl;

	//cout << "\n\n\n\n";
	//up33.reset();
	//up33.reset();
	//up33.reset();

	//up33.reset(new D);
	//if (up33)
	//	cout << "up33.i = " << up33->i << endl;

	



	cout << "\n\n\n------------------------------------------------------\n\n\n\n\n\n";





	//cout << "up = " << up.get() << endl;
	//unique_ptr<D> up2 = std::move(up);
	//cout << "up = " << up.get() << endl;
	//cout << "up2 = " << up2.get() << endl;


	int x = 0;
#endif // !

#endif // !_call_dll_
	// -------------------------------------------------------------------------------

	
	system("pause");
	return 0;
}

