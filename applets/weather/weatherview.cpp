/*
 *   Copyright 2008 Marco Martin <notmart@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "weatherview.h"

#include <QTreeView>
#include <QHeaderView>

#include <KIconLoader>
#include <KDebug>

#include "weatherdelegate.h"

namespace Plasma
{

WeatherView::WeatherView(QGraphicsWidget *parent)
        : TreeView(parent)
{
    setAcceptedMouseButtons(Qt::NoButton);
    
    QTreeView *native = nativeWidget();
    native->viewport()->setAutoFillBackground(false);
    native->header()->hide();
    native->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    native->setIconSize(QSize(KIconLoader::SizeMedium, KIconLoader::SizeMedium));
    native->setRootIsDecorated(false);

    m_delegate = new WeatherDelegate(this);
    native->setItemDelegate(m_delegate);
}

WeatherView::~WeatherView()
{
}

int WeatherView::columnWidth(int column) const
{
   return nativeWidget()->columnWidth(column);
}

int WeatherView::sizeHintForColumn(int column) 
{
   return qobject_cast<QAbstractItemView*>(nativeWidget())->sizeHintForColumn(column);
}


void WeatherView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QGraphicsProxyWidget::resizeEvent(event);

    int shownColumns = 0;
    for (int i = 0; i < nativeWidget()->header()->count(); i++) {
            if (!nativeWidget()->isColumnHidden(i)) {
                shownColumns++;
            }
    }

    const int newWidth = size().width() / shownColumns;

    for (int i = 0; i < shownColumns; ++i) {
        nativeWidget()->header()->resizeSection(i, newWidth);
        //kDebug() << "Column resized: " << model()->index(0, i).data(Qt::DisplayRole).toString();
    }

    int iconSize = int(KIconLoader::SizeSmall);
    if (model()) {
        iconSize = qMax(iconSize, int(size().height()/model()->rowCount()));
    }
    nativeWidget()->setIconSize(QSize(iconSize, iconSize));
    emit geometryChanged();
}

QSizeF WeatherView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::PreferredSize:
        return QSizeF(400, 120);
    case Qt::MinimumSize:
        return QSizeF(200, 90);
    case Qt::MaximumSize:
    default:
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
}

}

#include <weatherview.moc>

