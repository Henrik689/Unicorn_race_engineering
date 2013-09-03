package GUI_telemetry;

public class IIRFilter {

    private double filterKonstant = 0.0;
    private double yOld = 0.0;
    private double y = 0.0;

    // Constructers
    public IIRFilter()
    {
        this(0.1);
    }
    public IIRFilter(double konstant)
    {
        this.filterKonstant = konstant;
    }

    // Metoder
    int filterData(int data)
    {
        y = filterKonstant*data+(1-filterKonstant)*yOld;
        yOld = y;
        return (int)y;
    }
    double filterDataDouble(double data)
    {
        y = filterKonstant*data+(1-filterKonstant)*yOld;

        yOld = y;
        return y;
    }
}
