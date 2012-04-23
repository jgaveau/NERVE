#pragma once
#include "ui_centerout_example.h"
#include <QIntValidator>
#include <QString>
#include <QTabWidget>
#include <QLayout>
#include "nrvToolbox/RebroadcastManager.h"
#include "nrvToolbox/RebroadcastQtAdapter.h"

class CenterOutPlugin;

class CenterOutGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	CenterOutGui(CenterOutPlugin* p):plugin(p),uisAccepted(0)
	{
		ui.setupUi(this);
		
		connect(ui.MovementDuration, SIGNAL(valueChanged(double)),this,SLOT(on_MovementDuration_valueChanged2(double)));
		rebroadcastQtAdapter.setRebroadcastManager(&rebroadcastManager);
		rebroadcastQtAdapter.addWithChildren(this);

		
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
	RebroadcastManager* getRebroadcastManagerPtr()
	{
		return rebroadcastManager.createReciprocallyConnectedManager();
	}
public slots:
	void on_startButton_clicked();
	void on_initializeOptotrakButton_clicked();
	void on_initializeRealtimeButton_clicked();
	void on_MovementDuration_valueChanged2(double value);
protected:
	CenterOutPlugin* plugin;
	Ui::TutorialDialog ui;
	int uisAccepted;

	RebroadcastManager rebroadcastManager;
	RebroadcastQtAdapter rebroadcastQtAdapter;
};