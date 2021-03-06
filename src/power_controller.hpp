/*
 * Copyright (c) 2019 Analog Devices Inc.
 *
 * This file is part of Scopy
 * (see http://www.github.com/analogdevicesinc/scopy).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef POWER_CONTROLLER_HPP
#define POWER_CONTROLLER_HPP

#include <QPushButton>
#include <QTimer>

#include "apiObject.hpp"
#include "spinbox_a.hpp"
#include "tool.hpp"

extern "C" {
	struct iio_context;
	struct iio_channel;
}

namespace Ui {
	class PowerController;
}

class QJSEngine;
class QShowEvent;
class QHideEvent;

namespace adiscope {
	class PowerController_API;

	class PowerController : public Tool
	{
		friend class PowerController_API;
		friend class ToolLauncher_API;

		Q_OBJECT

	public:
		const int AVERAGE_COUNT = 5;

		explicit PowerController(struct iio_context *ctx,
				ToolMenuItem *toolMenuItem, QJSEngine *engine,
				ToolLauncher *parent = 0);
		~PowerController();

	public Q_SLOTS:
		void dac1_set_enabled(bool enabled);
		void dac2_set_enabled(bool enabled);
		void dac1_set_value(double value);
		void dac2_set_value(double value);
		void update_lcd();
		void sync_enabled(bool enabled);
		void run() override;
		void stop() override;

	private Q_SLOTS:
		void startStop(bool start);
		void ratioChanged(int percent);
		void toggleRunButton(bool enabled);

	private:
		Ui::PowerController *ui;
		PositionSpinButton *valuePos;
		PositionSpinButton *valueNeg;
		struct iio_channel *ch1w, *ch2w, *ch1r, *ch2r, *pd_pos, *pd_neg;
		QTimer timer;
		bool in_sync;
		QList<long long> averageVoltageCh1;
		QList<long long> averageVoltageCh2;
		QMap<QString, double> calibrationParam;

		void showEvent(QShowEvent *event);
		void hideEvent(QHideEvent *event);

	Q_SIGNALS:
		void showTool();
	};
}
#endif /* POWER_CONTROLLER_HPP */
