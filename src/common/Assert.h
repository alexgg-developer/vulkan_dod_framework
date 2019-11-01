#pragma once

#ifdef _DEBUG
	#ifdef _MSC_VER
		#define ASSERT(x) if (!(x)) __debugbreak();
	#else assert(x)
	#endif
#else
	#define ASSERT(x) assert(x)
#endif
