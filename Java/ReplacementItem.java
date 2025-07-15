import javax.swing.*;
import java.awt.*;

public class ReplacementItem {
    private JTextField beforeField;
    private JTextField afterField;
    private JPanel panel;

    public ReplacementItem(JTextField beforeField, JTextField afterField, JPanel panel) {
        this.beforeField = beforeField;
        this.afterField = afterField;
        this.panel = panel;
    }

    public String getBeforeText() {
        return beforeField.getText();
    }

    public String getAfterText() {
        return afterField.getText();
    }

    public JPanel getPanel() {
        return panel;
    }
}
