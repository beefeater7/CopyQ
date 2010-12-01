#ifndef CLIPBOARDSERVER_H
#define CLIPBOARDSERVER_H

#include "app.h"
#include <QLocalServer>
#include <QProcess>
#include "client_server.h"

class MainWindow;
class ClipboardItem;
class Arguments;

class ClipboardServer : public App
{
    Q_OBJECT

    enum Command {
        Cmd_Unknown = 0,
        Cmd_Toggle,
        Cmd_Exit,
        Cmd_Menu,
        Cmd_Action,
        Cmd_Add,
        Cmd_Write,
        Cmd_WriteNoUpdate,
        Cmd_Edit,
        Cmd_Select,
        Cmd_Remove,
        Cmd_Length,
        Cmd_List,
        Cmd_Read,
    };

public:
    explicit ClipboardServer(int &argc, char **argv);
    ~ClipboardServer();
    bool isListening() { return m_server->isListening(); }
    bool doCommand(Arguments &args, QByteArray &bytes);
    void sendMessage(QLocalSocket* client, const QByteArray &message, int exit_code = 0);
    void sendMessage(QLocalSocket* client, const QString &message, int exit_code = 0) {
        sendMessage( client, message.toLocal8Bit(), exit_code );
    }

    void startMonitoring();
    void stopMonitoring();
    bool isMonitoring() {
        return m_monitor && m_monitor->state() != QProcess::NotRunning;
    }

    static QString serverName() { return ::serverName("CopyQserver"); }
    static QString monitorServerName() { return ::serverName("CopyQmonitor_server"); }

private:
    QLocalServer *m_server;
    QLocalServer *m_monitorserver;
    QLocalSocket *m_socket;
    MainWindow* m_wnd;
    QMap<QString, Command> m_commands;
    QProcess *m_monitor;

public slots:
    void changeClipboard(const ClipboardItem *item);
    void loadSettings();

private slots:
    void newConnection();
    void newMonitorConnection();
    void readyRead();
    void monitorStateChanged( QProcess::ProcessState newState );
    void monitorStandardError();

};

#endif // CLIPBOARDSERVER_H
