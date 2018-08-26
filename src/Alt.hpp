/***************************************************************************//**
 * @file qfi_ALT.h
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
#ifndef __qfi_Alt_H__
#define __qfi_Alt_H__

#include <QGraphicsView>

class QWidget;
class QResizeEvent;
class QGraphicsScene;
class QGraphicsSvgItem;

namespace qfi {

//---------------------------------------------------
// Class: Alt
// Description: Barometric Altimeter
//---------------------------------------------------
class Alt : public QGraphicsView
{
    Q_OBJECT

public:
    Alt(QWidget* parent = nullptr);
    virtual ~Alt();

    // reinitiates widget
    void reinit();

    // refreshes (redraws) widget
    void update();

    void setAltitude(const float);
    void setPressure(const float);

protected:
    void resizeEvent(QResizeEvent*);

private:
    void init();
    void reset();
    void updateView();

    QGraphicsScene* m_scene{};

    QGraphicsSvgItem* m_itemFace_1{};
    QGraphicsSvgItem* m_itemFace_2{};
    QGraphicsSvgItem* m_itemFace_3{};
    QGraphicsSvgItem* m_itemHand_1{};
    QGraphicsSvgItem* m_itemHand_2{};
    QGraphicsSvgItem* m_itemCase{};

    float m_altitude{};
    float m_pressure{28.0f};

    float m_scaleX{1.0f};
    float m_scaleY{1.0f};

    const int m_originalHeight{240};
    const int m_originalWidth{240};

    QPointF m_originalAltCtr{120.0f, 120.0f};

    const int m_face1Z{-50};
    const int m_face2Z{-40};
    const int m_face3Z{-30};
    const int m_hand1Z{-20};
    const int m_hand2Z{-10};
    const int m_caseZ{10};
};

}

#endif
