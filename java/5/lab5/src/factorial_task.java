public class factorial_task implements task {
    private static final long serialVersionUID = 1L;
    private final int number;

    public factorial_task(int number) {
        this.number = number;
    }

    @Override
    public result execute() {
        return new factorial_result(computeFactorial(number));
    }

    private long computeFactorial(int num) {
        long result = 1;
        for (int i = 1; i <= num; i++) {
            result *= i;
        }
        return result;
    }
}
