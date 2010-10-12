/***************************************************************************
 *   Copyright (C) 2007 by Jesper Thomschutz <jesperht@yahoo.com>          *
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

#include "fifteen.h"
#include "piece.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QFontMetrics>

#include <KDebug>
#include <KGlobalSettings>

#include <Plasma/Animation>
#include <Plasma/Animator>

Fifteen::Fifteen(QGraphicsItem* parent, int size)
    : QGraphicsWidget(parent),
      m_size(0), // this will get overwritten in setSize(), but needs an initial value
      m_pixmap(0)
{
  m_pieces.resize(size*size);
  m_splitPixmap = false;
  m_numerals = true;
  m_wasShuffled = false;
  m_svg = new Plasma::Svg();
  setSize(size);
  setCacheMode(DeviceCoordinateCache);
}

Fifteen::~Fifteen()
{
  qDeleteAll(m_pieces);
  delete m_svg;
}


void Fifteen::setSize(int size)
{
    if (m_size == size) {
        return;
    }
    m_size = size;
    startBoard();
    setPreferredSize(48 * size, 48 * size);
    setMinimumSize(24 * size, 24 * size);
}

void Fifteen::setColor(const QColor& c)
{
    m_color = c;
    updatePieces();
}

int Fifteen::size() const
{
  return m_size;
}

const QColor& Fifteen:: color() const
{
  return m_color;
}

void Fifteen::setPixmap(QPixmap *pixmap)
{
  m_pixmap = pixmap;
  if (m_pixmap) {
    updatePieces();
    updatePixmaps();
  }
}

void Fifteen::updatePixmaps()
{
  if (!m_pixmap) {
      return;
  }
  QSize size = m_pieces[0]->size().toSize() * m_size;
  QPixmap copyPixmap = m_pixmap->scaled(size);
 
  for(int y = 0; y < m_size; y++) {
    for(int x = 0; x < m_size; x++) {
      int i = (y * m_size) + x;
  
      if(!m_pieces[i]) continue;
      QRect rect = m_pieces[i]->boundingRect().toRect();
      int posX = x * rect.width();
      int posY = y * rect.height();
      
      m_pieces[i]->setPartialPixmap(copyPixmap.copy(posX, posY, rect.width(), rect.height()));
    }
  }
}

void Fifteen::clearPieces()
{
  for (int i = 0; i < (m_size * m_size); ++i){
    delete m_pieces[i];
  }
}

void Fifteen::startBoard(){
  qDeleteAll(m_pieces);
  m_pieces.fill(NULL);
  int d = m_size * m_size;
  m_pieces.resize(d);
  for (int i = 0; i < d; ++i) {
    m_pieces[i] = new Piece(i+1, i, this, m_svg);
    connect(m_pieces[i], SIGNAL(pressed(Piece*)), this, SLOT(piecePressed(Piece*)));
  }
  m_blank = m_pieces[d-1];
  updatePieces();
  updatePixmaps();
}

void Fifteen::shuffle()
{
  int numSteps = m_size * m_size;
  while(--numSteps){
    bool orientation = (rand()%2 == 0); // true rols, false column
    int posClicked = rand()%m_size;
    while(posClicked == (orientation?m_blank->boardX():m_blank->boardY())){
      posClicked = rand()%m_size;
    }
    if(orientation){
      itemAt( posClicked, m_blank->boardY())->shuffling();
    }
    else{
      itemAt(m_blank->boardX(), posClicked)->shuffling();
    }
  }
  m_time.start();
  m_wasShuffled = true;
}

void Fifteen::resizeEvent(QGraphicsSceneResizeEvent *event)
{
  Q_UNUSED(event);

  QSizeF size = contentsRect().size();
  int width = size.width() / m_size;
  int height = size.height() / m_size;

  QString test = "99";
  QFont f = font();
  int smallest = KGlobalSettings::smallestReadableFont().pixelSize();
  Piece *x = m_pieces[0];
  int fontSize = x?x->boundingRect().height()/3:14;
  f.setBold(true);
  f.setPixelSize(fontSize);
  
  QFontMetrics fm(f);
  QRect rect = fm.boundingRect(test);
  while (rect.width() > width - 2 || rect.height() > height - 2) {
    --fontSize;
    f.setPixelSize(fontSize);

    if (fontSize <= smallest) {
        f = KGlobalSettings::smallestReadableFont();
        break;
    }

    fm = QFontMetrics(f);
    rect = fm.boundingRect(test);
  }

  m_font = f;

  updatePieces();
  updatePixmaps();
}

void Fifteen::setShowNumerals(bool show)
{
  m_numerals = show;
  updatePieces();
}

void Fifteen::setSvg(const QString &path, bool identicalPieces)
{
  m_svg->setImagePath(path);
  m_numerals = m_numerals || identicalPieces;
  updatePieces();
}

void Fifteen::updatePieces()
{
  bool sorted = true;
  QSizeF size = contentsRect().size();
  int width = size.width() / m_size;
  int height = size.height() / m_size;

  for (int i = 0; i < (m_size * m_size); ++i) {
    if (!m_pieces[i]) continue;
    m_pieces[i]->showNumeral(m_numerals);
    m_pieces[i]->setSplitImage( (m_pixmap)?true:false );
    m_pieces[i]->resize(QSizeF(width, height));
    m_pieces[i]->setPos(m_pieces[i]->boardX() * width, m_pieces[i]->boardY() * height);
    m_pieces[i]->setFont(m_font);
    m_pieces[i]->update();
    if(itemAt(i%m_size, i/m_size)->id() != i+1) sorted = false;
  }
  
  if (!m_pixmap) {
    m_svg->resize(width, height);
  }
  
  if(sorted && m_wasShuffled) {
    emit puzzleSorted(m_time.elapsed());
    m_wasShuffled = false;
  }
}

void Fifteen::piecePressed(Piece *item)
{
  int ix = item->boardX();
  int iy = item->boardY();
  int bx = m_blank->boardX();
  int by = m_blank->boardY();

  if (ix == bx && iy != by) {
    if (iy > by) {
      for (; by < iy; by++) {
        // swap the piece at ix,by+1 with blank
        swapPieceWithBlank(itemAt(ix, by + 1));
      }
    }
    else if (iy < by) {
      for (; by > iy; by--) {
        // swap the piece at ix,by-1 with blank
        swapPieceWithBlank(itemAt(ix, by - 1));
      }
    }
  }
  else if (iy == by && ix != bx) {
    if (ix > bx) {
      for (; bx < ix; bx++) {
        // swap the piece at bx+1,iy with blank
        swapPieceWithBlank(itemAt(bx + 1, iy));
      }
    }
    else if (ix < bx) {
      for (; bx > ix; bx--) {
        // swap the piece at bx-1,iy with blank
        swapPieceWithBlank(itemAt(bx - 1, iy));
      }
    }
  }
  updatePieces();
}

Piece* Fifteen::itemAt(int gameX, int gameY)
{
  int gamePos = (gameY * m_size) + gameX;
  for (int i = 0; i < (m_size * m_size); i++) {
    if (m_pieces[i]->boardPos() == gamePos) {
      return m_pieces[i];
    }
  }
  return NULL;
}

void Fifteen::swapPieceWithBlank(Piece *item)
{
  int width = contentsRect().size().width() / m_size;
  int height = contentsRect().size().height() / m_size;

  // swap widget positions
  QPointF pos = QPointF(item->boardX() * width, item->boardY() * height);

  Plasma::Animation *animation = m_animations.value(item).data();
  if (!animation) {
      animation = Plasma::Animator::create(Plasma::Animator::SlideAnimation, this);
      animation->setTargetWidget(item);
      animation->setProperty("easingCurve", QEasingCurve::InOutQuad);
      animation->setProperty("movementDirection", Plasma::Animation::MoveAny);
      m_animations[item] = animation;
  } else if (animation->state() == QAbstractAnimation::Running) {
      animation->pause();
  }

  animation->setProperty("distancePointF", m_blank->pos() - item->pos());
  animation->start(QAbstractAnimation::DeleteWhenStopped);

  m_blank->setPos(pos);

  // swap game positions
  int blankPos = m_blank->boardPos();
  m_blank->setGamePos(item->boardPos());
  item->setGamePos(blankPos);
}

