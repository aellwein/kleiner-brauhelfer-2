#include "modelhefegaben.h"
#include "database_defs.h"
#include "brauhelfer.h"
#include <QDateTime>
#include <cmath>

ModelHefegaben::ModelHefegaben(Brauhelfer* bh, QSqlDatabase db) :
    SqlTableModel(bh, db),
    bh(bh)
{
    mVirtualField.append("ZugabeDatum");
    mVirtualField.append("Abfuellbereit");
    connect(bh->modelSud(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&, const QVector<int>&)),
            this, SLOT(onSudDataChanged(const QModelIndex&)));
}

QVariant ModelHefegaben::dataExt(const QModelIndex &idx) const
{
    switch(idx.column())
    {
    case ColZugabeDatum:
    {
        QDateTime braudatum = bh->modelSud()->dataSud(data(idx.row(), ColSudID), ModelSud::ColBraudatum).toDateTime();
        if (braudatum.isValid())
            return braudatum.addDays(data(idx.row(), ColZugabeNach).toInt());
        return QDateTime();
    }
    case ColAbfuellbereit:
    {
        return data(idx.row(), ColZugegeben).toBool();
    }
    default:
        return QVariant();
    }
}

bool ModelHefegaben::setDataExt(const QModelIndex &idx, const QVariant &value)
{
    switch(idx.column())
    {
    case ColZugabeDatum:
    {
        QDateTime braudatum = bh->modelSud()->dataSud(data(idx.row(), ColSudID), ModelSud::ColBraudatum).toDateTime();
        if (braudatum.isValid())
            return QSqlTableModel::setData(index(idx.row(), ColZugabeNach), braudatum.daysTo(value.toDateTime()));
        return false;
    }
    default:
        return false;
    }
}

void ModelHefegaben::onSudDataChanged(const QModelIndex &idx)
{
    if (idx.column() == ModelSud::ColStatus)
    {
        int status = idx.data().toInt();
        QVariant sudId = bh->modelSud()->data(idx.row(), ModelSud::ColID);
        mSignalModifiedBlocked = true;
        if (status == Sud_Status_Rezept)
        {
            for (int row = 0; row < rowCount(); ++row)
            {
                if (data(row, ColSudID) == sudId)
                    QSqlTableModel::setData(index(row, ColZugegeben), false);
            }
        }
        else if (status == Sud_Status_Gebraut)
        {
            for (int row = 0; row < rowCount(); ++row)
            {
                if (data(row, ColSudID) == sudId && data(row, ColZugabeNach).toInt() == 0)
                    QSqlTableModel::setData(index(row, ColZugegeben), true);
            }
        }
        mSignalModifiedBlocked = false;
    }
}

void ModelHefegaben::defaultValues(QVariantMap &values) const
{
    if (values.contains("SudID"))
    {
        QVariant sudId = values.value("SudID");
        if (!values.contains("ZugabeNach"))
        {
            if (bh->modelSud()->dataSud(sudId, ModelSud::ColStatus).toInt() != Sud_Status_Rezept)
            {
                QDateTime braudatum = bh->modelSud()->dataSud(sudId, ModelSud::ColBraudatum).toDateTime();
                if (braudatum.isValid())
                    values.insert("ZugabeNach", braudatum.daysTo(QDateTime::currentDateTime()));
            }
        }
    }
    if (!values.contains("Name"))
        values.insert("Name", bh->modelHefe()->data(0, ModelHefe::ColBeschreibung));
    if (!values.contains("Menge"))
        values.insert("Menge", 1);
    if (!values.contains("Zugegeben"))
        values.insert("Zugegeben", 0);
    if (!values.contains("ZugabeNach"))
        values.insert("ZugabeNach", 0);
}
