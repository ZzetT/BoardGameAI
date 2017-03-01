#pragma once

#include <vector>
#include <type_traits>


/**
* This class uses a std vector if the size is unknown or a static vector if the maximum size is known in advance.
* A static vector results in constant memory that can be accessed directly (no pointer indirection) and no re-allocation at runtime.
*
*/
template<typename T, int maxSize>
class SmartVector : public std::conditional<(maxSize > 0), static_vector<T, maxSize>, std::vector<T>>::type
{
	
};