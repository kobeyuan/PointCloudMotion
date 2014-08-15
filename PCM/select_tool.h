#ifndef _SELECT_TOOL_H
#define _SELECT_TOOL_H
#include "tool.h"

class PaintCanvas;

class SelectTool : public Tool
{
public:

	enum SELECT_MODE{ POINT_SELECT, RECT_SELECT };

	SelectTool(PaintCanvas* canvas):Tool(canvas){}
	~SelectTool(){}

	virtual void move(QMouseEvent *e);
	virtual void drag(QMouseEvent *e);
	virtual void release(QMouseEvent *e);
	virtual void press(QMouseEvent* e);
	virtual void draw();

protected:
	void select();
	void post_select();

private:
	void draw_rectangle();

private:

	QPoint	mouse_pressed_pos_;
	QPoint	mouse_move_pos_;

	int		select_buffer_size_;
	unsigned int*	select_buffer_;

	SELECT_MODE		cur_select_mode_;

	QRect	rectangle_;

	int				select_region_width_;
	int				select_region_height_;

};


#endif