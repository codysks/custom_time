#include "custom_time.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <vector>

#include <cassert>
#ifdef NDEBUG
# error "NDEBUG must not be set"
#endif

#define assertnot(EXPR) assert(!(EXPR));

int main(void) {
	size_t section = 0;
	size_t test_number = 0;
	auto pt = [&](Time const& t, std::ostream& o = std::cout) {
		o << section << ":" << test_number << ":";
		o << t.second() << "." 
			<< std::setfill('0') << std::setw(10) << t.nsecond() << std::endl;
		++test_number;
	};
	{
// Constructors
//		Time(void);
//		Time(decltype(TIME_UTC) time_utc);
//		Time(time_t seconds, long nseconds);
//		Time(struct timespec const& ts);
//		Time(struct timespec&& ts);
		Time tsnow{TIME_UTC};
		auto pcont = std::vector<Time>{
			Time{},
			Time{TIME_UTC},

			Time{1739267836, 0},
			Time{1739267836, 1},
			Time{1739267836, 10},
			Time{1739267836, 1000},
			Time{1739267836, 999999998},
			Time{1739267837, 999999999},

			Time{tsnow},
			Time{Time{1739267838, 0}}
		};
		for (auto const& p : pcont) {
			pt(p);
		}
	}
// Comparison Operators
//		bool operator>(Time const& rhs) const;
//		bool operator<(Time const& rhs) const;
//		bool operator==(Time const& rhs) const;
//		bool operator!=(Time const& rhs) const;
//		bool operator<=(Time const& rhs) const;
//		bool operator>=(Time const& rhs) const;
	{
		Time t_hi  {1739267836, 750000001};
		Time t_low {1000000000, 500000000};
		assert(t_hi > t_low);
		assert(t_hi >= t_low);
		assert(!(t_hi == t_low));
		assert(t_hi != t_low);
		assert(!(t_hi <= t_low));
		assert(!(t_hi < t_low));
	}
	{
		Time t_hi  {1739267836, 499999999};
		Time t_low {1000000000, 500000000};
		assert(t_hi > t_low);
		assert(t_hi >= t_low);
		assert(!(t_hi == t_low));
		assert(t_hi != t_low);
		assert(!(t_hi <= t_low));
		assert(!(t_hi < t_low));
	}
	{
		Time t_hi  {1000000000, 000000000};
		Time t_low { 999999999, 999999999};
		assert(t_hi > t_low);
		assert(t_hi >= t_low);
		assert(!(t_hi == t_low));
		assert(t_hi != t_low);
		assert(!(t_hi <= t_low));
		assert(!(t_hi < t_low));
	}
	{
		Time t_hi  {1000000000, 500000001};
		Time t_low {1000000000, 500000000};
		assert(t_hi > t_low);
		assert(t_hi >= t_low);
		assert(!(t_hi == t_low));
		assert(t_hi != t_low);
		assert(!(t_hi <= t_low));
		assert(!(t_hi < t_low));
	}
	{
		Time t_mid  {1000000000, 500000000};
		Time t_mid2 {1000000000, 500000000};

		assertnot(t_mid > t_mid2);
		assert(t_mid >= t_mid2);

		assert(t_mid == t_mid2);
		assertnot(t_mid != t_mid2);

		assert(t_mid <= t_mid2);
		assertnot(t_mid < t_mid2);
	}

	{
		Time t_mid  {1000000000, 500000000};
		Time const& t_mid2 = t_mid;

		assertnot(t_mid > t_mid2);
		assert(t_mid >= t_mid2);

		assert(t_mid == t_mid2);
		assertnot(t_mid != t_mid2);

		assert(t_mid <= t_mid2);
		assertnot(t_mid < t_mid2);
	}
// Rewind and forward functions
	{
		++section; test_number = 0;
		Time t1(10, 999999999);
		pt(t1.pastby(1, 999999999));
		pt(t1.pastby(1, 999999998));
	}
// Set functions
	{
		++section; test_number = 0;
		Time p1{TIME_UTC};
		pt(p1);
		pt(p1.set());
		pt(p1.set());
		pt(p1.set());
		sleep(1);
		pt(p1.set());
		pt(p1.set());
		pt(p1.set());
	}
	{
		++section; test_number = 0;
		Time p1{TIME_UTC};
		pt(p1);
		struct timespec nsleepfor;
		nsleepfor.tv_sec = 0;
		nsleepfor.tv_nsec = 333333333;
		while (p1.ffwdcmpnow(5, 0)) {
			pt(Time{TIME_UTC});
			std::cerr << "Sleeping for 333333333 nseconds" << std::endl;
			nanosleep(&nsleepfor, nullptr);
		}
	}
	return EXIT_SUCCESS;
}
