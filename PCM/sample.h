#ifndef _EXAMPLE_H
#define _EXAMPLE_H
#include "selectable_item.h"
#include "vertex.h"
#include "pool_allocator.h"
#include "box.h"
#include "..\ICP\ICP.h"
#include "Eigen\Dense"
#include "basic_types.h"

class Sample:public SelectableItem
{
public:
	Sample():vertices_(),allocator_(),kd_tree_(nullptr),
				kd_tree_should_rebuild_(true){}
	~Sample();

	inline Vertex* operator[]( IndexType i ) const{ return vertices_[i]; }

	Vertex* add_vertex( const PointType& pos,const NormalType& n,
		const ColorType& c);

	void draw(ColorMode::ObjectColorMode);
	void draw(ColorMode::VertexColorMode);
	void draw(ColorMode::LabelColorMode);
	void draw_with_name();

	size_t num_vertices() const { return vertices_.size(); }

	typedef std::vector<Vertex*>::iterator	vtx_iterator;

	inline vtx_iterator begin(){ return vertices_.begin(); }
	inline vtx_iterator end(){ return vertices_.end(); }

	//Every time vertex change, the kdtree should rebuild
	void	build_kdtree();

	IndexType closest_vtx( const PointType& query_point ) const;
	bool		neighbours(const IndexType query_point_idx, const IndexType num_closet, IndexType* out_indices);
	/* 
		Get matrix for transforming world-sample space to 
		view-sample space , making sure all samples can be saw
		no matter what original coordinates it is
	*/
	inline Matrix44 matrix_to_scene_coord(  );

	/* Green channel to get all vertex position information */
	inline const Matrix3X&	vertices_matrix() const{ return vtx_matrix_; }

// 	const PointType box_center() const{ return box_.center(); }
// 	const ScalarType	box_diag() const { return box_.diag(); }

private:
	std::vector<Vertex*>	vertices_;
	PoolAllocator			allocator_;
	Box						box_;

	Matrix3X									vtx_matrix_; // 3 * NumOfVtx
	//Attention: kdtree is just a adapter, it means it use the reference of its data source
	nanoflann::KDTreeAdaptor<Matrix3X, 3>*		kd_tree_;
	bool										kd_tree_should_rebuild_;
};

#endif