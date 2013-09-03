package GUI_telemetry;

import javax.swing.*;
import java.awt.*;

public class barData extends JPanel{

    private Color bgColor = Color.darkGray;
    private Color barColorEdge = Color.BLACK;
    private Color barColor = Color.gray;
    private Color txtColor = Color.black;
    private Color markerColor = Color.black;
    private int xOffset = 60;
    private Font font = new Font("Dialog", Font.BOLD, 10);
    float dash1[] = { 5.0f, 5.0f };
    private Stroke markerStroke = new BasicStroke(2.0f,BasicStroke.CAP_BUTT,BasicStroke.CAP_BUTT,10.0f, dash1, 0.0f);
    private double data1 = 20;
    private double data2 = 20;
    private double data3 = 20;
    private int h = 0;
    private int w = 0;

    public void paintComponent(Graphics g)
    {
        // Recover Graphics2D
        Graphics2D g2 = (Graphics2D) g;

        // Antialiasing for glatte kurver
        g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        //g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

        // Baggrund
        h = getHeight();  // Panelets højde
        w = getWidth();   // Panelets bredde
        g2.setColor(bgColor);
        g2.fillRect(0, 0, w, h);

        // 3 x rectangles
        //RoundRectangle2D rectangle1 = new RoundRectangle2D.Double(xOffset, h/7, data1, h/6,10,10);
        //RoundRectangle2D rectangle2 = new RoundRectangle2D.Double(xOffset, 3*h/7, data2, h/6,10,10);
        //RoundRectangle2D rectangle3 = new RoundRectangle2D.Double(xOffset, 5*h/7, data3, h/6,10,10);

        // 3 x strings
        g2.setColor(txtColor);
        g2.setFont(font);
        g2.drawString("Throttle", 0, (19*h)/84 + 5);
        g2.drawString("Brake", 0, (43*h)/84 + 5);
        g2.drawString("Clutch", 0, (67*h)/84 + 5);

        g2.setColor(barColor);  // Bar color
        g2.fillRoundRect(xOffset, h/7, (int)data1, h/6,10,10);
        g2.fillRoundRect(xOffset, 3*h/7, (int)data2, h/6,10,10);
        g2.fillRoundRect(xOffset, 5*h/7, (int)data3, h/6,10,10);
/*
        // Rectangle 1
        g2.setColor(barColor);  // Bar color
        g2.fill(rectangle1);
        g2.setColor(barColorEdge);  // Bar edge color
        g2.draw(rectangle1);

        // Rectangle 2
        g2.setColor(barColor);  // Bar color
        g2.fill(rectangle2);
        g2.setColor(barColorEdge);  // Bar edge color
        g2.draw(rectangle2);

        // Rectangle 3
        g2.setColor(barColor);  // Bar color
        g2.fill(rectangle3);
        g2.setColor(barColorEdge);  // Bar edge color
        g2.draw(rectangle3);
*/
        // Marker (0% / 100%)
        g2.setColor(markerColor);
        g2.setStroke(markerStroke);
        g2.drawLine(xOffset-1,  h/7, xOffset-1,  (37*h)/42);
        g2.drawLine(w-21,  h/7, w-20,  (37*h)/42);

        // 0 % / 10% string
        g2.drawString("0%", xOffset-5,h/7-10);
        g2.drawString("100%", w-35,h/7-10);
    }
    void setBarData(int data1, int data2, int data3)
    {
        if(data1<100)
            this.data1 = (data1*((w-20-xOffset)))/100;
        else
            this.data1 = 100;
        this.data2 = (data2*((w-20-xOffset)))/100;
        this.data3 = (data3*((w-20-xOffset)))/100;
        this.repaint();
    }

}
