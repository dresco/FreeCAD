/***************************************************************************
 *   Copyright (c) 2012-2013 Luke Parry <l.parry@warwick.ac.uk>            *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef DRAWINGGUI_QGRAPHICSITEMVIEW_H
#define DRAWINGGUI_QGRAPHICSITEMVIEW_H

#include <Mod/TechDraw/TechDrawGlobal.h>

#include <boost_signals2.hpp>

#include <QColor>
#include <QFont>
#include <QGraphicsItemGroup>
#include <QObject>
#include <QPen>
#include <QPointF>

#include <Base/Parameter.h>
#include <Base/Vector3D.h>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

namespace App
{
class DocumentObject;
}

namespace Gui
{
class ViewProvider;
}

namespace TechDraw
{
class DrawView;
}

namespace TechDrawGui
{
class QGSPage;
class QGVPage;
class ViewProviderPage;
class QGCustomBorder;
class QGCustomLabel;
class QGCustomText;
class QGICaption;
class MDIViewPage;
class QGIViewClip;
class QGCustomImage;
class QGTracker;
class QGIVertex;

class TechDrawGuiExport  QGIView : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    QGIView();
    ~QGIView() override;

    enum {Type = QGraphicsItem::UserType + 101};
    int type() const override { return Type;}
    QRectF boundingRect() const override;
    void paint( QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget = nullptr ) override;

    const char *      getViewName() const;
    const std::string getViewNameAsString() const;
    void setViewFeature(TechDraw::DrawView *obj);
    TechDraw::DrawView * getViewObject() const;
    MDIViewPage* getMDIViewPage() const;

    double getScale();

    virtual bool getFrameState();
    virtual void toggleCache(bool state);
    virtual void updateView(bool update = false);
    virtual void drawBorder();
    virtual void isVisible(bool state);
    virtual bool isVisible();

    virtual void setGroupSelection(bool b);

    virtual void draw();
    virtual void drawCaption();
    virtual void rotateView();
    void makeMark(double x, double y, QColor c = Qt::red);
    void makeMark(Base::Vector3d v, QColor c = Qt::red);
    void makeMark(QPointF p, QColor c = Qt::red);


    /** Methods to ensure that Y-Coordinates are orientated correctly.
     * @{ */
    void setPosition(qreal x, qreal y);
    inline qreal getY() { return y() * -1; }
    bool isInnerView() { return m_innerView; }
    void isInnerView(bool state) { m_innerView = state; }
    double getYInClip(double y);
    /** @} */
    QGIViewClip* getClipGroup();


    void alignTo(QGraphicsItem*, const QString &alignment);
    void setLocked(bool b) { m_locked = b; }

    virtual QColor getNormalColor();  //preference
    virtual QColor getPreColor();     //preference
    virtual QColor getSelectColor();  //preference
    virtual QColor getCurrentColor() { return m_colCurrent; }
    virtual QColor getSettingColor() { return m_colSetting; }
    virtual void   setSettingColor(QColor c) { m_colSetting = c; }
    
    static Gui::ViewProvider* getViewProvider(App::DocumentObject* obj);
    static ViewProviderPage* getViewProviderPage(TechDraw::DrawView* dv);
    static QGVPage* getQGVPage(TechDraw::DrawView* dv);
    static QGSPage* getQGSPage(TechDraw::DrawView* dv);
    static int calculateFontPixelSize(double sizeInMillimetres);
    static int calculateFontPixelWidth(const QFont &font);
    static const double DefaultFontSizeInMM;

    static QString getPrefFont();
    static double getPrefFontSize();
    static double getDimFontSize();

    virtual void removeChild(QGIView* child);

    virtual void addArbitraryItem(QGraphicsItem* qgi);

    // Mouse handling
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    boost::signals2::signal<void (QGIView*, QPointF)> signalSelectPoint;

public Q_SLOTS:
    virtual void onSourceChange(TechDraw::DrawView* newParent);

protected:
    QGIView* getQGIVByName(std::string name);

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    // Preselection events:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    virtual QRectF customChildrenBoundingRect() const;
    void dumpRect(const char* text, QRectF r);

/*    QString getPrefFont(void);*/
/*    double getPrefFontSize(void);*/
/*    double getDimFontSize(void);*/

    Base::Reference<ParameterGrp> getParmGroupCol();

    TechDraw::DrawView *viewObj;
    std::string viewName;

    QHash<QString, QGraphicsItem*> alignHash;
    //std::string alignMode;
    //QGIView* alignAnchor;
    bool m_locked;
    bool m_innerView;                                                  //View is inside another View

    QPen m_pen;
    QBrush m_brush;
    QColor m_colCurrent;
    QColor m_colNormal;
    QColor m_colPre;
    QColor m_colSel;
    QColor m_colSetting;
    QFont m_font;
    QGCustomLabel* m_label;
    QGCustomBorder* m_border;
    QGICaption* m_caption;
    QGCustomImage* m_lock;
    QPen m_decorPen;
    double m_lockWidth;
    double m_lockHeight;
    int m_dragState;

};

} // namespace

#endif // DRAWINGGUI_QGRAPHICSITEMVIEW_H
