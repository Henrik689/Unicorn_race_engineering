package GUI_telemetry;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;

public class wheelAngle extends JPanel{

    private static Color bgcolor = Color.darkGray;
    private Font fontBig = new Font("Dialog", Font.BOLD, 18);
    private Color darkRed = new Color(150,0,0);
    private static BasicStroke wideStroke1 = new BasicStroke(16.0f,BasicStroke.CAP_BUTT,BasicStroke.JOIN_BEVEL);

    private int angle = 0;
    private char deg = '\u00b0';
    private String angleString = "0";

    public void paintComponent(Graphics g)
    {
        Graphics2D g2 = (Graphics2D) g;

        AffineTransform original = g2.getTransform();

        // Antialiasing for glatte kurver
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
                            RenderingHints.VALUE_ANTIALIAS_ON);

        g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
                            RenderingHints.VALUE_TEXT_ANTIALIAS_LCD_HRGB);

        // Baggrund
        int h = getHeight();  // Panelets højde
        int w = getWidth();   // Panelets bredde

        g2.setColor(bgcolor);
        g2.fillRect(0, 0, w, h);

        // Rotering
        g2.rotate(Math.toRadians(angle),w/2-20,h/2-10);
        
        // Wheel
        g2.setPaint(darkRed); 
        g2.setStroke(wideStroke1);          
        g2.drawLine(w/2-h/2+10, h/2-10, h/2+w/2-50, h/2-10);
        g2.drawLine(w/2-20, h/2-10, w/2-20, h-40);
        g2.drawOval(w/2-h/2+10, 20, h-60, h-60);

        g2.setTransform(original);

        // Digital hastighed
        g2.setFont(fontBig);
        g2.setPaint(Color.WHITE);
        g2.drawString(angleString+deg, (int)(w/2)-40,h-10);

    }

    public void setAngle(int angle)
    {
        this.angle = angle;
        if(this.angle > 0)
            angleString = ("+" + this.angle);
        else if(this.angle == 0)
            angleString = ("\u00b1" + this.angle);
        else
            angleString = ("" + this.angle);
        this.repaint();
    }
}


