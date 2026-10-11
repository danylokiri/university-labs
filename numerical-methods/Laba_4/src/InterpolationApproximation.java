import java.io.File;
import java.io.FileNotFoundException;
import java.util.InputMismatchException;
import java.util.Scanner;

public class InterpolationApproximation {

    // Вихідні дані для кількості дощів по місяцях
    private static final double[] x = {0, 1, 2, 3, 4, 5};
    private static final double[] y = {3, 5, 6, 2, 3, 5};

    // Метод для інтерполяції за допомогою полінома Лагранжа
    public static double lagrangeInterpolation(double xValue) {
        double result = 0;
        for (int i = 0; i < x.length; i++) {
            double term = y[i];
            for (int j = 0; j < x.length; j++) {
                if (i != j) {
                    term *= (xValue - x[j]) / (x[i] - x[j]);
                }
            }
            result += term;
        }
        return result;
    }

    // Метод для апроксимації поліномом першого ступеня (лінійна апроксимація)
    public static double linearApproximation(double xValue) {
        double xSum = 0, ySum = 0, xySum = 0, xSquareSum = 0;
        int n = x.length;
        for (int i = 0; i < n; i++) {
            xSum += x[i];
            ySum += y[i];
            xySum += x[i] * y[i];
            xSquareSum += x[i] * x[i];
        }
        double b = (n * xySum - xSum * ySum) / (n * xSquareSum - xSum * xSum);
        double a = (ySum - b * xSum) / n;
        return a + b * xValue;
    }

    // Метод для апроксимації поліномом другого ступеня (квадратична апроксимація)
    public static double quadraticApproximation(double xValue) {
        int n = x.length;
        double sumX = 0, sumX2 = 0, sumX3 = 0, sumX4 = 0;
        double sumY = 0, sumXY = 0, sumX2Y = 0;

        for (int i = 0; i < n; i++) {
            sumX += x[i];
            sumX2 += x[i] * x[i];
            sumX3 += x[i] * x[i] * x[i];
            sumX4 += x[i] * x[i] * x[i] * x[i];
            sumY += y[i];
            sumXY += x[i] * y[i];
            sumX2Y += x[i] * x[i] * y[i];
        }

        double[][] matrix = {
                {n, sumX, sumX2, sumY},
                {sumX, sumX2, sumX3, sumXY},
                {sumX2, sumX3, sumX4, sumX2Y}
        };

        double[] result = gaussianElimination(matrix);
        return result[0] + result[1] * xValue + result[2] * xValue * xValue;
    }

    // Метод для апроксимації поліномом третього ступеня (кубічна апроксимація)
    public static double cubicApproximation(double xValue) {
        int n = x.length;
        double sumX = 0, sumX2 = 0, sumX3 = 0, sumX4 = 0, sumX5 = 0, sumX6 = 0;
        double sumY = 0, sumXY = 0, sumX2Y = 0, sumX3Y = 0;

        for (int i = 0; i < n; i++) {
            sumX += x[i];
            sumX2 += x[i] * x[i];
            sumX3 += x[i] * x[i] * x[i];
            sumX4 += x[i] * x[i] * x[i] * x[i];
            sumX5 += x[i] * x[i] * x[i] * x[i] * x[i];
            sumX6 += x[i] * x[i] * x[i] * x[i] * x[i] * x[i];
            sumY += y[i];
            sumXY += x[i] * y[i];
            sumX2Y += x[i] * x[i] * y[i];
            sumX3Y += x[i] * x[i] * x[i] * y[i];
        }

        double[][] matrix = {
                {n, sumX, sumX2, sumX3, sumY},
                {sumX, sumX2, sumX3, sumX4, sumXY},
                {sumX2, sumX3, sumX4, sumX5, sumX2Y},
                {sumX3, sumX4, sumX5, sumX6, sumX3Y}
        };

        double[] result = gaussianElimination(matrix);
        return result[0] + result[1] * xValue + result[2] * xValue * xValue + result[3] * xValue * xValue * xValue;
    }

    // Метод для розв'язку системи лінійних рівнянь методом Гаусса
    private static double[] gaussianElimination(double[][] matrix) {
        int n = matrix.length;
        for (int i = 0; i < n; i++) {
            // Прямий хід
            for (int j = i + 1; j < n; j++) {
                double factor = matrix[j][i] / matrix[i][i];
                for (int k = i; k <= n; k++) {
                    matrix[j][k] -= factor * matrix[i][k];
                }
            }
        }

        double[] result = new double[n];
        for (int i = n - 1; i >= 0; i--) {
            result[i] = matrix[i][n] / matrix[i][i];
            for (int j = i - 1; j >= 0; j--) {
                matrix[j][n] -= matrix[j][i] * result[i];
            }
        }
        return result;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Виберіть спосіб введення даних (1 - консоль, 2 - файл): ");
        int inputChoice = scanner.nextInt();
        double xValue = 0;
        boolean validInput = false;

        if (inputChoice == 1) {
            System.out.println("Введіть значення для інтерполяції або апроксимації:");
            xValue = scanner.nextDouble();
            validInput = true;
        } else if (inputChoice == 2) {
            System.out.println("Введіть шлях до файлу:");
            String filePath = scanner.next();
            try {
                Scanner fileScanner = new Scanner(new File(filePath));
                xValue = fileScanner.nextDouble();
                validInput = true;
                fileScanner.close();
            } catch (FileNotFoundException e) {
                System.out.println("Помилка: файл не знайдено.");
            } catch (InputMismatchException e) {
                System.out.println("Помилка: некоректні дані у файлі.");
            }
        } else {
            System.out.println("Помилка: некоректний вибір способу введення.");
        }

        if (validInput) {
            System.out.println("Лагранжова інтерполяція: " + lagrangeInterpolation(xValue));
            System.out.println("Лінійна апроксимація: " + linearApproximation(xValue));
            System.out.println("Квадратична апроксимація: " + quadraticApproximation(xValue));
            System.out.println("Кубічна апроксимація: " + cubicApproximation(xValue));
        }
    }
}
