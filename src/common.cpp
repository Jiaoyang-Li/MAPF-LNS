#include "common.h"

std::ostream& operator<<(std::ostream& os, const Path& path)
{
	for (const auto& state : path)
	{
		os << state.location << "\t"; // << "(" << state.is_single() << "),";
	}
	return os;
}


bool isSamePath(const Path& p1, const Path& p2)
{
	if (p1.size() != p2.size())
		return false;
	for (unsigned i = 0; i < p1.size(); i++)
	{
		if (p1[i].location != p2[i].location)
			return false;
	}
	return true;
}

double getTimeDurationFrom(clock_t t) { return (double) (clock() - t) / CLOCKS_PER_SEC; }
double getTimeDuration(clock_t t1, clock_t t2) { return (double) (t2 - t1) / CLOCKS_PER_SEC; }