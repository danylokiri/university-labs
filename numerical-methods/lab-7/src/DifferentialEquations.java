import java.io.*;
import java.util.Scanner;

public class DifferentialEquations {
    // Оголошуємо функцію y'(x, y)
    public static double dydx(double x, double y) {
        return 0.2 * Math.pow(x, 2) + 0.3 * (Math.pow(x, 3) + y);
    }
    // Метод Ейлера
    public static void eulerMethod(double x0, double y0, double h, double xMax) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Метод Ейлера:");
        double x = x0;
        double y = y0;

        while (x <= xMax) {
            System.out.printf("x = %.2f, y = %.4f%n", x, y);
            y += h * dydx(x, y); // Формула методу Ейлера
            x += h;
        }
    }

    // Метод Рунге-Кутта 4-го порядку
    public static void rungeKuttaMethod(double x0, double y0, double h, double xMax) {
        System.out.println("Метод Рунге-Кутта:");
        double x = x0;
        double y = y0;

        while (x <= xMax) {
            System.out.printf("x = %.2f, y = %.4f%n", x, y);

            // Обчислення коефіцієнтів k1, k2, k3, k4
            double k1 = h * dydx(x, y);
            double k2 = h * dydx(x + h / 2.0, y + k1 / 2.0);
            double k3 = h * dydx(x + h / 2.0, y + k2 / 2.0);
            double k4 = h * dydx(x + h, y + k3);

            // Формула Рунге-Кутта
            y += (k1 + 2 * k2 + 2 * k3 + k4) / 6.0;
            x += h;
        }
    }

    public static void main(String[] args) {
        double x0 = 0;  // Початкова умова x(0)
        double y0 = 0;  // Початкова умова y(0)
        double h = 0.05; // Крок
        double xMax = 2; // Діапазон [0; 2]

        // Виклик методу Ейлера
        eulerMethod(x0, y0, h, xMax);
        System.out.println();

        // Виклик методу Рунге-Кутта
        rungeKuttaMethod(x0, y0, h, xMax);
    }
}
