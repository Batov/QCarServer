// Copyright 2013 Batov

//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at

//        http://www.apache.org/licenses/LICENSE-2.0

//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

#include <QStyleFactory>
#include <QDebug>
#include <QWSServer>

#include "CarCtrl.hpp"

int main(int argc, char *argv[])
{
	QApplication App(argc, argv);

#ifdef Q_WS_QWS
	QWSServer::setCursorVisible( false );
#endif
	App.setStyle(QStyleFactory::create("Cleanlooks"));

	CarCtrl ctrl;
	Q_UNUSED(ctrl)
	return App.exec();
}

