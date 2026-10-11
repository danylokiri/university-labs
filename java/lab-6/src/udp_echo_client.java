import java.net.*;
import java.util.Scanner;

class udp_echo_client {
    private static final String SERVER_ADDRESS = "localhost";
    private static final int SERVER_PORT = 9876;

    public static void main(String[] args) {
        try (DatagramSocket socket = new DatagramSocket(); Scanner scanner = new Scanner(System.in)) {
            InetAddress serverAddress = InetAddress.getByName(SERVER_ADDRESS);

            while (true) {
                System.out.print("Enter message: ");
                String message = scanner.nextLine();
                if (message.equalsIgnoreCase("exit")) {
                    break;
                }

                byte[] requestBytes = message.getBytes();
                DatagramPacket requestPacket = new DatagramPacket(requestBytes, requestBytes.length, serverAddress,
                        SERVER_PORT);
                socket.send(requestPacket);

                byte[] buffer = new byte[1024];
                DatagramPacket responsePacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(responsePacket);

                String response = new String(responsePacket.getData(), 0, responsePacket.getLength());
                System.out.println("Echo from server: " + response);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
