#ifndef REPLACEMENTROW_H
#define REPLACEMENTROW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QString>

/**
 * ReplacementRowWidget represents a single row in the replacement rules list.
 * It contains two text input fields (before/after) and a delete button.
 */
class ReplacementRowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReplacementRowWidget(QWidget *parent = nullptr);

    // Getters for the replacement rule
    QString getBeforeText() const;
    QString getAfterText() const;
    
    // Setters for the replacement rule
    void setBeforeText(const QString& text);
    void setAfterText(const QString& text);
    
    // Check if the rule is valid (has non-empty before text)
    bool isValid() const;
    
    // Clear both input fields
    void clear();
    
    // Focus on the "before" input field
    void focusBeforeInput();

    void updateTexts();

signals:
    // Emitted when the delete button is clicked
    void deleteRequested();
    
    // Emitted when the content of either input field changes
    void contentChanged();

private slots:
    void onDeleteClicked();
    void onTextChanged();

private:
    void setupUI();
    void setupConnections();
    
    // UI components
    QHBoxLayout *m_layout;
    QPushButton *m_deleteButton;
    QLineEdit *m_beforeInput;
    QLabel *m_arrowLabel;
    QLineEdit *m_afterInput;
};

#endif // REPLACEMENTROW_H