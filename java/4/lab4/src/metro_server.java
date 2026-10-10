import java.io.*;
import java.net.*;
import java.util.*;

class metro_server {
    private static Map<String, Double> cardDatabase = new HashMap<>();

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
            try (BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                    PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {

                String request;
                while ((request = in.readLine()) != null) {
                    String[] parts = request.split(" ");
                    String operation = parts[0];
                    String id = parts.length > 1 ? parts[1] : null;
                    double amount = parts.length > 2 ? Double.parseDouble(parts[2]) : 0;

                    String response = handleRequest(operation, id, amount);
                    out.println(response);
                }
            } catch (IOException ex) {
                ex.printStackTrace();
            }
        }

        private String handleRequest(String operation, String id, double amount) {
            switch (operation) {
                case "register":
                    return registerCard(id);
                case "info":
                    return getCardInfo(id);
                case "add":
                    return addBalance(id, amount);
                case "pay":
                    return payFare(id, amount);
                case "balance":
                    return getBalance(id);
                default:
                    return "Unknown operation";
            }
        }

        private String registerCard(String id) {
            if (cardDatabase.containsKey(id)) {
                return "Card already registered";
            }
            cardDatabase.put(id, 0.0);
            return "Card registered";
        }

        private String getCardInfo(String id) {
            if (cardDatabase.containsKey(id)) {
                return "Card ID: " + id + ", Balance: " + cardDatabase.get(id);
            }
            return "Card not found";
        }

        private String addBalance(String id, double amount) {
            if (cardDatabase.containsKey(id)) {
                double newBalance = cardDatabase.get(id) + amount;
                cardDatabase.put(id, newBalance);
                return "New balance: " + newBalance;
            }
            return "Card not found";
        }

        private String payFare(String id, double amount) {
            if (cardDatabase.containsKey(id)) {
                double currentBalance = cardDatabase.get(id);
                if (currentBalance >= amount) {
                    double newBalance = currentBalance - amount;
                    cardDatabase.put(id, newBalance);
                    return "Fare paid, new balance: " + newBalance;
                } else {
                    return "Insufficient funds";
                }
            }
            return "Card not found";
        }

        private String getBalance(String id) {
            if (cardDatabase.containsKey(id)) {
                return "Balance: " + cardDatabase.get(id);
            }
            return "Card not found";
        }
    }
}
