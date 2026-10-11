import java.lang.reflect.Method;

class FunctionNotFoundException extends Exception {
    public FunctionNotFoundException(String message) {
        super(message);
    }
}

public class zavdannya3 {
    public static Object callMethod(Object obj, String methodName, Object... params) throws FunctionNotFoundException {
        Class<?> objClass = obj.getClass();
        Method[] methods = objClass.getMethods();

        for (Method method : methods) {
            if (method.getName().equals(methodName)) {
                try {
                    return method.invoke(obj, params);
                } catch (Exception e) {
                    throw new FunctionNotFoundException("Error invoking method: " + e.getMessage());
                }
            }
        }

        throw new FunctionNotFoundException("Method '" + methodName + "' not found in object " + obj);
    }

    public static void main(String[] args) {
        class TestClass {
            public int testMethod(int x, int y) {
                return x + y;
            }
        }

        TestClass obj = new TestClass();

        // Прямий виклик методу для демонстрації
        int directResult = obj.testMethod(5, 3);
        System.out.println("Direct call result: " + directResult);

        // Виклик методу через рефлексію
        try {
            Object result = callMethod(obj, "testMethod", 5, 3);
            System.out.println("Reflection call result: " + result);
        } catch (FunctionNotFoundException e) {
            System.out.println(e.getMessage());
        }
    }
}
