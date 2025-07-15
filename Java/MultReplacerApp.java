import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MultReplacerApp extends JFrame {

    private JTextField filePathField;
    private JPanel replacementsPanel;
    private List<ReplacementItem> replacementItems;
    private JScrollPane scrollPane;

    public MultReplacerApp() {
        setTitle("置き換え君 v1.0.0");
        setSize(1280, 720);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null); // 中央に配置
        setLayout(new BorderLayout());

        replacementItems = new ArrayList<>();

        // File path selection panel
        JPanel filePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        filePanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 5, 10));
        filePanel.add(new JLabel("パス:"));
        filePathField = new JTextField(50);
        filePanel.add(filePathField);
        JButton browseButton = new JButton("参照");
        browseButton.addActionListener(e -> browseFile());
        filePanel.add(browseButton);
        add(filePanel, BorderLayout.NORTH);

        // Caution label
        JLabel cautionLabel = new JLabel("注意：大文字と小文字は区別されます。");
        cautionLabel.setForeground(Color.RED);
        cautionLabel.setBorder(BorderFactory.createEmptyBorder(0, 10, 5, 10));
        add(cautionLabel, BorderLayout.CENTER); // 仮の配置

        // Replacements panel
        replacementsPanel = new JPanel();
        replacementsPanel.setLayout(new BoxLayout(replacementsPanel, BoxLayout.Y_AXIS));
        replacementsPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        scrollPane = new JScrollPane(replacementsPanel);
        scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
        scrollPane.getVerticalScrollBar().setUnitIncrement(16); // スクロール速度調整

        // Add initial replacement row
        addReplacementRow();

        // Add scrollPane to a new panel to control its size
        JPanel centerPanel = new JPanel(new BorderLayout());
        centerPanel.add(cautionLabel, BorderLayout.NORTH); // 正しい配置
        centerPanel.add(scrollPane, BorderLayout.CENTER);
        add(centerPanel, BorderLayout.CENTER);


        // Buttons panel
        JPanel buttonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        buttonPanel.setBorder(BorderFactory.createEmptyBorder(5, 10, 10, 10));
        JButton addButton = new JButton("追加");
        addButton.addActionListener(e -> addReplacementRow());
        buttonPanel.add(addButton);

        JButton executeButton = new JButton("実行");
        executeButton.addActionListener(e -> executeReplacements());
        buttonPanel.add(executeButton);
        add(buttonPanel, BorderLayout.SOUTH);
    }

    private void browseFile() {
        JFileChooser fileChooser = new JFileChooser();
        fileChooser.setFileFilter(new FileNameExtensionFilter("Text Files (*.txt)", "txt"));
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            filePathField.setText(fileChooser.getSelectedFile().getAbsolutePath());
        }
    }

    private void addReplacementRow() {
        JPanel rowPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField beforeField = new JTextField(20);
        JTextField afterField = new JTextField(20);
        JButton deleteButton = new JButton("削除");

        ReplacementItem item = new ReplacementItem(beforeField, afterField, rowPanel);
        replacementItems.add(item);

        deleteButton.addActionListener(e -> deleteReplacementRow(item));

        rowPanel.add(deleteButton);
        rowPanel.add(beforeField);
        rowPanel.add(new JLabel("→"));
        rowPanel.add(afterField);

        replacementsPanel.add(rowPanel);
        replacementsPanel.revalidate();
        replacementsPanel.repaint();
        // スクロールを一番下へ
        SwingUtilities.invokeLater(() -> {
            scrollPane.getVerticalScrollBar().setValue(scrollPane.getVerticalScrollBar().getMaximum());
        });
    }

    private void deleteReplacementRow(ReplacementItem item) {
        replacementsPanel.remove(item.getPanel());
        replacementItems.remove(item);
        replacementsPanel.revalidate();
        replacementsPanel.repaint();
    }

    private void executeReplacements() {
        String filePath = filePathField.getText();
        if (filePath.isEmpty()) {
            JOptionPane.showMessageDialog(this, "ファイルパスを入力してください。", "エラー", JOptionPane.ERROR_MESSAGE);
            return;
        }

        try {
            String content = new String(Files.readAllBytes(Paths.get(filePath)), "UTF-8");
            String newContent = content;

            Map<String, String> replacementsMap = new HashMap<>();
            for (ReplacementItem item : replacementItems) {
                if (!item.getBeforeText().isEmpty()) {
                    replacementsMap.put(item.getBeforeText(), item.getAfterText());
                }
            }

            for (Map.Entry<String, String> entry : replacementsMap.entrySet()) {
                newContent = newContent.replace(entry.getKey(), entry.getValue());
            }

            // Show confirmation dialog
            ConfirmationDialog confirmationDialog = new ConfirmationDialog(this, newContent, filePath);
            confirmationDialog.setVisible(true);

        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "ファイルの読み込みに失敗しました: " + e.getMessage(), "エラー", JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            new MultReplacerApp().setVisible(true);
        });
    }
}
