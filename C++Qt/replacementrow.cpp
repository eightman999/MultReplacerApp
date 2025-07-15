#include "replacementrow.h"

ReplacementRowWidget::ReplacementRowWidget(QWidget *parent)
    : QWidget(parent)
    , m_layout(nullptr)
    , m_deleteButton(nullptr)
    , m_beforeInput(nullptr)
    , m_arrowLabel(nullptr)
    , m_afterInput(nullptr)
{
    setupUI();
    setupConnections();
}

void ReplacementRowWidget::setupUI()
{
    // Create the horizontal layout
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(10);
    
    // Create delete button
    m_deleteButton = new QPushButton("削除", this);
    m_deleteButton->setFixedSize(60, 30);
    m_deleteButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #e74c3c;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #c0392b;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a93226;"
        "}"
    );
    
    // Create "before" input field
    m_beforeInput = new QLineEdit(this);
    m_beforeInput->setPlaceholderText("置換前のテキスト");
    m_beforeInput->setMinimumHeight(30);
    
    // Create arrow label
    m_arrowLabel = new QLabel("→", this);
    m_arrowLabel->setAlignment(Qt::AlignCenter);
    m_arrowLabel->setStyleSheet("font-weight: bold; font-size: 14px; color: #7f8c8d;");
    m_arrowLabel->setFixedWidth(20);
    
    // Create "after" input field
    m_afterInput = new QLineEdit(this);
    m_afterInput->setPlaceholderText("置換後のテキスト");
    m_afterInput->setMinimumHeight(30);
    
    // Add widgets to layout
    m_layout->addWidget(m_deleteButton);
    m_layout->addWidget(m_beforeInput, 1); // stretch factor 1
    m_layout->addWidget(m_arrowLabel);
    m_layout->addWidget(m_afterInput, 1); // stretch factor 1
    
    // Set the layout
    setLayout(m_layout);
    
    // Style the widget
    setStyleSheet(
        "ReplacementRowWidget {"
        "    background-color: white;"
        "    border: 1px solid #ecf0f1;"
        "    border-radius: 4px;"
        "    margin: 2px;"
        "}"
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
}

void ReplacementRowWidget::setupConnections()
{
    connect(m_deleteButton, &QPushButton::clicked, this, &ReplacementRowWidget::onDeleteClicked);
    connect(m_beforeInput, &QLineEdit::textChanged, this, &ReplacementRowWidget::onTextChanged);
    connect(m_afterInput, &QLineEdit::textChanged, this, &ReplacementRowWidget::onTextChanged);
}

QString ReplacementRowWidget::getBeforeText() const
{
    return m_beforeInput->text();
}

QString ReplacementRowWidget::getAfterText() const
{
    return m_afterInput->text();
}

void ReplacementRowWidget::setBeforeText(const QString& text)
{
    m_beforeInput->setText(text);
}

void ReplacementRowWidget::setAfterText(const QString& text)
{
    m_afterInput->setText(text);
}

bool ReplacementRowWidget::isValid() const
{
    return !m_beforeInput->text().trimmed().isEmpty();
}

void ReplacementRowWidget::clear()
{
    m_beforeInput->clear();
    m_afterInput->clear();
}

void ReplacementRowWidget::focusBeforeInput()
{
    m_beforeInput->setFocus();
}

void ReplacementRowWidget::onDeleteClicked()
{
    emit deleteRequested();
}

void ReplacementRowWidget::onTextChanged()
{
    emit contentChanged();
}