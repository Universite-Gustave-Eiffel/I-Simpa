#include <iostream>
#include <boost/filesystem.hpp>

int main()
{
	boost::filesystem::remove_all("Punctual receivers");
	return 0;
}