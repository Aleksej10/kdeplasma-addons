/*
 *   Copyright (C) 2007 Ivan Cukic <ivan.cukic+kde@gmail.com>
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

#ifndef LANCELOT_STANDARD_ACTION_TREE_MODEL_H
#define LANCELOT_STANDARD_ACTION_TREE_MODEL_H

#include <lancelot/lancelot_export.h>

#include "ActionTreeModel.h"

namespace Lancelot
{

class LANCELOT_EXPORT StandardActionTreeModel: public ActionTreeModel {
protected:
    /**
     * This class represents an item in the list model.
     */
    class LANCELOT_EXPORT Item {
    public:
        Item(QString itemTitle = QString(),
             QString itemDescription = QString(),
             QIcon itemIcon = QIcon(),
             QVariant itemData = QVariant())
          : title(itemTitle), description(itemDescription), icon(itemIcon), data(itemData) {};

        QString title;
        QString description;
        QIcon icon;
        QVariant data;

        QList < Item > children;
    };

    StandardActionTreeModel(Item * root);

    Item * root() const;
    virtual StandardActionTreeModel * createChild(int index) = 0;


public:
    StandardActionTreeModel();

    virtual ~StandardActionTreeModel();

    // ActionTreeModel
    L_Override virtual ActionTreeModel * child(int index);
    L_Override virtual QString modelTitle() const;
    L_Override virtual QIcon modelIcon()  const;

    // ActionListModel
    L_Override virtual QString title(int index) const;
    L_Override virtual QString description(int index) const;
    L_Override virtual QIcon icon(int index) const;
    L_Override virtual bool isCategory(int index) const;

    L_Override virtual int size() const;

    /**
     * Adds a new item into the model
     * @param item new item
     */
    void add(const Item & item, Item * parent = NULL);

    /**
     * Adds a new item into the model
     * @param title the title for the new item
     * @param description the description of the new item
     * @param icon the icon for the new item
     * @param data data for the new item. Not shown to user
     */
    void add(const QString & title, const QString & description, QIcon icon, const QVariant & data, Item * parent = NULL);

    /**
     * Replaces existing item at specified index with a new one
     * @param index index of the item to be replaced
     * @param item new item
     */

    void set(int index, const Item & item, Item * parent = NULL);
    /**
     * Replaces existing item at specified index with a new one
     * @param index index of the item to be replaced
     * @param title the title for the new item
     * @param description the description of the new item
     * @param icon the icon for the new item
     * @param data data for the new item. Not shown to user
     */
    void set(int index, const QString & title, const QString & description, QIcon icon, const QVariant & data, Item * parent = NULL);

    /**
     * Removes an item
     * @param index index of the item to remove
     */
    void removeAt(int index, Item * parent = NULL);

    /**
     * @returns the specified item
     * @param index index of the item to return
     */
    Item & itemAt(int index, Item * parent = NULL);

    /**
     * Clears all items
     */
    void clear(Item * parent = NULL);

private:
    class Private;
    Private * const d;

};

} // namespace Lancelot

#endif /* LANCELOT_STANDARD_ACTION_TREE_MODEL_H */

