package GUI_telemetry;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class txtData {

    JPanel txtDataPanel;

    // Colors
    private Color red = Color.red;
    private Font font = new Font("Dialog", Font.BOLD, 10);

    // Text data spacing
    int smallspace = 9;
    int bigspace = 12;

    // Labels
    private JLabel lbltime = new JLabel("Time:"); // 1

    // Engine
    private JLabel lblengine = new JLabel("Engine");

    private JLabel lblSpeed = new JLabel("Speed:"); // 2
    private JLabel lblrpm = new JLabel("RPM:"); //3
    private JLabel lblwatertempin= new JLabel("Water temp in:"); //4
    private JLabel lblwatertempout = new JLabel("Water temp out:"); //5
    private JLabel lbloiltemp = new JLabel("Oil temp:"); // 6
    private JLabel lbloilpress = new JLabel("Oil press:"); // 7
    private JLabel lblmanifoldtemp = new JLabel("Manifold temp:"); // 8
    private JLabel lblmanifoldpress = new JLabel("Manifold press:"); // 9
    private JLabel lbllambda = new JLabel("Lambda:"); // 10

    // Driver
    private JLabel lbldriver = new JLabel("Driver");

    private JLabel lblthrottlepos = new JLabel("Throttle pos:"); // 11
    private JLabel lblbrakepos = new JLabel("Brake pos:"); // 12
    private JLabel lblclutchpos = new JLabel("Clutch pos:"); // 13
    private JLabel lblWheelpos = new JLabel("Wheel pos:"); // 14
    private JLabel lblGear = new JLabel("Gear:"); // 15
    private JLabel lblxacc = new JLabel("X-acc:"); // 16
    private JLabel lblyacc = new JLabel("Y-acc:"); // 17

    // Elektronik
    private JLabel lblelectronics = new JLabel("Electronics");

    private JLabel lblbattvolt = new JLabel("Batt voltage:"); //18

    // ECU
    private JLabel lblecu = new JLabel("ECU");

    private JLabel lblinjectorTime = new JLabel("Injector-Time:"); //19
    private JLabel lblignitionTime = new JLabel("Ignition-Time:"); //20
    private JLabel lblengload = new JLabel("Load:"); //21
    private JLabel lbltriggererr = new JLabel("Trigger-Eorror:"); //22



    public txtData()
    {
        //Text data Panel
        txtDataPanel = new JPanel();
        BoxLayout boxLayout1 = new BoxLayout(txtDataPanel, BoxLayout.Y_AXIS);
        txtDataPanel.setLayout(boxLayout1);
        txtDataPanel.setPreferredSize(new Dimension(200,0));
        txtDataPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 0, 0));
        txtDataPanel.add(Box.createHorizontalGlue());

        lbltime.setFont(font);
        txtDataPanel.add(lbltime);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        // ENGINE
        lblengine.setForeground(red);
        txtDataPanel.add(lblengine);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        lblSpeed.setFont(font);
        txtDataPanel.add(lblSpeed);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblrpm.setFont(font);
        txtDataPanel.add(lblrpm);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblwatertempout.setFont(font);
        txtDataPanel.add(lblwatertempout);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblwatertempin.setFont(font);
        txtDataPanel.add(lblwatertempin);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lbloiltemp.setFont(font);
        txtDataPanel.add(lbloiltemp);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lbloilpress.setFont(font);
        txtDataPanel.add(lbloilpress);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblmanifoldtemp.setFont(font);
        txtDataPanel.add(lblmanifoldtemp);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblmanifoldpress.setFont(font);
        txtDataPanel.add(lblmanifoldpress);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lbllambda.setFont(font);
        txtDataPanel.add(lbllambda);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        // Driver
        lbldriver.setForeground(red);
        txtDataPanel.add(lbldriver);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));
        
        lblthrottlepos.setFont(font);
        txtDataPanel.add(lblthrottlepos);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));
        
        lblbrakepos.setFont(font);
        txtDataPanel.add(lblbrakepos);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));
        
        lblclutchpos.setFont(font);
        txtDataPanel.add(lblclutchpos);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));
        
        lblWheelpos.setFont(font);
        txtDataPanel.add(lblWheelpos);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblGear.setFont(font);
        txtDataPanel.add(lblGear);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblxacc.setFont(font);
        txtDataPanel.add(lblxacc);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblyacc.setFont(font);
        txtDataPanel.add(lblyacc);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        // Electronic
        lblelectronics.setForeground(red);
        txtDataPanel.add(lblelectronics);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        lblbattvolt.setFont(font);
        txtDataPanel.add(lblbattvolt);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        // ECU
        lblecu.setForeground(red);
        txtDataPanel.add(lblecu);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, bigspace)));

        lblinjectorTime.setFont(font);
        txtDataPanel.add(lblinjectorTime);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblignitionTime.setFont(font);
        txtDataPanel.add(lblignitionTime);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));

        lblengload.setFont(font);
        txtDataPanel.add(lblengload);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));
        
        lbltriggererr.setFont(font);
        txtDataPanel.add(lbltriggererr);
        txtDataPanel.add(Box.createRigidArea(new Dimension(0, smallspace)));


    }

    public JPanel getTxtDataPanel()
    {
        return txtDataPanel;
    }

    public void setData(int datanr, int data)
    {
        switch(datanr)
        {
            case 1: lbltime.setText("Time: " + data); lbltime.repaint(); break;
            case 2: lblSpeed.setText("Speed: " + data + " km/h"); lblSpeed.repaint(); break;
            case 3: lblrpm.setText("RPM: " + data); lblrpm.repaint(); break;
            case 4: lblwatertempin.setText("Water temp in: " + data + "\u00b0"); lblwatertempin.repaint(); break;
            case 5: lblwatertempout.setText("Water temp out: " + data + "\u00b0"); lblwatertempout.repaint(); break;
            case 6: lbloiltemp.setText("Oil temp: " + data + "\u00b0"); lbloiltemp.repaint(); break;
            case 7: if(data>100){lbloilpress.setText("Oil press: OK");} else{lbloilpress.setText("Oil press: LOW!");} lbloilpress.repaint(); break;
            case 8: lblmanifoldtemp.setText("Manifold temp: " + data + "\u00b0"); lblmanifoldtemp.repaint(); break;
            case 9: lblmanifoldpress.setText("Manifold press: " + data + " mBar"); lblmanifoldpress.repaint(); break;
            case 10: lbllambda.setText("Lambda: " + (double)data/100); lbllambda.repaint(); break;
            case 11: if(data>0){lblthrottlepos.setText("Throttle pos: " + data + "%");} else{lblthrottlepos.setText("Throttle pos: 0%");} lblthrottlepos.repaint(); break;
            case 15: if(data<100){lblGear.setText("Gear: N");} else{lblGear.setText("Gear: -");} lblGear.repaint(); break;
            case 16: lblxacc.setText("X-acc: " + (double)data/10 + " g"); lblxacc.repaint(); break;
            case 17: lblyacc.setText("Y-acc: " + (double)data/10 + " g"); lblyacc.repaint(); break;
            case 18: lblbattvolt.setText("Batt voltage: " + (double)data/10 + " V"); lblbattvolt.repaint(); break;
            case 19: lblinjectorTime.setText("Injector-Time: " + data); lblinjectorTime.repaint(); break;
            case 20: lblignitionTime.setText("Ignition-Time: " + data); lblignitionTime.repaint(); break;
            case 21: lblengload.setText("Load: " + data); lblengload.repaint(); break;
            case 22: lbltriggererr.setText("Trigger Error: " + data); lbltriggererr.repaint(); break;
        }
    }
    
}
