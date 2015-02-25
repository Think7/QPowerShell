#include "QPowerShellCommand.h"

QPowerShellCommand::QPowerShellCommand(QObject *parent) : QObject(parent),
    m_command(QString("")),
    m_params(QMap<QString, QString>()),
    m_finalPSCommand(QString())
{
    //
}

QPowerShellCommand::~QPowerShellCommand()
{

}

QPowerShellCommand &QPowerShellCommand::addParameter(const QString name, const QString &value)
{
    this->m_params.insert(name, value);
    return *this;
}

QString QPowerShellCommand::command() const
{
    return m_command;
}

void QPowerShellCommand::setCommand(const QString &command)
{
    m_command = command;
}

QString QPowerShellCommand::rawCommand() const
{
    QString cmd = QString();
    cmd.append(this->m_command).append(' ');

    QMap<QString, QString>::const_iterator i;
    for (i = this->m_params.begin(); i != this->m_params.end(); i++)
    {
        cmd.append(i.key()).append(' ');
        cmd.append(i.value()).append(' ');
    }

    return cmd.trimmed();
}



