import java.io.*;
import java.net.*;
import java.util.Scanner;

class metro_client {
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    public metro_client(String address, int port) {
        try {
            socket = new Socket(address, port);
            out = new PrintWriter(socket.getOutputStream(), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public void sendRequest(String request) {
        out.println(request);
        try {
            System.out.println("Response: " + in.readLine());
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public String getCardInfo(String id) {
        out.println("info " + id);
        try {
            return in.readLine();
        } catch (IOException ex) {
            ex.printStackTrace();
            return null;
        }
    }

    public void closeConnection() {
        try {
            in.close();
            out.close();
            socket.close();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    public static void main(String[] args) {
        metro_client client = new metro_client("localhost", 12345);
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter card ID: ");
        String cardId = scanner.nextLine();

        while (true) {
            System.out.println("\nChoose an operation:");
            System.out.println("1. Register card");
            System.out.println("2. Add funds");
            System.out.println("3. Pay fare (5 units)");
            System.out.println("4. Get card info");
            System.out.println("5. Check balance");
            System.out.println("6. Exit");

            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1:
                    client.sendRequest("register " + cardId);
                    break;
                case 2:
                    System.out.print("Enter amount to add: ");
                    double amount = scanner.nextDouble();
                    client.sendRequest("add " + cardId + " " + amount);
                    break;
                case 3:
                    client.sendRequest("pay " + cardId + " 5");
                    break;
                case 4:
                    String info = client.getCardInfo(cardId);
                    System.out.println("Card Info: " + info);
                    break;
                case 5:
                    client.sendRequest("balance " + cardId);
                    break;
                case 6:
                    client.closeConnection();
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid choice. Please try again.");
            }
        }
    }
}
