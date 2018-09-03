#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QFile>
#include <QThread>

/**
 * @brief Writes in a file asynchronously
 *
 * The FileWriter class writes data in a file from another thread,
 * allowing the UI to stay responsive during the operation.
 * Call start() to launch the writing.
 * If the file doesn't exist, it is automatically created.
 */
class FileWriter : public QThread
{
    Q_OBJECT
public:
    FileWriter(const QString &fileName, const QByteArray& data, QObject* parent = 0);

    // QThread interface
protected:
    virtual void run() Q_DECL_OVERRIDE;

signals:
    void resultReady(bool success, const QString& errorStr);

private:
    QFile m_file;
    QByteArray m_data;
};

#endif // FILEWRITER_H
