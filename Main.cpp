#include <QStyleFactory>
#include <QDebug>
#include <QWSServer>
#include <QApplication>

#include "GunCtrl.hpp"

int main(int argc, char *argv[])
{
	QApplication App(argc, argv);

    GunCtrl ctrl;
	ctrl.show();
	Q_UNUSED(ctrl)
	return App.exec();
}

