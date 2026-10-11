import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.*;
import java.io.IOException;

class multicast_client_gui {
    private static final String MULTICAST_GROUP = "224.0.0.3";
    private static final int PORT = 9876;
    private static MulticastSocket socket;
    private static String userName;

    public static void main(String[] args) {
        JFrame frame = new JFrame("Multicast Client");
        JTextArea textArea = new JTextArea();
        JTextField textField = new JTextField();
        JButton sendButton = new JButton("Send");

        while (true) {
            userName = JOptionPane.showInputDialog(frame, "Enter your name:", "User Name", JOptionPane.PLAIN_MESSAGE);
            if (userName != null && !userName.trim().isEmpty()) {
                break;
            }
            JOptionPane.showMessageDialog(frame, "Name cannot be empty.", "Error", JOptionPane.ERROR_MESSAGE);
        }

        textArea.setEditable(false);
        frame.setLayout(new BorderLayout());
        frame.add(new JScrollPane(textArea), BorderLayout.CENTER);
        frame.add(textField, BorderLayout.SOUTH);
        frame.add(sendButton, BorderLayout.EAST);
        frame.setSize(400, 300);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        try {
            socket = new MulticastSocket(PORT);
            InetAddress group = InetAddress.getByName(MULTICAST_GROUP);
            socket.joinGroup(new InetSocketAddress(group, PORT),
                    NetworkInterface.getByInetAddress(InetAddress.getLocalHost()));

            new Thread(() -> {
                byte[] buffer = new byte[1024];
                while (true) {
                    try {
                        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                        socket.receive(packet);
                        String message = new String(packet.getData(), 0, packet.getLength());
                        if (!message.startsWith(userName + ":")) {
                            textArea.append(message + "\n");
                        }
                    } catch (IOException ex) {
                        if (socket.isClosed()) {
                            break;
                        }
                        ex.printStackTrace();
                    }
                }
            }).start();

            sendButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    try {
                        String message = textField.getText();
                        if (message.trim().isEmpty()) {
                            return;
                        }
                        String fullMessage = userName + ": " + message;
                        byte[] buffer = fullMessage.getBytes();
                        DatagramPacket packet = new DatagramPacket(buffer, buffer.length, group, PORT);
                        socket.send(packet);
                        textArea.append(message + "\n");
                        textField.setText("");
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            });

            textField.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    sendButton.doClick();
                }
            });

            frame.addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosing(WindowEvent e) {
                    if (socket != null && !socket.isClosed()) {
                        try {
                            socket.leaveGroup(new InetSocketAddress(group, PORT),
                                    NetworkInterface.getByInetAddress(InetAddress.getLocalHost()));
                        } catch (IOException ex) {
                            ex.printStackTrace();
                        }
                        socket.close();
                    }
                }
            });

        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }
}
