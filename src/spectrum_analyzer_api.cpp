#include "spectrum_analyzer_api.hpp"
#include "ui_spectrum_analyzer.h"
#include "channel_widget.hpp"
#include "db_click_buttons.hpp"

namespace adiscope {
int SpectrumChannel_API::type()
{
	return	spch->averageType();
}

int SpectrumChannel_API::window()
{
	return spch->fftWindow();
}

int SpectrumChannel_API::averaging()
{
	return spch->averaging();
}

bool SpectrumChannel_API::enabled()
{
	return spch->widget()->enableButton()->isChecked();
}

void SpectrumChannel_API::enable(bool en)
{
	spch->widget()->enableButton()->setChecked(en);
}

void SpectrumChannel_API::setType(int type)
{
	spch->setAverageType((adiscope::FftDisplayPlot::AverageType)type);
}

void SpectrumChannel_API::setWindow(int win)
{
	auto taps=sp->fft_size;
	spch->setFftWindow((adiscope::SpectrumAnalyzer::FftWinType)win,taps);
}

void SpectrumChannel_API::setAveraging(int avg)
{
	spch->setAveraging(avg);
}

QList<double> SpectrumChannel_API::data() const
{
	QList<double> list;
	int i = sp->ch_api.indexOf(const_cast<SpectrumChannel_API*>(this));
	int nr_samples = sp->fft_plot->Curve(0)->data()->size();
	for (int j = 0; j < nr_samples; ++j) {
		list.push_back(sp->fft_plot->Curve(i)->sample(j).y());
	}
	return list;
}

QList<double> SpectrumChannel_API::freq() const
{
	QList<double> frequency_data;
	int nr_samples = sp->fft_plot->Curve(0)->data()->size();
	for (int i = 0; i < nr_samples; ++i) {
		frequency_data.push_back(sp->fft_plot->Curve(0)->sample(i).x());
	}
	return frequency_data;
}

int SpectrumMarker_API::chId()
{
	return m_chid;
}

void SpectrumMarker_API::setChId(int val)
{
	m_chid=val;
}

int SpectrumMarker_API::mkId()
{
	return m_mkid;
}

void SpectrumMarker_API::setMkId(int val)
{
	m_mkid=val;
}

int SpectrumMarker_API::type()
{
	if (sp->fft_plot->markerEnabled(m_chid,m_mkid)) {
		return sp->fft_plot->markerType(m_chid,m_mkid);
	}
	return -1;

}

void SpectrumMarker_API::setType(int val)
{
	if (val > SpectrumAnalyzer::markerTypes.size()) {
		val = 0;
	}

	m_type = val;

	if (m_type == 1) { //if type is peak
		sp->fft_plot->marker_to_max_peak(m_chid, m_mkid);
	}
}

double SpectrumMarker_API::freq()
{
	if (sp->fft_plot->markerEnabled(m_chid,m_mkid)) {
		return sp->fft_plot->markerFrequency(m_chid,m_mkid);
	} else {
		return 0;
	}
}

void SpectrumMarker_API::setFreq(double pos)
{
	if (sp->fft_plot->markerEnabled(m_chid,m_mkid)) {
		if (m_type != 1) { //if type is not peak
			sp->fft_plot->setMarkerAtFreq(m_chid,m_mkid,pos);
		}

		//sp->crt_channel_id=m_chid;
		sp->updateWidgetsRelatedToMarker(m_mkid);
		sp->fft_plot->updateMarkerUi(m_chid, m_mkid);

		if (sp->crt_channel_id==m_chid) {
			sp->marker_selector->blockSignals(true);
			sp->marker_selector->setButtonChecked(m_mkid, true);
			sp->marker_selector->blockSignals(false);
		}

		if (m_type != 1) { //if type is not peak
			sp->ui->markerTable->updateMarker(m_mkid, m_chid,
			                                  sp->fft_plot->markerFrequency(m_chid, m_mkid),
							  sp->fft_plot->markerMagnitude(m_chid, m_mkid),
			                                  sp->markerTypes[m_type]);

		}
	}
}

double SpectrumMarker_API::magnitude()
{
	if (sp->fft_plot->markerEnabled(m_chid,m_mkid)) {
		return sp->fft_plot->markerMagnitude(m_chid,m_mkid);
	} else {
		return 0;
	}
}

bool SpectrumMarker_API::enabled()
{
	return sp->fft_plot->markerEnabled(m_chid,m_mkid);
}

void SpectrumMarker_API::setEnabled(bool en)
{
	sp->channels[m_chid]->widget()->nameButton()->setChecked(true);
	sp->channels[m_chid]->widget()->selected(true);
	sp->fft_plot->setMarkerEnabled(m_chid,m_mkid,en);
	sp->marker_selector->setButtonChecked(m_mkid, en);
	sp->updateWidgetsRelatedToMarker(m_mkid);
	sp->fft_plot->updateMarkerUi(m_chid, m_mkid);
}

void SpectrumAnalyzer_API::show()
{
	Q_EMIT sp->showTool();
}

QVariantList SpectrumAnalyzer_API::getMarkers()
{
	QVariantList list;

	for (SpectrumMarker_API *each : sp->marker_api) {
		list.append(QVariant::fromValue(each));
	}

	return list;
}


bool SpectrumAnalyzer_API::running()
{
	return sp->runButton()->isChecked();
}

void SpectrumAnalyzer_API::run(bool chk)
{
	sp->ui->run_button->setChecked(chk);
}

bool SpectrumAnalyzer_API::isSingle()
{
	return sp->ui->btnSingle->isChecked();
}
void SpectrumAnalyzer_API::single(bool chk)
{
	sp->ui->btnSingle->setChecked(chk);
}

QVariantList SpectrumAnalyzer_API::getChannels()
{
	QVariantList list;

	for (SpectrumChannel_API *each : sp->ch_api) {
		list.append(QVariant::fromValue(each));
	}

	return list;
}


int SpectrumAnalyzer_API::currentChannel()
{
	return sp->crt_channel_id;
}

void SpectrumAnalyzer_API::setCurrentChannel(int ch)
{
	sp->channels[ch]->widget()->nameButton()->setChecked(true);
	sp->channels[ch]->widget()->selected(true);
}

double SpectrumAnalyzer_API::startFreq()
{
	return sp->start_freq->value();
}
void SpectrumAnalyzer_API::setStartFreq(double val)
{
	sp->start_freq->setValue(val);
}

double SpectrumAnalyzer_API::stopFreq()
{
	return sp->stop_freq->value();
}
void SpectrumAnalyzer_API::setStopFreq(double val)
{
	sp->stop_freq->setValue(val);
}

QString SpectrumAnalyzer_API::resBW()
{
	return sp->ui->cmb_rbw->currentText();
}
void SpectrumAnalyzer_API::setResBW(QString s)
{
	sp->ui->cmb_rbw->setCurrentText(s);
}


QString SpectrumAnalyzer_API::units()
{
	return sp->ui->cmb_units->currentText();
}

void SpectrumAnalyzer_API::setUnits(QString s)
{
	sp->ui->cmb_units->setCurrentText(s);
}

double SpectrumAnalyzer_API::topScale()
{
	return sp->top->value();
}
void SpectrumAnalyzer_API::setTopScale(double val)
{
	sp->top->setValue(val);
}

double SpectrumAnalyzer_API::range()
{
	return sp->range->value();
}
void SpectrumAnalyzer_API::setRange(double val)
{
	sp->range->setValue(val);
}

bool SpectrumAnalyzer_API::markerTableVisible()
{
	return sp->ui->btnMarkerTable->isChecked();
}

void SpectrumAnalyzer_API::setMarkerTableVisible(bool en)
{
	sp->ui->btnMarkerTable->setChecked(en);
}
}