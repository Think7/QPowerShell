#ifndef QPOWERSHELLPIPELINE_H
#define QPOWERSHELLPIPELINE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QProcess>
#include <QDir>
#include <QRegExp>
#include <QDebug>

#include "QPowerShellCommand.h"

class QPowerShellPipeline : public QObject
{
    Q_OBJECT
public:

    typedef enum {
        QPowerShellErrorSrcUnknown,
        QPowerShellErrorSrcProcess,
        QPowerShellErrorSrcCommand
    } QPowerShellErrorSrc;

    struct QPSError
    {
        QPowerShellErrorSrc errorSource;
        QString errorMessage;
        QString rawErrorMessage;

        QPSError() :
            errorSource(QPowerShellErrorSrcUnknown),
            errorMessage(""),
            rawErrorMessage("")
        {
        }
    };

    explicit QPowerShellPipeline(QObject *parent = 0);
    ~QPowerShellPipeline();

    bool isAvailable() const;

    QPowerShellPipeline &addCommand(const QString &command);
    QPowerShellPipeline &addParam(const QString &param, const QString &value);
    QPowerShellPipeline &addStatement(const QString &statement);
    void invoke();

    QString description() const;
    void setDescription(const QString &description);

private:
    QString m_finalCommand;
    QPowerShellCommand *m_currentCommand;
    QList<QPowerShellCommand *> m_commands;
    QList<QString> m_statements;
    QProcess *m_psProcess;
    QByteArray m_readBufferStdOut;
    QByteArray m_readBufferError;
    bool m_isAvailable;
    QString m_description;
    bool m_hadPSError;

    void buildFinalCommand();
    void reset();
    qint64 writeToPSProcess(const QByteArray &data);

signals:
    void commandFinished(QPowerShellPipeline *pipe, bool success, QPowerShellPipeline::QPSError error, const QByteArray &result);

public slots:
    //

private slots:
    void readyReadStdOut();
    void readyReadStdError();
    void psProcessStateChanged(QProcess::ProcessState newState);
    void psProcessError(QProcess::ProcessError error);
};

#endif // QPOWERSHELLPIPELINE_H
