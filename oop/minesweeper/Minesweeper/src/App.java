import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class App {
    public static void main(String[] args) throws Exception {
        createInitialSettingsFile();
        Minesweeper minesweeper = new Minesweeper();
    }

    private static void createInitialSettingsFile() {
        String fileName = "size.txt";
        File file = new File(fileName);

        if (!file.exists()) {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter(fileName))) {
                writer.write("8 8 12\n");
            } catch (IOException e) {
                e.printStackTrace();
                System.err.println("Error: Initial settings file could not be created.");
            }
        }
    }
}