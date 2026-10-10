import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.ArrayList;
import java.util.Arrays;

public class zavdannya1 {

    public static String get_class_description(String className) {
        try {
            Class<?> clazz = Class.forName(className);
            return get_class_description(clazz);
        } catch (ClassNotFoundException e) {
            return "Class not found: " + className;
        }
    }

    public static String get_class_description(Class<?> clazz) {
        StringBuilder description = new StringBuilder();
        Package pkg = clazz.getPackage();
        if (pkg != null) {
            description.append("Package: ").append(pkg.getName()).append("\n");
        }

        description.append("Class: ").append(Modifier.toString(clazz.getModifiers())).append(" ")
                .append(clazz.getName()).append("\n");
        description.append("Superclass: ")
                .append(clazz.getSuperclass() != null ? clazz.getSuperclass().getName() : "None").append("\n");

        Class<?>[] interfaces = clazz.getInterfaces();
        description.append("Interfaces: ").append(interfaces.length > 0 ? Arrays.toString(interfaces) : "None")
                .append("\n");

        Field[] fields = clazz.getDeclaredFields();
        description.append("Fields: ").append(fields.length > 0 ? "\n" : "None\n");
        for (Field field : fields) {
            description.append("  ").append(Modifier.toString(field.getModifiers())).append(" ")
                    .append(field.getType().getName()).append(" ").append(field.getName()).append("\n");
        }

        Constructor<?>[] constructors = clazz.getDeclaredConstructors();
        description.append("Constructors: ").append(constructors.length > 0 ? "\n" : "None\n");
        for (Constructor<?> constructor : constructors) {
            description.append("  ").append(Modifier.toString(constructor.getModifiers())).append(" ")
                    .append(constructor.getName()).append("(");
            Class<?>[] paramTypes = constructor.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++) {
                description.append(paramTypes[i].getName());
                if (i < paramTypes.length - 1) {
                    description.append(", ");
                }
            }
            description.append(")\n");
        }

        Method[] methods = clazz.getDeclaredMethods();
        description.append("Methods: ").append(methods.length > 0 ? "\n" : "None\n");
        for (Method method : methods) {
            description.append("  ").append(Modifier.toString(method.getModifiers())).append(" ")
                    .append(method.getReturnType().getName()).append(" ").append(method.getName()).append("(");
            Class<?>[] paramTypes = method.getParameterTypes();
            for (int i = 0; i < paramTypes.length; i++) {
                description.append(paramTypes[i].getName());
                if (i < paramTypes.length - 1) {
                    description.append(", ");
                }
            }
            description.append(")\n");
        }

        return description.toString();
    }

    public static void main(String[] args) {
        System.out.println(get_class_description("java.util.ArrayList"));
        System.out.println(get_class_description(ArrayList.class));
    }
}
