#pragma once
#include "ui_centerout_example.h"
#include <QIntValidator>
#include <QString>
#include <QTabWidget>
#include <QLayout>

class CenterOutPlugin;

class CenterOutGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	CenterOutGui(CenterOutPlugin* p):plugin(p),uisAccepted(0)
	{
		ui.setupUi(this);
	}
	void acceptChildUI(QWidget* child)
	{
		switch(uisAccepted)
		{
		case 0:	ui.viewerScrollArea->setWidget(child); break;
		case 1: ui.optotrakScrollArea->setWidget(child); break;
		default: break;
		}
		++uisAccepted;
	}
public slots:
	void on_startButton_clicked();
	void on_initializeOptotrakButton_clicked();
	void on_initializeRealtimeButton_clicked();
protected:
	CenterOutPlugin* plugin;
	Ui::TutorialDialog ui;
	int uisAccepted;
};