#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QScreen>
#include <sstream>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_fileFrame(nullptr)
    , m_fileLayout(nullptr)
    , m_fileLabel(nullptr)
    , m_filePathEdit(nullptr)
    , m_browseButton(nullptr)
    , m_cautionLabel(nullptr)
    , m_rulesFrame(nullptr)
    , m_rulesLayout(nullptr)
    , m_rulesLabel(nullptr)
    , m_scrollArea(nullptr)
    , m_scrollWidget(nullptr)
    , m_scrollLayout(nullptr)
    , m_controlFrame(nullptr)
    , m_controlLayout(nullptr)
    , m_addRowButton(nullptr)
    , m_executeButton(nullptr)
{
    setupUI();
    setupConnections();
    setupMenuBar();
    setupStatusBar();
    
    // Add initial replacement row
    addReplacementRow();
    
    // Set window properties
    setWindowTitle("置き換え君 Qt版 v1.0");
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Center window on screen
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move((screenGeometry.width() - WINDOW_WIDTH) / 2, 
         (screenGeometry.height() - WINDOW_HEIGHT) / 2);
}

MainWindow::~MainWindow()
{
    // Clean up replacement rows (though Qt's parent-child system should handle this)
    for (auto* row : m_replacementRows) {
        if (row) {
            row->deleteLater();
        }
    }
    m_replacementRows.clear();
}

void MainWindow::setupUI()
{
    // Create central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // Main layout
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);
    m_mainLayout->setSpacing(10);
    
    // File selection section
    m_fileFrame = new QFrame(m_centralWidget);
    m_fileFrame->setFrameStyle(QFrame::StyledPanel);
    m_fileFrame->setStyleSheet(
        "QFrame {"
        "    background-color: white;"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
    );
    
    m_fileLayout = new QHBoxLayout(m_fileFrame);
    m_fileLayout->setContentsMargins(10, 10, 10, 10);
    m_fileLayout->setSpacing(10);
    
    m_fileLabel = new QLabel(Translations::tr("path_label"), m_fileFrame);
    m_fileLabel->setMinimumWidth(50);
    
    m_filePathEdit = new QLineEdit(m_fileFrame);
    m_filePathEdit->setPlaceholderText("ファイルパスを入力するか、参照ボタンでファイルを選択してください");
    m_filePathEdit->setStyleSheet(
        "QLineEdit {"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "    font-size: 12px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #3498db;"
        "}"
    );
    
    m_browseButton = new QPushButton(Translations::tr("browse"), m_fileFrame);
    m_browseButton->setMinimumSize(80, 30);
    m_browseButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #21618c;"
        "}"
    );
    
    m_fileLayout->addWidget(m_fileLabel);
    m_fileLayout->addWidget(m_filePathEdit, 1);
    m_fileLayout->addWidget(m_browseButton);
    m_langCombo = new QComboBox(m_fileFrame);
    m_langCombo->addItem("ja", QVariant::fromValue(static_cast<int>(Language::JA)));
    m_langCombo->addItem("en", QVariant::fromValue(static_cast<int>(Language::EN)));
    m_fileLayout->addWidget(m_langCombo);
    
    // Caution label
    m_cautionLabel = new QLabel(Translations::tr("caution"), m_centralWidget);
    m_cautionLabel->setStyleSheet(
        "QLabel {"
        "    color: #e74c3c;"
        "    font-weight: bold;"
        "    background-color: #ffeaa7;"
        "    padding: 8px;"
        "    border-radius: 4px;"
        "    border-left: 4px solid #e74c3c;"
        "}"
    );
    m_cautionLabel->setWordWrap(true);
    
    // Replacement rules section
    m_rulesFrame = new QFrame(m_centralWidget);
    m_rulesFrame->setFrameStyle(QFrame::StyledPanel);
    m_rulesFrame->setStyleSheet(
        "QFrame {"
        "    background-color: white;"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
    );
    
    m_rulesLayout = new QVBoxLayout(m_rulesFrame);
    m_rulesLayout->setContentsMargins(10, 10, 10, 10);
    m_rulesLayout->setSpacing(10);
    
    m_rulesLabel = new QLabel("置換ルール:", m_rulesFrame);
    m_rulesLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
    );
    
    // Scrollable area for replacement rows
    m_scrollArea = new QScrollArea(m_rulesFrame);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setMinimumHeight(SCROLL_AREA_HEIGHT);
    m_scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: 1px solid #ecf0f1;"
        "    border-radius: 4px;"
        "    background-color: #fafafa;"
        "}"
    );
    
    m_scrollWidget = new QWidget();
    m_scrollLayout = new QVBoxLayout(m_scrollWidget);
    m_scrollLayout->setContentsMargins(5, 5, 5, 5);
    m_scrollLayout->setSpacing(5);
    m_scrollLayout->addStretch(); // Add stretch at the end to keep rows at the top
    
    m_scrollArea->setWidget(m_scrollWidget);
    
    m_rulesLayout->addWidget(m_rulesLabel);
    m_rulesLayout->addWidget(m_scrollArea, 1);
    
    // Control buttons section
    m_controlFrame = new QFrame(m_centralWidget);
    m_controlFrame->setFrameStyle(QFrame::StyledPanel);
    m_controlFrame->setStyleSheet(
        "QFrame {"
        "    background-color: white;"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 4px;"
        "    padding: 5px;"
        "}"
    );
    
    m_controlLayout = new QHBoxLayout(m_controlFrame);
    m_controlLayout->setContentsMargins(10, 10, 10, 10);
    m_controlLayout->setSpacing(10);
    
    m_addRowButton = new QPushButton(Translations::tr("add"), m_controlFrame);
    m_addRowButton->setMinimumSize(100, 35);
    m_addRowButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #21618c;"
        "}"
    );
    
    m_executeButton = new QPushButton(Translations::tr("execute"), m_controlFrame);
    m_executeButton->setMinimumSize(100, 35);
    m_executeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #27ae60;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover:enabled {"
        "    background-color: #229954;"
        "}"
        "QPushButton:pressed:enabled {"
        "    background-color: #1e8449;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #bdc3c7;"
        "}"
    );
    m_executeButton->setEnabled(false);
    
    m_controlLayout->addWidget(m_addRowButton);
    m_controlLayout->addStretch();
    m_controlLayout->addWidget(m_executeButton);
    
    // Add all frames to main layout
    m_mainLayout->addWidget(m_fileFrame);
    m_mainLayout->addWidget(m_cautionLabel);
    m_mainLayout->addWidget(m_rulesFrame, 1); // Give rules frame the most space
    m_mainLayout->addWidget(m_controlFrame);
}

