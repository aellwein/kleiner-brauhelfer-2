#ifndef LINKLABELDELEGATE_H
#define LINKLABELDELEGATE_H

#include <QStyledItemDelegate>

class LinkLabelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LinkLabelDelegate(QObject *parent = nullptr);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // LINKLABELDELEGATE_H
