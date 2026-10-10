import java.io.*;
import java.net.*;

class task_client {
    public static void main(String[] args) {
        try (Socket socket = new Socket("localhost", 12345);
                ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream());
                ObjectInputStream in = new ObjectInputStream(socket.getInputStream())) {

            factorial_task task = new factorial_task(20);
            out.writeObject(task);

            result result = (result) in.readObject();
            long duration = in.readLong();

            System.out.println(result.getResult());
            System.out.println("Task executed in " + duration + " milliseconds");
        } catch (IOException | ClassNotFoundException ex) {
            ex.printStackTrace();
        }
    }
}
