import javax.swing.*;
import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

public class ConfirmationDialog extends JDialog {
    private String newContent;
    private String filePath;

    public ConfirmationDialog(Frame owner, String newContent, String filePath) {
        super(owner, "確認", true);
        this.newContent = newContent;
        this.filePath = filePath;

        setSize(800, 600);
        setLocationRelativeTo(owner);
        setLayout(new BorderLayout());

        JTextArea textArea = new JTextArea(newContent);
        textArea.setEditable(false);
        textArea.setLineWrap(true);
        textArea.setWrapStyleWord(true);
        JScrollPane scrollPane = new JScrollPane(textArea);
        add(scrollPane, BorderLayout.CENTER);

        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JButton cancelButton = new JButton("キャンセル");
        cancelButton.addActionListener(e -> dispose());
        buttonPanel.add(cancelButton);

        JButton executeButton = new JButton("実行");
        executeButton.addActionListener(e -> saveChanges());
        buttonPanel.add(executeButton);
        add(buttonPanel, BorderLayout.SOUTH);
    }

    private void saveChanges() {
        try {
            Files.write(Paths.get(filePath), newContent.getBytes("UTF-8"));
            JOptionPane.showMessageDialog(this, "置換が完了しました。", "完了", JOptionPane.INFORMATION_MESSAGE);
            dispose();
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "ファイルの保存に失敗しました: " + e.getMessage(), "エラー", JOptionPane.ERROR_MESSAGE);
        }
    }
}
