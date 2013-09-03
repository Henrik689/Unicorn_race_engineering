package GUI_telemetry;

import javax.swing.*;
import java.awt.*;

public class GUIdummy extends JPanel{

    private static Color bgcolor = Color.darkGray;
    private static Color linecolor = Color.BLACK;

    public void paintComponent(Graphics g)
    {
        // Recover Graphics2D
        Graphics2D g2 = (Graphics2D) g;

        // Antialiasing for glatte kurver
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        //g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        // Baggrund
        int h = getHeight();  // Panelets højde
        int w = getWidth();   // Panelets bredde
        g2.setColor(bgcolor);
        g2.fill(new Rectangle(0, 0, w, h));
        
        g2.setColor(linecolor);
        g2.drawLine(0, 0, w, h);
        g2.drawLine(w, 0, 0, h);
    }
}
