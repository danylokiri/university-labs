import java.util.List;

public class TabulatedFunction implements Function {
    private final List<Double> xValues;
    private final List<Double> yValues;

    public TabulatedFunction(List<Double> xValues, List<Double> yValues) {
        this.xValues = xValues;
        this.yValues = yValues;
    }

    @Override
    public double calculate(double x) {
        for (int i = 0; i < xValues.size() - 1; i++) {
            if (x >= xValues.get(i) && x <= xValues.get(i + 1)) {
                double t = (x - xValues.get(i)) / (xValues.get(i + 1) - xValues.get(i));
                return yValues.get(i) * (1 - t) + yValues.get(i + 1) * t;
            }
        }
        return Double.NaN;
    }
}
