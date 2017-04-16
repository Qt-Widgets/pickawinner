#include "saturationlightnesspicker_p.h"

#include <QtQuickTemplates2/private/qquickcontrol_p_p.h>

QT_BEGIN_NAMESPACE

/*!
    \qmltype SaturationLightnessPicker
    \inherits Control
    \instantiates SaturationLightnessPicker
    \inqmlmodule QtQuick.Controls
    \ingroup qtquickcontrols2-input
    \brief Provides a way to select both saturation and lightness simultaneously.

    \image qtquickcontrols2-slider.gif

    \code
    SaturationLightnessPicker {
    }
    \endcode

    \sa {Customizing SaturationLightnessPicker}, {Input Controls}
*/

class SaturationLightnessPickerPrivate : public QQuickControlPrivate
{
    Q_DECLARE_PUBLIC(SaturationLightnessPicker)

public:
    SaturationLightnessPickerPrivate() :
        hue(0),
        saturation(0),
        lightness(0),
        alpha(0),
        pressed(false),
        handle(nullptr)
    {
    }

    void setHandleZ();
    qreal saturationAt(const QPointF &pos) const;
    qreal lightnessAt(const QPointF &pos) const;

    void setHue(qreal hue, bool emitColorChanged = true);
    void setSaturation(qreal saturation, bool emitColorChanged = true);
    void setLightness(qreal lightness, bool emitColorChanged = true);
    void setAlpha(qreal alpha, bool emitColorChanged = true);

    qreal hue;
    qreal saturation;
    qreal lightness;
    qreal alpha;
    bool pressed;
    QPointF pressPoint;
    QQuickItem *handle;
};

void SaturationLightnessPickerPrivate::setHandleZ()
{
    if (qFuzzyIsNull(handle->z())) {
        if (contentItem)
            handle->setZ(contentItem->z() + 1);
        else if (background)
            handle->setZ(background->z() + 1);
    }
}

qreal SaturationLightnessPickerPrivate::saturationAt(const QPointF &pos) const
{
    if (pos.y() < 0)
        return 1.0;

    if (pos.y() >= height)
        return 0.0;

    return 1.0 - pos.y() / height;
}

qreal SaturationLightnessPickerPrivate::lightnessAt(const QPointF &pos) const
{
    if (pos.x() < 0)
        return 0.0;

    if (pos.x() >= width)
        return 1.0;

    return pos.x() / width;
}

void SaturationLightnessPickerPrivate::setHue(qreal hue, bool emitColorChanged)
{
    hue = qBound(0.0, hue, 360.0);

    if (qFuzzyCompare(this->hue, hue))
        return;

    this->hue = hue;

    Q_Q(SaturationLightnessPicker);
    emit q->hueChanged();
    if (emitColorChanged)
        emit q->colorChanged();
}

void SaturationLightnessPickerPrivate::setSaturation(qreal saturation, bool emitColorChanged)
{
    saturation = qBound(0.0, saturation, 1.0);

    if (qFuzzyCompare(this->saturation, saturation))
        return;

    this->saturation = saturation;

    if (handle)
        handle->setY((1.0 - this->saturation) * height - handle->height() / 2);

    Q_Q(SaturationLightnessPicker);
    emit q->saturationChanged();
    if (emitColorChanged)
        emit q->colorChanged();
}

void SaturationLightnessPickerPrivate::setLightness(qreal lightness, bool emitColorChanged)
{
    lightness = qBound(0.0, lightness, 1.0);

    if (qFuzzyCompare(this->lightness, lightness))
        return;

    this->lightness = lightness;

    if (handle)
        handle->setX(this->lightness * width - handle->width() / 2);

    Q_Q(SaturationLightnessPicker);
    emit q->lightnessChanged();
    if (emitColorChanged)
        emit q->colorChanged();
}

void SaturationLightnessPickerPrivate::setAlpha(qreal alpha, bool emitColorChanged)
{
    alpha = qBound(0.0, alpha, 1.0);
    if (this->alpha == alpha)
        return;

    Q_Q(SaturationLightnessPicker);
    this->alpha = alpha;
    emit q->alphaChanged();
    if (emitColorChanged)
        emit q->colorChanged();
}

