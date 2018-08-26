/***************************************************************************//**
 * @file qfi_NAV.h
 * @author  Marek M. Cel <marekcel@marekcel.pl>
 *
 * @section LICENSE
 *
 * Copyright (C) 2015 Marek M. Cel
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
 * Copyright (C) 2015 Marek M. Cel
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
#ifndef __qfi_Nav_H__
#define __qfi_Nav_H__

#include <QGraphicsView>

class QWidget;
class QResizeEvent;
class QGraphicsScene;
class QGraphicsSvgItem;

namespace qfi {

//---------------------------------------------------
// Class: Nav
// Description: Navigation Display
//---------------------------------------------------
class Nav : public QGraphicsView
{
    Q_OBJECT

public:
    explicit Nav(QWidget* parent = nullptr);
    virtual ~Nav();

    // reinitiates widget
    void reinit();

    // refreshes (redraws) widget
    void update();

    void setHeading(const float);
    void setHeadingBug(const float);
    void setCourse(const float);
    void setBearing(const float bearing, const bool visible = false);
    void setDeviation(const float deviation, const bool visible = false);
    void setDistance(const float distance, const bool visible = false);

protected:
    void resizeEvent(QResizeEvent*);

private:
    void init();
    void reset();
    void updateView();

    QGraphicsScene* m_scene{};            ///< graphics scene

    QGraphicsSvgItem* m_itemBack{};       ///< NAV background
    QGraphicsSvgItem* m_itemMask{};       ///< NAV mask
    QGraphicsSvgItem* m_itemMark{};

    QGraphicsSvgItem* m_itemBrgArrow{};
    QGraphicsSvgItem* m_itemCrsArrow{};
    QGraphicsSvgItem* m_itemDevBar{};
    QGraphicsSvgItem* m_itemDevScale{};
    QGraphicsSvgItem* m_itemHdgBug{};
    QGraphicsSvgItem* m_itemHdgScale{};

    QGraphicsTextItem* m_itemCrsText{};
    QGraphicsTextItem* m_itemHdgText{};
    QGraphicsTextItem* m_itemDmeText{};

    QColor m_crsTextColor{  0, 255,   0};
    QColor m_hdgTextColor{255,   0, 255};
    QColor m_dmeTextColor{255, 255, 255};

    QFont m_crsTextFont;
    QFont m_hdgTextFont;
    QFont m_dmeTextFont;

    float m_heading{};                    ///< [deg]
    float m_headingBug{};                 ///< [deg]
    float m_course{};
    float m_bearing{};
    float m_deviation{};
    float m_distance{};

    bool m_bearingVisible{true};
    bool m_deviationVisible{true};
    bool m_distanceVisible{true};

    float m_devBarDeltaX_new{};
    float m_devBarDeltaX_old{};
    float m_devBarDeltaY_new{};
    float m_devBarDeltaY_old{};

    float m_scaleX{1.0f};
    float m_scaleY{1.0f};

    float m_originalPixPerDev{52.5f};

    QPointF m_originalNavCtr{150.0f, 150.0f};

    QPointF m_originalCrsTextCtr{ 50.0f,  25.0f};
    QPointF m_originalHdgTextCtr{250.0f,  25.0f};
    QPointF m_originalDmeTextCtr{250.0f, 275.0f};

    const int m_originalHeight{300};         ///< [px]
    const int m_originalWidth{300};          ///< [px]

    const int m_backZ{};
    const int m_maskZ{100};
    const int m_markZ{200};

    const int m_brgArrowZ{60};
    const int m_crsArrowZ{70};
    const int m_crsTextZ{130};
    const int m_devBarZ{50};
    const int m_devScaleZ{10};
    const int m_hdgBugZ{120};
    const int m_hdgScaleZ{110};
    const int m_hdgTextZ{130};
    const int m_dmeTextZ{130};
};

}

#endif
