import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.Scanner;

public class dodatzavdannya1 {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        try {
            System.out.print("Enter the fully qualified class name: ");
            String className = scanner.nextLine();
            Class<?> clazz = Class.forName(className);

            Constructor<?>[] constructors = clazz.getConstructors();
            System.out.println("Constructors:");
            for (int i = 0; i < constructors.length; i++) {
                System.out.println(i + ": " + constructors[i]);
            }

            System.out.print("Choose a constructor (enter number): ");
            int constructorIndex = scanner.nextInt();
            Constructor<?> constructor = constructors[constructorIndex];

            Class<?>[] parameterTypes = constructor.getParameterTypes();
            Object[] initArgs = new Object[parameterTypes.length];
            for (int i = 0; i < parameterTypes.length; i++) {
                System.out.print("Enter value for parameter " + parameterTypes[i].getName() + ": ");
                initArgs[i] = scanner.next();
            }

            Object obj = constructor.newInstance(initArgs);
            print_object_state(obj);

            Method[] methods = clazz.getMethods();
            System.out.println("Methods:");
            for (int i = 0; i < methods.length; i++) {
                System.out.println(i + ": " + methods[i]);
            }

            System.out.print("Choose a method (enter number): ");
            int methodIndex = scanner.nextInt();
            Method method = methods[methodIndex];

            parameterTypes = method.getParameterTypes();
            Object[] methodArgs = new Object[parameterTypes.length];
            for (int i = 0; i < parameterTypes.length; i++) {
                System.out.print("Enter value for parameter " + parameterTypes[i].getName() + ": ");
                methodArgs[i] = scanner.next();
            }

            Object result = method.invoke(obj, methodArgs);
            System.out.println("Method result: " + result);
            print_object_state(obj);

        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            scanner.close();
        }
    }

    private static void print_object_state(Object obj) {
        try {
            Field[] fields = obj.getClass().getDeclaredFields();
            System.out.println("Object state:");
            for (Field field : fields) {
                field.setAccessible(true);
                System.out.println(field.getName() + ": " + field.get(obj));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
