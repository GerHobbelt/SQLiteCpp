#ifndef _utils_h
#define _utils_h
#include <map>

namespace sqlite_db
{
	class hutils
	{
	public:
		virtual ~hutils() {}
		explicit hutils()
		{
			
			//_map_sqlite_open_type.insert(std::make_pair(8, 8));
		}

		// -------------------------------------------------------------------------------
		hutils(const hutils& instance) = delete;
		hutils& operator = (const hutils& instance) = delete;
		// -------------------------------------------------------------------------------


		/* @ brief: get the right open_type of sqlite, if cannot find that, it will return defaule(1)
		*  @ const int & open_type - find_type
		*  @ return - int
		*
		*/
		static int get_sqlite_open_type(const int& open_type)
		{
			// <open_type, opentype>
			static std::map<int, int> map_sqlite_open_type;

			map_sqlite_open_type.insert(std::make_pair(1, 1));
			map_sqlite_open_type.insert(std::make_pair(2, 2));
			map_sqlite_open_type.insert(std::make_pair(4, 4));
			auto it = map_sqlite_open_type.find(open_type);

			
			return (map_sqlite_open_type.end() == it)? 1: open_type;
		}


		template <typename T, typename U>
		static bool is_same_type(T , U )
		{
			return std::is_same<typename std::decay<T>::type, U>::value;
		}
		


	};

}

#endif // !_utils_h