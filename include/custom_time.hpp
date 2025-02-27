#ifndef CUSTOM_TIME_HPP
#define CUSTOM_TIME_HPP

#include <ctime>

class Time {
	constexpr static long nsecond_lower_bound = 0;
	constexpr static long nsecond_upper_bound = 999999999;
	constexpr static long nsecond_to_second_ratio = nsecond_upper_bound + 1;
	public:
		Time(void);
		Time(decltype(TIME_UTC) time_utc);
		Time(time_t seconds, long nseconds);
		Time(struct timespec const& ts);
		Time(struct timespec&& ts);

		bool operator>(Time const& rhs) const;
		bool operator<(Time const& rhs) const;
		bool operator==(Time const& rhs) const;
		bool operator!=(Time const& rhs) const;
		bool operator<=(Time const& rhs) const;
		bool operator>=(Time const& rhs) const;
		[[nodiscard]] double as_double(void) const;
		[[nodiscard]] time_t second(void) const;
		[[nodiscard]] long nsecond(void) const;

		Time& set(struct timespec const& ts);
		Time& set(struct timespec&& ts);
		Time& set(Time const& t);
		Time& set(Time&& t);
		Time& set(void);
		Time& rewind(time_t seconds, long nseconds = 0);
		Time& fforward(time_t seconds, long nseconds = 0);
		[[nodiscard]] Time futureby(time_t seconds, long nseconds = 0) const;
		[[nodiscard]] Time pastby(time_t seconds, long nseconds = 0) const;
/*
 *  Compares a fast-forwarded time to the current time.
 *  If the forwarded time is more recent than the current time,
 *  then return true. This is useful for setting thresholds for
 *  cyclic actions such as heartbeats by forwarding the last
 *  heartbeat time and comparing with the current time to
 *  determine if a heartbeat should be sent
 */
		bool ffwdcmpnow(time_t seconds, long nseconds = 0) const;

		Time absdiff(Time const &other) const;

	private:
/*
 *  Unchecked variant of set. This may cause the state of the object
 *  to become invalid without throwing any exceptions or indicate any
 *  errors.
 */
		void fset(time_t second, long nseconds = 0);
		bool carry_once_sub(void);
		bool carry_once_add(void);
		struct timespec _t;
};


#endif /* CUSTOM_TIME_HPP */
