import java.util.ArrayList;
import java.util.List;

public class FilterPipeline {
    private final List<ImageFilter> filters = new ArrayList<>();

    public void addFilter(ImageFilter f) {
        filters.add(f);
    }

    public boolean removeFilter(int index) {
        if (index >= 0 && index < filters.size()) {
            filters.remove(index);
            return true;
        }
        return false;
    }

    public void clear() {
        filters.clear();
    }

    public int size() {
        return filters.size();
    }

    public void printFilters() {
        if (filters.isEmpty()) {
            System.out.println("Filters list is empty.");
            return;
        }
        System.out.println("Current filters:");
        for (int i = 0; i < filters.size(); i++) {
            System.out.println(i + ": " + filters.get(i).getName());
        }
    }

    public void applyAll(byte[] pixels, int width, int height) {
        for (ImageFilter f : filters) {
            f.apply(pixels, width, height);
        }
    }
}
