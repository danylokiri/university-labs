import java.io.*;
import java.util.Scanner;

public class NumericalIntegration {
    // Функція, яку будемо інтегрувати
    public static double function(double x) {
        return (x * x - 1.3) / (1.2 * x * x * x + 1);
    }

    // Метод прямокутників (метод середніх прямокутників)
    public static double rectangleMethod(double a, double b, int n) {
        double h = (b - a) / n;
        double result = 0;
        for (int i = 0; i < n; i++) {
            double x = a + h * (i + 0.5); // Середня точка
            result += function(x);
        }
        return result * h;
    }

    // Метод трапецій
    public static double trapezoidalMethod(double a, double b, int n) {
        double h = (b - a) / n;
        double result = (function(a) + function(b)) / 2.0;
        for (int i = 1; i < n; i++) {
            result += function(a + i * h);
        }
        return result * h;
    }

    // Метод Сімпсона
    public static double simpsonMethod(double a, double b, int n) {
        if (n % 2 != 0) n++; // n має бути парним для методу Сімпсона
        double h = (b - a) / n;
        double result = function(a) + function(b);

        for (int i = 1; i < n; i += 2) {
            result += 4 * function(a + i * h);
        }
        for (int i = 2; i < n - 1; i += 2) {
            result += 2 * function(a + i * h);
        }

        return result * h / 3.0;
    }

    // Метод для зчитування даних з файлу
    public static double[] readDataFromFile(String filename) {
        try (Scanner fileScanner = new Scanner(new File(filename))) {
            double a = fileScanner.nextDouble();
            double b = fileScanner.nextDouble();
            return new double[]{a, b};
        } catch (FileNotFoundException e) {
            System.out.println("Файл не знайдено.");
        } catch (Exception e) {
            System.out.println("Помилка при зчитуванні даних з файлу.");
        }
        return null;
    }

    // Головний метод для запуску програми
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        double a = 0, b = 0;
        int n = 10; // Кількість відрізків

        System.out.println("Виберіть спосіб введення даних:");
        System.out.println("1. Ввести з клавіатури");
        System.out.println("2. Зчитати з файлу");
        int choice = scanner.nextInt();

        if (choice == 1) {
            // Введення даних з клавіатури
            try {
                System.out.println("Введіть нижню межу інтегралу (a): ");
                a = scanner.nextDouble();
                System.out.println("Введіть верхню межу інтегралу (b): ");
                b = scanner.nextDouble();
            } catch (Exception e) {
                System.out.println("Помилка введення. Будь ласка, введіть коректні числові значення.");
                return;
            }
        } else if (choice == 2) {
            // Зчитування даних з файлу
            System.out.println("Введіть назву файлу: ");
            String filename = scanner.next();
            double[] data = readDataFromFile(filename);
            if (data == null) {
                System.out.println("Не вдалося зчитати дані з файлу.");
                return;
            }
            a = data[0];
            b = data[1];
        } else {
            System.out.println("Неправильний вибір.");
            return;
        }

        // Перевірка на помилки введення
        if (n <= 0 || a >= b) {
            System.out.println("Неправильні вхідні дані: переконайтесь, що n > 0 та a < b.");
            return;
        }

        // Обчислення за допомогою кожного з методів
        double rectangleResult = rectangleMethod(a, b, n);
        double trapezoidalResult = trapezoidalMethod(a, b, n);
        double simpsonResult = simpsonMethod(a, b, n);

        // Виведення результатів
        System.out.printf("Метод прямокутників: %.5f%n", rectangleResult);
        System.out.printf("Метод трапецій: %.5f%n", trapezoidalResult);
        System.out.printf("Метод Сімпсона: %.5f%n", simpsonResult);
    }
}
