#include "QPowerShellPipeline.h"

const char *eosToken = "-###-DONE-###-";

QPowerShellPipeline::QPowerShellPipeline(QObject *parent) : QObject(parent),
    m_finalCommand(QString()),
    m_currentCommand(NULL),
    m_commands(QList<QPowerShellCommand *>()),
    m_statements(QList<QString>()),
    m_psProcess(NULL),
    m_readBufferStdOut(QByteArray()),
    m_readBufferError(QByteArray()),
    m_isAvailable(false),
    m_description(QString()),
    m_hadPSError(false)
{
    this->m_psProcess = new QProcess(this);

    QObject::connect(this->m_psProcess, &QProcess::readyReadStandardOutput, this, &QPowerShellPipeline::readyReadStdOut);
    QObject::connect(this->m_psProcess, &QProcess::readyReadStandardError, this, &QPowerShellPipeline::readyReadStdError);
    QObject::connect(this->m_psProcess, (void (QProcess::*)(QProcess::ProcessError))&QProcess::error, this, &QPowerShellPipeline::psProcessError);
    QObject::connect(this->m_psProcess, &QProcess::stateChanged, this, &QPowerShellPipeline::psProcessStateChanged);

    QStringList args = QStringList();
    args.append("-NoExit");
    args.append("-InputFormat");
    args.append("Text");
    args.append("-Command");
    args.append("-");

    QString psPath = QString();
    psPath.append(qgetenv("WINDIR"));
    psPath.append("\\system32\\WindowsPowerShell\\v1.0\\powershell.exe");
    psPath = QDir::fromNativeSeparators(psPath);

    this->m_psProcess->setArguments(args);
    this->m_psProcess->setProgram(psPath);
    this->m_psProcess->start(QProcess::ReadWrite);
}

QPowerShellPipeline::~QPowerShellPipeline()
{
    if (this->m_psProcess->isOpen())
    {
        // Prevent firing of the signal pipe is destroyed.
        QObject::disconnect(this->m_psProcess, (void (QProcess::*)(QProcess::ProcessError))&QProcess::error, this, &QPowerShellPipeline::psProcessError);
        QObject::disconnect(this->m_psProcess, &QProcess::stateChanged, this, &QPowerShellPipeline::psProcessStateChanged);
        this->m_psProcess->close();
    }
}

QPowerShellPipeline &QPowerShellPipeline::addCommand(const QString &command)
{
    if (!this->m_currentCommand)
    {
        this->m_currentCommand = new QPowerShellCommand(this);
        this->m_currentCommand->setCommand(command);
    }
    else
    {
        this->m_finalCommand.append(this->m_currentCommand->rawCommand()).append(" | ");
        this->m_currentCommand = new QPowerShellCommand(this);
        this->m_currentCommand->setCommand(command);
    }

    return *this;
}

QPowerShellPipeline &QPowerShellPipeline::addParam(const QString &param, const QString &value)
{
    this->m_currentCommand->addParameter(param, value);
    return *this;
}

QPowerShellPipeline &QPowerShellPipeline::addStatement(const QString &statement)
{
    this->m_statements.append(statement);
    return *this;
}

void QPowerShellPipeline::invoke()
{
    this->buildFinalCommand();
    QString lfc = QString();
    lfc.append(this->m_finalCommand).append(" ;ECHO ").append(eosToken).append('\n');

    this->writeToPSProcess(lfc.toUtf8());
    this->m_currentCommand = NULL;
    this->reset();
}

void QPowerShellPipeline::buildFinalCommand()
{
    if (this->m_currentCommand)
    {
        this->m_finalCommand.append(this->m_currentCommand->rawCommand());
    }

    QList<QString>::iterator i;
    for (i = this->m_statements.begin(); i != this->m_statements.end(); i++)
    {
        this->m_finalCommand.append(QString(" ; %1").arg(*i));
    }
}

void QPowerShellPipeline::reset()
{
    this->m_finalCommand.clear();
    this->m_readBufferStdOut.clear();
    this->m_readBufferError.clear();
    this->m_statements.clear();
}

qint64 QPowerShellPipeline::writeToPSProcess(const QByteArray &data)
{
    this->m_isAvailable = false;
    return this->m_psProcess->write(data);
}

void QPowerShellPipeline::readyReadStdOut()
{
    this->m_psProcess->setReadChannel(QProcess::StandardOutput);

    if (this->m_psProcess->bytesAvailable() > 0)
    {
        if (!this->m_psProcess->canReadLine())
        {
            // Got no usefull lines.
            return;
        }

        QByteArray line = this->m_psProcess->readLine().trimmed();

        if (!line.contains(eosToken))
        {
            this->m_readBufferStdOut.append(line).append("\r\n");
        }
        else
        {
            emit this->commandFinished(this, true, QPSError(), this->m_readBufferStdOut.trimmed());
            this->reset();
            this->m_isAvailable = true;
        }
    }
}

void QPowerShellPipeline::readyReadStdError()
{
    this->m_psProcess->setReadChannel(QProcess::StandardError);
    this->m_hadPSError = true;

    if (this->m_psProcess->bytesAvailable() > 0)
    {
        if (!this->m_psProcess->canReadLine())
        {
            // Got no usefull lines.
            return;
        }

        QByteArray line = this->m_psProcess->readLine();
        if ((line != " \r\n"))
        {
            this->m_readBufferError.append(line);
        }
        else
        {
            QPSError error = QPSError();
            error.errorSource = QPowerShellErrorSrcCommand;
            error.errorMessage = "There was a problem with the command. Check rawErrorMessage";
            error.rawErrorMessage = this->m_readBufferError.simplified().trimmed();
            emit this->commandFinished(this, false, error, QByteArray());
            this->reset();
            this->m_isAvailable = true;
        }
    }
}

void QPowerShellPipeline::psProcessStateChanged(QProcess::ProcessState newState)
{
    if (newState != QProcess::Running)
    {
        this->m_isAvailable = false;
        return;
    }
    this->m_isAvailable = true;
}

void QPowerShellPipeline::psProcessError(QProcess::ProcessError error)
{
    this->m_isAvailable = false;

    QPSError e = QPSError();
    e.errorSource = QPowerShellErrorSrcProcess;
    e.errorMessage = QString("QProcess Error: %1").arg(QString::number(error));
    emit this->commandFinished(this, false, e, QByteArray());
    this->reset();
}

bool QPowerShellPipeline::isAvailable() const
{
    return m_isAvailable;
}

QString QPowerShellPipeline::description() const
{
    return m_description;
}

void QPowerShellPipeline::setDescription(const QString &description)
{
    m_description = description;
}

