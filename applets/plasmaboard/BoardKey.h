/***************************************************************************
 *   Copyright (C) 2009 by Björn Ruberg <bjoern@ruberg-wegener.de>         *
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

#ifndef BOARDKEY_H
#define BOARDKEY_H

#include <widget.h>

class BoardKey  {

public:
    BoardKey(QPoint relativePosition, QSize relativeSize, unsigned int keycode);
    virtual ~BoardKey();

    bool contains (const QPoint &point) const;
    bool intersects (const QRectF &rect) const;
    unsigned int getKeycode() const;
    unsigned int getKeysymbol(int level) const;
    virtual QString label() const;
    virtual void paint(QPainter *painter);
    QPoint position() const;
    /**
      * called when button is pressed
      */
    virtual void pressed();

    virtual void pressRepeated();
    QRectF rect() const;
    QSize relativeSize() const;
    /**
      * called when button is pressed
      */
    virtual void released();
    /**
      * Called to "unpress" the button
      */
    virtual void reset();
    virtual void setPixmap(QPixmap *pixmap);
    QSize size() const;
    virtual void unpressed();
    void updateDimensions(double factor_x, double factor_y);

protected:    
    void sendKey();
    void sendKeyPress();
    void sendKeyRelease();
    void setUpPainter(QPainter *painter) const;

private:
    unsigned int m_keycode;
    QPixmap* m_pixmap;
    QPoint m_position;
    QPoint m_relativePosition;
    QRectF m_rect;
    QSize m_relativeSize;
    QSize m_size;

};

#endif /* BOARDKEY_H */
