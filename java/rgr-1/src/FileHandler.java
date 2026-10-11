import java.io.*;
import java.util.ArrayList;
import java.util.List;

public class FileHandler {
    public List<Double> readValuesFromFile(String filename) throws IOException {
        List<Double> values = new ArrayList<>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        String line;
        while ((line = reader.readLine()) != null) {
            String[] splitValues = line.split(" ");
            values.add(Double.parseDouble(splitValues[0]));
            values.add(Double.parseDouble(splitValues[1]));
        }
        reader.close();
        return values;
    }

    public void writeValuesToFile(String filename, List<Double> xValues, List<Double> yValues) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
        for (int i = 0; i < xValues.size(); i++) {
            writer.write(xValues.get(i) + " " + yValues.get(i));
            writer.newLine();
        }
        writer.close();
    }

    public void saveResults(String filename, List<Double> xValues, List<Double> yValues, List<Double> derivatives)
            throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(filename));
        for (int i = 0; i < xValues.size(); i++) {
            writer.write(xValues.get(i) + "," + yValues.get(i) + "," + derivatives.get(i));
            writer.newLine();
        }
        writer.close();
    }
}
