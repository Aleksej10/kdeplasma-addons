/***************************************************************************
 *   Copyright (C) 2009 by Björn Ruberg <bjoern@ruberg-wegener.de          *
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


#ifndef PANELICON_H
#define PANELICON_H

#include "plasma/popupapplet.h"
#include "widget.h"

#include "ui_config.h"

class Layout;

class PanelIcon: public Plasma::PopupApplet {
    Q_OBJECT
public:
    PanelIcon(QObject *parent, const QVariantList &args);
    virtual ~PanelIcon();
    QGraphicsWidget *graphicsWidget();

    void constraintsEvent(Plasma::Constraints constraints);

    /*
     * Adds the layout switching entry to the context menu
     */
    void configChanged();
    //QList<QAction*> contextualActions();


public Q_SLOTS:
    void configAccepted();
    void layoutNameChanged(const QString &name);
    /**
      * Initialize the keyboard with the configured layout
      */
    void initKeyboard();

protected:
    void createConfigurationInterface(KConfigDialog *parent);

    /*
     * Clears all pressed keys when keyboard is cloased
     */
    virtual void popupEvent(bool show);

private:
    void init();
    void initKeyboard(const QString &layoutFile);
    void saveLayout(const QString &path);

private:
    QString m_layout;
    QList<Layout*> m_layouts;
    PlasmaboardWidget *m_plasmaboard;
    const QVariantList args;

    Ui::config ui;

};

#endif /* PANELICON_H */


