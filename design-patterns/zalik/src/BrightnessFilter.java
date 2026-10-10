public class BrightnessFilter implements ImageFilter {
    private final int delta;

    public BrightnessFilter(int delta) {
        this.delta = delta;
    }

    @Override
    public void apply(byte[] pixels, int width, int height) {
        for (int i = 0; i < pixels.length; i++) {
            int v = (pixels[i] & 0xFF) + delta;
            if (v < 0) v = 0;
            if (v > 255) v = 255;
            pixels[i] = (byte) v;
        }
        System.out.println("Applied: " + getName());
    }

    @Override
    public String getName() {
        return "BrightnessFilter(" + delta + ")";
    }
}
