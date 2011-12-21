/*
 *   Copyright (C) 2007, 2008, 2009, 2010, 2011, 2012 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser/Library General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser/Library General Public License for more details
 *
 *   You should have received a copy of the GNU Lesser/Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LANCELOT_DATA_RUNNER_H
#define LANCELOT_DATA_RUNNER_H

#include <lancelot/lancelot_export.h>

#include "BaseModel.h"

#include <QBasicTimer>

#include <Plasma/RunnerManager>
#include <Plasma/QueryMatch>

/**
 * Interface model for KRunner
 */
class Runner: public BaseModel {
    Q_OBJECT

    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString)

public:
    /**
     * Creates a new instance of the Runner model
     * @param limitRunners sets whether default whitelisting
     *      of runners should be enforced
     * @param search initial search string
     */
    explicit Runner(bool limitRunners = false, QString search = QString());

    /**
     * Creates a new instance of the Runner model
     * @param allowedRunners sets which runners should be enabled
     * @param search initial search string
     */
    explicit Runner(QStringList allowedRunners, QString search = QString());

    /**
     * Destroys this Runner model
     */
    virtual ~Runner();

    /**
     * @returns current search string
     */
    QString searchString() const;

    /**
     * @returns which runner is used, if only one runner is used
     */
    QString runnerName() const;

    /**
     * Sets the model to return results only from one runner.
     * Not to be confused with whitelisting and allowed runners.
     */
    void setRunnerName(const QString & name);

    /**
     * Reloads the configuration for the runner
     */
    void reloadConfiguration();

    // L_Override bool hasContextActions(int index) const;
    // L_Override void setContextActions(int index, Lancelot::PopupMenu * menu);
    // L_Override void contextActivate(int index, QAction * context);

    // L_Override QMimeData * mimeData(int index) const;
    // L_Override void setDropActions(int index,
    //         Qt::DropActions & actions, Qt::DropAction & defaultAction);

public Q_SLOTS:
    /**
     * Sets the search string
     * @param search
     */
    void setSearchString(const QString & search);

protected Q_SLOTS:
    /**
     * Sets the search results
     * @param matches results
     */
    void setQueryMatches(const QList<Plasma::QueryMatch> &matches);

protected:
    void activate(int index);
    void load();
    void timerEvent(QTimerEvent * event);

private:
    class Private;
    Private * const d;
};

#endif /* LANCELOT_DATA_DEVICES_H */
