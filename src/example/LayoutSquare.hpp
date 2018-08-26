/***************************************************************************//**
 * @file example/LayoutSquare.h
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * This file is part of QFlightInstruments. You can redistribute and modify it
 * under the terms of GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Further information about the GNU General Public License can also be found
 * on the world wide web at http://www.gnu.org.
 *
 * ---
 *
 * Copyright (C) 2013 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/
#ifndef __LAYOUTSQUARE_H__
#define __LAYOUTSQUARE_H__

#include <QLayout>

class QWidget;
class QLayoutItem;

#include <QRect>

class LayoutSquare : public QLayout
{
    Q_OBJECT

public:
    explicit LayoutSquare(QWidget* parent, const int spacing = -1);
    explicit LayoutSquare(const int spacing = -1);
    ~LayoutSquare();

    // QLayout interface
    void addItem(QLayoutItem*) override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QLayoutItem* takeAt(int index) override;

    void addWidget(QWidget*);

    Qt::Orientations expandingDirections() const;

    QRect geometry();

    bool hasHeightForWidth() const;

    bool hasItem() const;

    QSize minimumSize() const;

    QLayoutItem* replaceItem(QLayoutItem*);

    void setGeometry(const QRect&);

    QSize sizeHint() const;

    QLayoutItem* take();

private:
    void init(const int spacing);
    void setRectLast(const QRect& rect);
    bool areRectsEqual(const QRect&, const QRect&) const;
    QPoint calculateCenterPnt(QSize fromSize, QSize itemSize) const;
    QSize calculateProperSize(QSize fromSize) const;

    QLayoutItem* m_item{};

    QRect* m_rectLast{};
    QRect* m_geometry{};
};

#endif
