#include "QPowerShell.h"

QPowerShell::QPowerShell(QObject *parent) : QObject(parent),
    m_pipePool(QList<QPowerShellPipeline *>()),
    m_maxPipes(5)
{
    //
}

QPowerShell::~QPowerShell()
{
    //
}

QPowerShellPipeline *QPowerShell::newPipe()
{
    if (this->m_pipePool.length() + 1 <= this->maxPipes() )
    {
        QPowerShellPipeline *p = new QPowerShellPipeline(this);
        QObject::connect(p, &QPowerShellPipeline::commandFinished, this, &QPowerShell::pipeFinished);
        this->m_pipePool.append(p);
        return p;
    }

    qDebug() << "QPowerShell Warning: Attempting to exceeded maximum allowed number of pipes!";

    return NULL;
}

void QPowerShell::closeAllPipes()
{
    qDeleteAll(this->m_pipePool);
    m_pipePool.clear();
}

quint8 QPowerShell::openPipes() const
{
    return this->m_pipePool.length();
}

quint8 QPowerShell::availablePipes() const
{
    quint8 available = 0;

    QList<QPowerShellPipeline *>::const_iterator i;
    for (i = this->m_pipePool.begin(); i != this->m_pipePool.end(); i++)
    {
        QPowerShellPipeline *cp = *i;
        if (cp->isAvailable())
        {
            available++;
        }
    }

    return available;
}

QPowerShellPipeline *QPowerShell::nextAvailablePipe() const
{
    QList<QPowerShellPipeline *>::const_iterator i;
    for (i = this->m_pipePool.begin(); i != this->m_pipePool.end(); i++)
    {
        QPowerShellPipeline *cp = *i;
        if (cp->isAvailable())
        {
            return cp;
        }
    }

    return NULL;
}

void QPowerShell::closePipe(QPowerShellPipeline *pipe)
{
    this->m_pipePool.removeOne(pipe);
    delete pipe;
}

quint8 QPowerShell::maxPipes() const
{
    return m_maxPipes;
}

void QPowerShell::setMaxPipes(const quint8 &maxPipes)
{
    m_maxPipes = maxPipes;
}

