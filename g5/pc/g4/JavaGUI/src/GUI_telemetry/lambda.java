package GUI_telemetry;

import javax.swing.*;
import java.awt.*;

public class lambda extends JPanel{
    
    private static Color axisColor = Color.black;
    private Color markerColor = Color.red;
    private static Color bgcolor = Color.darkGray;
    private Stroke lineStroke = new BasicStroke(3.0f);
    float dash1[] = { 5.0f, 5.0f };
    private Stroke markerStroke = new BasicStroke(3.0f,BasicStroke.CAP_BUTT,BasicStroke.CAP_BUTT,10.0f, dash1, 0.0f);
    private Font fontBig = new Font("Dialog", Font.BOLD, 18);
    private int h = 0;  // Panelets højde
    private int w = 0;   // Panelets bredde
    private int numOfticks;
    private int lambda = 10;
    private String lambdaString = "1.0";
    
    public lambda()
    {
        this(6);
    }

    public lambda(int ticks)
    {
        this.numOfticks = ticks;
        if((numOfticks % 2) == 1)
            numOfticks++;
    }

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

        // Akse
        g2.setColor(axisColor);
        g2.setStroke(lineStroke);
        g2.drawLine(20,h/2,w-20,h/2);

        // Inddelinger
        g2.drawLine(((w)/2),(h/2)-15,((w*3)/6),(h/2)+15);

        int i = 0;
        for(int x = 1; x < numOfticks; x++)
        {
            g2.drawLine(((w*x)/numOfticks),(h/2)-10,((w*x)/numOfticks),(h/2)+10);

            i = ((10*x)/numOfticks)+5;
            if(i<10)
            {
                g2.drawString("0."+Integer.toString(i),((w*x)/numOfticks),(h/2)-25);
            }
            if(i>10)
            {
                i = i - 9;
                g2.drawString("1."+Integer.toString(i),((w*x)/numOfticks),(h/2)-25);
            }
            if(i==10)
            {
                g2.drawString("1.0",((w*x)/numOfticks),(h/2)-25);
            }          
        }

        // Rich / Lean
        g2.setFont(fontBig);
        g2.drawString("Lean",(3*w/5),(7*h/10));
        g2.drawString("Rich",(w/5),(7*h/10));

        // Marker
        g2.setColor(markerColor);
        g2.setStroke(markerStroke);
        g2.drawLine((w*(lambda - 4))/12,(h/2)-20,(w*(lambda - 4))/12,(h/2)+25);

        // Lambda value
        g2.drawString(lambdaString,(w/2)-16,(8*h/10));
    }

    void setLambda(int lambda)
    {
        this.lambda = lambda/10;

        if(this.lambda < 5)
            this.lambda = 5;
        if(this.lambda > 15)
            this.lambda = 15;

        lambdaString = "" + (double)lambda/100;

        this.repaint();
    }
}

