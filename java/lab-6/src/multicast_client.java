import java.net.*;
import java.util.Scanner;
import java.io.IOException;

class multicast_client {
    private static final String MULTICAST_GROUP = "224.0.0.3";
    private static final int PORT = 9876;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter your name: ");
        String userName = scanner.nextLine();

        try (MulticastSocket socket = new MulticastSocket(PORT)) {
            InetAddress group = InetAddress.getByName(MULTICAST_GROUP);
            socket.joinGroup(new InetSocketAddress(group, PORT),
                    NetworkInterface.getByInetAddress(InetAddress.getLocalHost()));

            System.out.println("Multicast client is running...");

            new Thread(() -> {
                try {
                    byte[] buffer = new byte[1024];
                    while (true) {
                        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                        socket.receive(packet);
                        String message = new String(packet.getData(), 0, packet.getLength());
                        System.out.println(message);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }).start();

            while (true) {
                String message = scanner.nextLine();
                if (message.equalsIgnoreCase("exit")) {
                    break;
                }
                message = userName + ": " + message;
                byte[] buffer = message.getBytes();
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, PORT);
                socket.send(packet);
            }

            socket.leaveGroup(new InetSocketAddress(group, PORT),
                    NetworkInterface.getByInetAddress(InetAddress.getLocalHost()));
            scanner.close();

        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
