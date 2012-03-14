/***************************************************************************
 *   Copyright (C) 2008 by Montel Laurent <montel@kde.org>                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA          *
 ***************************************************************************/

#include "konsoleprofilesapplet.h"

#include <QFileInfo>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsGridLayout>
#include <QStandardItemModel>
#include <QTimer>
#include <QTreeView>

#include <KDirWatch>
#include <KIO/Job>
#include <KGlobalSettings>
#include <KStandardDirs>
#include <KToolInvocation>

KonsoleProfilesApplet::KonsoleProfilesApplet(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args), m_listView( 0 )
{
    setPopupIcon("utilities-terminal");
}

KonsoleProfilesApplet::~KonsoleProfilesApplet()
{
    delete m_listView;
}

void KonsoleProfilesApplet::init()
{
    KDirWatch *dirwatch = new KDirWatch( this );
    const QStringList lst = KGlobal::dirs()->findDirs( "data", "konsole/" );
    for ( int i = 0; i < lst.count(); i++ )
    {
        dirwatch->addDir( lst[i] );
    }
    connect( dirwatch, SIGNAL(dirty(QString)), this, SLOT(slotUpdateSessionMenu()) );
}

QWidget *KonsoleProfilesApplet::widget()
{
    if ( !m_listView )
    {
        m_listView = new QTreeView();
        m_listView->setAttribute(Qt::WA_NoSystemBackground);
        m_listView->setEditTriggers( QAbstractItemView::NoEditTriggers );
        m_listView->setRootIsDecorated(false);
        m_listView->setHeaderHidden(true);
        m_listView->setMouseTracking(true);

        m_konsoleModel = new QStandardItemModel(this);
        m_listView->setModel(m_konsoleModel);
        m_listView->setMouseTracking(true);

        initSessionFiles();

        connect(m_listView, SIGNAL(activated(QModelIndex)),
            this, SLOT(slotOnItemClicked(QModelIndex)));
    }
    return m_listView;
}


void KonsoleProfilesApplet::slotUpdateSessionMenu()
{
    m_konsoleModel->clear();
    initSessionFiles();
}

void KonsoleProfilesApplet::initSessionFiles()
{
    const QStringList list = KGlobal::dirs()->findAllResources( "data", "konsole/*.profile", KStandardDirs::NoDuplicates );
    const QStringList::ConstIterator end = list.constEnd();
    for (QStringList::ConstIterator it = list.constBegin(); it != end; ++it)
    {
        QFileInfo info( *it );
        const QString profileName = KIO::decodeFileName( info.baseName() );
        QString niceName=profileName;
        KConfig cfg( *it, KConfig::SimpleConfig );
        if ( cfg.hasGroup( "General" ) )
        {
            KConfigGroup grp( &cfg, "General" );
            if ( grp.hasKey( "Name" ) )
                niceName = grp.readEntry( "Name" );
            QStandardItem* item = new QStandardItem();
            item->setData(niceName, Qt::DisplayRole);
            item->setData( profileName, ProfilesName );
            m_konsoleModel->appendRow( item);
        }

    }
    m_konsoleModel->sort( 0 );
}

void KonsoleProfilesApplet::slotOnItemClicked(const QModelIndex &index)
{
    hidePopup();
    m_toLaunch = index.data(ProfilesName).toString();
    QTimer::singleShot(0, this, SLOT(slotLaunch()));
}

void KonsoleProfilesApplet::slotLaunch()
{
    if (m_toLaunch.isEmpty()) {
        return;
    }

    QStringList args;
    args << "--profile" << m_toLaunch;
    KToolInvocation::kdeinitExec("konsole", args);
    m_toLaunch.clear();
}

#include "konsoleprofilesapplet.moc"
