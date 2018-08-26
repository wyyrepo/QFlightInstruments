/***************************************************************************//**
 * @file qfi_PFD.h
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
#ifndef __qfi_Pfd_H__
#define __qfi_Pfd_H__

#include <QGraphicsView>

class QWidget;
class QResizeEvent;
class QGraphicsScene;
class QGraphicsSvgItem;

namespace qfi {

//---------------------------------------------------
// Class: Pfd
// Description: Primary Flight Display widget
//---------------------------------------------------
class Pfd : public QGraphicsView
{
    Q_OBJECT

public:

    // altimeter pressure units
    enum PressureUnit
    {
        STD = 0,    ///< standard (displays STD instead of numerical value)
        MB,         ///< milibars
        IN          ///< inches of mercury
    };

    explicit Pfd(QWidget* parent = nullptr);
    ~Pfd();

    // reinitiates widget
    void reinit();

    // refreshes (redraws) widget
    void update();

    inline void setRoll(const float roll)
    {
        m_adi->setRoll( roll );
    }

    inline void setPitch(const float pitch)
    {
        m_adi->setPitch( pitch );
    }

    // angle of attack [deg]
    // angle of sideslip [deg]
    // flight path marker visibility
    inline void setFlightPathMarker(const float aoa, const float sideslip, const bool visible = true )
    {
        m_adi->setFlightPathMarker( aoa, sideslip, visible );
    }

    // normalized slip or skid (range from -1.0 to 1.0)
    inline void setSlipSkid(const float slipSkid)
    {
        m_adi->setSlipSkid( slipSkid );
    }

    //
    // normalized turn rate (range from -1.0 to 1.0),
    // hash marks positions are set to be -0.5 and 0.5
    //
    inline void setTurnRate(const float turnRate)
    {
        m_adi->setTurnRate( turnRate );
    }

    // normalized horizontal deviation bar position (range from -1.0 to 1.0)
    // horizontal deviation bar visibility
    inline void setBarH(const float barH, const bool visible = true)
    {
        m_adi->setBarH( barH, visible );
    }

    // normalized vertical deviation bar position (range from -1.0 to 1.0)
    // vertical deviation bar visibility
    inline void setBarV(const float barV, const bool visible = true)
    {
        m_adi->setBarV( barV, visible );
    }

    // normalized horizontal deviation dot position (range from -1.0 to 1.0)
    // horizontal deviation dot visibility
    inline void setDotH(const float dotH, const bool visible = true)
    {
        m_adi->setDotH( dotH, visible );
    }

    // normalized vertical deviation dot position (range from -1.0 to 1.0)
    // vertical deviation dot visibility
    inline void setDotV(const float dotV, const bool visible = true )
    {
        m_adi->setDotV( dotV, visible );
    }

    // altitude (dimensionless numeric value)
    inline void setAltitude(const float altitude)
    {
        m_alt->setAltitude( altitude );
    }

    // pressure (dimensionless numeric value)
    // pressure unit according to GraphicsPFD::PressureUnit
    inline void setPressure(const float pressure, const PressureUnit pressureUnit )
    {
        m_alt->setPressure( pressure, pressureUnit );
    }

    // airspeed (dimensionless numeric value)
    inline void setAirspeed(const float airspeed)
    {
        m_asi->setAirspeed( airspeed );
    }

    // Mach number
    inline void setMachNo(const float machNo)
    {
        m_asi->setMachNo( machNo );
    }

    // heading [deg]
    inline void setHeading(const float heading)
    {
        m_hsi->setHeading( heading );
    }

    // climb rate (dimensionless numeric value)
    inline void setClimbRate(const float climbRate)
    {
        m_vsi->setClimbRate( climbRate );
    }

protected:
    void resizeEvent(QResizeEvent*);

private:

    class ADI
    {
    public:
        ADI(QGraphicsScene*);
        void init(const float scaleX, const float scaleY);
        void update(const float scaleX, const float scaleY);
        void setRoll(const float);
        void setPitch(const float);
        void setFlightPathMarker(const float aoa, const float sideslip, const bool visible = true);
        void setSlipSkid(const float);
        void setTurnRate(const float);
        void setBarH(const float barH, const bool visible = true);
        void setBarV(const float barV, const bool visible = true);
        void setDotH(const float dotH, const bool visible = true);
        void setDotV(const float dotV, const bool visible = true);

    private:
        void reset();
        void updateLadd(const float delta, const float sinRoll, const float cosRoll);
        void updateLaddBack(const float delta, const float sinRoll, const float cosRoll);
        void updateRoll();
        void updateSlipSkid(const float sinRoll, const float cosRoll);
        void updateTurnRate();
        void updateFlightPath();
        void updateBars();
        void updateDots();

        QGraphicsScene* m_scene{};

        QGraphicsSvgItem* m_itemBack{};
        QGraphicsSvgItem* m_itemLadd{};
        QGraphicsSvgItem* m_itemRoll{};
        QGraphicsSvgItem* m_itemSlip{};
        QGraphicsSvgItem* m_itemTurn{};
        QGraphicsSvgItem* m_itemPath{};
        QGraphicsSvgItem* m_itemMark{};
        QGraphicsSvgItem* m_itemBarH{};
        QGraphicsSvgItem* m_itemBarV{};
        QGraphicsSvgItem* m_itemDotH{};
        QGraphicsSvgItem* m_itemDotV{};
        QGraphicsSvgItem* m_itemMask{};
        QGraphicsSvgItem* m_itemScaleH{};
        QGraphicsSvgItem* m_itemScaleV{};

        float m_roll{};
        float m_pitch{};
        float m_angleOfAttack{};
        float m_sideslipAngle{};
        float m_slipSkid{};
        float m_turnRate{};
        float m_barH{};
        float m_barV{};
        float m_dotH{};
        float m_dotV{};

        bool m_pathValid{true};

        bool m_pathVisible{true};
        bool m_barHVisible{true};
        bool m_barVVisible{true};
        bool m_dotHVisible{true};
        bool m_dotVVisible{true};

        float m_laddDeltaX_new{};
        float m_laddDeltaX_old{};
        float m_laddBackDeltaX_new{};
        float m_laddBackDeltaX_old{};
        float m_laddBackDeltaY_new{};
        float m_laddBackDeltaY_old{};
        float m_laddDeltaY_new{};
        float m_laddDeltaY_old{};
        float m_slipDeltaX_new{};
        float m_slipDeltaX_old{};
        float m_slipDeltaY_new{};
        float m_slipDeltaY_old{};
        float m_turnDeltaX_new{};
        float m_turnDeltaX_old{};
        float m_pathDeltaX_new{};
        float m_pathDeltaX_old{};
        float m_pathDeltaY_new{};
        float m_pathDeltaY_old{};
        float m_markDeltaX_new{};
        float m_markDeltaX_old{};
        float m_markDeltaY_new{};
        float m_markDeltaY_old{};
        float m_barHDeltaX_new{};
        float m_barHDeltaX_old{};
        float m_barVDeltaY_new{};
        float m_barVDeltaY_old{};
        float m_dotHDeltaX_new{};
        float m_dotHDeltaX_old{};
        float m_dotVDeltaY_new{};
        float m_dotVDeltaY_old{};

        float m_scaleX{1.0f};
        float m_scaleY{1.0f};

        const float m_originalPixPerDeg{3.0f};
        const float m_deltaLaddBack_max{52.5f};
        const float m_deltaLaddBack_min{-52.5f};
        const float m_maxSlipDeflection{20.0f};
        const float m_maxTurnDeflection{55.0f};
        const float m_maxBarsDeflection{40.0f};
        const float m_maxDotsDeflection{50.0f};

        QPointF m_originalAdiCtr{150.0f,   125.0f};
        QPointF m_originalBackPos{ 45.0f,  -85.0f};
        QPointF m_originalLaddPos{110.0f, -175.0f};
        QPointF m_originalRollPos{ 45.0f,   20.0f};
        QPointF m_originalSlipPos{145.5f,   68.5f};
        QPointF m_originalTurnPos{142.5f,  206.0f};
        QPointF m_originalPathPos{135.0f,  113.0f};
        QPointF m_originalBarHPos{149.0f,   85.0f};
        QPointF m_originalBarVPos{110.0f,  124.0f};
        QPointF m_originalDotHPos{145.0f,  188.0f};
        QPointF m_originalDotVPos{213.0f,  120.0f};
        QPointF m_originalScaleHPos{0.0f,    0.0f};
        QPointF m_originalScaleVPos{0.0f,    0.0f};

        const int m_backZ{10};
        const int m_laddZ{20};
        const int m_rollZ{30};
        const int m_slipZ{40};
        const int m_pathZ{40};
        const int m_barsZ{50};
        const int m_dotsZ{50};
        const int m_scalesZ{51};
        const int m_maskZ{60};
        const int m_turnZ{70};
    };

    class ALT
    {
    public:
        ALT(QGraphicsScene*);
        void init(const float scaleX, const float scaleY);
        void update(const float scaleX, const float scaleY);
        void setAltitude(const float);
        void setPressure(const float pressure, const int pressureUnit);

    private:
        void reset();
        void updateAltitude();
        void updatePressure();
        void updateScale();
        void updateScaleLabels();

        QGraphicsScene* m_scene{};

        QGraphicsSvgItem* m_itemBack{};
        QGraphicsSvgItem* m_itemScale1{};
        QGraphicsSvgItem* m_itemScale2{};
        QGraphicsTextItem* m_itemLabel1{};
        QGraphicsTextItem* m_itemLabel2{};
        QGraphicsTextItem* m_itemLabel3{};
        QGraphicsSvgItem* m_itemGround{};
        QGraphicsSvgItem* m_itemFrame{};
        QGraphicsTextItem* m_itemAltitude{};
        QGraphicsTextItem* m_itemPressure{};

        QColor m_frameTextColor{255, 255, 255};
        QColor m_pressTextColor{  0, 255,   0};
        QColor m_labelsColor{255, 255, 255};

        QFont m_frameTextFont;
        QFont m_labelsFont;

        float m_altitude{};
        float m_pressure{};

        int m_pressureUnit{};

        float m_scale1DeltaY_new{};
        float m_scale1DeltaY_old{};
        float m_scale2DeltaY_new{};
        float m_scale2DeltaY_old{};
        float m_groundDeltaY_new{};
        float m_groundDeltaY_old{};
        float m_labelsDeltaY_new{};
        float m_labelsDeltaY_old{};

        float m_scaleX{1.0};
        float m_scaleY{1.0};

        const float m_originalPixPerAlt{0.150f};
        const float m_originalScaleHeight{300.0f};
        const float m_originalLabelsX{250.0f};
        const float m_originalLabel1Y{50.0f};
        const float m_originalLabel2Y{125.0f};
        const float m_originalLabel3Y{200.0f};

        QPointF m_originalBackPos{231.0f, 37.5f};
        QPointF m_originalScale1Pos{231.0f, -174.5f};
        QPointF m_originalScale2Pos{231.0f, -474.5f};
        QPointF m_originalGroundPos{231.5f,  124.5f};
        QPointF m_originalFramePos{225.0f, 110.0f};
        QPointF m_originalAltitudeCtr{254.0f, 126.0f};
        QPointF m_originalPressureCtr{254.0f, 225.0f};

        const int m_backZ{70};
        const int m_scaleZ{77};
        const int m_labelsZ{78};
        const int m_groundZ{79};
        const int m_frameZ{110};
        const int m_frameTextZ{120};
    };

    class ASI
    {
    public:
        ASI(QGraphicsScene*);
        void init(const float scaleX, const float scaleY);
        void update(const float scaleX, const float scaleY);
        void setAirspeed(const float);
        void setMachNo(const float);

    private:
        void reset();
        void updateAirspeed();
        void updateScale();
        void updateScaleLabels();

        QGraphicsScene* m_scene{};

        QGraphicsSvgItem* m_itemBack{};
        QGraphicsSvgItem* m_itemScale1{};
        QGraphicsSvgItem* m_itemScale2{};
        QGraphicsTextItem* m_itemLabel1{};
        QGraphicsTextItem* m_itemLabel2{};
        QGraphicsTextItem* m_itemLabel3{};
        QGraphicsTextItem* m_itemLabel4{};
        QGraphicsTextItem* m_itemLabel5{};
        QGraphicsTextItem* m_itemLabel6{};
        QGraphicsTextItem* m_itemLabel7{};
        QGraphicsSvgItem* m_itemFrame{};
        QGraphicsTextItem* m_itemAirspeed{};
        QGraphicsTextItem* m_itemMachNo{};

        QColor m_frameTextColor{255, 255, 255};
        QColor m_labelsColor{255, 255, 255};

        QFont m_frameTextFont{};
        QFont m_labelsFont{};

        float m_airspeed{};
        float m_machNo{};

        float m_scale1DeltaY_new{};
        float m_scale1DeltaY_old{};
        float m_scale2DeltaY_new{};
        float m_scale2DeltaY_old{};
        float m_labelsDeltaY_new{};
        float m_labelsDeltaY_old{};

        float m_scaleX{1.0f};
        float m_scaleY{1.0f};

        const float m_originalPixPerSpd{1.5f};
        const float m_originalScaleHeight{300.0f};
        const float m_originalLabelsX{43.0f};
        const float m_originalLabel1Y{35.0f};
        const float m_originalLabel2Y{65.0f};
        const float m_originalLabel3Y{95.0f};
        const float m_originalLabel4Y{125.0f};
        const float m_originalLabel5Y{155.0f};
        const float m_originalLabel6Y{185.0f};
        const float m_originalLabel7Y{215.0f};

        QPointF m_originalBackPos{25.0f, 37.5f};
        QPointF m_originalScale1Pos{56.0f, -174.5f};
        QPointF m_originalScale2Pos{56.0f, -474.5f};
        QPointF m_originalFramePos{0.0f, 110.0f};
        QPointF m_originalAirspeedCtr{40.0f, 126.0f};
        QPointF m_originalMachNoCtr{43.0f, 225.0f};

        const int m_backZ{70};
        const int m_scaleZ{80};
        const int m_labelsZ{90};
        const int m_frameZ{110};
        const int m_frameTextZ{120};
    };

    class HSI
    {
    public:
        HSI(QGraphicsScene*);
        void init(const float scaleX, const float scaleY);
        void update(const float scaleX, const float scaleY);
        void setHeading(const float);

    private:
        void reset();
        void updateHeading();

        QGraphicsScene* m_scene{};

        QGraphicsSvgItem* m_itemBack{};
        QGraphicsSvgItem* m_itemFace{};
        QGraphicsSvgItem* m_itemMarks{};
        QGraphicsTextItem* m_itemFrameText{};

        QColor m_frameTextColor{255, 255, 255};

        QFont m_frameTextFont{};

        float m_heading{};

        float m_scaleX{1.0f};
        float m_scaleY{1.0f};

        QPointF m_originalHsiCtr{150.0f, 345.0f};
        QPointF m_originalBackPos{60.0f, 240.0f};
        QPointF m_originalFacePos{45.0f, 240.0f};
        QPointF m_originalMarksPos{134.0f, 219.0f};
        QPointF m_originalFrameTextCtr{149.5f, 227.5f};

        const int m_backZ{80};
        const int m_faceZ{90};
        const int m_marksZ{110};
        const int m_frameTextZ{120};
    };

    class VSI
    {
    public:
        VSI(QGraphicsScene*);
        void init(const float scaleX, const float scaleY);
        void update(const float scaleX, const float scaleY);
        void setClimbRate(const float);

    private:
        void reset();
        void updateVSI();

        QGraphicsScene* m_scene{};

        QGraphicsSvgItem* m_itemScale{};
        QGraphicsSvgItem* m_itemArrow{};

        float m_climbRate{};

        float m_arrowDeltaY_new{};
        float m_arrowDeltaY_old{};

        float m_scaleX{1.0f};
        float m_scaleY{1.0f};

        const float m_originalMarkeHeight{75.0f};
        const float m_originalPixPerSpd1{30.0f};
        const float m_originalPixPerSpd2{20.0f};
        const float m_originalPixPerSpd4{5.0f};

        QPointF m_originalScalePos{275.0f,  50.0f};
        QPointF m_originalArrowPos{284.0f, 124.0f};

        const int m_scaleZ{70};
        const int m_arrowZ{80};
    };

    void init();
    void reset();
    void updateView();

    QGraphicsScene* m_scene{};

    ADI* m_adi{};
    ALT* m_alt{};
    ASI* m_asi{};
    HSI* m_hsi{};
    VSI* m_vsi{};

    QGraphicsSvgItem* m_itemBack{};
    QGraphicsSvgItem* m_itemMask{};

    float m_scaleX{1.0f};
    float m_scaleY{1.0f};

    const int m_originalHeight{300};
    const int m_originalWidth{300};

    const int m_backZ{};
    const int m_maskZ{100};
};

}

#endif
