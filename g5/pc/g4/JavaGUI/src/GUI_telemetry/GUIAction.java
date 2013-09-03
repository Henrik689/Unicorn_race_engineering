package GUI_telemetry;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.*;
import java.io.IOException;
import javax.swing.*;
import java.net.*;


public class GUIAction implements ActionListener{

    // Frame
    private JFrame frame;
    private final int FRAME_WIDTH = 1200;
    private final int FRAME_HEIGHT = 700;

    // Panels
    protected JPanel controls;
    protected JPanel txtdata;
    protected JPanel graphic;
    
    // Labels / textfields / buttons
    private JTextField hostTXT = new JTextField("localhost");
    private JTextField portTXT = new JTextField("9999");

    // Socket
    int portint = 9999;
    String ip = "localhost";
    private static Socket clientSocket = null;
    private socketIn socketInthread = null;
    
    // Data
    dataStore datastore = null;

    // Button
    JButton disconnectButton = new JButton("Disconnect");
    JButton connectButton = new JButton("Connect");

    public GUIAction(){

        frame = new JFrame();
        frame.setSize(FRAME_WIDTH, FRAME_HEIGHT);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle("Unicorn G4 Telemetri");
        frame.setMinimumSize(new Dimension(FRAME_WIDTH,FRAME_HEIGHT));

        //Text data
        txtdata = new JPanel();
        BoxLayout boxLayout1 = new BoxLayout(txtdata, BoxLayout.Y_AXIS);
        txtdata.setLayout(boxLayout1);
        txtdata.setPreferredSize(new Dimension(200,0));
        txtdata.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        txtdata.add(Box.createHorizontalGlue());
        
        // --- Kontrol-knapperne ---

        // Panel til kontrol-knapperne nederst
        controls = new JPanel();
        BoxLayout boxLayout2 = new BoxLayout(controls, BoxLayout.X_AXIS);
        controls.setLayout(boxLayout2);
        controls.setBorder(BorderFactory.createEmptyBorder(0, 10, 10, 10));
        controls.add(Box.createHorizontalGlue());

        // connectButton
        //JButton connectButton = new JButton("Connect");
        connectButton.addActionListener(this);
        controls.add(connectButton);
        controls.add(Box.createRigidArea(new Dimension(10, 0))); // Afstand

        // disconnectButton       
        //JButton disconnectButton = new JButton("Disconnect");
        disconnectButton.addActionListener(this);
        controls.add(disconnectButton);
        controls.add(Box.createRigidArea(new Dimension(15, 0))); // Afstand

        // hostTXTlabel
        JLabel hostTXTlabel = new JLabel("Host:");
        controls.add(hostTXTlabel);
        controls.add(Box.createRigidArea(new Dimension(5, 0))); // Afstand

        // hostTXT        
        controls.add(hostTXT);
        controls.add(Box.createRigidArea(new Dimension(10, 0))); // Afstand

        // hostTXTlabel
        JLabel portTXTlabel = new JLabel("Port:");
        controls.add(portTXTlabel);
        controls.add(Box.createRigidArea(new Dimension(5, 0))); // Afstand

        // hostTXT        
        controls.add(portTXT);
        controls.add(Box.createRigidArea(new Dimension(10, 0))); // Afstand

        // exitButton
        JButton exitButton = new JButton("Exit");
        exitButton.addActionListener(this);
        controls.add(exitButton);

        // txtData Panel
        txtData txtDataPanel = new txtData();
        
        // Grafik panel delt i grids
        graphic = new JPanel(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();

        // Grafiske elementer
        lambda lambda0 = new lambda(6);

        // Speed / RPM
        speedAnalog speed0 = new speedAnalog();
        speedAnalogSimple speedSimple0 = new speedAnalogSimple();

        // Wheel angle
        wheelAngle wheel0 = new wheelAngle();

        // Pedal positions
         barData pedalPos = new barData();

         // Acc plot
         accPlot accplot = new accPlot();
        
        // Graphs
        GrafPlot graph1 = new GrafPlot(20,130,Color.gray,Color.darkGray,Color.black,true);
        GrafPlot graph2 = new GrafPlot(20,100,Color.gray,Color.darkGray,Color.black,true);
        GrafPlot graph3 = new GrafPlot(20,15,Color.gray,Color.darkGray,Color.black,true);

        // txt headers, top
        // Grid 0,0
        c.gridx = 0;
        c.gridy = 0;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Rear wheel speed"), c);

        // Grid 1,0
        c.gridx = 1;
        c.gridy = 0;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Engine RPM"), c);

        // Grid 2,0
        c.gridx = 2;
        c.gridy = 0;
        c.weightx = 0.7;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("RPM graph (x100)",Color.black), c);

        // Grid 0,1
        c.gridx = 0;
        c.gridy = 1;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.fill = GridBagConstraints.BOTH;
        c.insets = new Insets(0,0,0,0);
        graphic.add(speed0, c);

        // Grid 1,1
        c.gridx = 1;
        c.gridy = 1;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.fill = GridBagConstraints.BOTH;
        c.insets = new Insets(0,0,0,0);
        graphic.add(speedSimple0, c);
        
        // Grid 2,1
        c.gridx = 2;
        c.gridy = 1;
        c.weightx = 0.7;
        c.weighty = 0.3;
        c.fill = GridBagConstraints.BOTH;
        c.insets = new Insets(0,0,0,0);
        graphic.add(graph1, c);

        // txt headers, middle
        // Grid 0,2
        c.gridx = 0;
        c.gridy = 2;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Steering wheel position",Color.BLACK,true), c);

        // Grid 1,2
        c.gridx = 1;
        c.gridy = 2;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Pedal positions",Color.black,true), c);

        // Grid 2,2
        c.gridx = 2;
        c.gridy = 2;
        c.weightx = 0.7;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Throttle pos graph (0-100%)",Color.black,true), c);

        // Grid 0,3
        c.gridx = 0;
        c.gridy = 3;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.insets = new Insets(0,0,0,0);
        graphic.add(wheel0, c);

        // Grid 1,3
        c.gridx = 1;
        c.gridy = 3;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.insets = new Insets(0,0,0,0);
        graphic.add(pedalPos, c);

        // Grid 2,3
        c.gridx = 2;
        c.gridy = 3;
        c.weightx = 0.7;
        c.weighty = 0.3;
        c.insets = new Insets(0,0,0,0);
        graphic.add(graph2, c);

        // txt headers, buttom
        // Grid 0,4
        c.gridx = 0;
        c.gridy = 4;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Acceleration (g-force)",Color.BLACK,true), c);

        // Grid 1,4
        c.gridx = 1;
        c.gridy = 4;
        c.weightx = 0.3;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Lambda",Color.black,true), c);

        // Grid 2,4
        c.gridx = 2;
        c.gridy = 4;
        c.weightx = 0.7;
        c.weighty = 0.03;
        c.fill = GridBagConstraints.BOTH;
        graphic.add(new headerTxt("Lambda graph (x10)",Color.black,true), c);

        // Grid 0,5
        c.gridx = 0;
        c.gridy = 5;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.insets = new Insets(0,0,10,0);
        graphic.add(accplot, c);

        // Grid 1,5
        c.gridx = 1;
        c.gridy = 5;
        c.weightx = 0.3;
        c.weighty = 0.3;
        c.insets = new Insets(0,0,10,0);
        graphic.add(lambda0, c);

        // Grid 2,5
        c.gridx = 2;
        c.gridy = 5;
        c.weightx = 0.7;
        c.weighty = 0.3;
        //c.insets = new Insets(0,0,0,0);
        c.insets = new Insets(0,0,10,0);  //Bund padding
        graphic.add(graph3, c);

/*      // Grid 0,2
        c.gridx = 0;
        c.gridy = 2;
        //c.weightx = 0;
        c.weighty = 0.3;
        c.gridwidth = 3;
        c.weightx = 0.0;
        c.insets = new Insets(0,0,10,0);  //Bund padding
        graphic.add(gUI2D7, c);
        //graphic.add(gUI2D1, c);
 */        
        // Frame 
        frame.add(graphic,BorderLayout.CENTER);
        frame.add(txtDataPanel.getTxtDataPanel(),BorderLayout.EAST);
        frame.add(controls, BorderLayout.PAGE_END);

        // DataStote
        datastore = new dataStore(txtDataPanel,graph1,graph2,graph3,speed0,wheel0,speedSimple0,lambda0,pedalPos,accplot);

        frame.setVisible(true);
    }

