/*
 * Data kommer ind gennem tcp-socket som streng adskilt med kolon
 * Denne data sendes videre til dataStore
 */

package GUI_telemetry;

import java.net.*;
import java.io.*;

public class socketIn extends Thread{

    private Socket socket = null;
    private dataStore datastore;
    private String[] dataArray;

    // Constructor
    public socketIn(Socket socket, dataStore datastore)
    {
        this.socket = socket;
        this.datastore = datastore;
    }

    // Socket tråden
    public void run()
    {
        try
        {
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String inputLine;

            while ((inputLine = in.readLine()) != null)
            {
                datastore.setData(inputLine);
            }
            in.close();
            socket.close();
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }

    }

}
