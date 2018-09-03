#include "FileWriter.h"

#include <QDebug>

FileWriter::FileWriter(const QString &fileName, const QByteArray &data, QObject *parent) :
    QThread(parent),
    m_file(fileName),
    m_data(data)
{
}

void FileWriter::run()
{
    if (!m_file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Failed to open file :" << m_file.fileName();
        emit resultReady(false, m_file.errorString());
        return;
    }

    qint64 bytesWritten = m_file.write(m_data);

    emit resultReady(bytesWritten != -1, m_file.errorString());
}