    public void actionPerformed(ActionEvent ev) 
    {
        String command = ev.getActionCommand();
        System.out.println("Received command: " + command);

        if (command.equals("Exit"))
        {
            if(socketInthread != null)
                socketInthread.stop();
            
            if(clientSocket != null)
            {
                try
                {
                    clientSocket.close();
                }
                catch(IOException e)
                {
                    System.exit(0);
                }
                System.out.println("Connection to: " + ip + " on port: " + portint + " closed");
            }
           
            System.exit(0);
        }
        
        if (command.equals("Connect"))
        {
            try
            {
                if(!portTXT.getText().equals(""))
                    portint = Integer.parseInt(portTXT.getText());

                if(!hostTXT.getText().equals(""))
                    ip = hostTXT.getText();

                clientSocket = new Socket(ip, portint);
            }
            catch (UnknownHostException e)
            {
                System.err.println("Couldn't find host: " + ip);
                System.exit(0);
            }
            catch (IOException e)
            {
                System.err.println("Could not connect to: " + ip + " on port: " +portint );
                System.exit(0);
            }

            System.out.println("Connected to: " + ip + " on port: " + portint);
            connectButton.setEnabled(false);

            socketInthread =  new socketIn(clientSocket,datastore);
            socketInthread.start();
        }
        if (command.equals("Disconnect"))
        {
            if(socketInthread != null)
                socketInthread.stop();

            if(clientSocket!=null)
            {
                try
                {
                    clientSocket.close();
                    System.out.println("Connection to: " + ip + " on port: " + portint + " closed");
                }
                catch(IOException e)
                {
                    System.out.println("Could not disconnect from any server");
                }
            }
            else if(clientSocket==null)
            {
                System.out.println("Not connected to any server");
            }
            connectButton.setEnabled(true);
        }
    }
}
