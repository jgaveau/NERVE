#include "GuiHeader.h"
#include "plugin.h"

void CenterOutGui::on_startButton_clicked()
{
	plugin->startTask();
}
void CenterOutGui::on_initializeOptotrakButton_clicked()
{
	plugin->initOptotrak();
}
void CenterOutGui::on_initializeRealtimeButton_clicked()
{
	plugin->initRealtime();
}
void CenterOutGui::on_setZero_clicked()
{
	plugin->setZero();
}