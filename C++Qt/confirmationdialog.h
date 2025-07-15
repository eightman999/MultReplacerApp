#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>
#include <QPushButton>

/**
 * ConfirmationDialog displays a preview of the modified text and allows
 * the user to confirm or cancel the replacement operation.
 */
class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmationDialog(QWidget *parent = nullptr);
    
    // Set the content to be displayed in the preview
    void setContent(const QString& originalText, const QString& modifiedText);
    
    // Get the user's choice (true for execute, false for cancel)
    bool wasAccepted() const;
    
    // Static convenience method to show the dialog and get result
    static bool showConfirmation(QWidget *parent, 
                                const QString& originalText, 
                                const QString& modifiedText);

public slots:
    void accept() override;
    void reject() override;

private slots:
    void onExecuteClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupConnections();
    void resizeToOptimalSize();
    
    // UI components
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_instructionLabel;
    QPlainTextEdit *m_previewText;
    QHBoxLayout *m_buttonLayout;
    QPushButton *m_cancelButton;
    QPushButton *m_executeButton;
    
    // State
    bool m_accepted;
    QString m_originalText;
    QString m_modifiedText;
};

#endif // CONFIRMATIONDIALOG_H