import java.io.*;

class serializable_example implements Serializable {
    private static final long serialVersionUID = 1L;

    private String name;
    private int value;

    public serializable_example(String name, int value) {
        this.name = name;
        this.value = value;
    }

    @Override
    public String toString() {
        return "serializable_example{name='" + name + "', value=" + value + '}';
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }
}

public class dodatzavdannya2 {

    public static void main(String[] args) {
        serializable_example obj = new serializable_example("Test", 42);

        try {
            String serializedString = serialize_object_to_string(obj);
            System.out.println("Serialized string: " + serializedString);

            serializable_example deserializedObj = (serializable_example) deserialize_object_from_string(
                    serializedString);
            System.out.println("Deserialized object: " + deserializedObj);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static String serialize_object_to_string(Serializable obj) throws IOException {
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        ObjectOutputStream objectOutputStream = new ObjectOutputStream(byteArrayOutputStream);
        objectOutputStream.writeObject(obj);
        objectOutputStream.close();
        return byteArrayOutputStream.toString("ISO-8859-1");
    }

    public static Object deserialize_object_from_string(String str) throws IOException, ClassNotFoundException {
        byte[] data = str.getBytes("ISO-8859-1");
        ObjectInputStream objectInputStream = new ObjectInputStream(new ByteArrayInputStream(data));
        Object obj = objectInputStream.readObject();
        objectInputStream.close();
        return obj;
    }
}
