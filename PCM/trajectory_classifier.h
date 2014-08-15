#ifndef _TRAJECTORY_CLASSIFIER_H
#define _TRAJECTORY_CLASSIFIER_H

#include <QThread>
#include "sample_set.h"


class TrajectoryClassifier : public QThread
{
	Q_OBJECT

public:
	void run() Q_DECL_OVERRIDE;

	signals:
		void finish_compute();

private:
	void derive_rotation_by_svd( VecX& rot,const MatrixX3 &X, const MatrixX3& Y);
};

#endif