package GUI_telemetry;

import javax.swing.*;
import java.awt.*;

public class headerTxt extends JPanel{

    private static Color bgcolor = Color.DARK_GRAY;
    private Color txtColor = Color.red;
    private Color seperatorColor = Color.black;
    private String header;
    private Font font = new Font("Dialog", Font.BOLD, 12);
    private boolean seperator = false;
    
    public headerTxt()
    {
        this("Header");
    }

    public headerTxt(String header)
    {
        this(header,Color.black);
    }

    public headerTxt(String header, Color fontcolor)
    {
        this(header,fontcolor,false);
    }

    public headerTxt(String header, Color fontcolor, boolean seperator)
    {
        this.header = header;
        this.txtColor = fontcolor;
        this.seperator = seperator;
    }

    public void paintComponent(Graphics g)
    {
        // Recover Graphics2D
        Graphics2D g2 = (Graphics2D) g;

        // Antialiasing for glatte kurver
        g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        // Baggrund
        int h = getHeight();  // Panelets højde
        int w = getWidth();   // Panelets bredde
        g2.setColor(bgcolor);
        g2.fill(new Rectangle(0, 0, w, h));

        if(seperator)
        {
            g2.setColor(seperatorColor);
            g2.drawLine(0,0,w,0);
        }

        g2.setColor(txtColor);
        g2.setFont(font);
        g2.drawString(header, 10, h/2);

    }

    void setHeader(String header)
    {
        this.header = header;
        this.repaint();
    }
}