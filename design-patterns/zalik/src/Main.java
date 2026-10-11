import javax.imageio.ImageIO;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Enter path to image: ");
        String path = sc.nextLine().trim();

        BufferedImage src;
        try {
            src = ImageIO.read(new File(path));
        } catch (IOException e) {
            System.out.println("Cannot read image: " + e.getMessage());
            return;
        }
        if (src == null) {
            System.out.println("Unsupported image format.");
            return;
        }

        int width = src.getWidth();
        int height = src.getHeight();
        System.out.println("Image loaded. Size: " + width + " x " + height);

        BufferedImage gray = new BufferedImage(width, height, BufferedImage.TYPE_BYTE_GRAY);
        Graphics2D g2 = gray.createGraphics();
        g2.drawImage(src, 0, 0, null);
        g2.dispose();

        DataBufferByte buffer = (DataBufferByte) gray.getRaster().getDataBuffer();
        byte[] pixels = buffer.getData();

        System.out.println("Pixels array length: " + pixels.length);
        if (pixels.length <= 20) {
            System.out.println("First pixels: " + Arrays.toString(pixels));
        }

        FilterPipeline pipeline = new FilterPipeline();

        while (true) {
            System.out.println();
            System.out.println("=== MENU ===");
            System.out.println("1 - Add filter");
            System.out.println("2 - Show filters");
            System.out.println("3 - Delete filter by index");
            System.out.println("4 - Clear filters");
            System.out.println("5 - Apply filters and save");
            System.out.println("0 - Exit");

            String choice = sc.nextLine().trim();

            switch (choice) {
                case "1":
                    addFilterMenu(sc, pipeline);
                    break;
                case "2":
                    pipeline.printFilters();
                    break;
                case "3":
                    removeFilterMenu(sc, pipeline);
                    break;
                case "4":
                    pipeline.clear();
                    System.out.println("All filters removed.");
                    break;
                case "5":
                    applyAndSave(sc, pipeline, pixels, width, height, gray);
                    break;
                case "0":
                    System.out.println("Exit.");
                    return;
                default:
                    System.out.println("Unknown option.");
            }
        }
    }

    private static void addFilterMenu(Scanner sc, FilterPipeline pipeline) {
        System.out.println();
        System.out.println("Choose filter to add:");
        System.out.println("1 - Black & White (threshold)");
        System.out.println("2 - Change brightness");
        System.out.println("3 - Blur (simple box blur)");
        String c = sc.nextLine().trim();

        switch (c) {
            case "1":
                pipeline.addFilter(new BlackWhiteFilter());
                System.out.println("Added: BlackWhiteFilter");
                break;
            case "2":
                System.out.print("Enter brightness delta : ");
                try {
                    int delta = Integer.parseInt(sc.nextLine().trim());
                    pipeline.addFilter(new BrightnessFilter(delta));
                    System.out.println("Added: BrightnessFilter(" + delta + ")");
                } catch (NumberFormatException e) {
                    System.out.println("Not a number.");
                }
                break;
            case "3":
                pipeline.addFilter(new BlurFilter());
                pipeline.addFilter(new BlurFilter());
                pipeline.addFilter(new BlurFilter());
                pipeline.addFilter(new BlurFilter());
                System.out.println("Added: BlurFilter");
                break;
            default:
                System.out.println("Unknown filter.");
        }
    }

    private static void removeFilterMenu(Scanner sc, FilterPipeline pipeline) {
        if (pipeline.size() == 0) {
            System.out.println("No filters.");
            return;
        }
        pipeline.printFilters();
        System.out.print("Enter index to remove: ");
        try {
            int idx = Integer.parseInt(sc.nextLine().trim());
            boolean ok = pipeline.removeFilter(idx);
            if (ok) {
                System.out.println("Filter removed.");
            } else {
                System.out.println("Wrong index.");
            }
        } catch (NumberFormatException e) {
            System.out.println("Not a number.");
        }
    }

    private static void applyAndSave(Scanner sc,
                                     FilterPipeline pipeline,
                                     byte[] pixels,
                                     int width,
                                     int height,
                                     BufferedImage grayImage) {

        if (pipeline.size() == 0) {
            System.out.println("No filters to apply.");
            return;
        }

        System.out.println("Applying filters...");
        pipeline.applyAll(pixels, width, height);

        System.out.print("Enter path to save: ");
        String outPath = sc.nextLine().trim();

        String format = "png";
        int dot = outPath.lastIndexOf('.');
        if (dot != -1 && dot < outPath.length() - 1) {
            format = outPath.substring(dot + 1);
        }

        try {
            boolean ok = ImageIO.write(grayImage, format, new File(outPath));
            if (!ok) {
                System.out.println("Could not find writer for format: " + format);
            } else {
                System.out.println("Saved to: " + outPath);
            }
        } catch (IOException e) {
            System.out.println("Error saving image: " + e.getMessage());
        }
    }
}
