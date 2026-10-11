import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class SimpleIterationMethod {
    public static void main(String[] args) {
        double tolerance = 1e-5;
        int maxIterations = 1000;

        // Вхідні дані x0 для початкового наближення
        double[] x = new double[4];

        // Вибір способу введення даних
        Scanner scanner = new Scanner(System.in);
        System.out.println("Виберіть спосіб введення даних (1 - консоль, 2 - файл): ");
        int inputChoice = scanner.nextInt();

        boolean validInput = false;

        if (inputChoice == 1) {
            // Введення даних через консоль
            System.out.println("Введіть початкові значення для x1, x2, x3, x4:");
            while (!validInput) {
                try {
                    for (int i = 0; i < x.length; i++) {
                        x[i] = scanner.nextDouble();
                    }
                    validInput = true;
                } catch (InputMismatchException e) {
                    System.out.println("Помилка: введіть дійсні числа. Спробуйте ще раз.");
                    scanner.next(); // Очищення некоректного введення
                }
            }
        } else if (inputChoice == 2) {
            // Введення даних з файлу
            System.out.println("Введіть шлях до файлу:");
            String filePath = scanner.next();
            try {
                Scanner fileScanner = new Scanner(new File(filePath));
                for (int i = 0; i < x.length; i++) {
                    if (fileScanner.hasNextDouble()) {
                        x[i] = fileScanner.nextDouble();
                    } else {
                        throw new InputMismatchException("Файл містить некоректні дані.");
                    }
                }
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

        // Якщо введення даних було успішним, виконуємо ітерації
        if (validInput) {
            boolean converged = false;
            int iterations = 0;

            while (!converged && iterations < maxIterations) {
                double[] oldX = x.clone();

                x[0] = (42 - x[1] + x[2] - x[3]) / 5;
                x[1] = (8 - x[0] + x[2] + x[3]) / -4;
                x[2] = (1 + x[0] - x[1] - x[3]) / 4;
                x[3] = (16 - x[0] - 2 * x[1] - x[2]) / -5;

                double maxDifference = 0.0;
                for (int i = 0; i < x.length; i++) {
                    maxDifference = Math.max(maxDifference, Math.abs(x[i] - oldX[i]));
                }

                double thickness = calculateThickness(x);
                if (maxDifference < tolerance && thickness >= 17 && thickness <= 50) {
                    converged = true;
                }

                iterations++;
            }

            if (converged) {
                System.out.println("Розв'язок збігся за " + iterations + " ітерацій.");
                System.out.println("x1 = " + x[0]);
                System.out.println("x2 = " + x[1]);
                System.out.println("x3 = " + x[2]);
                System.out.println("x4 = " + x[3]);

                double thickness = calculateThickness(x);
                System.out.println("Товщина виготовленої плівки: " + thickness);
            } else {
                System.out.println("Розв'язок не збігся за максимальну кількість ітерацій.");
            }
        }
    }

    public static double calculateThickness(double[] x) {
        return x[0] + x[1] + x[2] + x[3];
    }
}