SaturationLightnessPicker::SaturationLightnessPicker(QQuickItem *parent) :
    QQuickControl(*(new SaturationLightnessPickerPrivate), parent)
{
    setActiveFocusOnTab(true);
    setFocusPolicy(Qt::StrongFocus);
    setAcceptedMouseButtons(Qt::LeftButton);
}

/*!
    \qmlproperty real PickAWinner::SaturationLightnessPicker::hue

    This property controls the hue of the colors that are displayed on the
    picker. It is typically bound to a hue slider:

    TODO: un-uglify slider

    \qml
    import QtQuick 2.6
    import QtQuick.Layouts 1.0
    import QtQuick.Controls 2.0

    ApplicationWindow {
        width: layout.implicitWidth + 40
        height: layout.implicitHeight + 40
        visible: true

        RowLayout {
            id: layout
            spacing: 10
            anchors.centerIn: parent

            Slider {
                id: hueSlider
                from: 1
                to: 0
                orientation: Qt.Vertical
                background: Rectangle {
                    implicitWidth: 40
                    implicitHeight: 200

                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#ff0000"
                        }
                        GradientStop {
                            position: 0.166666
                            color: "#ffff00"
                        }
                        GradientStop {
                            position: 0.333333
                            color: "#00ff00"
                        }
                        GradientStop {
                            position: 0.5
                            color: "#00ffff"
                        }
                        GradientStop {
                            position: 0.666666
                            color: "#0000ff"
                        }
                        GradientStop {
                            position: 0.833333
                            color: "#ff00ff"
                        }
                        GradientStop {
                            position: 1
                            color: "#ff0000"
                        }
                    }
                }
            }
            SaturationLightnessPicker {
                id: picker
                hue: hueSlider.value
            }
        }
    }
    \endqml

    The default value is \c 0.0.

    \sa saturation, lightness
*/
qreal SaturationLightnessPicker::hue() const
{
    Q_D(const SaturationLightnessPicker);
    return d->hue;
}

void SaturationLightnessPicker::setHue(qreal hue)
{
    Q_D(SaturationLightnessPicker);
    d->setHue(hue);
}

/*!
    \qmlproperty real PickAWinner::SaturationLightnessPicker::saturation

    This property holds the saturation.

    The default value is \c 0.0.

    \sa lightness, hue
*/
qreal SaturationLightnessPicker::saturation() const
{
    Q_D(const SaturationLightnessPicker);
    return d->saturation;
}

void SaturationLightnessPicker::setSaturation(qreal saturation)
{
    Q_D(SaturationLightnessPicker);
    d->setSaturation(saturation);
}

/*!
    \qmlproperty real PickAWinner::SaturationLightnessPicker::lightness

    This property holds the lightness.

    The default value is \c 0.0.

    \sa saturation, hue
*/
qreal SaturationLightnessPicker::lightness() const
{
    Q_D(const SaturationLightnessPicker);
    return d->lightness;
}

void SaturationLightnessPicker::setLightness(qreal lightness)
{
    Q_D(SaturationLightnessPicker);
    d->setLightness(lightness);
}

/*!
    \qmlproperty real PickAWinner::SaturationLightnessPicker::alpha

    This property holds the alpha.

    The default value is \c 0.0.

    \sa hue, saturation, lightness
*/
qreal SaturationLightnessPicker::alpha() const
{
    Q_D(const SaturationLightnessPicker);
    return d->alpha;
}

void SaturationLightnessPicker::setAlpha(qreal alpha)
{
    Q_D(SaturationLightnessPicker);
    d->setAlpha(alpha);
}

QColor SaturationLightnessPicker::color() const
{
    Q_D(const SaturationLightnessPicker);
    return QColor::fromHslF(d->hue, d->saturation, d->lightness, d->alpha);
}

void SaturationLightnessPicker::setColor(const QColor &color)
{
    Q_D(SaturationLightnessPicker);
    if (this->color() == color)
        return;

    QColor hsl = color.toHsl();
    d->setHue(hsl.hslHueF(), false);
    d->setSaturation(hsl.hslSaturationF(), false);
    d->setLightness(hsl.lightnessF(), false);
    d->setAlpha(color.alphaF(), false);
    emit colorChanged();
}

