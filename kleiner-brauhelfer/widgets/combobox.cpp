#include "combobox.h"
#include <QAbstractItemView>
#include "widgetdecorator.h"
#include "settings.h"

extern Settings *gSettings;

ComboBox::ComboBox(QWidget *parent) :
    QComboBox(parent),
    mError(false)
{
    setFocusPolicy(Qt::StrongFocus);
    connect(this, &QComboBox::currentIndexChanged, [this](){WidgetDecorator::valueChanged(this, hasFocus());});
    connect(this, &QComboBox::currentTextChanged, [this](){WidgetDecorator::valueChanged(this, hasFocus());});
}

void ComboBox::wheelEvent(QWheelEvent *event)
{
    if (hasFocus())
        QComboBox::wheelEvent(event);
}

void ComboBox::paintEvent(QPaintEvent *event)
{
    if (WidgetDecorator::contains(this))
        setPalette(gSettings->paletteChanged);
    else if (!isEnabled())
        setPalette(gSettings->palette);
    else if (mError)
        setPalette(gSettings->paletteErrorButton);
    else
        setPalette(gSettings->paletteInput);
    QComboBox::paintEvent(event);
}

void ComboBox::setError(bool e)
{
    if (mError != e)
    {
        mError = e;
        update();
    }
}

void ComboBox::setToolTip(const QString &str)
{
    QComboBox::setToolTip(str);
    QAbstractItemView* v = view();
    if (v)
        v->setToolTip(str);
}
