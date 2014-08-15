#ifndef _TOOL_H
#define _TOOL_H
#include <QPoint>
#include <QMouseEvent>

class PaintCanvas;

enum ToolType{ EMPTY_TOOL, SELECT_TOOL };

class Tool
{
public:
	Tool( PaintCanvas* canvas ):canvas_(canvas){}
	virtual ~Tool(){}

public:
	virtual void press(QMouseEvent *e) = 0;
	virtual void move(QMouseEvent *e) = 0;
	virtual void release(QMouseEvent *e) = 0;
	virtual void drag(QMouseEvent *e) = 0;
	virtual void draw();
	ToolType	tool_type () const { return tool_type_; }
	void	set_tool_type( ToolType type ){ tool_type_ = type; }
	unsigned int	cur_sample_to_operate() const { return cur_sample_to_operate_; }
	void set_cur_smaple_to_operate( unsigned int sample_idx )
	{
		cur_sample_to_operate_ = sample_idx;
	}
protected:
	ToolType	tool_type_;
	unsigned int	cur_sample_to_operate_;

	PaintCanvas*	canvas_;
};

#endif