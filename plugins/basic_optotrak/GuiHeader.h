#include "ui_basic_optotrak.h"
#include <QIntValidator>
#include <QString>
#include "Opto.h"
class TutorialGui : public QDialog
{
	Q_OBJECT//this macro flags the class for the moc tool
public:
	TutorialGui()
	{
        
		ui.setupUi(this);
		QIntValidator* validator = new QIntValidator(this);
		ui.val1->setValidator(validator);
		ui.val2->setValidator(validator);
		connect(ui.val1,SIGNAL(textChanged(const QString&)),this,SLOT(val1changed(const QString&)));
		connect(ui.val2,SIGNAL(textChanged(const QString&)),this,SLOT(val2changed(const QString&)));

		
	}
public slots:
	void val1changed(const QString& s)
	{
		val1=s.toInt();
		add();
	}
	void val2changed(const QString& s)
	{
		val2=s.toInt();
		add();
	}
	void on_connectButton_clicked()
	{
		testOptotrak();
	}
protected:
	void add()
	{
		ui.val3->setText(QString::number(val1+val2));
	}

	Ui::TutorialDialog ui;
	int val1;
	int val2;
};