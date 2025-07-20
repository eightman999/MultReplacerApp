#include "confirmationdialog.h"
#include "translations.h"
#include <QApplication>
#include <QScreen>

ConfirmationDialog::ConfirmationDialog(QWidget *parent)
    : QDialog(parent)
    , m_mainLayout(nullptr)
    , m_titleLabel(nullptr)
    , m_instructionLabel(nullptr)
    , m_previewText(nullptr)
    , m_buttonLayout(nullptr)
    , m_cancelButton(nullptr)
    , m_executeButton(nullptr)
    , m_accepted(false)
{
    setupUI();
    setupConnections();
    resizeToOptimalSize();
}

void ConfirmationDialog::setupUI()
{
    setWindowTitle(Translations::tr("confirm"));
    setModal(true);
    
    // Main layout
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    m_mainLayout->setSpacing(15);
    
    // Title label
    m_titleLabel = new QLabel(Translations::tr("confirm"), this);
    m_titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "    margin-bottom: 10px;"
        "}"
    );
    
    // Instruction label
    m_instructionLabel = new QLabel(Translations::tr("confirm_message"), this);
    m_instructionLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 12px;"
        "    color: #7f8c8d;"
        "    margin-bottom: 10px;"
        "}"
    );
    m_instructionLabel->setWordWrap(true);
    
    // Preview text area
    m_previewText = new QPlainTextEdit(this);
    m_previewText->setReadOnly(true);
    m_previewText->setStyleSheet(
        "QPlainTextEdit {"
        "    background-color: #2c3e50;"
        "    color: #ecf0f1;"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 11px;"
        "    border: 1px solid #34495e;"
        "    border-radius: 4px;"
        "    padding: 10px;"
        "}"
    );
    
    // Button layout
    m_buttonLayout = new QHBoxLayout();
    m_buttonLayout->setSpacing(10);
    
    // Cancel button
    m_cancelButton = new QPushButton(Translations::tr("cancel"), this);
    m_cancelButton->setMinimumSize(100, 35);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #95a5a6;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #7f8c8d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #6c7b7d;"
        "}"
    );
    
    // Execute button
    m_executeButton = new QPushButton(Translations::tr("execute_save"), this);
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
        "QPushButton:hover {"
        "    background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e8449;"
        "}"
    );
    
    // Add buttons to button layout
    m_buttonLayout->addStretch();
    m_buttonLayout->addWidget(m_cancelButton);
    m_buttonLayout->addWidget(m_executeButton);
    
    // Add all widgets to main layout
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_instructionLabel);
    m_mainLayout->addWidget(m_previewText, 1); // stretch factor 1
    m_mainLayout->addLayout(m_buttonLayout);
    
    setLayout(m_mainLayout);
}

void ConfirmationDialog::setupConnections()
{
    connect(m_cancelButton, &QPushButton::clicked, this, &ConfirmationDialog::onCancelClicked);
    connect(m_executeButton, &QPushButton::clicked, this, &ConfirmationDialog::onExecuteClicked);
}

void ConfirmationDialog::resizeToOptimalSize()
{
    // Get screen geometry
    QScreen *screen = QApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    
    // Set size to 80% of screen size
    int width = static_cast<int>(screenGeometry.width() * 0.8);
    int height = static_cast<int>(screenGeometry.height() * 0.8);
    
    resize(width, height);
    
    // Center the dialog
    move((screenGeometry.width() - width) / 2, (screenGeometry.height() - height) / 2);
}

void ConfirmationDialog::setContent(const QString& originalText, const QString& modifiedText)
{
    m_originalText = originalText;
    m_modifiedText = modifiedText;
    m_previewText->setPlainText(modifiedText);
}

bool ConfirmationDialog::wasAccepted() const
{
    return m_accepted;
}

bool ConfirmationDialog::showConfirmation(QWidget *parent, const QString& originalText, const QString& modifiedText)
{
    ConfirmationDialog dialog(parent);
    dialog.setContent(originalText, modifiedText);
    dialog.exec();
    return dialog.wasAccepted();
}

void ConfirmationDialog::accept()
{
    m_accepted = true;
    QDialog::accept();
}

void ConfirmationDialog::reject()
{
    m_accepted = false;
    QDialog::reject();
}

void ConfirmationDialog::onExecuteClicked()
{
    accept();
}

void ConfirmationDialog::onCancelClicked()
{
    reject();
}