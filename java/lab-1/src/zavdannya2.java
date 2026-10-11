import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class zavdannya2 {

    public static String analyze_object(Object obj) {
        Class<?> clazz = obj.getClass();
        StringBuilder analysis = new StringBuilder();
        analysis.append("Class: ").append(clazz.getName()).append("\n");

        analysis.append("Fields:\n");
        Field[] fields = clazz.getDeclaredFields();
        for (Field field : fields) {
            analysis.append("  ").append(Modifier.toString(field.getModifiers())).append(" ")
                    .append(field.getType().getName()).append(" ").append(field.getName()).append(" = ");
            try {
                field.setAccessible(true);
                analysis.append(field.get(obj));
            } catch (IllegalAccessException e) {
                analysis.append("Could not access");
            } catch (Exception e) {
                analysis.append("Access not allowed");
            }
            analysis.append("\n");
        }

        analysis.append("Methods:\n");
        Method[] methods = clazz.getDeclaredMethods();
        for (Method method : methods) {
            if (Modifier.isPublic(method.getModifiers()) && method.getParameterCount() == 0) {
                analysis.append("  ").append(Modifier.toString(method.getModifiers())).append(" ")
                        .append(method.getReturnType().getName()).append(" ").append(method.getName()).append("()\n");
            }
        }

        return analysis.toString();
    }

    public static void invoke_method(Object obj, String methodName) {
        try {
            Method method = obj.getClass().getMethod(methodName);
            Object result = method.invoke(obj);
            System.out.println("Result of " + methodName + ": " + result);
        } catch (Exception e) {
            System.out.println("Could not invoke method: " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        ArrayList<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(2);
        list.add(3);

        System.out.println(analyze_object(list));

        Scanner scanner = new Scanner(System.in);
        List<String> methodNames = new ArrayList<>();

        Method[] methods = list.getClass().getDeclaredMethods();
        for (Method method : methods) {
            if (Modifier.isPublic(method.getModifiers()) && method.getParameterCount() == 0) {
                methodNames.add(method.getName());
            }
        }

        System.out.println("Available methods to invoke:");
        for (int i = 0; i < methodNames.size(); i++) {
            System.out.println((i + 1) + ": " + methodNames.get(i));
        }

        System.out.print("Enter the number of the method you want to invoke: ");
        int methodIndex = scanner.nextInt() - 1;

        if (methodIndex >= 0 && methodIndex < methodNames.size()) {
            String methodName = methodNames.get(methodIndex);
            invoke_method(list, methodName);
        } else {
            System.out.println("Invalid method number.");
        }

        scanner.close();
    }
}
