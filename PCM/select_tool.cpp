#include "select_tool.h"
#include "sample_set.h"
#include "windows.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "paint_canvas.h"

void SelectTool::move(QMouseEvent *e)
{

}

void SelectTool::drag(QMouseEvent *e)
{

}

void SelectTool::release(QMouseEvent *e)
{

}

void SelectTool::press(QMouseEvent* e)
{
	Sample* sample = SampleSet::get_instance()[cur_sample_to_operate_];

	int select_buffer_size_ = sample->num_vertices()*4;

	select_buffer_ = new unsigned int[select_buffer_size_];
}

void SelectTool::draw()
{

	if ( !select_buffer_ )
	{
		return;
	}
	glSelectBuffer( select_buffer_size_, select_buffer_ );
	glRenderMode(GL_SELECT);
	glInitNames();

	// Loads the matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	static GLint viewport[4];
	canvas_->camera()->getViewport(viewport);
	gluPickMatrix(mouse_pressed_pos_.x(), mouse_pressed_pos_.y(), 
		select_region_width_, select_region_height_, viewport);

	// loadProjectionMatrix() first resets the GL_PROJECTION matrix with a glLoadIdentity().
	// The false parameter prevents this and hence multiplies the matrices.
	canvas_->camera()->loadProjectionMatrix(false);
	// Reset the original (world coordinates) modelview matrix
	canvas_->camera()->loadModelViewMatrix();
}


void post_select()
{

}