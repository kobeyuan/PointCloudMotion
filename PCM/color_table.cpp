#include "color_table.h"
#include <time.h>
#include <algorithm>

namespace Color_Utility
{
	ColorType color_from_table(IndexType index)
	{
		if((255 < index) && (index <= 512)) 
			index = 512 - index;
		if((512 < index) && (index <= 768)) 
			index = index - 512;
		if(index > 768) 
			index = 1024-index;

		if(index < 0) 
			index = 0;
		if(index > 255) 
			index = 255;

		ScalarType r = static_cast<ScalarType>( color_table[index][0] );
		ScalarType g = static_cast<ScalarType>( color_table[index][1] );
		ScalarType b = static_cast<ScalarType>( color_table[index][2] );

		return ColorType(r,g,b,1.);
	}

	ColorType random_color_from_table() {

		IndexType random_int = rand() / 256;

		int rand_idx_array[256];
		for (int i=0 ; i <256; i++)
		{
			rand_idx_array[i] = i;
		}

		std::random_shuffle(rand_idx_array, rand_idx_array+256);

		return color_from_table(rand_idx_array[random_int]);
	}
}