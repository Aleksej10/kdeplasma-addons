/***************************************************************************
 *   Copyright (C) 2007-2009 by Shawn Starr <shawn.starr@rogers.com>       *
 *   Copyright (C) 2012 by Luís Gabriel Lima <lampih@gmail.com>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef WEATHERAPPLET_H
#define WEATHERAPPLET_H

#include <Plasma/PopupApplet>
#include <Plasma/DataEngine>
#include <plasmaweather/weatherpopupapplet.h>

class QGraphicsGridLayout;
class QGraphicsLinearLayout;
class QStandardItem;
class QStandardItemModel;

class KDialog;

namespace Plasma
{
    class IconWidget;
    class DeclarativeWidget;
}

class WeatherApplet : public WeatherPopupApplet
{
    Q_OBJECT
public:
    WeatherApplet(QObject *parent, const QVariantList &args);
    ~WeatherApplet();

    void init();
    //QGraphicsWidget *graphicsWidget();

public Q_SLOTS:
    void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data);

protected Q_SLOTS:
    void configAccepted();
    void toolTipAboutToShow();
    void clearCurrentWeatherIcon();

protected:
    void constraintsEvent(Plasma::Constraints constraints);

private Q_SLOTS:
    void invokeBrowser(const QString& url) const;

private:
    Plasma::DeclarativeWidget *m_declarativeWidget;

    Plasma::DataEngine::Data m_currentData; // Current data returned from ion

    QStandardItemModel *m_fiveDaysModel;
    QStandardItemModel *m_detailsModel;

    // Layout
    int m_setupLayout;

    QFont m_titleFont;

    bool isValidData(const QVariant &data) const;
    void weatherContent(const Plasma::DataEngine::Data &data);
    QString convertTemperature(KUnitConversion::UnitPtr format, QString value,
                               int type, bool rounded, bool degreesOnly);
};

K_EXPORT_PLASMA_APPLET(weather, WeatherApplet)

#endif
