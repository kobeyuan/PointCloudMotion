#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"
#include "paint_canvas.h"
#include "globals.h"
#include <string>
#include <map>
#include <QLabel>

using namespace std;


class main_window : public QMainWindow
{
	Q_OBJECT

public:
	main_window(QWidget *parent = 0);
	~main_window();
	static std::string title() { return "[MainWindow]: "; }

	void showCoordinateAndIndexUnderMouse( const QPoint& point );

	public slots:
		bool openFile();
		bool openFiles();
		void selectedSampleChanged(QTreeWidgetItem * item, int column);

		bool setSampleVisible();
		bool setSampleInvisible();

		void doClustering();
		void finishClustering();

		void setObjectColorMode();
		void setVertexColorMode();
		void setLabelColorMode();

private:
		void createAction();
		void createFileMenuAction();
		void createPaintSettingAction();
		void createAlgorithmAction();
		void createTreeWidgetItems();
		void createStatusBar();
		void resetSampleSet();


private:

	//UI
	Ui::main_windowClass ui;
	PaintCanvas*		main_canvas_;

	QLabel*			coord_underMouse_label_;
	QLabel*			vtx_idx_underMouse_label_;

	//Samples Info
	vector< pair<string,string> >		cur_import_files_attr_;
	int			cur_select_sample_idx_;
	int			last_select_sample_idx_;



};


#endif // MAIN_WINDOW_H
