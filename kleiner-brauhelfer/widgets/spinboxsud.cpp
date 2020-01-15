#include "spinboxsud.h"
#include "brauhelfer.h"

extern Brauhelfer* bh;

SpinBoxSud::SpinBoxSud(QWidget *parent) :
    SpinBox(parent),
    mCol(-1)
{
}

void SpinBoxSud::setColumn(int col)
{
    mCol = col;
    if (mCol != -1)
        connect(this, SIGNAL(valueChanged(int)), SLOT(on_valueChanged(int)));
}

int SpinBoxSud::column() const
{
    return mCol;
}

void SpinBoxSud::updateValue()
{
    if (!hasFocus() || isReadOnly())
        setValue(bh->sud()->getValue(mCol).toInt());
}

void SpinBoxSud::on_valueChanged(int value)
{
    if (hasFocus() && !isReadOnly())
        bh->sud()->setValue(mCol, value);
}