void MainWindow::setupConnections()
{
    connect(m_browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseClicked);
    connect(m_addRowButton, &QPushButton::clicked, this, &MainWindow::onAddRowClicked);
    connect(m_executeButton, &QPushButton::clicked, this, &MainWindow::onExecuteClicked);
    connect(m_filePathEdit, &QLineEdit::textChanged, this, &MainWindow::updateExecuteButtonState);
    connect(m_langCombo, &QComboBox::currentIndexChanged, [this](int index){
        Language lang = static_cast<Language>(m_langCombo->currentData().toInt());
        Translations::load(lang);
        // Update texts
        setWindowTitle(Translations::tr("title"));
        m_fileLabel->setText(Translations::tr("path_label"));
        m_browseButton->setText(Translations::tr("browse"));
        m_cautionLabel->setText(Translations::tr("caution"));
        m_addRowButton->setText(Translations::tr("add"));
        m_executeButton->setText(Translations::tr("execute"));
        for (auto* row : m_replacementRows) {
            row->updateTexts();
        }
    });
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("ファイル(&F)");
    
    QAction *openAction = fileMenu->addAction("開く(&O)");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::onBrowseClicked);
    
    fileMenu->addSeparator();
    
    QAction *exitAction = fileMenu->addAction("終了(&X)");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Edit menu
    QMenu *editMenu = menuBar->addMenu("編集(&E)");
    
    QAction *addRowAction = editMenu->addAction("ルールを追加(&A)");
    addRowAction->setShortcut(QKeySequence("Ctrl+A"));
    connect(addRowAction, &QAction::triggered, this, &MainWindow::onAddRowClicked);
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("ヘルプ(&H)");
    
    QAction *aboutAction = helpMenu->addAction("このアプリについて(&A)");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "このアプリについて", 
                          "置き換え君 Qt版 v1.0\n\n"
                          "テキストファイルの一括置換ツール\n"
                          "Copyright (C) 2024");
    });
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage("ファイルを選択してください");
}

void MainWindow::addReplacementRow()
{
    ReplacementRowWidget *row = new ReplacementRowWidget(m_scrollWidget);
    
    // Connect signals
    connect(row, &ReplacementRowWidget::deleteRequested, this, &MainWindow::onDeleteRowRequested);
    connect(row, &ReplacementRowWidget::contentChanged, this, &MainWindow::onRowContentChanged);
    
    // Add to layout (before the stretch)
    m_scrollLayout->insertWidget(m_scrollLayout->count() - 1, row);
    
    // Add to our list
    m_replacementRows.append(row);
    row->updateTexts();
    
    // Focus on the new row
    row->focusBeforeInput();
    
    // Scroll to the bottom to show the new row
    QTimer::singleShot(50, [this]() {
        m_scrollArea->ensureWidgetVisible(m_replacementRows.last());
    });
}

void MainWindow::onBrowseClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "ファイルを選択",
        QString(),
        "テキストファイル (*.txt);;すべてのファイル (*.*)"
    );
    
    if (!fileName.isEmpty()) {
        m_filePathEdit->setText(fileName);
        loadFile(fileName);
    }
}

