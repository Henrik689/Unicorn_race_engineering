package GUI_telemetry;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Arc2D;
import java.awt.geom.Ellipse2D;
import java.awt.geom.Line2D;

public class speedAnalog extends JPanel{

    private static Color bgcolor = Color.darkGray;
    private static BasicStroke wideStroke1 = new BasicStroke(16.0f,BasicStroke.CAP_BUTT,BasicStroke.JOIN_BEVEL);
    private static BasicStroke wideStroke2 = new BasicStroke(6.0f, BasicStroke.CAP_SQUARE,BasicStroke.JOIN_BEVEL);
    private static BasicStroke wideStroke3 = new BasicStroke(4.0f, BasicStroke.CAP_ROUND,BasicStroke.JOIN_BEVEL);
    private Font font = new Font("Dialog", Font.BOLD, 12);
    private Font fontBig = new Font("Dialog", Font.BOLD, 18);
    private Color darkRed = new Color(220,0,0);
    private Color darkGreen = new Color(0,220,0);

    private double data = 4.7;
    private String dataString = "0 km/t";

    private int centerx = 0;
    private int centery = 0;
    private int radius = 0;

    private int h = 0;  // Panelets højde
    private int w = 0;   // Panelets bredde
    private int x = 10;
    private int y = 10;

    Line2D pil = new Line2D.Double();

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

        centerx = (x+(h-20)/2);
        centery = (y+(h-20)/2);

        g2.setColor(bgcolor);
        g2.fill(new Rectangle(0, 0, w, h));

        // Ydre bue
        g2.setStroke(wideStroke1);
        g2.setPaint(darkGreen);
        g2.draw(new Arc2D.Double(x, y, h-20, h-20, 90, 200,Arc2D.OPEN));

        // Ydre bue rød
        g2.setPaint(darkRed);
        g2.draw(new Arc2D.Double(x, y, h-20, h-20, 30, 60,Arc2D.OPEN));

        // Indre bue
        g2.setStroke(wideStroke2);
        g2.setPaint(Color.LIGHT_GRAY);
        g2.draw(new Arc2D.Double(x+70, y+70, h-160, h-160, 30, 260,Arc2D.OPEN));

        // Prik i midten
        g2.setPaint(Color.BLACK);
        g2.fill(new Ellipse2D.Double(centerx-5.5,centery-5.5,12,12));
        //g2.draw(new Arc2D.Double(x+90, y+90, h-200, h-200,0,360,Arc2D.OPEN));

        // Tal
        double vinkel = 4.7;
        int counter = 0;
        int radiusScaled = (int)((h-20)/2.4);
        radius = (int)((h-20)/2);

        g2.setFont(font);
        g2.setColor(Color.WHITE);
        g2.setStroke(wideStroke3);

        for(int i=1; i<14; i++)
        {
            // Tal
            if(vinkel < 8)
            {
            g2.drawString("" + counter, (centerx-((int)(Math.cos(vinkel)*radiusScaled))-5), (centery-((int)(Math.sin(vinkel)*radiusScaled)))+5);
            }

            // Streger
            g2.draw(new Line2D.Double(centerx-((int)(Math.cos(vinkel)*radius*0.95)), centery-((int)(Math.sin(vinkel)*radius*0.95)), centerx-((int)(Math.cos(vinkel)*radius)*1.07), centery-((int)(Math.sin(vinkel)*radius*1.07)) ));

            counter = counter + 10;
            vinkel = vinkel + 0.34;
        }

        // Digital hastighed
        g2.setFont(fontBig);
        g2.drawString("Speed:", (int)(radius*1.4),(int)(radius*1.2));
        g2.drawString(dataString , (int)(radius*1.4),(int)(radius*1.4));

        // Pilen
        g2.setPaint(Color.white);
        pil.setLine(centerx, centery, centerx-((int)(Math.cos(data)*radius)*1.07), centery-((int)(Math.sin(data)*radius*1.07)));
        g2.draw(pil);

    }

    public void setData(int data)
    {
        this.data = (data*0.034 + 4.7);
        dataString = ("" + data + " km /t");
        this.repaint();
    }

}
