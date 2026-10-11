public class Differentiator {
    private final double h;

    public Differentiator(double h) {
        this.h = h;
    }

    public double differentiate(Function function, double x) {
        return (function.calculate(x + h) - function.calculate(x - h)) / (2 * h);
    }
}
