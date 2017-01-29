#ifndef UTIL_H
#define UTIL_H

#include <iostream>

// Super print!
void suprint(const char * info, bool error = false) {
	std::cout << info << std::endl;
	if (error) {
		system("pause");
		exit(-1);
	}
}

#endif
