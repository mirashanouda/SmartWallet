#ifndef USERRECORD_H
#define USERRECORD_H

#include <QObject>

class UserRecord : public QObject
{
    Q_OBJECT
public:
    explicit UserRecord(QObject *parent = nullptr);

signals:

};

#endif // USERRECORD_H
