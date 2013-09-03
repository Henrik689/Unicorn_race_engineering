package GUI_telemetry;

public class dataStore {

    txtData txtDataLabel;
    GrafPlot graph1;
    GrafPlot graph2;
    GrafPlot graph3;
    wheelAngle wheel0;
    speedAnalog Analog;
    speedAnalogSimple RPM;
    lambda lambda0;
    barData pedalPos;
    accPlot acc;

    // Data
    private String[] dataArray;

    private int throttlepos = 0;
    private int lambda = 0;
    private int rpm = 0;
    private int accX = 0;
    private int accY = 0;
    private int speed = 0;

    // Fir filter
    IIRFilter dataFilter1 = null;
    //IIRFilter dataFilter2 = null;
    //IIRFilter dataFilter3 = null;

    public dataStore(txtData txtDataLabel, GrafPlot graph, GrafPlot graph2, GrafPlot graph3, speedAnalog Analog, wheelAngle wheel0 , speedAnalogSimple speedSimple0, lambda lambda0, barData pedalPos, accPlot acc)
    {
        this.txtDataLabel = txtDataLabel;
        this.graph1 = graph;
        this.graph2 = graph2;
        this.graph3 = graph3;
        this.wheel0 = wheel0;
        this.Analog = Analog;
        this.RPM = speedSimple0;
        this.lambda0 = lambda0;
        this.pedalPos = pedalPos;
        this.acc = acc;

        // Data Filter 
        dataFilter1 = new IIRFilter(0.05);
        //dataFilter2 = new IIRFilter(0.5);
        //dataFilter3 = new IIRFilter(0.5);
    }

    public void setData(String dataString)
    {
        dataArray = dataString.split(",");

        throttlepos = Integer.parseInt(dataArray[5]);
        lambda = Integer.parseInt(dataArray[14]);
        rpm = Integer.parseInt(dataArray[0]);
        accX = Integer.parseInt(dataArray[12]);
        accY = Integer.parseInt(dataArray[13]);
        speed = Integer.parseInt(dataArray[1]);

        // Opdatering af data tekst
        txtDataLabel.setData(3, Integer.parseInt(dataArray[0])); // RPM
        txtDataLabel.setData(2, speed); // Speed
        txtDataLabel.setData(5, Integer.parseInt(dataArray[2])); // Water temp out
        txtDataLabel.setData(8, Integer.parseInt(dataArray[3])); // Manifoltemp
        txtDataLabel.setData(18, Integer.parseInt(dataArray[4])); // batt voltage
        txtDataLabel.setData(11, throttlepos); // Throttle-pos
        txtDataLabel.setData(9, Integer.parseInt(dataArray[6])); // MAP
        txtDataLabel.setData(15, Integer.parseInt(dataArray[7])); // Gear neutral
        txtDataLabel.setData(7, Integer.parseInt(dataArray[8])); // Oil press
        txtDataLabel.setData(4, Integer.parseInt(dataArray[9])); // Water temp in
        txtDataLabel.setData(6, Integer.parseInt(dataArray[10])); // Oil temp
        txtDataLabel.setData(1, Integer.parseInt(dataArray[11])); // Time
        txtDataLabel.setData(16, Integer.parseInt(dataArray[12])); // accX
        txtDataLabel.setData(17, Integer.parseInt(dataArray[13])); // accY
        txtDataLabel.setData(10,lambda); // Lambda
        txtDataLabel.setData(19, Integer.parseInt(dataArray[15])); // Injector
        txtDataLabel.setData(20, Integer.parseInt(dataArray[16])); // Ignition
        txtDataLabel.setData(21, Integer.parseInt(dataArray[17])); // eng load
        txtDataLabel.setData(22, Integer.parseInt(dataArray[18])); // trigger error

        // Opdatering af analog hastighed
        Analog.setData(speed);

        // Opdatering af analog RPM
        RPM.setData(rpm);
        graph1.updateData(rpm/100);

        // Lambda
        lambda0.setLambda(lambda);
        graph3.updateData(lambda/10);

        // Pedal postion
        pedalPos.setBarData(throttlepos,0,0);
        if(throttlepos>0)
            graph2.updateData(throttlepos);
        else
            graph2.updateData(0);

        // Acceleration
        acc.setAcc(accX, accY);

        // Test      
        // wheel0.setAngle(Integer.parseInt(dataArray[1]));
    }
}
