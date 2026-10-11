import java.io.*;
import java.net.*;

class task_server {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(12345)) {
            System.out.println("Server is listening on port 12345");
            while (true) {
                new ClientHandler(serverSocket.accept()).start();
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private static class ClientHandler extends Thread {
        private Socket socket;

        public ClientHandler(Socket socket) {
            this.socket = socket;
        }

        public void run() {
            try (ObjectInputStream in = new ObjectInputStream(socket.getInputStream());
                    ObjectOutputStream out = new ObjectOutputStream(socket.getOutputStream())) {

                task task = (task) in.readObject();
                long startTime = System.currentTimeMillis();
                result result = task.execute();
                long endTime = System.currentTimeMillis();
                long duration = endTime - startTime;

                System.out.println("Task executed in " + duration + " milliseconds");

                out.writeObject(result);
                out.writeLong(duration);
            } catch (IOException | ClassNotFoundException ex) {
                ex.printStackTrace();
            }
        }
    }
}
