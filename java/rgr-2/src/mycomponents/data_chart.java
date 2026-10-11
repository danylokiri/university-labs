package mycomponents;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class data_chart extends JPanel {
    private List<Point> points;

    public data_chart() {
        points = new ArrayList<>();
    }

    public void setPoints(List<Point> points) {
        this.points = points;
        repaint();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        int width = getWidth();
        int height = getHeight();
        int scale = 20;

        g2d.setColor(Color.LIGHT_GRAY);
        for (int i = 0; i <= width; i += scale) {
            g2d.drawLine(i, 0, i, height);
        }
        for (int i = 0; i <= height; i += scale) {
            g2d.drawLine(0, i, width, i);
        }

        g2d.setColor(Color.BLACK);
        g2d.drawLine(0, height / 2, width, height / 2);
        g2d.drawLine(width / 2, 0, width / 2, height);

        g2d.setColor(Color.RED);
        for (Point point : points) {
            int x = width / 2 + point.x * scale;
            int y = height / 2 - point.y * scale;
            g2d.fillOval(x - 3, y - 3, 6, 6);
        }
    }
}
