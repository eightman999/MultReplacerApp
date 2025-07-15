#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QFrame>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QSplitter>
#include <QString>
#include <QList>
#include <QTimer>
#include <map>

#include "replacementrow.h"
#include "confirmationdialog.h"

/**
 * MainWindow is the main application window that manages the file selection,
 * replacement rules, and orchestrates the text replacement process.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBrowseClicked();
    void onAddRowClicked();
    void onDeleteRowRequested();
    void onExecuteClicked();
    void onRowContentChanged();

private:
    void setupUI();
    void setupConnections();
    void setupMenuBar();
    void setupStatusBar();
    void addReplacementRow();
    void updateExecuteButtonState();
    void loadFile(const QString& filePath);
    void saveFile(const QString& filePath, const QString& content);
    std::map<std::string, std::string> collectReplacements() const;
    std::string multiReplace(const std::string& source, const std::map<std::string, std::string>& replacements) const;
    
    // UI components - File selection section
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QFrame *m_fileFrame;
    QHBoxLayout *m_fileLayout;
    QLabel *m_fileLabel;
    QLineEdit *m_filePathEdit;
    QPushButton *m_browseButton;
    
    // Caution label
    QLabel *m_cautionLabel;
    
    // Replacement rules section
    QFrame *m_rulesFrame;
    QVBoxLayout *m_rulesLayout;
    QLabel *m_rulesLabel;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollWidget;
    QVBoxLayout *m_scrollLayout;
    
    // Control buttons
    QFrame *m_controlFrame;
    QHBoxLayout *m_controlLayout;
    QPushButton *m_addRowButton;
    QPushButton *m_executeButton;
    
    // Data
    QList<ReplacementRowWidget*> m_replacementRows;
    QString m_currentFilePath;
    QString m_currentFileContent;
    
    // Constants
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;
    static const int SCROLL_AREA_HEIGHT = 400;
};

#endif // MAINWINDOW_H