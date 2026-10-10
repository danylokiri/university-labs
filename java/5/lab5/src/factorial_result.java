public class factorial_result implements result {
    private static final long serialVersionUID = 1L;
    private final long value;

    public factorial_result(long value) {
        this.value = value;
    }

    @Override
    public String getResult() {
        return "Factorial result: " + value;
    }
}
