#ifndef QPOWERSHELL_H
#define QPOWERSHELL_H

#include <QObject>
#include <QList>
#include <QProcess>
#include "QPowerShellPipeline.h"

class QPowerShell : public QObject
{
    Q_OBJECT
public:
    explicit QPowerShell(QObject *parent = 0);
    ~QPowerShell();

    QPowerShellPipeline *newPipe();

    void closeAllPipes();

    quint8 openPipes() const;
    quint8 availablePipes() const;

    QPowerShellPipeline *nextAvailablePipe() const;
    void closePipe(QPowerShellPipeline *pipe);

    quint8 maxPipes() const;
    void setMaxPipes(const quint8 &maxPipes);

private:
    QList<QPowerShellPipeline *> m_pipePool;
    quint8 m_maxPipes;

signals:
    void pipeFinished(QPowerShellPipeline *pipe, bool success, QPowerShellPipeline::QPSError error, const QByteArray &result);

public slots:

};

#endif // QPOWERSHELL_H
