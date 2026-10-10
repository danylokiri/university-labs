public class BlurFilter implements ImageFilter {

    @Override
    public void apply(byte[] pixels, int width, int height) {
        if (width < 3 || height < 3) {
            System.out.println("Image is too small for blur.");
            return;
        }
        byte[] srcCopy = pixels.clone();

        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int sum = 0;
                for (int dy = -1; dy <= 1; dy++) {
                    for (int dx = -1; dx <= 1; dx++) {
                        int nx = x + dx;
                        int ny = y + dy;
                        int idx = ny * width + nx;
                        sum += srcCopy[idx] & 0xFF;
                    }
                }
                int avg = sum / 9;
                int idx = y * width + x;
                pixels[idx] = (byte) avg;
            }
        }
        System.out.println("Applied: " + getName());
    }

    @Override
    public String getName() {
        return "BlurFilter";
    }
}
