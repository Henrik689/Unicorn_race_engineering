package GUI_telemetry;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.Line2D;

public class GrafPlot extends JPanel
{
    private int xTicks = 0;
    private int yTicks = 0;
    private double[] datay;
    private double[] datax;
    private int yAxisToTicksSpacing = 0;

    private Color grafcolor = null;
    private Color bgColor = null;
    private Color axisColor = null;
    private Color seperatorColor = Color.black;
    private boolean antiA = false;

    Stroke lineStroke = new BasicStroke(3.0f);

    // -------- Constructors --------
    public GrafPlot()
    {
        this(40,10,Color.RED,Color.BLACK,Color.GREEN,true);
    }
    
    public GrafPlot(int xTicks, int yTicks)
    {
        this(xTicks,yTicks,Color.RED,Color.BLACK,Color.GREEN,true);
    }

    public GrafPlot(int xTicks, int yTicks, Color grafcolor)
    {
        this(xTicks,yTicks,grafcolor,Color.BLACK,Color.GREEN,true);
    }

    public GrafPlot(int xTicks, int yTicks, Color grafcolor,Color bgColor)
    {
        this(xTicks,yTicks,grafcolor,bgColor,Color.GREEN,true);
    }

    public GrafPlot(int xTicks, int yTicks, Color grafcolor,Color bgColor, Color axisColor)
    {
        this(xTicks,yTicks,grafcolor,bgColor,axisColor,true);
    }
    
    public GrafPlot(int xTicks, int yTicks, Color grafcolor, Color bgColor, Color axisColor, boolean antiA)
    {
        this.xTicks = xTicks;
        this.yTicks = yTicks;
        this.grafcolor = grafcolor;
        this.antiA = antiA;
        this.bgColor = bgColor;
        this.axisColor = axisColor;

        // Afstanden mellem y-aksen og y-akse tallene
        if(yTicks>99)
            this.yAxisToTicksSpacing = 20;
        else if(yTicks>9)
            this.yAxisToTicksSpacing = 15;
        else
            this.yAxisToTicksSpacing = 5;

        datay = new double[xTicks+1];
        datax = new double[xTicks+1];

        // Sætter data til 0
        for (int counter = 0; counter <= xTicks; counter++)
        {
            datay[counter] = 0;
        }
    }
    // -------- Constructors End ----

    // Her tegnes grafen
    public void paintComponent(Graphics g)
    {
        // Recover Graphics2D
        Graphics2D g2 = (Graphics2D) g;

        if(antiA)
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        // Baggrund
        float h = getHeight();
        float w = getWidth();
        g2.setColor(bgColor);
        g2.fill(new Rectangle(0, 0, (int) w, (int) h));

        float xOffset = 20;
        float yOffset = 20;
        float xTickSpace = (w-xOffset*2)/xTicks;
        float yTickSpace = (h-yOffset*2)/yTicks;

        // seperator left
        //g2.setColor(seperatorColor);
        //g2.drawLine(0, 0, 0, (int)h);
        
        // Koordinat-system
        g2.setColor(axisColor);
        g2.drawLine((int) xOffset,(int) (h-yOffset), (int) w, (int) (h-yOffset));
        g2.drawLine((int) xOffset, 0, (int) xOffset, (int) (h-yOffset));

        // X-akse tal
        for (int counter = 0; counter <= xTicks; counter++)
        {
            if(counter%(xTicks/20)==0) // Fjerne nogle af x-tal. Så der kun er ~20 på aksen
                g2.drawString("" + counter, xOffset+(xTickSpace*counter), (h-yOffset)+12);
            datax[counter] = xOffset+(xTickSpace*counter);
        }

        // Y-akse tal
        for (int counter = 0; counter <= yTicks; counter++)
        {
            if(counter%(yTicks/10)==0) // Fjerne nogle af y-tal. Så der kun er ~10 på aksen
                g2.drawString("" + counter, xOffset-yAxisToTicksSpacing, (h-yOffset)-counter*yTickSpace+4);
        }

        // Graf farver
        g2.setColor(grafcolor);

        // Her plottes selve grafen. Først skalering.
        float scale = (h-2*yOffset)/yTicks;

        g2.setStroke(lineStroke);
        for (int counter = 0; counter < xTicks; counter++)
        {
            g2.draw(new Line2D.Double(datax[counter],(h-yOffset)-(datay[counter]*scale),datax[counter+1],(h-yOffset)-(datay[counter+1])*scale));
        }
    }

    // -------- Metoder --------
    public void updateData(int newData)
    {
        // FIFO-kø
        for (int counter = 0; counter < xTicks; counter++)
        {
            datay[counter] = datay[counter+1];
        }
        datay[xTicks]= newData;

        this.repaint();
    }
}
