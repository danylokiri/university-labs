import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class NonlinearEquationSolver {

    // Функція f(x) = x^3 - 4.1x^2 + 2.2x + 1.4
    public static double f(double x) {
        return Math.pow(x, 3) - 4.1 * Math.pow(x, 2) + 2.2 * x + 1.4;
    }

    // Метод дихотомії
    public static double dichotomyMethod(double a, double b, double epsilon) {
        System.out.println("\nМетод дихотомії:");
        while ((b - a) / 2 > epsilon) {
            double c = (a + b) / 2;
            if (f(c) == 0) return c;
            if (f(a) * f(c) < 0) b = c;
            else a = c;
            System.out.printf("a = %.6f, b = %.6f, c = %.6f, f(c) = %.6f%n", a, b, c, f(c));
        }
        return (a + b) / 2;
    }

    // Метод золотого перетину
    public static double goldenSectionMethod(double a, double b, double epsilon) {
        System.out.println("\nМетод золотого перетину:");
        double phi = (1 + Math.sqrt(5)) / 2;
        double resPhi = 2 - phi;

        double x1 = a + resPhi * (b - a);
        double x2 = b - resPhi * (b - a);
        while (Math.abs(b - a) > epsilon) {
            if (f(x1) < f(x2)) {
                b = x2;
                x2 = x1;
                x1 = a + resPhi * (b - a);
            } else {
                a = x1;
                x1 = x2;
                x2 = b - resPhi * (b - a);
            }
            System.out.printf("a = %.6f, b = %.6f, x1 = %.6f, x2 = %.6f, f(x1) = %.6f, f(x2) = %.6f%n",
                    a, b, x1, x2, f(x1), f(x2));
        }
        return (a + b) / 2;
    }

    // Метод Фібоначчі
    public static double fibonacciMethod(double a, double b, double epsilon) {
        System.out.println("\nМетод Фібоначчі:");
        int n = 1;
        while (fibonacci(n) < (b - a) / epsilon) n++;
        double x1 = a + fibonacci(n - 2) * (b - a) / fibonacci(n);
        double x2 = a + fibonacci(n - 1) * (b - a) / fibonacci(n);

        for (int k = 1; k < n; k++) {
            if (f(x1) < f(x2)) {
                b = x2;
                x2 = x1;
                x1 = a + fibonacci(n - k - 2) * (b - a) / fibonacci(n - k);
            } else {
                a = x1;
                x1 = x2;
                x2 = a + fibonacci(n - k - 1) * (b - a) / fibonacci(n - k);
            }
            System.out.printf("a = %.6f, b = %.6f, x1 = %.6f, x2 = %.6f, f(x1) = %.6f, f(x2) = %.6f%n",
                    a, b, x1, x2, f(x1), f(x2));
        }
        return (a + b) / 2;
    }

    // Обчислення чисел Фібоначчі
    public static int fibonacci(int n) {
        if (n <= 1) return n;
        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    // Основний метод
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        double a = -5, b = 7, epsilon = 1e-4;
        boolean validInput = false;

        System.out.println("Виберіть спосіб введення даних (1 - консоль, 2 - файл): ");
        int inputChoice = scanner.nextInt();

        if (inputChoice == 1) {
            System.out.println("Введіть початкові значення інтервалу та точність:");
            while (!validInput) {
                try {
                    System.out.print("Початок інтервалу (a): ");
                    a = scanner.nextDouble();
                    System.out.print("Кінець інтервалу (b): ");
                    b = scanner.nextDouble();
                    System.out.print("Точність (epsilon): ");
                    epsilon = scanner.nextDouble();
                    validInput = true;
                } catch (InputMismatchException e) {
                    System.out.println("Помилка: введіть дійсні числа. Спробуйте ще раз.");
                    scanner.next();
                }
            }
        } else if (inputChoice == 2) {
            System.out.println("Введіть шлях до файлу:");
            String filePath = scanner.next();
            try {
                Scanner fileScanner = new Scanner(new File(filePath));
                a = fileScanner.nextDouble();
                b = fileScanner.nextDouble();
                epsilon = fileScanner.nextDouble();
                validInput = true;
                fileScanner.close();
            } catch (FileNotFoundException e) {
                System.out.println("Помилка: файл не знайдено.");
            } catch (InputMismatchException e) {
                System.out.println("Помилка: некоректні дані у файлі.");
            }
        } else {
            System.out.println("Помилка: некоректний вибір способу введення.");
            return;
        }

        // Якщо введення даних було успішним
        if (validInput) {
            System.out.printf("Інтервал: [%.6f, %.6f], Точність: %.6f%n", a, b, epsilon);

            double dichotomyResult = dichotomyMethod(a, b, epsilon);
            System.out.printf("Результат методу дихотомії: x = %.6f%n", dichotomyResult);

            double goldenSectionResult = goldenSectionMethod(a, b, epsilon);
            System.out.printf("Результат методу золотого перетину: x = %.6f%n", goldenSectionResult);

            double fibonacciResult = fibonacciMethod(a, b, epsilon);
            System.out.printf("Результат методу Фібоначчі: x = %.6f%n", fibonacciResult);
        }
    }
}