/*!
    \qmlproperty bool PickAWinner::SaturationLightnessPicker::pressed

    This property holds whether the slider is pressed.
*/
bool SaturationLightnessPicker::isPressed() const
{
    Q_D(const SaturationLightnessPicker);
    return d->pressed;
}

void SaturationLightnessPicker::setPressed(bool pressed)
{
    Q_D(SaturationLightnessPicker);
    if (d->pressed == pressed)
        return;

    d->pressed = pressed;
    setAccessibleProperty("pressed", pressed);
    emit pressedChanged();
}

/*!
    \qmlproperty Item PickAWinner::SaturationLightnessPicker::handle

    This property holds the handle item.

    \sa {Customizing Slider}
*/
QQuickItem *SaturationLightnessPicker::handle() const
{
    Q_D(const SaturationLightnessPicker);
    return d->handle;
}

void SaturationLightnessPicker::setHandle(QQuickItem *handle)
{
    Q_D(SaturationLightnessPicker);
    if (d->handle == handle)
        return;

    delete d->handle;
    d->handle = handle;

    if (d->handle) {
        if (!d->handle->parentItem())
            d->handle->setParentItem(this);

        d->setHandleZ();
    }

    emit handleChanged();
}

void SaturationLightnessPicker::increaseSaturation()
{
    Q_D(SaturationLightnessPicker);
    setSaturation(d->saturation + 0.05);
}

void SaturationLightnessPicker::decreaseSaturation()
{
    Q_D(SaturationLightnessPicker);
    setSaturation(d->saturation - 0.05);
}

void SaturationLightnessPicker::increaseLightness()
{
    Q_D(SaturationLightnessPicker);
    setLightness(d->lightness + 0.05);
}

void SaturationLightnessPicker::decreaseLightness()
{
    Q_D(SaturationLightnessPicker);
    setLightness(d->lightness - 0.05);
}

void SaturationLightnessPicker::componentComplete()
{
    Q_D(SaturationLightnessPicker);
    QQuickControl::componentComplete();
    d->setHandleZ();
}

void SaturationLightnessPicker::keyPressEvent(QKeyEvent *event)
{
    QQuickControl::keyPressEvent(event);
    switch (event->key()) {
    case Qt::Key_Left:
        setPressed(true);
        if (isMirrored())
            increaseLightness();
        else
            decreaseLightness();
        event->accept();
        break;
    case Qt::Key_Right:
        setPressed(true);
        if (isMirrored())
            decreaseLightness();
        else
            increaseLightness();
        event->accept();
        break;
    case Qt::Key_Up:
        setPressed(true);
        increaseSaturation();
        event->accept();
        break;
    case Qt::Key_Down:
        setPressed(true);
        decreaseSaturation();
        event->accept();
        break;
    }
}

void SaturationLightnessPicker::keyReleaseEvent(QKeyEvent *event)
{
    QQuickControl::keyReleaseEvent(event);
    setPressed(false);
}

void SaturationLightnessPicker::mousePressEvent(QMouseEvent *event)
{
    Q_D(SaturationLightnessPicker);
    QQuickControl::mousePressEvent(event);
    d->pressPoint = event->pos();
    setPressed(true);
    setSaturation(d->saturationAt(event->pos()));
    setLightness(d->lightnessAt(event->pos()));
}

void SaturationLightnessPicker::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(SaturationLightnessPicker);
    QQuickControl::mouseMoveEvent(event);
    setSaturation(d->saturationAt(event->pos()));
    setLightness(d->lightnessAt(event->pos()));
}

void SaturationLightnessPicker::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(SaturationLightnessPicker);
    QQuickControl::mouseReleaseEvent(event);
    d->pressPoint = QPoint();
    setSaturation(d->saturationAt(event->pos()));
    setLightness(d->lightnessAt(event->pos()));
}

void SaturationLightnessPicker::mouseUngrabEvent()
{
    Q_D(SaturationLightnessPicker);
    QQuickControl::mouseUngrabEvent();
    d->pressPoint = QPoint();
    setPressed(false);
}

QT_END_NAMESPACE
