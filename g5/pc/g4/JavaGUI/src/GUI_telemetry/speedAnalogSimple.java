package GUI_telemetry;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Arc2D;

public class speedAnalogSimple extends JPanel{

    private static Color bgcolor = Color.darkGray;
    private static BasicStroke wideStroke1 = new BasicStroke(16.0f);
    private Font fontBig = new Font("Dialog", Font.BOLD, 18);
    private Color darkGreen = new Color(0,220,0);

    private double data = 4.7;
    private String dataString = "0";

    private int h = 0;  // Panelets højde
    private int w = 0;   // Panelets bredde
    private int x = 10;
    private int y = 10;

    private int red = 0;
    private int green = 0;

    private IIRFilter dataFilter1 = new IIRFilter(0.1);

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
        g2.fill(new Rectangle(0, 0, w, h));

        // Ydre bue
        g2.setStroke(wideStroke1);

        // Color
        red = (int)-data;
        if(red > 255) red = 255;
        if(red <= 0) red = 0;
        
        green = (int)(255 - red);
        if(green > 255) green = 255;
        if(green <= 0) green = 0;
        
        Color colorArc = new Color(red,green,0);

        g2.setPaint(colorArc);
        g2.draw(new Arc2D.Double(x, y, h-20, h-20, 290,data,Arc2D.OPEN));

        // Digital hastighed
        g2.setPaint(Color.WHITE);
        g2.setFont(fontBig);
        g2.drawString("RPM:", (int)((w/2)-40),(int)((h/2)*1.0));
        g2.drawString(dataString , (int)((w/2)-40),(int)((h/2)*1.2));
    }

    public void setData(int data)
    {
        this.data =  dataFilter1.filterDataDouble((data*-2)/100);
        dataString = ("" + data);
        this.repaint();
    }

}
