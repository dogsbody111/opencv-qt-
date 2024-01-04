//#include "logviewer.h"
//#include <QHeaderView>
//#include <QLoggingCategory>
//#include <QDebug>

//LogViewer::LogViewer(QWidget *parent)
//    : QWidget(parent)
//{
//    setupUi();
//    updateLogTable();
//}

//LogViewer::~LogViewer()
//{
//    // Destructor
//}

//void LogViewer::setupUi()
//{
//    logTable = new QTableWidget(this);
//    logTable->setColumnCount(2);
//    logTable->setHorizontalHeaderLabels(QStringList() << "Category" << "Message");
//    logTable->horizontalHeader()->setStretchLastSection(true);

//    QVBoxLayout *layout = new QVBoxLayout(this);
//    layout->addWidget(logTable);
//    setLayout(layout);
//}

//void LogViewer::addLogEntry(const QString &category, const QString &message)
//{
//    int row = logTable->rowCount();
//    logTable->insertRow(row);
//    logTable->setItem(row, 0, new QTableWidgetItem(category));
//    logTable->setItem(row, 1, new QTableWidgetItem(message));
//}

//void LogViewer::updateLogTable()
//{
//    connect(qApp, &QCoreApplication::aboutToQuit, this, &LogViewer::close);

//    QLoggingCategory::setFilterRules(QStringLiteral("logviewer.debug=true"));

//    qCInfo(logviewer) << "LogViewer started.";

//    connect(qApp, &QCoreApplication::aboutToQuit, this, &LogViewer::close);
//    connect(qApp, &QCoreApplication::aboutToQuit, this, &LogViewer::close);

//    // Add some example log entries
//    qCDebug(logviewer) << "This is a debug message.";
//    qCInfo(logviewer) << "This is an info message.";
//    qCWarning(logviewer) << "This is a warning message.";
//    qCCritical(logviewer) << "This is a critical message.";

//    // Add entries to the log table
//    addLogEntry("Debug", "This is a debug message.");
//    addLogEntry("Info", "This is an info message.");
//    addLogEntry("Warning", "This is a warning message.");
//    addLogEntry("Critical", "This is a critical message.");
//}
