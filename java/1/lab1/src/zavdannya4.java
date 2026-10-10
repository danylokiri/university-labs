import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.util.Scanner;

class function_not_found_exception extends Exception {
    public function_not_found_exception(String message) {
        super(message);
    }
}

public class zavdannya4 {
    public static Object call_method(Object obj, String methodName, Object... params)
            throws function_not_found_exception {
        Class<?> objClass = obj.getClass();
        Method[] methods = objClass.getMethods();

        for (Method method : methods) {
            if (method.getName().equals(methodName)) {
                try {
                    return method.invoke(obj, params);
                } catch (Exception e) {
                    throw new function_not_found_exception("Error invoking method: " + e.getMessage());
                }
            }
        }

        throw new function_not_found_exception("Method '" + methodName + "' not found in object " + obj);
    }

    public static Object create_array(Class<?> type, int size) {
        return Array.newInstance(type, size);
    }

    public static Object create_matrix(Class<?> type, int rows, int cols) {
        return Array.newInstance(type, rows, cols);
    }

    public static Object resize_array(Object array, int newSize) {
        int length = Array.getLength(array);
        Object newArray = Array.newInstance(array.getClass().getComponentType(), newSize);
        System.arraycopy(array, 0, newArray, 0, Math.min(length, newSize));
        return newArray;
    }

    public static Object resize_matrix(Object matrix, int newRows, int newCols) {
        int rows = Array.getLength(matrix);
        int cols = Array.getLength(Array.get(matrix, 0));
        Object newMatrix = Array.newInstance(matrix.getClass().getComponentType().getComponentType(), newRows, newCols);

        for (int i = 0; i < Math.min(rows, newRows); i++) {
            System.arraycopy(Array.get(matrix, i), 0, Array.get(newMatrix, i), 0, Math.min(cols, newCols));
        }
        return newMatrix;
    }

    public static String array_to_string(Object array) {
        int length = Array.getLength(array);
        StringBuilder sb = new StringBuilder("[");
        for (int i = 0; i < length; i++) {
            sb.append(Array.get(array, i));
            if (i < length - 1) {
                sb.append(", ");
            }
        }
        sb.append("]");
        return sb.toString();
    }

    public static String matrix_to_string(Object matrix) {
        int rows = Array.getLength(matrix);
        StringBuilder sb = new StringBuilder("[\n");
        for (int i = 0; i < rows; i++) {
            sb.append(array_to_string(Array.get(matrix, i))).append("\n");
        }
        sb.append("]");
        return sb.toString();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Enter array size: ");
        int arraySize = scanner.nextInt();
        Integer[] intArray = (Integer[]) create_array(Integer.class, arraySize);

        for (int i = 0; i < arraySize; i++) {
            System.out.print("Enter value for intArray[" + i + "]: ");
            intArray[i] = scanner.nextInt();
        }
        System.out.println("Original array: " + array_to_string(intArray));

        System.out.print("Enter new array size: ");
        int newArraySize = scanner.nextInt();
        intArray = (Integer[]) resize_array(intArray, newArraySize);
        System.out.println("Resized array: " + array_to_string(intArray));

        System.out.print("Enter number of rows for matrix: ");
        int rows = scanner.nextInt();
        System.out.print("Enter number of columns for matrix: ");
        int cols = scanner.nextInt();
        Integer[][] intMatrix = (Integer[][]) create_matrix(Integer.class, rows, cols);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                System.out.print("Enter value for intMatrix[" + i + "][" + j + "]: ");
                intMatrix[i][j] = scanner.nextInt();
            }
        }
        System.out.println("Original matrix: " + matrix_to_string(intMatrix));

        System.out.print("Enter new number of rows for matrix: ");
        int newRows = scanner.nextInt();
        System.out.print("Enter new number of columns for matrix: ");
        int newCols = scanner.nextInt();
        intMatrix = (Integer[][]) resize_matrix(intMatrix, newRows, newCols);
        System.out.println("Resized matrix: " + matrix_to_string(intMatrix));

        scanner.close();
    }
}
