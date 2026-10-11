public class ExampleClass {
    private String name;
    private int value;

    public ExampleClass() {
        this.name = "default";
        this.value = 0;
    }

    public ExampleClass(String name, int value) {
        this.name = name;
        this.value = value;
    }

    public void printDetails() {
        System.out.println("Name: " + name + ", Value: " + value);
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public String getName() {
        return name;
    }

    public int getValue() {
        return value;
    }
}
