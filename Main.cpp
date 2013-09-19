#include <QStyleFactory>
#include <QDebug>
#include <QWSServer>
#include <QApplication>

#include "CarCtrl.hpp"

int main(int argc, char *argv[])
{
	QApplication App(argc, argv);

	CarCtrl ctrl;
	ctrl.show();
	Q_UNUSED(ctrl)
	return App.exec();
}

