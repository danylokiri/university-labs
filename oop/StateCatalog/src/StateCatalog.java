import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;

abstract class State implements Serializable {
    private static transient int idCounter = 1;

    protected final int id;
    protected String location;
    protected String name;
    protected double area;
    protected int population;

    public State(String location, String name, double area, int population) {
        this.id = idCounter++;
        this.location = location;
        this.name = name;
        this.area = area;
        this.population = population;
    }

    @Override
    public String toString() {
        return "ID: " + id +
                ", Location: " + location +
                ", Name: " + name +
                ", Area: " + area +
                ", Population: " + population;
    }

    private void writeObject(ObjectOutputStream out) throws IOException {
        out.defaultWriteObject();
        out.writeInt(idCounter);
    }

    private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
        in.defaultReadObject();
        idCounter = in.readInt();
    }
}

class Republic extends State {
    private int numberOfDeputies;

    public Republic() {
        super("", "", 0.0, 0);
    }

    public Republic(String location, String name, double area, int population, int numberOfDeputies) {
        super(location, name, area, population);
        this.numberOfDeputies = numberOfDeputies;
    }

    @Override
    public String toString() {
        return super.toString() + ", Type: Republic, Deputies: " + numberOfDeputies;
    }
}

class Monarchy extends State {
    private String monarchName;

    public Monarchy(String location, String name, double area, int population, String monarchName) {
        super(location, name, area, population);
        this.monarchName = monarchName;
    }

    @Override
    public String toString() {
        return super.toString() + ", Type: Monarchy, Monarch: " + monarchName;
    }
}

public class StateCatalog {
    private final ArrayList<State> catalog = new ArrayList<>();

    public void displayCatalog() {
        for (State state : catalog) {
            System.out.println(state);
        }
    }

    public void displayMonarchies() {
        for (State state : catalog) {
            if (state instanceof Monarchy) {
                System.out.println(state);
            }
        }
    }

    public void displayRepublics() {
        for (State state : catalog) {
            if (state instanceof Republic) {
                System.out.println(state);
            }
        }
    }

    public void addState(State state) {
        catalog.add(state);
        System.out.println("State added successfully.");
    }

    public void removeState(int id) {
        boolean removed = catalog.removeIf(state -> state.id == id);
        if (removed) {
            System.out.println("State with ID " + id + " removed successfully.");
        } else {
            System.out.println("No state found with ID " + id + ". Removal failed.");
        }
    }

    public ArrayList<State> searchCatalog(String keyword) {
        ArrayList<State> results = new ArrayList<>();
        for (State state : catalog) {
            if (state.toString().toLowerCase().contains(keyword.toLowerCase())) {
                results.add(state);
            }
        }
        return results;
    }

    public ArrayList<State> searchByLocation(String location) {
        ArrayList<State> results = new ArrayList<>();
        for (State state : catalog) {
            if (state.location.toLowerCase().contains(location.toLowerCase())) {
                results.add(state);
            }
        }
        return results;
    }

    public void serializeCatalog() {
        try (ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("catalog.txt"))) {
            oos.writeObject(catalog);
            System.out.println("Catalog serialized successfully.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @SuppressWarnings("unchecked")
    public void deserializeCatalog() {
        File file = new File("catalog.txt");
        if (!file.exists()) {
            System.out.println("Catalog file does not exist. Creating a new one.");
            return;
        }

        try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file))) {
            catalog.addAll((ArrayList<State>) ois.readObject());
            System.out.println("Catalog deserialized successfully.");
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        StateCatalog catalog = new StateCatalog();
        catalog.deserializeCatalog(); // Deserialize the catalog from a file (if it exists)

        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("1. Display Catalog");
            System.out.println("2. Display Monarchies");
            System.out.println("3. Display Republics");
            System.out.println("4. Add State");
            System.out.println("5. Remove State");
            System.out.println("6. Search Catalog");
            System.out.println("7. Search by Location"); // Added new option
            System.out.println("8. Exit");

            int choice = scanner.nextInt();
            scanner.nextLine(); // Consume newline

            switch (choice) {
                case 1:
                    catalog.displayCatalog();
                    break;
                case 2:
                    catalog.displayMonarchies();
                    break;
                case 3:
                    catalog.displayRepublics();
                    break;
                case 4:
                    System.out.print("Enter state type (Republic/Monarchy): ");
                    String stateType = scanner.nextLine();
                    System.out.print("Enter location: ");
                    String location = scanner.nextLine();
                    System.out.print("Enter name: ");
                    String name = scanner.nextLine();
                    System.out.print("Enter area: ");
                    double area = scanner.nextDouble();
                    System.out.print("Enter population: ");
                    int population = scanner.nextInt();
                    scanner.nextLine(); // Consume newline

                    if ("Republic".equalsIgnoreCase(stateType)) {
                        System.out.print("Enter number of deputies: ");
                        int deputies = scanner.nextInt();
                        scanner.nextLine(); // Consume newline
                        Republic republic = new Republic(location, name, area, population, deputies);
                        catalog.addState(republic);
                    } else if ("Monarchy".equalsIgnoreCase(stateType)) {
                        System.out.print("Enter monarch name: ");
                        String monarchName = scanner.nextLine();
                        Monarchy monarchy = new Monarchy(location, name, area, population, monarchName);
                        catalog.addState(monarchy);
                    } else {
                        System.out.println("Invalid state type. Please enter Republic or Monarchy.");
                    }
                    break;
                case 5:
                    System.out.print("Enter state ID to remove: ");
                    int removeId = scanner.nextInt();
                    scanner.nextLine(); // Consume newline
                    catalog.removeState(removeId);
                    break;
                case 6:
                    System.out.print("Enter search keyword: ");
                    String keyword = scanner.nextLine();
                    ArrayList<State> searchResults = catalog.searchCatalog(keyword);
                    System.out.println("Search Results:");
                    for (State result : searchResults) {
                        System.out.println(result);
                    }
                    break;
                case 7:
                    System.out.print("Enter location: ");
                    String locationKeyword = scanner.nextLine();
                    ArrayList<State> locationSearchResults = catalog.searchByLocation(locationKeyword);
                    System.out.println("Location Search Results:");
                    for (State result : locationSearchResults) {
                        System.out.println(result);
                    }
                    break;
                case 8:
                    catalog.serializeCatalog(); // Serialize the catalog before exiting
                    System.out.println("Exiting program.");
                    System.exit(0);
                    break;
                default:
                    System.out.println("Invalid choice. Please enter a number between 1 and 8.");
            }
        }
    }
}
