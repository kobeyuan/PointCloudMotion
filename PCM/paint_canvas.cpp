#include "paint_canvas.h"
#include "sample_set.h"
#include "main_window.h"
#include "basic_types.h"
using namespace qglviewer;

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif


PaintCanvas::PaintCanvas(const QGLFormat& format, QWidget *parent):
	QGLViewer(format, parent),
	coord_system_region_size_(150),
	which_color_mode_(OBJECT_COLOR)
{
	main_window_ = (main_window*)parent;

	camera()->setPosition(Vec(1.0,  1.0, 1.0));	

	camera()->lookAt(sceneCenter());
	camera()->setType(Camera::PERSPECTIVE);
	camera()->showEntireScene();
}

void PaintCanvas::draw()
{
	glEnable(GL_MULTISAMPLE);

	drawCornerAxis();

	SampleSet& set = SampleSet::get_instance();
	if ( !set.empty() )
	{
		glDisable(GL_MULTISAMPLE);
		for (size_t i = 0; i <set.size(); i++ )
		{
			LOCK(set[i]);
			switch (which_color_mode_)
			{
			case PaintCanvas::VERTEX_COLOR:
				set[i]->draw(ColorMode::VertexColorMode());
				break;
			case PaintCanvas::OBJECT_COLOR:
				set[i]->draw(ColorMode::ObjectColorMode());
				break;
			case PaintCanvas::LABEL_COLOR:
				set[i]->draw(ColorMode::LabelColorMode());
				break;
			default:
				break;
			}
			UNLOCK(set[i]);
		}
		glEnable(GL_MULTISAMPLE);
	}
}

void PaintCanvas::init()
{
	setStateFileName("");
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	QColor background_color = Qt::white;
	setBackgroundColor(background_color);

	setGridIsDrawn();
	//camera()->frame()->setSpinningSensitivity(100.f);
	setMouseTracking(true);
	// light0
	GLfloat	light_position[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Setup light parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); /*GL_FALSE*/
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_LIGHT0);		// Enable Light 0
	glEnable(GL_LIGHTING);

	//////////////////////////////////////////////////////////////////////////

	/* to use facet color, the GL_COLOR_MATERIAL should be enabled */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	/* to use material color, the GL_COLOR_MATERIAL should be disabled */
	//glDisable(GL_COLOR_MATERIAL);

}


void PaintCanvas::drawCornerAxis()  
{
	int viewport[4];
	int scissor[4];

	glDisable(GL_LIGHTING);

	// The viewport and the scissor are changed to fit the lower left
	// corner. Original values are saved.
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_SCISSOR_BOX, scissor);

	// Axis viewport size, in pixels
	glViewport(0, 0, coord_system_region_size_, coord_system_region_size_);
	glScissor(0, 0, coord_system_region_size_, coord_system_region_size_);

	// The Z-buffer is cleared to make the axis appear over the
	// original image.
	glClear(GL_DEPTH_BUFFER_BIT);

	// Tune for best line rendering
	glLineWidth(5.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(camera()->orientation().inverse().matrix());

	float axis_size = 1.2f; // other 0.2 space for drawing the x, y, z labels
	drawAxis(axis_size); 

	// Draw text id
	glDisable(GL_LIGHTING);
	glColor3f(0, 0, 0);
	renderText(axis_size, 0, 0, "X");
	renderText(0, axis_size, 0, "Y");
	renderText(0, 0, axis_size, "Z");

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// The viewport and the scissor are restored.
	glScissor(scissor[0], scissor[1], scissor[2], scissor[3]);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glEnable(GL_LIGHTING);
}

void PaintCanvas::mousePressEvent(QMouseEvent *e)
{
	QGLViewer::mousePressEvent(e);
}

void PaintCanvas::mouseMoveEvent(QMouseEvent *e)
{
	main_window_->showCoordinateAndIndexUnderMouse( e->pos() );

	QGLViewer::mouseMoveEvent(e);
}

void PaintCanvas::mouseReleaseEvent(QMouseEvent *e)
{
	QGLViewer::mouseReleaseEvent(e);
}

void PaintCanvas::wheelEvent(QWheelEvent *e)
{

	if (e->modifiers() == Qt::ControlModifier)
	{
		int numDegrees = e->delta() / 120;

		Paint_Param::g_point_size += 1.f * numDegrees;
		if (Paint_Param::g_point_size < 0.)
		{
			Paint_Param::g_point_size = 0.;
		}

		updateGL();
	}

	QGLViewer::wheelEvent(e);
}