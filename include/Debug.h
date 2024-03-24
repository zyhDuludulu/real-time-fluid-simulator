#include <iostream>

#define DEBUG(x) 						\
	do { 								\
		std::cerr << x << std::endl; 	\
	} while(0)

#define ASSERT(x, y) 					\
	do { 								\
		if (!(x)) { 					\
			DEBUG(y); 					\
			abort(); 					\
		} 								\
	} while(0)
