#include "file_io.h"
#include "basic_types.h"

namespace FileIO
{
	Sample* load_point_cloud_file( std::string filename, FILE_TYPE type )
	{
		FILE* in_file = fopen(filename.c_str(), "r");

		if(in_file==NULL)
			return nullptr;

		Sample* new_sample = new Sample;
//		ScalarType vx0,vy0,vz0;
// 		bool first_vertex = true;
		while ( true )
		{

			ScalarType vx,vy,vz,nx,ny,nz,cx,cy,cz;
			int status = fscanf(in_file, "%f %f %f %f %f %f %f %f %f\n",&vx,&vy,&vz,
				&nx,&ny,&nz,
				&cx,&cy,&cz);
			if(status==EOF)break;
// 			if ( first_vertex )
// 			{
// 				vx0 = vx,vy0 = vy, vz0 = vz;
// 				PointType v(0.,0.,0.);
// 				ColorType cv(cx/255.,cy/255.,cz/255.,1.);
// 				NormalType nv(nx,ny,nz);
// 				new_sample->add_vertex(v, nv, cv);
// 				first_vertex = false;
// 				continue;
// 			}
// 			PointType v(vx - vx0,vy - vy0,vz - vz0);
			PointType v(vx,vy,vz);
			ColorType cv(cx/255.,cy/255.,cz/255.,1.);
			NormalType nv(nx,ny,nz);

			new_sample->add_vertex(v, nv, cv);

		}

		//give the new sample a color
		new_sample->set_visble(false);
		new_sample->set_color( Color_Utility::random_color_from_table() );
		new_sample->build_kdtree();
		return new_sample;

	}
}