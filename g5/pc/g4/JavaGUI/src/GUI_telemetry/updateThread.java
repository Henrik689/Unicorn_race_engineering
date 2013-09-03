 package GUI_telemetry;

import java.util.Random;

public class updateThread extends Thread
{
    // Data
    private dataStore datastore;
    private Random generator = new Random();
    private int speed = 0;

    // Constructor
    public updateThread(dataStore datastore)
    {
       this.datastore = datastore;
    }

    // Socket tråden
    public void run()
    {
        while(true)
        {
            try
            {
                sleep(70); // Hastighed hvormed ny data kommer
                int randomIndex = generator.nextInt(11); // Random tal
                int randomDatanr = generator.nextInt(5); // Random tal

                //datastore.setData(randomDatanr, randomIndex);
                //datastore.setData(5, speed);

                speed = speed + 1;

                if(speed > 120) speed = 0;
                
            }
            catch (InterruptedException e)
            {
            }
        }
    }
}