void MainWindow::onAddRowClicked()
{
    addReplacementRow();
}

void MainWindow::onDeleteRowRequested()
{
    ReplacementRowWidget *row = qobject_cast<ReplacementRowWidget*>(sender());
    if (!row) return;
    
    // Remove from layout
    m_scrollLayout->removeWidget(row);
    
    // Remove from our list
    m_replacementRows.removeAll(row);
    
    // Delete the widget
    row->deleteLater();
    
    // Ensure at least one row remains
    if (m_replacementRows.isEmpty()) {
        addReplacementRow();
    }
    
    updateExecuteButtonState();
}

void MainWindow::onExecuteClicked()
{
    if (m_currentFileContent.isEmpty()) {
        QMessageBox::warning(this, "エラー", "ファイルが読み込まれていません。");
        return;
    }
    
    // Collect replacements
    std::map<std::string, std::string> replacements = collectReplacements();
    
    if (replacements.empty()) {
        QMessageBox::warning(this, "エラー", "有効な置換ルールがありません。");
        return;
    }
    
    try {
        // Perform replacement
        std::string originalContent = m_currentFileContent.toStdString();
        std::string modifiedContent = multiReplace(originalContent, replacements);
        QString modifiedQString = QString::fromStdString(modifiedContent);
        
        // Show confirmation dialog
        bool confirmed = ConfirmationDialog::showConfirmation(this, m_currentFileContent, modifiedQString);
        
        if (confirmed) {
            // Save the file
            saveFile(m_currentFilePath, modifiedQString);
            QMessageBox::information(this, "完了", "置換が完了しました。");
            statusBar()->showMessage("置換が完了しました", 3000);
        }
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "エラー", QString("置換処理中にエラーが発生しました: %1").arg(e.what()));
    }
}

void MainWindow::onRowContentChanged()
{
    updateExecuteButtonState();
}

void MainWindow::updateExecuteButtonState()
{
    bool hasFile = !m_currentFileContent.isEmpty();
    bool hasValidRules = false;
    
    for (const auto* row : m_replacementRows) {
        if (row && row->isValid()) {
            hasValidRules = true;
            break;
        }
    }
    
    m_executeButton->setEnabled(hasFile && hasValidRules);
}

void MainWindow::loadFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "エラー", QString("ファイルを開けません: %1").arg(file.errorString()));
        return;
    }
    
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    
    m_currentFileContent = in.readAll();
    m_currentFilePath = filePath;
    
    file.close();
    
    statusBar()->showMessage(QString("ファイルを読み込みました: %1 (%2 文字)").arg(
        QFileInfo(filePath).fileName()).arg(m_currentFileContent.length()));
    
    updateExecuteButtonState();
}

void MainWindow::saveFile(const QString& filePath, const QString& content)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "エラー", QString("ファイルを保存できません: %1").arg(file.errorString()));
        return;
    }
    
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << content;
    
    file.close();
}

std::map<std::string, std::string> MainWindow::collectReplacements() const
{
    std::map<std::string, std::string> replacements;
    
    for (const auto* row : m_replacementRows) {
        if (row && row->isValid()) {
            QString beforeText = row->getBeforeText().trimmed();
            QString afterText = row->getAfterText();
            
            if (!beforeText.isEmpty()) {
                replacements[beforeText.toStdString()] = afterText.toStdString();
            }
        }
    }
    
    return replacements;
}

std::string MainWindow::multiReplace(const std::string& source, const std::map<std::string, std::string>& replacements) const
{
    if (source.empty() || replacements.empty()) {
        return source;
    }
    
    // Create a vector of replacement pairs sorted by pattern length (descending)
    std::vector<std::pair<std::string, std::string>> sortedReplacements;
    for (const auto& [find_str, replace_str] : replacements) {
        if (!find_str.empty()) {
            sortedReplacements.emplace_back(find_str, replace_str);
        }
    }
    
    // Sort by length (descending), so longer patterns are checked first
    std::sort(sortedReplacements.begin(), sortedReplacements.end(),
              [](const auto& a, const auto& b) {
                  return a.first.length() > b.first.length();
              });
    
    std::stringstream result;
    size_t pos = 0;
    
    while (pos < source.length()) {
        bool found = false;
        
        // Check patterns in order of length (longest first)
        for (const auto& [find_str, replace_str] : sortedReplacements) {
            if (pos + find_str.length() <= source.length()) {
                if (source.substr(pos, find_str.length()) == find_str) {
                    // Found a match, apply replacement
                    result << replace_str;
                    pos += find_str.length();
                    found = true;
                    break; // Stop checking other patterns
                }
            }
        }
        
        if (!found) {
            // No replacement found, copy the original character
            result << source[pos];
            pos++;
        }
    }
    
    return result.str();
}