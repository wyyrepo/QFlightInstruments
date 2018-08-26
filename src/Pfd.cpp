/***************************************************************************//**
 * @file qfi_PFD.cpp
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

#include "Pfd.hpp"

#include <QGraphicsSvgItem>

#include <cmath>

#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

namespace qfi {

Pfd::Pfd(QWidget* parent) : QGraphicsView(parent)
{
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    m_scene->clear();

    m_adi = new ADI(m_scene);
    m_alt = new ALT(m_scene);
    m_asi = new ASI(m_scene);
    m_hsi = new HSI(m_scene);
    m_vsi = new VSI(m_scene);

    init();
}

Pfd::~Pfd()
{
    if ( m_scene ) {
        m_scene->clear();
        delete m_scene;
        m_scene = nullptr;
    }

    reset();

    if ( m_adi ) { delete m_adi; m_adi = nullptr; }
    if ( m_alt ) { delete m_alt; m_alt = nullptr; }
    if ( m_asi ) { delete m_asi; m_asi = nullptr; }
    if ( m_hsi ) { delete m_hsi; m_hsi = nullptr; }
    if ( m_vsi ) { delete m_vsi; m_vsi = nullptr; }
}

void Pfd::reinit()
{
    if (m_scene) {
        m_scene->clear();
        init();
    }
}

void Pfd::update()
{
    updateView();
}

void Pfd::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    reinit();
}

void Pfd::init()
{
    m_scaleX = static_cast<float>(width())  / static_cast<float>(m_originalWidth);
    m_scaleY = static_cast<float>(height()) / static_cast<float>(m_originalHeight);

    m_adi->init( m_scaleX, m_scaleY );
    m_alt->init( m_scaleX, m_scaleY );
    m_asi->init( m_scaleX, m_scaleY );
    m_hsi->init( m_scaleX, m_scaleY );
    m_vsi->init( m_scaleX, m_scaleY );

    m_itemBack = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemBack );

    m_itemMask = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_mask.svg" );
    m_itemMask->setCacheMode( QGraphicsItem::NoCache );
    m_itemMask->setZValue( m_maskZ );
    m_itemMask->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemMask );

    centerOn( width() / 2.0f , height() / 2.0f );

    updateView();
}

void Pfd::reset()
{
    m_itemBack = nullptr;
    m_itemMask = nullptr;
}

void Pfd::updateView()
{
    m_scaleX = static_cast<float>(width())  / static_cast<float>(m_originalWidth);
    m_scaleY = static_cast<float>(height()) / static_cast<float>(m_originalHeight);

    m_adi->update( m_scaleX, m_scaleY );
    m_alt->update( m_scaleX, m_scaleY );
    m_asi->update( m_scaleX, m_scaleY );
    m_hsi->update( m_scaleX, m_scaleY );
    m_vsi->update( m_scaleX, m_scaleY );

    m_scene->update();
}

Pfd::ADI::ADI(QGraphicsScene* scene) : m_scene(scene)
{
    reset();
}

void Pfd::ADI::init(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->setTransformOriginPoint( m_originalAdiCtr - m_originalBackPos );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemLadd = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_ladd.svg" );
    m_itemLadd->setCacheMode( QGraphicsItem::NoCache );
    m_itemLadd->setZValue( m_laddZ );
    m_itemLadd->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLadd->setTransformOriginPoint( m_originalAdiCtr - m_originalLaddPos );
    m_itemLadd->moveBy( m_scaleX * m_originalLaddPos.x(), m_scaleY * m_originalLaddPos.y() );
    m_scene->addItem( m_itemLadd );

    m_itemRoll = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_roll.svg" );
    m_itemRoll->setCacheMode( QGraphicsItem::NoCache );
    m_itemRoll->setZValue( m_rollZ );
    m_itemRoll->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemRoll->setTransformOriginPoint( m_originalAdiCtr - m_originalRollPos );
    m_itemRoll->moveBy( m_scaleX * m_originalRollPos.x(), m_scaleY * m_originalRollPos.y() );
    m_scene->addItem( m_itemRoll );

    m_itemSlip = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_slip.svg" );
    m_itemSlip->setCacheMode( QGraphicsItem::NoCache );
    m_itemSlip->setZValue( m_slipZ );
    m_itemSlip->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemSlip->setTransformOriginPoint( m_originalAdiCtr - m_originalSlipPos );
    m_itemSlip->moveBy( m_scaleX * m_originalSlipPos.x(), m_scaleY * m_originalSlipPos.y() );
    m_scene->addItem( m_itemSlip );

    m_itemTurn = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_turn.svg" );
    m_itemTurn->setCacheMode( QGraphicsItem::NoCache );
    m_itemTurn->setZValue( m_turnZ );
    m_itemTurn->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemTurn->moveBy( m_scaleX * m_originalTurnPos.x(), m_scaleY * m_originalTurnPos.y() );
    m_scene->addItem( m_itemTurn );

    m_itemPath = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_path.svg" );
    m_itemPath->setCacheMode( QGraphicsItem::NoCache );
    m_itemPath->setZValue( m_pathZ );
    m_itemPath->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemPath->moveBy( m_scaleX * m_originalPathPos.x(), m_scaleY * m_originalPathPos.y() );
    m_scene->addItem( m_itemPath );

    m_itemMark = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_mark.svg" );
    m_itemMark->setCacheMode( QGraphicsItem::NoCache );
    m_itemMark->setZValue( m_pathZ );
    m_itemMark->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemMark->moveBy( m_scaleX * m_originalPathPos.x(), m_scaleY * m_originalPathPos.y() );
    m_scene->addItem( m_itemMark );

    m_itemBarH = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_barh.svg" );
    m_itemBarH->setCacheMode( QGraphicsItem::NoCache );
    m_itemBarH->setZValue( m_barsZ );
    m_itemBarH->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBarH->moveBy( m_scaleX * m_originalBarHPos.x(), m_scaleY * m_originalBarHPos.y() );
    m_scene->addItem( m_itemBarH );

    m_itemBarV = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_barv.svg" );
    m_itemBarV->setCacheMode( QGraphicsItem::NoCache );
    m_itemBarV->setZValue( m_barsZ );
    m_itemBarV->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBarV->moveBy( m_scaleX * m_originalBarVPos.x(), m_scaleY * m_originalBarVPos.y() );
    m_scene->addItem( m_itemBarV );

    m_itemDotH = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_doth.svg" );
    m_itemDotH->setCacheMode( QGraphicsItem::NoCache );
    m_itemDotH->setZValue( m_dotsZ );
    m_itemDotH->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemDotH->moveBy( m_scaleX * m_originalDotHPos.x(), m_scaleY * m_originalDotHPos.y() );
    m_scene->addItem( m_itemDotH );

    m_itemDotV = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_dotv.svg" );
    m_itemDotV->setCacheMode( QGraphicsItem::NoCache );
    m_itemDotV->setZValue( m_dotsZ );
    m_itemDotV->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemDotV->moveBy( m_scaleX * m_originalDotVPos.x(), m_scaleY * m_originalDotVPos.y() );
    m_scene->addItem( m_itemDotV );

    m_itemScaleH = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_scaleh.svg" );
    m_itemScaleH->setCacheMode( QGraphicsItem::NoCache );
    m_itemScaleH->setZValue( m_scalesZ );
    m_itemScaleH->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScaleH->moveBy( m_scaleX * m_originalScaleHPos.x(), m_scaleY * m_originalScaleHPos.y() );
    m_scene->addItem( m_itemScaleH );

    m_itemScaleV = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_scalev.svg" );
    m_itemScaleV->setCacheMode( QGraphicsItem::NoCache );
    m_itemScaleV->setZValue( m_scalesZ );
    m_itemScaleV->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScaleV->moveBy( m_scaleX * m_originalScaleVPos.x(), m_scaleY * m_originalScaleVPos.y() );
    m_scene->addItem( m_itemScaleV );

    m_itemMask = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_adi_mask.svg" );
    m_itemMask->setCacheMode( QGraphicsItem::NoCache );
    m_itemMask->setZValue( m_maskZ );
    m_itemMask->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_scene->addItem( m_itemMask );

    update( scaleX, scaleY );
}

void Pfd::ADI::update(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    const float delta{static_cast<float>(m_originalPixPerDeg * m_pitch)};

    const float roll_rad{static_cast<float>(M_PI * m_roll / 180.0f)};

    const float sinRoll{static_cast<float>(std::sin(roll_rad))};
    const float cosRoll{static_cast<float>(std::cos(roll_rad))};

    updateLadd( delta, sinRoll, cosRoll );
    updateLaddBack( delta, sinRoll, cosRoll );
    updateRoll();
    updateSlipSkid( sinRoll, cosRoll );
    updateTurnRate();
    updateBars();
    updateDots();
    updateFlightPath();

    m_laddDeltaX_old     = m_laddDeltaX_new;
    m_laddDeltaY_old     = m_laddDeltaY_new;
    m_laddBackDeltaX_old = m_laddBackDeltaX_new;
    m_laddBackDeltaY_old = m_laddBackDeltaY_new;
    m_slipDeltaX_old     = m_slipDeltaX_new;
    m_slipDeltaY_old     = m_slipDeltaY_new;
    m_turnDeltaX_old     = m_turnDeltaX_new;
    m_pathDeltaX_old     = m_pathDeltaX_new;
    m_pathDeltaY_old     = m_pathDeltaY_new;
    m_markDeltaX_old     = m_markDeltaX_new;
    m_markDeltaY_old     = m_markDeltaY_new;
    m_barHDeltaX_old     = m_barHDeltaX_new;
    m_barVDeltaY_old     = m_barVDeltaY_new;
    m_dotHDeltaX_old     = m_dotHDeltaX_new;
    m_dotVDeltaY_old     = m_dotVDeltaY_new;
}

void Pfd::ADI::setRoll(const float roll)
{
    m_roll = roll;

    if      ( m_roll < -180.0f ) m_roll = -180.0f;
    else if ( m_roll >  180.0f ) m_roll =  180.0f;
}

void Pfd::ADI::setPitch(const float pitch)
{
    m_pitch = pitch;

    if      ( m_pitch < -90.0f ) m_pitch = -90.0f;
    else if ( m_pitch >  90.0f ) m_pitch =  90.0f;
}

void Pfd::ADI::setFlightPathMarker(const float aoa, const float sideslip, const bool visible)
{
    m_angleOfAttack = aoa;
    m_sideslipAngle = sideslip;

    m_pathValid = true;

    if ( m_angleOfAttack < -15.0f ) {
        m_angleOfAttack = -15.0f;
        m_pathValid = false;
    } else if ( m_angleOfAttack > 15.0f ) {
        m_angleOfAttack = 15.0f;
        m_pathValid = false;
    }

    if ( m_sideslipAngle < -10.0f ) {
        m_sideslipAngle = -10.0f;
        m_pathValid = false;
    } else if ( m_sideslipAngle > 10.0f ) {
        m_sideslipAngle = 10.0f;
        m_pathValid = false;
    }

    m_pathVisible = visible;
}

void Pfd::ADI::setSlipSkid(const float slipSkid)
{
    m_slipSkid = slipSkid;

    if      ( m_slipSkid < -1.0f ) m_slipSkid = -1.0f;
    else if ( m_slipSkid >  1.0f ) m_slipSkid =  1.0f;
}

void Pfd::ADI::setTurnRate(const float turnRate)
{
    m_turnRate = turnRate;

    if      ( m_turnRate < -1.0f ) m_turnRate = -1.0f;
    else if ( m_turnRate >  1.0f ) m_turnRate =  1.0f;
}

void Pfd::ADI::setBarH(const float barH, const bool visible )
{
    m_barH = barH;

    if      ( m_barH < -1.0f ) m_barH = -1.0f;
    else if ( m_barH >  1.0f ) m_barH =  1.0f;

    m_barHVisible = visible;
}

void Pfd::ADI::setBarV(const float barV, const bool visible )
{
    m_barV = barV;

    if      ( m_barV < -1.0f ) m_barV = -1.0f;
    else if ( m_barV >  1.0f ) m_barV =  1.0f;

    m_barVVisible = visible;
}

void Pfd::ADI::setDotH(const float dotH, const bool visible )
{
    m_dotH = dotH;

    if      ( m_dotH < -1.0f ) m_dotH = -1.0f;
    else if ( m_dotH >  1.0f ) m_dotH =  1.0f;

    m_dotHVisible = visible;
}

void Pfd::ADI::setDotV(const float dotV, const bool visible )
{
    m_dotV = dotV;

    if      ( m_dotV < -1.0f ) m_dotV = -1.0f;
    else if ( m_dotV >  1.0f ) m_dotV =  1.0f;

    m_dotVVisible = visible;
}

void Pfd::ADI::reset()
{
    m_itemBack   = 0;
    m_itemLadd   = 0;
    m_itemRoll   = 0;
    m_itemSlip   = 0;
    m_itemTurn   = 0;
    m_itemPath   = 0;
    m_itemMark   = 0;
    m_itemBarH   = 0;
    m_itemBarV   = 0;
    m_itemDotH   = 0;
    m_itemDotV   = 0;
    m_itemMask   = 0;
    m_itemScaleH = 0;
    m_itemScaleV = 0;

    m_roll          = 0.0f;
    m_pitch         = 0.0f;
    m_angleOfAttack = 0.0f;
    m_sideslipAngle = 0.0f;
    m_slipSkid      = 0.0f;
    m_turnRate      = 0.0f;
    m_barH          = 0.0f;
    m_barV          = 0.0f;
    m_dotH          = 0.0f;
    m_dotV          = 0.0f;

    m_pathValid = true;

    m_pathVisible = true;
    m_barHVisible = true;
    m_barVVisible = true;
    m_dotHVisible = true;
    m_dotVVisible = true;

    m_laddDeltaX_new     = 0.0f;
    m_laddDeltaX_old     = 0.0f;
    m_laddBackDeltaX_new = 0.0f;
    m_laddBackDeltaX_old = 0.0f;
    m_laddBackDeltaY_new = 0.0f;
    m_laddBackDeltaY_old = 0.0f;
    m_laddDeltaY_new     = 0.0f;
    m_laddDeltaY_old     = 0.0f;
    m_slipDeltaX_new     = 0.0f;
    m_slipDeltaX_old     = 0.0f;
    m_slipDeltaY_new     = 0.0f;
    m_slipDeltaY_old     = 0.0f;
    m_turnDeltaX_new     = 0.0f;
    m_turnDeltaX_old     = 0.0f;
    m_pathDeltaX_new     = 0.0f;
    m_pathDeltaX_old     = 0.0f;
    m_pathDeltaY_new     = 0.0f;
    m_pathDeltaY_old     = 0.0f;
    m_markDeltaX_new     = 0.0f;
    m_markDeltaX_old     = 0.0f;
    m_markDeltaY_new     = 0.0f;
    m_markDeltaY_old     = 0.0f;
    m_barHDeltaX_new     = 0.0f;
    m_barHDeltaX_old     = 0.0f;
    m_barVDeltaY_new     = 0.0f;
    m_barVDeltaY_old     = 0.0f;
    m_dotHDeltaX_new     = 0.0f;
    m_dotHDeltaX_old     = 0.0f;
    m_dotVDeltaY_new     = 0.0f;
    m_dotVDeltaY_old     = 0.0f;
}

void Pfd::ADI::updateLadd(const float delta, const float sinRoll, const float cosRoll)
{
    m_itemLadd->setRotation(-m_roll);

    m_laddDeltaX_new = m_scaleX * delta * sinRoll;
    m_laddDeltaY_new = m_scaleY * delta * cosRoll;

    m_itemLadd->moveBy( m_laddDeltaX_new - m_laddDeltaX_old, m_laddDeltaY_new - m_laddDeltaY_old );
}

void Pfd::ADI::updateLaddBack(const float delta, const float sinRoll, const float cosRoll)
{
    m_itemBack->setRotation(-m_roll);

    float deltaLaddBack = 0.0;

    if (delta > m_deltaLaddBack_max) {
        deltaLaddBack = m_deltaLaddBack_max;
    } else if ( delta < m_deltaLaddBack_min ) {
        deltaLaddBack = m_deltaLaddBack_min;
    } else {
        deltaLaddBack = delta;
    }

    m_laddBackDeltaX_new = m_scaleX * deltaLaddBack * sinRoll;
    m_laddBackDeltaY_new = m_scaleY * deltaLaddBack * cosRoll;

    m_itemBack->moveBy( m_laddBackDeltaX_new - m_laddBackDeltaX_old, m_laddBackDeltaY_new - m_laddBackDeltaY_old );
}

void Pfd::ADI::updateRoll()
{
    m_itemRoll->setRotation(-m_roll);
}

void Pfd::ADI::updateSlipSkid(const float sinRoll, const float cosRoll)
{
    m_itemSlip->setRotation(-m_roll);

    const float deltaSlip = m_maxSlipDeflection * m_slipSkid;

    m_slipDeltaX_new =  m_scaleX * deltaSlip * cosRoll;
    m_slipDeltaY_new = -m_scaleY * deltaSlip * sinRoll;

    m_itemSlip->moveBy( m_slipDeltaX_new - m_slipDeltaX_old, m_slipDeltaY_new - m_slipDeltaY_old );
}

void Pfd::ADI::updateTurnRate()
{
    m_turnDeltaX_new = m_scaleX * m_maxTurnDeflection * m_turnRate;
    m_itemTurn->moveBy( m_turnDeltaX_new - m_turnDeltaX_old, 0.0 );
}

void Pfd::ADI::updateFlightPath()
{
    if (m_pathVisible) {
        m_itemPath->setVisible( true );

        m_pathDeltaX_new = m_scaleX * m_originalPixPerDeg * m_sideslipAngle;
        m_pathDeltaY_new = m_scaleY * m_originalPixPerDeg * m_angleOfAttack;

        m_itemPath->moveBy( m_pathDeltaX_new - m_pathDeltaX_old, m_pathDeltaY_old - m_pathDeltaY_new );

        if (!m_pathValid) {
            m_itemMark->setVisible( true );

            m_markDeltaX_new = m_pathDeltaX_new;
            m_markDeltaY_new = m_pathDeltaY_new;

            m_itemMark->moveBy( m_markDeltaX_new - m_markDeltaX_old, m_markDeltaY_old - m_markDeltaY_new );
        } else {
            m_itemMark->setVisible( false );
            m_markDeltaX_new = m_markDeltaX_old;
            m_markDeltaY_new = m_markDeltaY_old;
        }
    } else {
        m_itemPath->setVisible( false );
        m_pathDeltaX_new = m_pathDeltaX_old;
        m_pathDeltaY_new = m_pathDeltaY_old;

        m_itemMark->setVisible( false );
        m_markDeltaX_new = m_markDeltaX_old;
        m_markDeltaY_new = m_markDeltaY_old;
    }
}

void Pfd::ADI::updateBars()
{
    if (m_barVVisible) {
        m_itemBarV->setVisible( true );

        m_barVDeltaY_new = m_scaleY * m_maxBarsDeflection * m_barV;

        m_itemBarV->moveBy( 0.0f, m_barVDeltaY_old - m_barVDeltaY_new );
    } else {
        m_itemBarV->setVisible( false );
        m_barVDeltaY_new = m_barVDeltaY_old;
    }

    if (m_barHVisible) {
        m_itemBarH->setVisible( true );

        m_barHDeltaX_new = m_scaleX * m_maxBarsDeflection * m_barH;

        m_itemBarH->moveBy( m_barHDeltaX_new - m_barHDeltaX_old, 0.0f );
    } else {
        m_itemBarH->setVisible( false );
        m_barHDeltaX_new = m_barHDeltaX_old;
    }
}

void Pfd::ADI::updateDots()
{
    if (m_dotHVisible) {
        m_itemDotH->setVisible( true );
        m_itemScaleH->setVisible( true );

        m_dotHDeltaX_new = m_scaleX * m_maxDotsDeflection * m_dotH;

        m_itemDotH->moveBy( m_dotHDeltaX_new - m_dotHDeltaX_old, 0.0f );
    } else {
        m_itemDotH->setVisible( false );
        m_itemScaleH->setVisible( false );

        m_dotHDeltaX_new = m_dotHDeltaX_old;
    }

    if (m_dotVVisible) {
        m_itemDotV->setVisible( true );
        m_itemScaleV->setVisible( true );

        m_dotVDeltaY_new = m_scaleY * m_maxDotsDeflection * m_dotV;

        m_itemDotV->moveBy( 0.0f, m_dotVDeltaY_old - m_dotVDeltaY_new );
    } else {
        m_itemDotV->setVisible( false );
        m_itemScaleV->setVisible( false );

        m_dotVDeltaY_new = m_dotVDeltaY_old;
    }
}

Pfd::ALT::ALT(QGraphicsScene* scene) : m_scene(scene)
{
#   ifdef WIN32
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 9.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "Courier" );
    m_labelsFont.setPointSizeF( 7.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   else
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 10.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "Courier" );
    m_labelsFont.setPointSizeF( 8.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   endif

    reset();
}

void Pfd::ALT::init(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_alt_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemScale1 = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_alt_scale.svg" );
    m_itemScale1->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale1->setZValue( m_scaleZ );
    m_itemScale1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale1->moveBy( m_scaleX * m_originalScale1Pos.x(), m_scaleY * m_originalScale1Pos.y() );
    m_scene->addItem( m_itemScale1 );

    m_itemScale2 = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_alt_scale.svg" );
    m_itemScale2->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale2->setZValue( m_scaleZ );
    m_itemScale2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale2->moveBy( m_scaleX * m_originalScale2Pos.x(), m_scaleY * m_originalScale2Pos.y() );
    m_scene->addItem( m_itemScale2 );

    m_itemLabel1 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel1->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel1->setZValue( m_labelsZ );
    m_itemLabel1->setDefaultTextColor( m_labelsColor );
    m_itemLabel1->setFont( m_labelsFont );
    m_itemLabel1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel1->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel1->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel1Y - m_itemLabel1->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel1 );

    m_itemLabel2 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel2->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel2->setZValue( m_labelsZ );
    m_itemLabel2->setDefaultTextColor( m_labelsColor );
    m_itemLabel2->setFont( m_labelsFont );
    m_itemLabel2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel2->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel2->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel2Y - m_itemLabel2->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel2 );

    m_itemLabel3 = new QGraphicsTextItem( QString( "99999" ) );
    m_itemLabel3->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel3->setZValue( m_labelsZ );
    m_itemLabel3->setDefaultTextColor( m_labelsColor );
    m_itemLabel3->setFont( m_labelsFont );
    m_itemLabel3->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel3->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel3->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel3Y - m_itemLabel3->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel3 );

    m_itemGround = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_alt_ground.svg" );
    m_itemGround->setCacheMode( QGraphicsItem::NoCache );
    m_itemGround->setZValue( m_groundZ );
    m_itemGround->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemGround->moveBy( m_scaleX * m_originalGroundPos.x(), m_scaleY * m_originalGroundPos.y() );
    m_scene->addItem( m_itemGround );

    m_itemFrame = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_alt_frame.svg" );
    m_itemFrame->setCacheMode( QGraphicsItem::NoCache );
    m_itemFrame->setZValue( m_frameZ );
    m_itemFrame->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFrame->moveBy( m_scaleX * m_originalFramePos.x(), m_scaleY * m_originalFramePos.y() );
    m_scene->addItem( m_itemFrame );

    m_itemAltitude = new QGraphicsTextItem( QString( "    0" ) );
    m_itemAltitude->setCacheMode( QGraphicsItem::NoCache );
    m_itemAltitude->setZValue( m_frameTextZ );
    m_itemAltitude->setDefaultTextColor( m_frameTextColor );
    m_itemAltitude->setFont( m_frameTextFont );
    m_itemAltitude->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemAltitude->moveBy( m_scaleX * ( m_originalAltitudeCtr.x() - m_itemAltitude->boundingRect().width()  / 2.0f ),
                           m_scaleY * ( m_originalAltitudeCtr.y() - m_itemAltitude->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemAltitude );

    m_itemPressure = new QGraphicsTextItem( QString( "  STD  " ) );
    m_itemPressure->setCacheMode( QGraphicsItem::NoCache );
    m_itemPressure->setZValue( m_frameTextZ );
    m_itemPressure->setDefaultTextColor( m_pressTextColor );
    m_itemPressure->setFont( m_frameTextFont );
    m_itemPressure->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemPressure->moveBy( m_scaleX * ( m_originalPressureCtr.x() - m_itemPressure->boundingRect().width()  / 2.0f ),
                           m_scaleY * ( m_originalPressureCtr.y() - m_itemPressure->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemPressure );

    update( scaleX, scaleY );
}

void Pfd::ALT::update(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateAltitude();
    updatePressure();

    m_scale1DeltaY_old = m_scale1DeltaY_new;
    m_scale2DeltaY_old = m_scale2DeltaY_new;
    m_groundDeltaY_old = m_groundDeltaY_new;
    m_labelsDeltaY_old = m_labelsDeltaY_new;

}

void Pfd::ALT::setAltitude(const float altitude)
{
    m_altitude = altitude;

    if      ( m_altitude <     0.0f ) m_altitude =     0.0f;
    else if ( m_altitude > 99999.0f ) m_altitude = 99999.0f;
}

void Pfd::ALT::setPressure(const float pressure, const int pressureUnit)
{
    m_pressure = pressure;

    if      ( m_pressure <    0.0f ) m_pressure =    0.0f;
    else if ( m_pressure > 2000.0f ) m_pressure = 2000.0f;

    m_pressureUnit = 0;

    if      ( pressureUnit == 1 ) m_pressureUnit = 1;
    else if ( pressureUnit == 2 ) m_pressureUnit = 2;
}

void Pfd::ALT::reset()
{
    m_itemBack     = 0;
    m_itemScale1   = 0;
    m_itemScale2   = 0;
    m_itemLabel1   = 0;
    m_itemLabel2   = 0;
    m_itemLabel3   = 0;
    m_itemGround   = 0;
    m_itemFrame    = 0;
    m_itemAltitude = 0;
    m_itemPressure = 0;

    m_altitude = 0.0f;
    m_pressure = 0.0f;

    m_pressureUnit = 0;

    m_scale1DeltaY_new = 0.0f;
    m_scale1DeltaY_old = 0.0f;
    m_scale2DeltaY_new = 0.0f;
    m_scale2DeltaY_old = 0.0f;
    m_groundDeltaY_new = 0.0f;
    m_groundDeltaY_old = 0.0f;
    m_labelsDeltaY_new = 0.0f;
    m_labelsDeltaY_old = 0.0f;
}

void Pfd::ALT::updateAltitude()
{
    m_itemAltitude->setPlainText( QString("%1").arg(m_altitude, 5, 'f', 0, QChar(' ')) );

    updateScale();
    updateScaleLabels();
}

void Pfd::ALT::updatePressure()
{
    if (m_pressureUnit == 0) {
        m_itemPressure->setPlainText( QString( "  STD  " ) );
    } else if ( m_pressureUnit == 1 ) {
        m_itemPressure->setPlainText( QString::number( m_pressure, 'f', 0 ) + QString( " MB" ) );
    } else if ( m_pressureUnit == 2 ) {
        m_itemPressure->setPlainText( QString::number( m_pressure, 'f', 2 ) + QString( " IN" ) );
    }
}

void Pfd::ALT::updateScale()
{
    m_scale1DeltaY_new = m_scaleY * m_originalPixPerAlt * m_altitude;
    m_scale2DeltaY_new = m_scale1DeltaY_new;
    m_groundDeltaY_new = m_scale1DeltaY_new;

    const float scaleSingleHeight = m_scaleY * m_originalScaleHeight;
    const float scaleDoubleHeight = m_scaleY * m_originalScaleHeight * 2.0f;

    while ( m_scale1DeltaY_new > scaleSingleHeight + m_scaleY * 74.5f ) {
        m_scale1DeltaY_new = m_scale1DeltaY_new - scaleDoubleHeight;
    }

    while ( m_scale2DeltaY_new > scaleDoubleHeight + m_scaleY * 74.5f ) {
        m_scale2DeltaY_new = m_scale2DeltaY_new - scaleDoubleHeight;
    }

    if ( m_groundDeltaY_new > m_scaleY * 100.0f ) m_groundDeltaY_new = m_scaleY * 100.0f;

    m_itemScale1->moveBy( 0.0f, m_scale1DeltaY_new - m_scale1DeltaY_old );
    m_itemScale2->moveBy( 0.0f, m_scale2DeltaY_new - m_scale2DeltaY_old );
    m_itemGround->moveBy( 0.0f, m_groundDeltaY_new - m_groundDeltaY_old );
}

void Pfd::ALT::updateScaleLabels()
{
    const int tmp = std::floor( m_altitude + 0.5f );
    const int alt = tmp - ( tmp % 500 );

    float alt1{static_cast<float>(alt + 500.0f)};
    float alt2{static_cast<float>(alt)};
    float alt3{static_cast<float>(alt - 500.0f)};

    m_labelsDeltaY_new = m_scaleY * m_originalPixPerAlt * m_altitude;

    while ( m_labelsDeltaY_new > m_scaleY * 37.5f ) {
        m_labelsDeltaY_new = m_labelsDeltaY_new - m_scaleY * 75.0f;
    }

    if ( m_labelsDeltaY_new < 0.0f && m_altitude > alt2 ) {
        alt1 += 500.0f;
        alt2 += 500.0f;
        alt3 += 500.0f;
    }

    m_itemLabel1->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel2->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel3->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );

    if ( alt1 > 0.0f && alt1 <= 100000.0f ) {
        m_itemLabel1->setVisible( true );
        m_itemLabel1->setPlainText( QString("%1").arg(alt1, 5, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel1->setVisible( false );
    }

    if ( alt2 > 0.0f && alt2 <= 100000.0f ) {
        m_itemLabel2->setVisible( true );
        m_itemLabel2->setPlainText( QString("%1").arg(alt2, 5, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel2->setVisible( false );
    }

    if ( alt3 > 0.0f && alt3 <= 100000.0f ) {
        m_itemLabel3->setVisible( true );
        m_itemLabel3->setPlainText( QString("%1").arg(alt3, 5, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel3->setVisible( false );
    }
}

Pfd::ASI::ASI(QGraphicsScene* scene) : m_scene(scene)
{
#   ifdef WIN32
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 9.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "Courier" );
    m_labelsFont.setPointSizeF( 7.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   else
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 10.0f );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    m_labelsFont.setFamily( "Courier" );
    m_labelsFont.setPointSizeF( 8.0f );
    m_labelsFont.setStretch( QFont::Condensed );
    m_labelsFont.setWeight( QFont::Bold );
#   endif

    reset();
}

void Pfd::ASI::init(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_asi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemScale1 = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_asi_scale.svg" );
    m_itemScale1->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale1->setZValue( m_scaleZ );
    m_itemScale1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale1->moveBy( m_scaleX * m_originalScale1Pos.x(), m_scaleY * m_originalScale1Pos.y() );
    m_scene->addItem( m_itemScale1 );

    m_itemScale2 = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_asi_scale.svg" );
    m_itemScale2->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale2->setZValue( m_scaleZ );
    m_itemScale2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale2->moveBy( m_scaleX * m_originalScale2Pos.x(), m_scaleY * m_originalScale2Pos.y() );
    m_scene->addItem( m_itemScale2 );

    m_itemLabel1 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel1->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel1->setZValue( m_labelsZ );
    m_itemLabel1->setDefaultTextColor( m_labelsColor );
    m_itemLabel1->setFont( m_labelsFont );
    m_itemLabel1->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel1->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel1->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel1Y - m_itemLabel1->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel1 );

    m_itemLabel2 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel2->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel2->setZValue( m_labelsZ );
    m_itemLabel2->setDefaultTextColor( m_labelsColor );
    m_itemLabel2->setFont( m_labelsFont );
    m_itemLabel2->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel2->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel2->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel2Y - m_itemLabel2->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel2 );

    m_itemLabel3 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel3->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel3->setZValue( m_labelsZ );
    m_itemLabel3->setDefaultTextColor( m_labelsColor );
    m_itemLabel3->setFont( m_labelsFont );
    m_itemLabel3->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel3->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel3->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel3Y - m_itemLabel3->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel3 );

    m_itemLabel4 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel4->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel4->setZValue( m_labelsZ );
    m_itemLabel4->setDefaultTextColor( m_labelsColor );
    m_itemLabel4->setFont( m_labelsFont );
    m_itemLabel4->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel4->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel4->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel4Y - m_itemLabel4->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel4 );

    m_itemLabel5 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel5->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel5->setZValue( m_labelsZ );
    m_itemLabel5->setDefaultTextColor( m_labelsColor );
    m_itemLabel5->setFont( m_labelsFont );
    m_itemLabel5->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel5->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel5->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel5Y - m_itemLabel5->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel5 );

    m_itemLabel6 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel6->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel6->setZValue( m_labelsZ );
    m_itemLabel6->setDefaultTextColor( m_labelsColor );
    m_itemLabel6->setFont( m_labelsFont );
    m_itemLabel6->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel6->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel6->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel6Y - m_itemLabel6->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel6 );

    m_itemLabel7 = new QGraphicsTextItem( QString( "999" ) );
    m_itemLabel7->setCacheMode( QGraphicsItem::NoCache );
    m_itemLabel7->setZValue( m_labelsZ );
    m_itemLabel7->setDefaultTextColor( m_labelsColor );
    m_itemLabel7->setFont( m_labelsFont );
    m_itemLabel7->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemLabel7->moveBy( m_scaleX * ( m_originalLabelsX - m_itemLabel7->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalLabel7Y - m_itemLabel7->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemLabel7 );

    m_itemFrame = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_asi_frame.svg" );
    m_itemFrame->setCacheMode( QGraphicsItem::NoCache );
    m_itemFrame->setZValue( m_frameZ );
    m_itemFrame->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFrame->moveBy( m_scaleX * m_originalFramePos.x(), m_scaleY * m_originalFramePos.y() );
    m_scene->addItem( m_itemFrame );

    m_itemAirspeed = new QGraphicsTextItem( QString( "000" ) );
    m_itemAirspeed->setCacheMode( QGraphicsItem::NoCache );
    m_itemAirspeed->setZValue( m_frameTextZ );
    m_itemAirspeed->setTextInteractionFlags( Qt::NoTextInteraction );
    m_itemAirspeed->setDefaultTextColor( m_frameTextColor );
    m_itemAirspeed->setFont( m_frameTextFont );
    m_itemAirspeed->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemAirspeed->moveBy( m_scaleX * ( m_originalAirspeedCtr.x() - m_itemAirspeed->boundingRect().width()  / 2.0f ),
                           m_scaleY * ( m_originalAirspeedCtr.y() - m_itemAirspeed->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemAirspeed );

    m_itemMachNo = new QGraphicsTextItem( QString( ".000" ) );
    m_itemMachNo->setCacheMode( QGraphicsItem::NoCache );
    m_itemMachNo->setZValue( m_frameTextZ );
    m_itemMachNo->setTextInteractionFlags( Qt::NoTextInteraction );
    m_itemMachNo->setDefaultTextColor( m_frameTextColor );
    m_itemMachNo->setFont( m_frameTextFont );
    m_itemMachNo->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemMachNo->moveBy( m_scaleX * ( m_originalMachNoCtr.x() - m_itemMachNo->boundingRect().width()  / 2.0f ),
                         m_scaleY * ( m_originalMachNoCtr.y() - m_itemMachNo->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemMachNo );

    update( scaleX, scaleY );
}

void Pfd::ASI::update(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateAirspeed();

    m_scale1DeltaY_old = m_scale1DeltaY_new;
    m_scale2DeltaY_old = m_scale2DeltaY_new;
    m_labelsDeltaY_old = m_labelsDeltaY_new;
}

void Pfd::ASI::setAirspeed(const float airspeed)
{
    m_airspeed = airspeed;

    if      ( m_airspeed <    0.0f ) m_airspeed =    0.0f;
    else if ( m_airspeed > 9999.0f ) m_airspeed = 9999.0f;
}

void Pfd::ASI::setMachNo(const float machNo)
{
    m_machNo = machNo;

    if      ( m_machNo <  0.0f ) m_machNo =  0.0f;
    else if ( m_machNo > 99.9f ) m_machNo = 99.9f;
}

void Pfd::ASI::reset()
{
    m_itemBack     = 0;
    m_itemScale1   = 0;
    m_itemScale2   = 0;
    m_itemLabel1   = 0;
    m_itemLabel2   = 0;
    m_itemLabel3   = 0;
    m_itemLabel4   = 0;
    m_itemLabel5   = 0;
    m_itemLabel6   = 0;
    m_itemLabel7   = 0;
    m_itemFrame    = 0;
    m_itemAirspeed = 0;
    m_itemMachNo   = 0;

    m_airspeed = 0.0f;
    m_machNo   = 0.0f;

    m_scale1DeltaY_new = 0.0f;
    m_scale1DeltaY_old = 0.0f;
    m_scale2DeltaY_new = 0.0f;
    m_scale2DeltaY_old = 0.0f;
    m_labelsDeltaY_new = 0.0f;
    m_labelsDeltaY_old = 0.0f;
}

void Pfd::ASI::updateAirspeed()
{
    m_itemAirspeed->setPlainText( QString("%1").arg(m_airspeed, 3, 'f', 0, QChar('0')) );

    if ( m_machNo < 1.0f ) {
        float machNo = 1000.0f * m_machNo;
        m_itemMachNo->setPlainText( QString(".%1").arg(machNo, 3, 'f', 0, QChar('0')) );
    } else {
        if ( m_machNo < 10.0f ) {
            m_itemMachNo->setPlainText( QString::number( m_machNo, 'f', 2 ) );
        } else {
            m_itemMachNo->setPlainText( QString::number( m_machNo, 'f', 1 ) );
        }
    }

    updateScale();
    updateScaleLabels();
}

void Pfd::ASI::updateScale()
{
    m_scale1DeltaY_new = m_scaleY * m_originalPixPerSpd * m_airspeed;
    m_scale2DeltaY_new = m_scale1DeltaY_new;

    const float scaleSingleHeight = m_scaleY * m_originalScaleHeight;
    const float scaleDoubleHeight = m_scaleY * m_originalScaleHeight * 2.0f;

    while ( m_scale1DeltaY_new > scaleSingleHeight + m_scaleY * 74.5f ) {
        m_scale1DeltaY_new = m_scale1DeltaY_new - scaleDoubleHeight;
    }

    while ( m_scale2DeltaY_new > scaleDoubleHeight + m_scaleY * 74.5f ) {
        m_scale2DeltaY_new = m_scale2DeltaY_new - scaleDoubleHeight;
    }

    m_itemScale1->moveBy( 0.0f, m_scale1DeltaY_new - m_scale1DeltaY_old );
    m_itemScale2->moveBy( 0.0f, m_scale2DeltaY_new - m_scale2DeltaY_old );
}

void Pfd::ASI::updateScaleLabels()
{
    m_labelsDeltaY_new = m_scaleY * m_originalPixPerSpd * m_airspeed;

    const int tmp = std::floor( m_airspeed + 0.5f );
    const int spd = tmp - ( tmp % 20 );

    float spd1{static_cast<float>(spd + 60.0f)};
    float spd2{static_cast<float>(spd + 40.0f)};
    float spd3{static_cast<float>(spd + 20.0f)};
    float spd4{static_cast<float>(spd)};
    float spd5{static_cast<float>(spd - 20.0f)};
    float spd6{static_cast<float>(spd - 40.0f)};
    float spd7{static_cast<float>(spd - 60.0f)};

    while ( m_labelsDeltaY_new > m_scaleY * 15.0f ) {
        m_labelsDeltaY_new = m_labelsDeltaY_new - m_scaleY * 30.0f;
    }

    if ( m_labelsDeltaY_new < 0.0 && m_airspeed > spd4 ) {
        spd1 += 20.0f;
        spd2 += 20.0f;
        spd3 += 20.0f;
        spd4 += 20.0f;
        spd5 += 20.0f;
        spd6 += 20.0f;
        spd7 += 20.0f;
    }

    m_itemLabel1->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel2->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel3->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel4->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel5->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel6->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );
    m_itemLabel7->moveBy( 0.0f, m_labelsDeltaY_new - m_labelsDeltaY_old );

    if ( spd1 >= 0.0f && spd1 <= 10000.0f ) {
        m_itemLabel1->setVisible( true );
        m_itemLabel1->setPlainText( QString("%1").arg(spd1, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel1->setVisible( false );
    }

    if ( spd2 >= 0.0f && spd2 <= 10000.0f ) {
        m_itemLabel2->setVisible( true );
        m_itemLabel2->setPlainText( QString("%1").arg(spd2, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel2->setVisible( false );
    }

    if ( spd3 >= 0.0f && spd3 <= 10000.0f ) {
        m_itemLabel3->setVisible( true );
        m_itemLabel3->setPlainText( QString("%1").arg(spd3, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel3->setVisible( false );
    }

    if ( spd4 >= 0.0f && spd4 <= 10000.0f ) {
        m_itemLabel4->setVisible( true );
        m_itemLabel4->setPlainText( QString("%1").arg(spd4, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel4->setVisible( false );
    }

    if ( spd5 >= 0.0f && spd5 <= 10000.0f ) {
        m_itemLabel5->setVisible( true );
        m_itemLabel5->setPlainText( QString("%1").arg(spd5, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel5->setVisible( false );
    }

    if ( spd6 >= 0.0f && spd6 <= 10000.0f ) {
        m_itemLabel6->setVisible( true );
        m_itemLabel6->setPlainText( QString("%1").arg(spd6, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel6->setVisible( false );
    }

    if ( spd7 >= 0.0f && spd7 <= 10000.0f ) {
        m_itemLabel7->setVisible( true );
        m_itemLabel7->setPlainText( QString("%1").arg(spd7, 3, 'f', 0, QChar(' ')) );
    } else {
        m_itemLabel7->setVisible( false );
    }
}

Pfd::HSI::HSI(QGraphicsScene* scene) : m_scene(scene)
{
    m_frameTextFont.setFamily( "Courier" );
    m_frameTextFont.setPointSizeF( 10.0 );
    m_frameTextFont.setStretch( QFont::Condensed );
    m_frameTextFont.setWeight( QFont::Bold );

    reset();
}

void Pfd::HSI::init( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemBack = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_hsi_back.svg" );
    m_itemBack->setCacheMode( QGraphicsItem::NoCache );
    m_itemBack->setZValue( m_backZ );
    m_itemBack->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemBack->moveBy( m_scaleX * m_originalBackPos.x(), m_scaleY * m_originalBackPos.y() );
    m_scene->addItem( m_itemBack );

    m_itemFace = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_hsi_face.svg" );
    m_itemFace->setCacheMode( QGraphicsItem::NoCache );
    m_itemFace->setZValue( m_faceZ );
    m_itemFace->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFace->setTransformOriginPoint( m_originalHsiCtr - m_originalFacePos );
    m_itemFace->moveBy( m_scaleX * m_originalFacePos.x(), m_scaleY * m_originalFacePos.y() );
    m_scene->addItem( m_itemFace );

    m_itemMarks = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_hsi_marks.svg" );
    m_itemMarks->setCacheMode( QGraphicsItem::NoCache );
    m_itemMarks->setZValue( m_marksZ );
    m_itemMarks->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemMarks->moveBy( m_scaleX * m_originalMarksPos.x(), m_scaleY * m_originalMarksPos.y() );
    m_scene->addItem( m_itemMarks );

    m_itemFrameText = new QGraphicsTextItem( QString( "000" ) );
    m_itemFrameText->setCacheMode( QGraphicsItem::NoCache );
    m_itemFrameText->setZValue( m_frameTextZ );
    m_itemFrameText->setTextInteractionFlags( Qt::NoTextInteraction );
    m_itemFrameText->setDefaultTextColor( m_frameTextColor );
    m_itemFrameText->setFont( m_frameTextFont );
    m_itemFrameText->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemFrameText->moveBy( m_scaleX * ( m_originalFrameTextCtr.x() - m_itemFrameText->boundingRect().width()  / 2.0f ),
                            m_scaleY * ( m_originalFrameTextCtr.y() - m_itemFrameText->boundingRect().height() / 2.0f ) );
    m_scene->addItem( m_itemFrameText );

    update( scaleX, scaleY );
}

void Pfd::HSI::update( float scaleX, float scaleY )
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateHeading();
}

void Pfd::HSI::setHeading( float heading )
{
    m_heading = heading;

    while ( m_heading < 0.0f ) {
        m_heading += 360.0f;
    }

    while ( m_heading > 360.0f ) {
        m_heading -= 360.0f;
    }
}

void Pfd::HSI::reset()
{
    m_itemBack      = nullptr;
    m_itemFace      = nullptr;
    m_itemMarks     = nullptr;
    m_itemFrameText = nullptr;

    m_heading  = 0.0f;
}

void Pfd::HSI::updateHeading()
{
    m_itemFace->setRotation( - m_heading );
    const float fHeading{static_cast<float>(floor(m_heading + 0.5f))};
    m_itemFrameText->setPlainText( QString("%1").arg(fHeading, 3, 'f', 0, QChar('0')) );
}

Pfd::VSI::VSI(QGraphicsScene* scene) : m_scene(scene)
{
    reset();
}

void Pfd::VSI::init(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    reset();

    m_itemScale = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_vsi_scale.svg" );
    m_itemScale->setCacheMode( QGraphicsItem::NoCache );
    m_itemScale->setZValue( m_scaleZ );
    m_itemScale->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemScale->moveBy( m_scaleX * m_originalScalePos.x(), m_scaleY * m_originalScalePos.y() );
    m_scene->addItem( m_itemScale );

    m_itemArrow = new QGraphicsSvgItem( ":/qfi/images/pfd/pfd_vsi_arrow.svg" );
    m_itemArrow->setCacheMode( QGraphicsItem::NoCache );
    m_itemArrow->setZValue( m_arrowZ );
    m_itemArrow->setTransform( QTransform::fromScale( m_scaleX, m_scaleY ), true );
    m_itemArrow->moveBy( m_scaleX * m_originalArrowPos.x(), m_scaleY * m_originalArrowPos.y() );
    m_scene->addItem( m_itemArrow );

    update( scaleX, scaleY );
}

void Pfd::VSI::update(const float scaleX, const float scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;

    updateVSI();

    m_arrowDeltaY_old = m_arrowDeltaY_new;
}

void Pfd::VSI::setClimbRate(const float climbRate)
{
    m_climbRate = climbRate;

    if      ( m_climbRate >  6.3f ) m_climbRate =  6.3f;
    else if ( m_climbRate < -6.3f ) m_climbRate = -6.3f;
}

void Pfd::VSI::reset()
{
    m_itemScale = 0;
    m_itemArrow = 0;

    m_climbRate = 0.0;

    m_arrowDeltaY_new = 0.0f;
    m_arrowDeltaY_old = 0.0f;
}

void Pfd::VSI::updateVSI()
{
    const float climbRateAbs{static_cast<float>(fabs(m_climbRate))};
    float arrowDeltaY{};

    if ( climbRateAbs <= 1.0f ) {
        arrowDeltaY = m_originalPixPerSpd1 * climbRateAbs;
    } else if ( climbRateAbs <= 2.0f ) {
        arrowDeltaY = m_originalPixPerSpd1 + m_originalPixPerSpd2 * ( climbRateAbs - 1.0f );
    } else {
        arrowDeltaY = m_originalPixPerSpd1 + m_originalPixPerSpd2 + m_originalPixPerSpd4 * ( climbRateAbs - 2.0f );
    }

    if ( m_climbRate < 0.0f ) arrowDeltaY *= -1.0f;
    m_arrowDeltaY_new = m_scaleY * arrowDeltaY;
    m_itemArrow->moveBy( 0.0f, m_arrowDeltaY_old - m_arrowDeltaY_new );
}

}
