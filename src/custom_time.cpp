#include "custom_time.hpp"

#include <utility>
#include <stdexcept>

Time::Time(void) {}
Time::Time([[maybe_unused]] decltype(TIME_UTC) time_utc) { set(); }
Time::Time(time_t seconds, long nseconds) { _t.tv_sec = seconds; _t.tv_nsec = nseconds; }
Time::Time(struct timespec const& ts) : _t(ts) {}
Time::Time(struct timespec&& ts) : _t(std::move(ts)) {}


bool Time::operator==(Time const& rhs) const {
	auto const& lhs_s = _t.tv_sec;
	auto const& rhs_s = rhs._t.tv_sec;
	auto const& lhs_ns = _t.tv_nsec;
	auto const& rhs_ns = rhs._t.tv_nsec;
	if (lhs_s == rhs_s && lhs_ns == rhs_ns) {
		return true;
	}
	return false;
}
bool Time::operator!=(Time const& rhs) const {
	return !operator==(rhs);
}
bool Time::operator>=(Time const& rhs) const {
	auto const& lhs_s = _t.tv_sec;
	auto const& rhs_s = rhs._t.tv_sec;
	auto const& lhs_ns = _t.tv_nsec;
	auto const& rhs_ns = rhs._t.tv_nsec;
	if (lhs_s > rhs_s) {
		return true;
	}
	if (lhs_s < rhs_s) {
		return false;
	}
	return lhs_ns >= rhs_ns ? true : false;
}

bool Time::operator<(Time const& rhs) const {
	return !operator>=(rhs);
}

bool Time::operator<=(Time const& rhs) const {
	return operator<(rhs) || operator==(rhs);
}

bool Time::operator>(Time const& rhs) const {
	return !operator<=(rhs);
}
double Time::as_double(void) const {
	return (double)_t.tv_sec + (double)_t.tv_nsec / (nsecond_to_second_ratio);
}

time_t Time::second(void) const { return _t.tv_sec; }
long Time::nsecond(void) const { return _t.tv_nsec; }

Time Time::pastby(time_t seconds, long nseconds) const {
	struct timespec ts{_t};

	// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	if ((_t.tv_nsec - nseconds) < 0) {
		if(__builtin_add_overflow(seconds, 1, &seconds)) {
			throw std::exception();
		}
		ts.tv_nsec += nsecond_to_second_ratio;
		// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	}
	if (__builtin_sub_overflow(ts.tv_sec, seconds, &ts.tv_sec)) {
		throw std::exception();
	}
	ts.tv_nsec -= nseconds;
	// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	return Time{ts};
}
Time& Time::rewind(time_t seconds, long nseconds)
	{ _t = pastby(seconds, nseconds)._t; return *this; }
	
Time Time::futureby(time_t seconds, long nseconds) const {
	struct timespec ts{_t};
	// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	ts.tv_nsec += nseconds;
	if (ts.tv_nsec > nsecond_upper_bound) {
		ts.tv_nsec -= nsecond_to_second_ratio;
		if(__builtin_add_overflow(seconds, 1, &seconds)) {
			throw std::exception();
		}
		// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	}
	if (__builtin_add_overflow(ts.tv_sec, seconds, &ts.tv_sec)) {
		throw std::exception();
	}
	// std::cerr << "nanos: " << ts.tv_nsec << std::endl;
	return Time{ts};
}
Time& Time::fforward(time_t seconds, long nseconds)
	{ _t = futureby(seconds, nseconds)._t; return *this; }

Time& Time::set(void) {
	timespec_get(&_t, TIME_UTC);
	return *this;
}
Time& Time::set(struct timespec const& ts) { _t = ts; return *this; }
Time& Time::set(struct timespec&& ts) { _t = ts; return *this; }
Time& Time::set(Time const& t) { _t = t._t; return *this; }
Time& Time::set(Time&& t) { _t = t._t; return *this; }


bool Time::ffwdcmpnow(time_t seconds, long nseconds) const {
	return futureby(seconds, nseconds) > Time{TIME_UTC};
}

Time Time::absdiff(Time const& other) const {
	Time ret;
	if (*this > other) {
		ret.fset(this->_t.tv_sec - other._t.tv_sec, this->_t.tv_nsec - other._t.tv_nsec);
		ret.carry_once_sub();
	} else {
		ret.fset(other._t.tv_sec - this->_t.tv_sec, other._t.tv_nsec - this->_t.tv_nsec);
		ret.carry_once_sub();
	}

	return ret;
}

Time Time::time_elapsed(void) const {
	return absdiff(Time{TIME_UTC});
}

bool Time::carry_once_sub(void) {
	if (_t.tv_nsec < nsecond_lower_bound) {
		_t.tv_nsec += nsecond_to_second_ratio;
		if (__builtin_sub_overflow(_t.tv_sec, 1, &_t.tv_sec)) {
			throw std::exception();
		}
		return true;
	}
	return false;
}

bool Time::carry_once_add(void) {
	if (_t.tv_nsec > nsecond_upper_bound) {
		_t.tv_nsec -= nsecond_to_second_ratio;
		if (__builtin_add_overflow(_t.tv_sec, 1, &_t.tv_sec)) {
			throw std::exception();
		}
		return true;
	}
	return false;
}

void Time::fset(time_t seconds, long nseconds) {
	_t.tv_sec = seconds;
	_t.tv_nsec = nseconds;
}
