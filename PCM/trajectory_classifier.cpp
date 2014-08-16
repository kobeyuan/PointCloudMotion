#include "trajectory_classifier.h"
#include "globals.h"
#include "basic_types.h"
#include "sample.h"
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>

void TrajectoryClassifier::run(){

	Logger << "Begin Clustering.\n";

	SampleSet& set = SampleSet::get_instance();

	if (set.empty())
	{
		Logger<< " End Clustering(Warning: Empty Set).\n";
		emit finish_compute();
		return;
	}

	const IndexType num_of_neighbours = 9;

	IndexType	num_sample = set.size();
	IndexType	num_vtx	= set[0]->num_vertices();



	//Rotation feature for clustering
	MatrixXX	rot_feature_mat( num_vtx, (num_sample-1)*9 );


	//Step 1: compute rotate feature

	for(IndexType s_idx = 0; s_idx < num_sample - 1;
			s_idx++ )
	{
		const Matrix3X&	orig_vtx_coord_matrix = set[s_idx]->vertices_matrix();
		const Matrix3X&	dest_vtx_coord_matrix = set[s_idx + 1]->vertices_matrix();
		for (IndexType v_idx = 0; v_idx < num_vtx; v_idx++)
		{
			
			VecX rot(9,1);
			IndexType origin_neighbours[num_of_neighbours];
			IndexType dest_neighbours[num_of_neighbours];

			//Get neighbours of the specific vertex between two sample
			set[s_idx]->neighbours(v_idx, num_of_neighbours, origin_neighbours);
			set[s_idx + 1]->neighbours(v_idx, num_of_neighbours, dest_neighbours);

			std::sort( origin_neighbours, origin_neighbours+num_of_neighbours );
			std::sort( dest_neighbours, dest_neighbours + num_of_neighbours );

			MatrixX3	X(num_of_neighbours, 3);
			MatrixX3	Y(num_of_neighbours, 3);

			for ( int j = 0; j<num_of_neighbours; j++ )
			{
				X.row(j) << orig_vtx_coord_matrix(0, origin_neighbours[j]),
							orig_vtx_coord_matrix(1, origin_neighbours[j]),
							orig_vtx_coord_matrix(2, origin_neighbours[j]);
				Y.row(j) << dest_vtx_coord_matrix(0, dest_neighbours[j]),
							dest_vtx_coord_matrix(1, dest_neighbours[j]),
							dest_vtx_coord_matrix(2, dest_neighbours[j]);
			}
			derive_rotation_by_svd(rot, X, Y);

			rot_feature_mat.block<1,9>( v_idx,s_idx*9 ) << rot.transpose();
		}
	}

	//Step 2: Begin Clustering, use opencv 's k-means clustering
	using namespace cv;
	Mat	cluster_data;
	Mat labels;
	Mat centers;
	eigen2cv( rot_feature_mat, cluster_data );
	kmeans( cluster_data, 2, labels,  
		TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 200, 0.00001),
		5, KMEANS_PP_CENTERS, centers);


	//Step 3: Store the results
	LOCK( set[0] );
	Sample* sample0 = set[0];
	IndexType i = 0;
	for (Sample::vtx_iterator v_iter = sample0->begin();
		v_iter != sample0->end();
		v_iter++,i++ )
	{
		(*v_iter)->set_label( labels.at<IndexType>(i) );
	}
	UNLOCK(set[0]);

	Logger<< " End Clustering.\n";
	emit finish_compute();
}


void TrajectoryClassifier::derive_rotation_by_svd( VecX& rot, const MatrixX3& X, const MatrixX3& Y)
{


	Matrix33 sigma = (X.rowwise() - X.colwise().mean()).transpose() * (Y.rowwise() - Y.colwise().mean());
	Matrix33 rot_mat;

	Eigen::JacobiSVD<Matrix33> svd(sigma, Eigen::ComputeFullU | Eigen::ComputeFullV);
	if(svd.matrixU().determinant()*svd.matrixV().determinant() < 0.0) {
		Vec3 S = Vec3::Ones(); S(2) = -1.0;
		 rot_mat = svd.matrixV()*S.asDiagonal()*svd.matrixU().transpose();
	} else {
		rot_mat = svd.matrixV()*svd.matrixU().transpose();
	}

	rot.block<3,1>(0,0) << rot_mat.row(0).transpose();
	rot.block<3,1>(3,0) << rot_mat.row(1).transpose();
	rot.block<3,1>(6,0) << rot_mat.row(2).transpose();
}