#include <string>

#ifndef TI_INSTANCEREADER
#define TI_INSTANCEREADER

#include "TSPInstance.hpp"

/**
 * Singleton that implements the method that reads the instance file
 * and generates the appropriate instance
 */
class InstanceReader {
	private:
	InstanceReader();

	public:
	InstanceReader(InstanceReader const&);
	void operator=(InstanceReader const&); 
	static InstanceReader getObject();
	TSPInstance readTSPInstance(std::string);
};

#endif
