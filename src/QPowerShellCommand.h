#ifndef QPOWERSHELLCOMMAND_H
#define QPOWERSHELLCOMMAND_H

#include <QObject>
#include <QMap>
#include <QString>

#include <QDebug>

class QPowerShellCommand : public QObject
{
    Q_OBJECT
public:
    explicit QPowerShellCommand(QObject *parent = 0);
    ~QPowerShellCommand();

public:
    QString command() const;
    void setCommand(const QString &command);

    QPowerShellCommand &addParameter(const QString name, const QString &value);

    QString rawCommand() const;

private:
    QString m_command;
    QMap<QString, QString> m_params;
    QString m_finalPSCommand;
};

#endif // QPOWERSHELLCOMMAND_H
