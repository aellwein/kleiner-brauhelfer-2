#ifndef WDGRAST_H
#define WDGRAST_H

#include "wdgabstractproxy.h"

namespace Ui {
class WdgRast;
}

class WdgRast : public WdgAbstractProxy
{
    Q_OBJECT

public:
    explicit WdgRast(int row, QLayout *parentLayout, QWidget *parent = nullptr);
    ~WdgRast();
    bool isEnabled() const;
    QString name() const;
    double prozentWasser() const;
    void setFehlProzentWasser(double value);

public slots:
    void updateListe();
    void updateValuesFromListe(int index);
    void updateValues();

private slots:
    void focusChanged(QWidget *old, QWidget *now);
    void cbRastTextEdited();
    void on_cbRast_currentTextChanged(const QString &text);
    void on_cbRast_currentIndexChanged(int index);
    void on_cbTyp_currentIndexChanged(int index);
    void on_tbMengeEinmaischen_valueChanged(double value);
    void on_tbVerhaeltnisEinmaischen_valueChanged(double value);
    void on_tbMalzTempEinmaischen_valueChanged(double value);
    void on_tbTempEinmaischen_valueChanged(double value);
    void on_tbDauerEinmaischen_valueChanged(int value);
    void on_tbTempRast_valueChanged(double value);
    void on_tbDauerRast_valueChanged(int value);
    void on_tbMengeInfusion_valueChanged(double value);
    void on_tbVerhaeltnisInfusion_valueChanged(double value);
    void on_tbTempInfusion_valueChanged(double value);
    void on_tbWassertempInfusion_valueChanged(double value);
    void on_tbDauerInfusion_valueChanged(int value);
    void on_tbMengeDekoktion_valueChanged(double value);
    void on_tbVerhaeltnisDekoktion_valueChanged(double value);
    void on_tbRasttempDekoktion_valueChanged(double value);
    void on_tbRastdauerDekoktion_valueChanged(int value);
    void on_tbKochdauerDekoktion_valueChanged(int value);
    void on_tbTeiltempDekoktion_valueChanged(double value);
    void on_tbTempDekoktion_valueChanged(double value);
    void on_tbDauerDekoktion_valueChanged(int value);
    void on_btnLoeschen_clicked();
    void on_btnNachUnten_clicked();
    void on_btnNachOben_clicked();

private:
    struct Rast
    {
        QString name;
        double temperatur;
        double dauer;
    };
    static const QList<Rast> rasten;

private:
    void checkEnabled();

private:
    Ui::WdgRast *ui;
    bool mEnabled;
    bool mRastNameManuallyEdited;
};

#endif // WDGRAST_H
