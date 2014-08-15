#ifndef _PAINT_CANVAS_H
#define _PAINT_CANVAS_H

#include "QGLViewer/qglviewer.h"
#include "tool.h"

class main_window;

class PaintCanvas: public QGLViewer
{
	Q_OBJECT
public:
	PaintCanvas(const QGLFormat& format, QWidget *parent);

public:
	std::string title() const { return "[PaintCanvas]:"; }

	void forceUpdate(){};
	void updateCanvas(){};

protected:
	virtual void draw();
	virtual void init();
	void drawCornerAxis();

	// Mouse events functions
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

	// wheel event
	virtual void wheelEvent(QWheelEvent *e);
private:
	int				coord_system_region_size_;
	main_window*	main_window_;
	Tool*			single_operate_tool_;

public:
	enum WhichColorMode{ VERTEX_COLOR, OBJECT_COLOR, LABEL_COLOR };
	WhichColorMode	which_color_mode_;
};

#endif