import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.function.DoubleUnaryOperator;

public class zavdannya1 {

    interface function {
        double calculate(double x);
    }

    static class analytical_function implements function {
        private final DoubleUnaryOperator function;

        public analytical_function(DoubleUnaryOperator function) {
            this.function = function;
        }

        @Override
        public double calculate(double x) {
            return function.applyAsDouble(x);
        }
    }

    static class tabulated_function implements function {
        private final List<Double> x_values;
        private final List<Double> y_values;

        public tabulated_function(List<Double> x_values, List<Double> y_values) {
            this.x_values = x_values;
            this.y_values = y_values;
        }

        @Override
        public double calculate(double x) {
            for (int i = 0; i < x_values.size() - 1; i++) {
                if (x >= x_values.get(i) && x <= x_values.get(i + 1)) {
                    double t = (x - x_values.get(i)) / (x_values.get(i + 1) - x_values.get(i));
                    return y_values.get(i) * (1 - t) + y_values.get(i + 1) * t;
                }
            }
            return Double.NaN;
        }
    }

    public static double differentiate(function function, double x, double h) {
        return (function.calculate(x + h) - function.calculate(x - h)) / (2 * h);
    }

    public static void save_results(String filename, List<Double> x_values, List<Double> y_values,
            List<Double> derivatives) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
        for (int i = 0; i < x_values.size(); i++) {
            writer.write(x_values.get(i) + "," + y_values.get(i) + "," + derivatives.get(i));
            writer.newLine();
        }
        writer.close();
    }

    public static List<Double> read_values_from_file(String filename) throws IOException {
        List<Double> x_values = new ArrayList<>();
        List<Double> y_values = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = reader.readLine()) != null) {
            String[] values = line.split(" ");
            x_values.add(Double.parseDouble(values[0]));
            y_values.add(Double.parseDouble(values[1]));
        }
        reader.close();
        List<Double> result = new ArrayList<>();
        result.addAll(x_values);
        result.addAll(y_values);
        return result;
    }

    public static void write_values_to_file(String filename, List<Double> x_values, List<Double> y_values)
            throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
        for (int i = 0; i < x_values.size(); i++) {
            writer.write(x_values.get(i) + " " + y_values.get(i));
            writer.newLine();
        }
        writer.close();
    }

    public static void main(String[] args) {
        try {
            List<Double> x_values = new ArrayList<>();
            List<Double> y_values = new ArrayList<>();
            List<Double> derivatives = new ArrayList<>();
            double h = 0.05;

            function func1 = new analytical_function(x -> Math.exp(-x * x) * Math.sin(x));
            for (double x = 1.5; x <= 6.5; x += 0.05) {
                x_values.add(x);
                y_values.add(func1.calculate(x));
                derivatives.add(differentiate(func1, x, h));
            }
            save_results("results_func1.txt", x_values, y_values, derivatives);
            System.out.println("Results for analytical function saved to results_func1.txt");

            double[] a_values = { 0.5, 1.0, 1.5 };
            for (double a : a_values) {
                function func2 = new analytical_function(x -> Math.exp(-a * x * x) * Math.sin(x));
                x_values.clear();
                y_values.clear();
                derivatives.clear();
                for (double x = 1.5; x <= 6.5; x += 0.05) {
                    x_values.add(x);
                    y_values.add(func2.calculate(x));
                    derivatives.add(differentiate(func2, x, h));
                }
                save_results("results_func2_a_" + a + ".txt", x_values, y_values, derivatives);
                System.out.println("Results for function with a = " + a + " saved to results_func2_a_" + a + ".txt");
            }

            List<Double> values_from_file = read_values_from_file("xy_values.txt");
            List<Double> tabulated_x = values_from_file.subList(0, values_from_file.size() / 2);
            List<Double> tabulated_y = values_from_file.subList(values_from_file.size() / 2, values_from_file.size());
            function tabulated_func = new tabulated_function(tabulated_x, tabulated_y);
            x_values.clear();
            y_values.clear();
            derivatives.clear();
            for (double x : tabulated_x) {
                y_values.add(tabulated_func.calculate(x));
                derivatives.add(differentiate(tabulated_func, x, h));
            }
            save_results("results_tabulated.txt", tabulated_x, y_values, derivatives);
            System.out.println("Results for tabulated function saved to results_tabulated.txt");

        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
