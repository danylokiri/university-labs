import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class Zavdannya2 {

    public static void main(String[] args) {
        try {
            List<Double> xValues = new ArrayList<>();
            List<Double> yValues = new ArrayList<>();
            List<Double> derivatives = new ArrayList<>();
            double h = 0.05;
            Differentiator differentiator = new Differentiator(h);
            FileHandler fileHandler = new FileHandler();

            Function func1 = new AnalyticalFunction(x -> Math.exp(-x * x) * Math.sin(x));
            for (double x = 1.5; x <= 6.5; x += 0.05) {
                xValues.add(x);
                yValues.add(func1.calculate(x));
                derivatives.add(differentiator.differentiate(func1, x));
            }
            fileHandler.saveResults("results_func1.txt", xValues, yValues, derivatives);
            System.out.println("Results for analytical function saved to results_func1.txt");
            printResults("Analytical function f(x) = exp(-x^2) * sin(x)", xValues, yValues, derivatives);

            double[] aValues = { 0.5, 1.0, 1.5 };
            for (double a : aValues) {
                Function func2 = new AnalyticalFunction(x -> Math.exp(-a * x * x) * Math.sin(x));
                xValues.clear();
                yValues.clear();
                derivatives.clear();
                for (double x = 1.5; x <= 6.5; x += 0.05) {
                    xValues.add(x);
                    yValues.add(func2.calculate(x));
                    derivatives.add(differentiator.differentiate(func2, x));
                }
                fileHandler.saveResults("results_func2_a_" + a + ".txt", xValues, yValues, derivatives);
                System.out.println("Results for function with a = " + a + " saved to results_func2_a_" + a + ".txt");
                printResults("Function f(x) = exp(-" + a + " * x^2) * sin(x)", xValues, yValues, derivatives);
            }

            List<Double> valuesFromFile = fileHandler.readValuesFromFile("xy_values.txt");
            List<Double> tabulatedX = valuesFromFile.subList(0, valuesFromFile.size() / 2);
            List<Double> tabulatedY = valuesFromFile.subList(valuesFromFile.size() / 2, valuesFromFile.size());
            Function tabulatedFunc = new TabulatedFunction(tabulatedX, tabulatedY);
            xValues.clear();
            yValues.clear();
            derivatives.clear();
            for (double x : tabulatedX) {
                yValues.add(tabulatedFunc.calculate(x));
                derivatives.add(differentiator.differentiate(tabulatedFunc, x));
            }
            fileHandler.saveResults("results_tabulated.txt", tabulatedX, yValues, derivatives);
            System.out.println("Results for tabulated function saved to results_tabulated.txt");
            printResults("Tabulated function", tabulatedX, yValues, derivatives);

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void printResults(String title, List<Double> xValues, List<Double> yValues,
            List<Double> derivatives) {
        System.out.println(title);
        for (int i = 0; i < xValues.size(); i++) {
            System.out.printf("x: %.2f, y: %.5f, dy/dx: %.5f%n", xValues.get(i), yValues.get(i), derivatives.get(i));
        }
    }
}
