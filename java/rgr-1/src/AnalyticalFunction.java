import java.util.function.DoubleUnaryOperator;

public class AnalyticalFunction implements Function {
    private final DoubleUnaryOperator function;

    public AnalyticalFunction(DoubleUnaryOperator function) {
        this.function = function;
    }

    @Override
    public double calculate(double x) {
        return function.applyAsDouble(x);
    }
}
