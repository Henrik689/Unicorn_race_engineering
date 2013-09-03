package GUI_telemetry;

import javax.swing.*;
import java.awt.*;

public class accPlot extends JPanel{

    private Color bgcolor = Color.darkGray;
    private Color blackcolor = Color.black;
    private static BasicStroke wideStroke1 = new BasicStroke(3.0f);
    private Font font = new Font("Dialog", Font.PLAIN, 10);
    private int x = 0;
    private int y = 0;
    private int h = 0;
    private int w = 0;

    public void paintComponent(Graphics g)
    {
        Graphics2D g2 = (Graphics2D) g;

        // Antialiasing for glatte kurver
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                            RenderingHints.VALUE_ANTIALIAS_ON);

        g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                            RenderingHints.VALUE_TEXT_ANTIALIAS_LCD_HRGB);

        // Baggrund
        h = getHeight();  // Panelets højde
        w = getWidth();   // Panelets bredde

        g2.setColor(bgcolor);
        g2.fillRect(0, 0, w, h);

        g2.setColor(blackcolor);

        // Akser
        g2.setStroke(wideStroke1);
        g2.drawOval(2, 2, h-4, h-4);
        g2.drawOval(h/6 +4/3, h/6+4/3, (2*h)/3-8/3, (2*h)/3-8/3);
        g2.drawOval(h/3-4/3, h/3-4/3,h/3-4/3,h/3-4/3);
        g2.drawLine(2, h/2, h-2, h/2);
        g2.drawLine(2+(h-4)/2, 2, 2+(h-4)/2, h-2);

        // txt
        g2.setColor(Color.white);
        g2.setFont(font);
        g2.drawString("0.5g", (2*h)/3-8/3, h/2-1);
        g2.drawString("1.0g", (5*h)/6-4/3, h/2-1);
        g2.drawString("1.5g", h-2, h/2-1);

        // Prik
        g2.setColor(Color.red);
        g2.fillOval(h/2 +x-7, h/2 +y-7, 14, 14);
    }
    void setAcc(int y, int x)
    {
        this.x = x*(h-4)/30;
        this.y = y*(h-4)/30;

        this.repaint();
    }
}