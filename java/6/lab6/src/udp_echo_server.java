import java.net.*;

class udp_echo_server {
    private static final int PORT = 9876;

    public static void main(String[] args) {
        try (DatagramSocket socket = new DatagramSocket(PORT)) {
            System.out.println("Echo server is listening on port " + PORT);
            byte[] buffer = new byte[1024];

            while (true) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);

                String received = new String(packet.getData(), 0, packet.getLength());
                System.out.println("Received: " + received);

                InetAddress address = packet.getAddress();
                int port = packet.getPort();
                DatagramPacket responsePacket = new DatagramPacket(packet.getData(), packet.getLength(), address, port);
                socket.send(responsePacket);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}
