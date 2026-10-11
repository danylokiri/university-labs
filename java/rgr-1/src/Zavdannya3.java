import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeMap;
import java.util.function.DoubleUnaryOperator;

public class Zavdannya3 {

    interface Function {
        double calculate(double x);
    }

    static class AnalyticalFunction implements Function {
        private final DoubleUnaryOperator function;

        public AnalyticalFunction(DoubleUnaryOperator function) {
            this.function = function;
        }

        @Override
        public double calculate(double x) {
            return function.applyAsDouble(x);
        }
    }

    static class TabulatedFunction implements Function {
        private final TreeMap<Double, Double> values;

        public TabulatedFunction(TreeMap<Double, Double> values) {
            this.values = values;
        }

        @Override
        public double calculate(double x) {
            Double floorKey = values.floorKey(x);
            Double ceilingKey = values.ceilingKey(x);

            if (floorKey == null || ceilingKey == null || floorKey.equals(ceilingKey)) {
                return values.getOrDefault(floorKey, Double.NaN);
            }

            double y1 = values.get(floorKey);
            double y2 = values.get(ceilingKey);
            double t = (x - floorKey) / (ceilingKey - floorKey);

            return y1 * (1 - t) + y2 * t;
        }
    }

    static class Differentiator {
        private final double h;

        public Differentiator(double h) {
            this.h = h;
        }

        public double differentiate(Function function, double x) {
            return (function.calculate(x + h) - function.calculate(x - h)) / (2 * h);
        }
    }

    static class FileHandler {
        public TreeMap<Double, Double> readCSVFile(String filename) throws IOException {
            TreeMap<Double, Double> values = new TreeMap<>();
            BufferedReader reader = new BufferedReader(new FileReader(filename));
            String line;
            while ((line = reader.readLine()) != null) {
                String[] splitValues = line.split(",");
                values.put(Double.parseDouble(splitValues[0]), Double.parseDouble(splitValues[1]));
            }
            reader.close();
            return values;
        }

        public void writeValuesToFile(String filename, List<Double> xValues, List<Double> yValues) throws IOException {
            BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
            for (int i = 0; i < xValues.size(); i++) {
                writer.write(xValues.get(i) + "," + yValues.get(i));
                writer.newLine();
            }
            writer.close();
        }

        public void saveResults(String filename, List<Double> xValues, List<Double> yValues, List<Double> derivatives)
                throws IOException {
            BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
            for (int i = 0; i < xValues.size(); i++) {
                writer.write(xValues.get(i) + "," + yValues.get(i) + "," + derivatives.get(i));
                writer.newLine();
            }
            writer.close();
        }
    }

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

            // Tabulated function
            TreeMap<Double, Double> valuesFromFile = fileHandler.readCSVFile("xy_values.csv");
            Function tabulatedFunc = new TabulatedFunction(valuesFromFile);
            xValues.clear();
            yValues.clear();
            derivatives.clear();
            for (double x : valuesFromFile.keySet()) {
                yValues.add(tabulatedFunc.calculate(x));
                derivatives.add(differentiator.differentiate(tabulatedFunc, x));
            }
            fileHandler.saveResults("results_tabulated.txt", new ArrayList<>(valuesFromFile.keySet()), yValues,
                    derivatives);
            System.out.println("Results for tabulated function saved to results_tabulated.txt");
            printResults("Tabulated function", new ArrayList<>(valuesFromFile.keySet()), yValues, derivatives);

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
