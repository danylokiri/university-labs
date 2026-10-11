import java.net.*;
import java.util.*;

class udp_server {
    private static final int PORT = 9876;
    private static Map<String, String> clients = new HashMap<>();

    public static void main(String[] args) {
        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            System.out.println("Server is listening on port " + PORT);
            byte[] buffer = new byte[1024];

            while (true) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);

                String received = new String(packet.getData(), 0, packet.getLength());
                InetAddress address = packet.getAddress();
                int port = packet.getPort();

                if (received.equals("register")) {
                    String clientKey = address.getHostAddress() + ":" + port;
                    clients.put(clientKey, address.getHostAddress() + ":" + port);
                    System.out.println("Registered: " + clientKey);
                }

                String response = "Registered clients:\n" + String.join("\n", clients.values());
                byte[] responseBytes = response.getBytes();

                DatagramPacket responsePacket = new DatagramPacket(responseBytes, responseBytes.length, address, port);
                socket.send(responsePacket);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
