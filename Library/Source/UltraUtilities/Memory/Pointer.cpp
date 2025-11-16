#include "UltraUtilities/Memory/Pointer.hpp"

namespace UU
{
	Reference::Reference(void* pointer)
	{
		this->pointer = pointer;
		this->count = 1;
		this->handle = nextHandle++;
		refMap.Insert(this->handle, this);
	}

	/*virtual*/ Reference::~Reference()
	{
		refMap.Remove(this->handle);
	}

	unsigned long long Reference::nextHandle = 1;
	HashMap<unsigned long long, Reference*> Reference::refMap;
}