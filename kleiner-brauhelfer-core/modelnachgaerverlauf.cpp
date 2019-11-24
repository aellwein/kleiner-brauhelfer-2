#include "modelnachgaerverlauf.h"
#include "brauhelfer.h"

ModelNachgaerverlauf::ModelNachgaerverlauf(Brauhelfer* bh, QSqlDatabase db) :
    SqlTableModel(bh, db),
    bh(bh)
{
}

QVariant ModelNachgaerverlauf::dataExt(const QModelIndex &idx) const
{
    switch(idx.column())
    {
    case ColZeitstempel:
    {
        return QDateTime::fromString(QSqlTableModel::data(idx).toString(), Qt::ISODate);
    }
    default:
        return QVariant();
    }
}

bool ModelNachgaerverlauf::setDataExt(const QModelIndex &idx, const QVariant &value)
{
    switch(idx.column())
    {
    case ColZeitstempel:
    {
        return QSqlTableModel::setData(idx, value.toDateTime().toString(Qt::ISODate));
    }
    case ColDruck:
    case ColTemp:
    {
        if (QSqlTableModel::setData(idx, value))
        {
            double p = data(idx.row(), ColDruck).toDouble();
            double T = data(idx.row(), ColTemp).toDouble();
            double co2 = BierCalc::co2(p, T);
            QSqlTableModel::setData(index(idx.row(), ColCO2), co2);
            return true;
        }
        return false;
    }
    default:
        return false;
    }
}

int ModelNachgaerverlauf::getLastRow(const QVariant &sudId) const
{
    int row = -1;
    QDateTime lastDt;
    for (int r = 0; r < rowCount(); ++r)
    {
        if (data(r, ColSudID).toInt() == sudId)
        {
            QDateTime dt = data(r, ColZeitstempel).toDateTime();
            if (!lastDt.isValid() || dt > lastDt)
            {
                lastDt = dt;
                row = r;
            }
        }
    }
    return row;
}

QDateTime ModelNachgaerverlauf::getLastDateTime(const QVariant &sudId) const
{
    QDateTime lastDt;
    for (int r = 0; r < rowCount(); ++r)
    {
        if (data(r, ColSudID) == sudId)
        {
            QDateTime dt = data(r, ColZeitstempel).toDateTime();
            if (!lastDt.isValid() || dt > lastDt)
                lastDt = dt;
        }
    }
    return lastDt;
}

double ModelNachgaerverlauf::getLastCO2(const QVariant &sudId) const
{
    double co2 = 0.0;
    QDateTime lastDt;
    for (int r = 0; r < rowCount(); ++r)
    {
        if (data(r, ColSudID) == sudId)
        {
            QDateTime dt = data(r, ColZeitstempel).toDateTime();
            if (!lastDt.isValid() || dt > lastDt)
            {
                lastDt = dt;
                co2 = data(r, ColCO2).toDouble();
            }
        }
    }
    return co2;
}

void ModelNachgaerverlauf::defaultValues(QVariantMap &values) const
{
    if (!values.contains("Zeitstempel"))
        values.insert("Zeitstempel", QDateTime::currentDateTime());
    if (values.contains("SudID"))
    {
        int id = values.value("SudID", -1).toInt();
        int row = getLastRow(id);
        if (row >= 0)
        {
            if (!values.contains("Druck"))
                values.insert("Druck", data(row, ColDruck));
            if (!values.contains("Temp"))
                values.insert("Temp", data(row, ColTemp));
        }
    }
    if (!values.contains("Druck"))
        values.insert("Druck", 0.0);
    if (!values.contains("Temp"))
        values.insert("Temp", 20.0);
}
